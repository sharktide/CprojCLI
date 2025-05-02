#include "data.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>

int main(int argc, char* argv[]) {
    FavoritesManager manager;

    if (argc < 2) {
        std::cerr << "Usage:\n"
                  << "  cproj -a <path> <id>   (Add directory)\n"
                  << "  cproj [-c] <id>        (Open favorite) [-c]: Open with Cisual Studio Code\n"
                  << "  cproj -d <id>          (Delete directory from mem)\n";
        return 1;
    }

    std::string command = argv[1];
    if (command == "-a") {
        if (argc < 4) {
            std::cerr << "Error: Missing parameters for adding directory.\n";
            return 1;
        }
        manager.addFavorite(argv[3], argv[2]);
    } else if (command == "-d") {
        if (argc < 3) {
            std::cerr << "Error: Missing ID for deletion.\n";
            return 1;
        }
        manager.deleteFavorite(argv[2]);
    } else {
        bool openInCode = (command == "-c");
        std::string id = openInCode ? argv[2] : argv[1];
        manager.openFavorite(id, openInCode);
    }

    return 0;
}




FavoritesManager::FavoritesManager() {
    char *appdata = getenv("APPDATA");
    if (!appdata) {
        std::cerr << "Could not find AppData path\n";
        exit(1);
    }
    appDataPath = std::string(appdata) + "\\cproj";
    jsonFilePath = appDataPath + "\\favorites.json";

    loadFavorites();
}

void FavoritesManager::loadFavorites() {
    std::ifstream file(jsonFilePath);
    if (!file.is_open()) {
        std::cerr << "Data file missing, initializing new JSON.\n";
        favorites = nlohmann::json::object(); // Create an empty JSON object
        saveFavorites(); // Save a fresh JSON file
        return;
    }

    try {
        file >> favorites;
    } catch (const std::exception &e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
}



void FavoritesManager::saveFavorites() {
    // Ensure the directory exists
    std::filesystem::create_directories(appDataPath);

    // Open file for writing
    std::ofstream file(jsonFilePath, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << jsonFilePath << std::endl;
        return;
    }

    // Write JSON data
    file << favorites.dump(4);
    file.close();
    std::cout << "Favorites saved successfully to: " << jsonFilePath << std::endl;
}


void FavoritesManager::addFavorite(const std::string &id, const std::string &path) {
    std::filesystem::path resolvedPath = std::filesystem::absolute(path);
    favorites[id] = resolvedPath.string();
    saveFavorites();
    std::cout << "Added favorite: " << id << " -> " << resolvedPath.string() << std::endl;
}

void FavoritesManager::openFavorite(const std::string &id, bool openInCode) {
    if (!favorites.contains(id)) {
        std::cerr << "Favorite not found: " << id << std::endl;
        return;
    }

    std::string path = favorites[id].get<std::string>();

    std::string command;
    if (openInCode) {
        command = "code \"" + path + "\""; // Open in VS Code
    } else {
#ifdef _WIN32
        command = "explorer \"" + path + "\""; // Windows Explorer
#elif __APPLE__
        command = "open \"" + path + "\""; // Mac Finder
#else
        command = "xdg-open \"" + path + "\""; // Linux File Manager
#endif
    }

    system(command.c_str());
}


void FavoritesManager::deleteFavorite(const std::string &id) {
    if (favorites.contains(id)) {
        favorites.erase(id);
        saveFavorites();
        std::cout << "Deleted: " << id << std::endl;
    } else {
        std::cerr << "Favorite not found: " << id << std::endl;
    }
}
