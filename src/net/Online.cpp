#include "Online.h"
#include <SFML/Network.hpp>
#include <cctype>
#include <cstdio>
#include <sstream>

// Doi thanh ten tai khoan PythonAnywhere cua nhom
static const char* SERVER = "http://dtd2x.pythonanywhere.com";
static const unsigned short PORT = 80;
static const sf::Time TIMEOUT = sf::seconds(4);

static const char* modeName(Mode m) {
    switch (m) {
    case Mode::Sprint: return "sprint";
    case Mode::Ultra:  return "ultra";
    default:           return "marathon";
    }
}

static std::string urlEncode(const std::string& s) {
    std::string out;
    for (unsigned char c : s) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.') out += char(c);
        else { char buf[4]; std::snprintf(buf, sizeof buf, "%%%02X", c); out += buf; }
    }
    return out;
}

static bool send(sf::Http::Request::Method method, const std::string& uri,
                 const std::string& body, std::string& reply) {
    sf::Http http(SERVER, PORT);
    sf::Http::Request req(uri, method, body);
    if (method == sf::Http::Request::Post)
        req.setField("Content-Type", "application/x-www-form-urlencoded");
    sf::Http::Response res = http.sendRequest(req, TIMEOUT);
    reply = res.getBody();
    return res.getStatus() == sf::Http::Response::Ok;
}

static std::vector<OnlineEntry> parseTop(const std::string& body) {
    std::vector<OnlineEntry> out;
    std::istringstream in(body);
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream row(line);
        OnlineEntry e;
        std::string score, lines, ms;
        if (std::getline(row, e.name, '\t') && std::getline(row, score, '\t') &&
            std::getline(row, lines, '\t') && std::getline(row, ms, '\t') && std::getline(row, e.date)) {
            try {
                e.score = std::stoi(score);
                e.lines = std::stoi(lines);
                e.seconds = std::stoi(ms) / 1000.f;
                out.push_back(e);
            } catch (...) {}                   // dong hong thi bo qua
        }
    }
    return out;
}

Online::~Online() {
    if (worker.joinable()) worker.join();
}

void Online::run(std::function<void()> job) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (working) return;                 // dang co yeu cau khac, bo qua
        working = true;
        msg = "Dang tai...";
    }
    if (worker.joinable()) worker.join();
    worker = std::thread([this, job] {
        job();
        std::lock_guard<std::mutex> lock(mtx);
        working = false;
    });
}

void Online::fetchTop(Mode m) {
    run([this, m] {
        std::string reply;
        bool ok = send(sf::Http::Request::Get, std::string("/api/top?mode=") + modeName(m), "", reply);
        std::lock_guard<std::mutex> lock(mtx);
        if (ok) { boards[int(m)] = parseTop(reply); msg = ""; }
        else msg = "Khong ket noi duoc";
    });
}

void Online::submit(const std::string& name, Mode m, int score, int lines, float seconds) {
    std::string body = "name=" + urlEncode(name) + "&mode=" + modeName(m) +
                       "&score=" + std::to_string(score) + "&lines=" + std::to_string(lines) +
                       "&time=" + std::to_string(int(seconds * 1000));
    run([this, m, body] {
        std::string reply, topReply;
        bool ok = send(sf::Http::Request::Post, "/api/score", body, reply);
        bool okTop = ok && send(sf::Http::Request::Get, std::string("/api/top?mode=") + modeName(m), "", topReply);
        std::lock_guard<std::mutex> lock(mtx);
        if (!ok) { msg = "Khong ket noi duoc"; return; }
        int rank = 0;
        std::sscanf(reply.c_str(), "ok\t%d", &rank);
        msg = rank > 0 ? "Hang " + std::to_string(rank) + " online" : "Da gui diem";
        if (okTop) boards[int(m)] = parseTop(topReply);
    });
}

bool Online::busy() const {
    std::lock_guard<std::mutex> lock(mtx);
    return working;
}

std::string Online::status() const {
    std::lock_guard<std::mutex> lock(mtx);
    return msg;
}

std::vector<OnlineEntry> Online::top(Mode m) const {
    std::lock_guard<std::mutex> lock(mtx);
    return boards[int(m)];
}
