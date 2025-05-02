#ifndef FAVORITES_MANAGER_H
#define FAVORITES_MANAGER_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cstdlib>
#include "json.hpp" // Using the nlohmann/json library

class FavoritesManager {
private:
    std::string appDataPath;
    std::string jsonFilePath;
    nlohmann::json favorites;

    void loadFavorites();
    void saveFavorites();

public:
    FavoritesManager();
    void addFavorite(const std::string &id, const std::string &path);
    void openFavorite(const std::string &idm, bool openInCode);
    void deleteFavorite(const std::string &id);
};

#endif // FAVORITES_MANAGER_H
