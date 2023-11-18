#include "kernel.h"

std::tuple<USI, USI> get_player_stats() {
    USI level; USI score;
    std::fstream stats("stats.txt", std::fstream::in);
    if (!stats.is_open()) { // Check if the file has opened successfully
        throw std::invalid_argument("Could not open the file 'stats.txt'."); //If true, throw an exception
    } else {
        std::string line;
        USI i=0;
        while (std::getline(stats, line)) { //Reading and spreading the data correctly
            if (i==0) {
                level = std::stoi(line); // Level
            } else if (i==1) {
                score = std::stoi(line); // Score
            }
            i++;
        }
        stats.close();
        return std::make_tuple(level, score); // Return data received
    }
}

std::tuple<USI, USI, USI, bool, char, std::string> get_entity_stats(std::string entity) {
    USI x; USI y; USI dir; bool off; int dir_c; std::string name; // Chars to return 
    std::fstream preferences("preferences.txt", std::fstream::in);
    if (!preferences.is_open()) { // Check if the file has opened successfully
        throw std::invalid_argument("Could not open the file 'preferences.txt'"); //If true, throw an exception
    } else {
        std::string line;
        USI i=0;
        bool todo = false;
        while (std::getline(preferences, line)) {
            if (line == entity || todo) {
                todo = true;
                switch (i) {
                    case 0:
                        name = line; // Get name
                        break;
                    case 1:
                        x = std::stoi(line); // Get x coordinate
                        break;
                    case 2:
                        y = std::stoi(line); // Get y coordinate
                        break;
                    case 3:
                        dir = std::stoi(line); // Get direction
                        break;
                    case 4:
                        if (line == "true") off = true; // Get offensive 
                        else off = false;
                        break;
                }
                if (line == "$END") {
                    break;
                }
                i++;
            }
        }
        preferences.close();
        dir_c = dir + '0';
        return std::make_tuple(x, y, dir, off, dir_c, name);
    }
}

void Save(USI level, USI points) {
    std::ofstream stats("stats.txt");
    if (!stats.is_open()) { // Check if the file has opened successfully
        throw std::invalid_argument("Could not open the file 'stats.txt'."); //If true, throw an exception
    } else {
        stats << level << std::endl << points;
        stats.close();
    }
}
