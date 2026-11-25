#include "Paths.h"
#include <filesystem>

namespace fs = std::filesystem;
static fs::path exeDir;

void initPaths(const char* argv0) {
    exeDir = fs::absolute(fs::path(argv0)).parent_path();
}

std::string assetPath(const std::string& rel) {
    return (exeDir / "assets" / rel).string();
}

std::string dataPath(const std::string& file) {
    return (exeDir / file).string();
}
