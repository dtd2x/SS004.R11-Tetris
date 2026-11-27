import os
import sqlite3
from datetime import datetime, timedelta, timezone

from flask import Flask, Response, g, request

DB_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "scores.db")
MODES = ("marathon", "sprint", "ultra")
TOP_N = 10
VN = timezone(timedelta(hours=7))

app = Flask(__name__)


def db():
    if "db" not in g:
        g.db = sqlite3.connect(DB_PATH)
        g.db.execute(
            "CREATE TABLE IF NOT EXISTS scores ("
            " id INTEGER PRIMARY KEY AUTOINCREMENT,"
            " name TEXT NOT NULL, mode TEXT NOT NULL,"
            " score INTEGER NOT NULL, lines INTEGER NOT NULL,"
            " time_ms INTEGER NOT NULL, created TEXT NOT NULL)"
        )
    return g.db


@app.teardown_appcontext
def close_db(_):
    conn = g.pop("db", None)
    if conn is not None:
        conn.close()


def text(body, status=200):
    return Response(body, status=status, mimetype="text/plain; charset=utf-8")


def order_by(mode):
    # Sprint xep theo thoi gian (nhanh len truoc), mode khac xep theo diem
    return "time_ms ASC, score DESC" if mode == "sprint" else "score DESC, time_ms ASC"


def top(mode):
    # Moi ten chi lay ket qua tot nhat
    sql = (
        "SELECT name, score, lines, time_ms, created FROM ("
        " SELECT *, ROW_NUMBER() OVER (PARTITION BY name ORDER BY " + order_by(mode) + ") AS r"
        " FROM scores WHERE mode = ?)"
        " WHERE r = 1 ORDER BY " + order_by(mode) + " LIMIT ?"
    )
    return db().execute(sql, (mode, TOP_N)).fetchall()


def read_int(form, key, lo, hi):
    v = form.get(key, "")
    if not v.isdigit() or not lo <= int(v) <= hi:
        raise ValueError(key)
    return int(v)


@app.get("/")
def index():
    return text("Tetris SS004.R11 - bang xep hang online\n")


@app.get("/api/top")
def api_top():
    mode = request.args.get("mode", "")
    if mode not in MODES:
        return text("error: mode\n", 400)
    lines = ["\t".join(str(c) for c in row) for row in top(mode)]
    return text("".join(l + "\n" for l in lines))


@app.post("/api/score")
def api_score():
    f = request.form
    name = f.get("name", "").strip()
    mode = f.get("mode", "")
    if mode not in MODES:
        return text("error: mode\n", 400)
    if not 1 <= len(name) <= 16 or any(c in name for c in "\t\r\n"):
        return text("error: name\n", 400)
    try:
        score = read_int(f, "score", 0, 99_999_999)
        lines = read_int(f, "lines", 0, 99_999)
        time_ms = read_int(f, "time", 0, 86_400_000)
    except ValueError as e:
        return text(f"error: {e}\n", 400)
    if mode == "sprint" and lines < 40:
        return text("error: sprint\n", 400)

    created = datetime.now(VN).strftime("%d/%m/%Y")
    conn = db()
    conn.execute(
        "INSERT INTO scores (name, mode, score, lines, time_ms, created) VALUES (?, ?, ?, ?, ?, ?)",
        (name, mode, score, lines, time_ms, created),
    )
    conn.commit()

    rank = next((i + 1 for i, row in enumerate(top(mode)) if row[0] == name), 0)
    return text(f"ok\t{rank}\n")


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
