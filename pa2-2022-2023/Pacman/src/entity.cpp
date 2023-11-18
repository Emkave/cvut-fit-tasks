#include "kernel.h"

inline void AlterBFS(USI & x_ghost, USI & y_ghost, MAP::Map & map, bool state, std::vector<ENTITY::Entity *> & entities, ENTITY::Entity *& entity) {
    UI n = map.xSize * map.ySize; // Number of edges 
    UI s = y_ghost * map.xSize + x_ghost; // Starting position on the map

    UI tar_x;
    UI tar_y;
    UI tar_pos;

    if (state) { // If offenseive
        tar_x = std::get<0>(entities[0]->chrs); // Offensive target
        tar_y = std::get<1>(entities[0]->chrs);
        tar_pos = tar_y * map.xSize + tar_x;
    } else {
        tar_x = map.xSize/2; // Defensive target
        tar_y = map.ySize/2;
        tar_pos = tar_y * map.xSize + tar_x;
    }

    std::queue<int> q; // Queue of cell IDs 
    q.emplace(s); // EMplace starting cell 
    std::vector<bool> used (n); // Vector of visited cells
    std::vector<int> p (n); // Vector of available ID cells
    used[s] = true;
    p[s] = -1; // Not reacheable 

    while (!q.empty()) { // Loops while not empty
        int v = q.front(); // Gettting the first ID 
        q.pop(); // Deleting the first cell
        for (ULI i=0; i<map.bmap[v].size(); i++) {
            if (map.bmap[v][i] == 1 && !used[i]) { // Searching for the not visited cell and obstacle
                used[i] = 1;
                q.emplace(i); // Storing the ID of visited cell 
                p[i] = v;
            }
        }
    }

    std::vector<int> path; // Creating the path to the target
    for (int v=tar_pos; v!=-1; v=p[v]) {
        path.emplace_back(v);
        if (v == 0) {
            break;
        }
    }

    UI next_pos = path[path.size()-2]; // Converting back to normal 
    y_ghost = next_pos / map.xSize; // Changing the position of the ghost on the y-axis
    x_ghost = next_pos - (y_ghost * map.xSize); // Changing the position of the ghost on the x-axis
 
    if (x_ghost == map.xSize/2 && y_ghost == map.ySize/2) { // If ghost is at the center of the map 
        std::get<3>(entity->chrs) = true;
    } 
}

inline void StandardSearch(MAP::Map & map, ENTITY::Entity *& entity) { //Standard Search is a function when an entity's next move is mainly random
    if (ENTITY::entity_clock_tick % 7 == 0) { // By using the internal game clock, we can change the entity's behaving
        std::get<4>(entity->chrs) = ' ';
    }

    if (std::get<4>(entity->chrs) == ' ') {
        do {
            std::get<4>(entity->chrs) = map.available[rand() % map.available.length()]; // Select the way to go randomly
        } while (std::get<4>(entity->chrs) == '$');
    }

    switch (std::get<4>(entity->chrs)) { // Managing the teleportation for the entites
        case '0': // If going down
            map.available[0] = '0'; //Preventing the ghost to move circularly
            map.available[1] = '1';
            map.available[2] = '$';
            map.available[3] = '3';
            if (std::get<0>(entity->chrs) == (int)map.xSize/2 && std::get<1>(entity->chrs) == map.ySize-1) {
                std::get<1>(entity->chrs) = 1;
            } else if (map.map_structs[std::get<1>(entity->chrs)+1][std::get<0>(entity->chrs)] != 0) { //Make ghost to go till it hits obstacle
                std::get<1>(entity->chrs)++; 
            } else if (map.map_skeleton[std::get<1>(entity->chrs)+1][std::get<0>(entity->chrs)] == 0) {
                std::get<4>(entity->chrs) = ' '; // if hits, reset the direction
            } else {
                std::get<4>(entity->chrs) = '0'; //Keep the ghost facing 
            }
            break;
                                    
        case '1': // Same as above // Goind left
            map.available[0] = '0';
            map.available[1] = '1';
            map.available[2] = '2';
            map.available[3] = '$';
            if (std::get<0>(entity->chrs) == 0 && std::get<1>(entity->chrs) == (USI)map.ySize/2) {
                std::get<0>(entity->chrs) = map.xSize-1;
            } else if (map.map_structs[std::get<1>(entity->chrs)][std::get<0>(entity->chrs)-1] != 0) {
                std::get<0>(entity->chrs)--;
            } else if (map.map_structs[std::get<1>(entity->chrs)][std::get<0>(entity->chrs)-1] == 0) {
                std::get<4>(entity->chrs) = ' ';
            } else {
                std::get<4>(entity->chrs) = '1';
            }
            break;
                                    
        case '2': // Up
            map.available[0] = '$';
            map.available[1] = '1';
            map.available[2] = '2';
            map.available[3] = '3';
            if (std::get<0>(entity->chrs) == (USI)map.xSize/2 && std::get<1>(entity->chrs) == 0) {
                std::get<1>(entity->chrs) = map.ySize-1;
            } else if (map.map_structs[std::get<1>(entity->chrs)-1][std::get<0>(entity->chrs)] != 0) {
                std::get<1>(entity->chrs)--;
            } else if (map.map_structs[std::get<1>(entity->chrs)-1][std::get<0>(entity->chrs)] == 0) {
                std::get<4>(entity->chrs) = ' ';
            } else {
                std::get<4>(entity->chrs) = '2';
            }
            break;  
                                    
        case '3': // Right
            map.available[0] = '0';
            map.available[1] = '$';
            map.available[2] = '2';
            map.available[3] = '3';
            if (std::get<0>(entity->chrs) == map.xSize-1 && std::get<1>(entity->chrs) == (int)map.ySize/2) {
                std::get<0>(entity->chrs) = 0;
            } else if (map.map_structs[std::get<1>(entity->chrs)][std::get<0>(entity->chrs)+1] != 0) {
                std::get<0>(entity->chrs)++;
            } else if (map.map_structs[std::get<1>(entity->chrs)][std::get<0>(entity->chrs)+1] == 0) {
                std::get<4>(entity->chrs) = ' ';
            } else {
                std::get<4>(entity->chrs) = '3';
            }
            break;
    }
}

inline void BlinkySearch(MAP::Map & map, ENTITY::Entity *& entity) {
    if (ENTITY::entity_clock_tick % 7 == 0) { // By using the internal game clock, we can change the entity's behaving
        std::get<4>(entity->chrs) = ' ';
    }

    if (std::get<4>(entity->chrs) == ' ') {
        do {
            std::get<4>(entity->chrs) = map.available[rand() % map.available.length()]; // Select the way to go randomly
        } while (std::get<4>(entity->chrs) == '$');
    }

    switch (std::get<4>(entity->chrs)) { // Managing the teleportation for the entites
        case '0': // If going down
            map.available[0] = '0'; //Preventing the ghost to move circularly
            map.available[1] = '1';
            map.available[2] = '$';
            map.available[3] = '3';
            if (std::get<0>(entity->chrs) == (int)map.xSize/2 && std::get<1>(entity->chrs) == map.ySize-1) {
                std::get<1>(entity->chrs) = 1;
            } else if (map.map_structs[std::get<1>(entity->chrs)+1][std::get<0>(entity->chrs)] != 0) { //Make ghost to go till it hits obstacle
                std::get<1>(entity->chrs)++; 
            } else if (map.map_skeleton[std::get<1>(entity->chrs)+1][std::get<0>(entity->chrs)] == 0) {
                std::get<4>(entity->chrs) = ' '; // if hits, reset the direction
            } else {
                std::get<4>(entity->chrs) = '0'; //Keep the ghost facing 
            }
            break;
                                    
        case '1': // Same as above // Goind left
            map.available[0] = '0';
            map.available[1] = '1';
            map.available[2] = '2';
            map.available[3] = '$';
            if (std::get<0>(entity->chrs) == 0 && std::get<1>(entity->chrs) == (USI)map.ySize/2) {
                std::get<0>(entity->chrs) = map.xSize-1;
            } else if (map.map_structs[std::get<1>(entity->chrs)][std::get<0>(entity->chrs)-1] != 0) {
                std::get<0>(entity->chrs)--;
            } else if (map.map_structs[std::get<1>(entity->chrs)][std::get<0>(entity->chrs)-1] == 0) {
                std::get<4>(entity->chrs) = ' ';
            } else {
                std::get<4>(entity->chrs) = '1';
            }
            break;
                                    
        case '2': // Up
            map.available[0] = '$';
            map.available[1] = '1';
            map.available[2] = '2';
            map.available[3] = '3';
            if (std::get<0>(entity->chrs) == (USI)map.xSize/2 && std::get<1>(entity->chrs) == 0) {
                std::get<1>(entity->chrs) = map.ySize-1;
            } else if (map.map_structs[std::get<1>(entity->chrs)-1][std::get<0>(entity->chrs)] != 0) {
                std::get<1>(entity->chrs)--;
            } else if (map.map_structs[std::get<1>(entity->chrs)-1][std::get<0>(entity->chrs)] == 0) {
                std::get<4>(entity->chrs) = ' ';
            } else {
                std::get<4>(entity->chrs) = '2';
            }
            break;  
                                    
        case '3': // Right
            map.available[0] = '0';
            map.available[1] = '$';
            map.available[2] = '2';
            map.available[3] = '3';
            if (std::get<0>(entity->chrs) == map.xSize-1 && std::get<1>(entity->chrs) == (int)map.ySize/2) {
                std::get<0>(entity->chrs) = 0;
            } else if (map.map_structs[std::get<1>(entity->chrs)][std::get<0>(entity->chrs)+1] != 0) {
                std::get<0>(entity->chrs)++;
            } else if (map.map_structs[std::get<1>(entity->chrs)][std::get<0>(entity->chrs)+1] == 0) {
                std::get<4>(entity->chrs) = ' ';
            } else {
                std::get<4>(entity->chrs) = '3';
            }
            break;
    }
}

inline void ClydeSearch(MAP::Map & map, ENTITY::Entity *& entity) {
    std::string fw = "";                         
    if (ENTITY::entity_clock_tick % 7 == 0) { // By using the internal game clock, we can change the entity's behaving
        std::get<4>(entity->chrs) = ' ';
    }

    if (std::get<4>(entity->chrs) == ' ') {
        for (USI i=0; i<4; i++) { // Check where the entity is able to go and register them
            switch (i) { 
                case 0:
                    if (map.map_structs[std::get<1>(entity->chrs)+1][std::get<0>(entity->chrs)] != map.O) {
                        fw += std::to_string(i);
                    }
                    break;
                                        
                case 1:
                    if (map.map_structs[std::get<1>(entity->chrs)][std::get<0>(entity->chrs)-1] != map.O) {
                        fw += std::to_string(i);
                    }
                    break;
                                                    
                case 2:
                    if (map.map_structs[std::get<1>(entity->chrs)-1][std::get<0>(entity->chrs)] != map.O) {
                        fw += std::to_string(i);
                    }
                    break;
                                                    
                case 3:
                    if (map.map_structs[std::get<1>(entity->chrs)][std::get<0>(entity->chrs)+1] != map.O) {
                        fw += std::to_string(i);
                    }
                    break;
            }
        }
    }

    if (std::get<4>(entity->chrs) == ' ') {
        std::get<4>(entity->chrs) = fw[rand() % fw.length()]; // Select the way to go randomly
    }
                            
    switch (std::get<4>(entity->chrs)) { // Managing the teleportation for the entites
        case '0':
            if (std::get<0>(entity->chrs) == (int)map.xSize/2 && std::get<1>(entity->chrs) == map.ySize-1) { // If entity is about to pass upper bound
                std::get<0>(entity->chrs) = (int)map.xSize/2;
                std::get<1>(entity->chrs) = 0;
            }
            
            if (map.map_structs[std::get<1>(entity->chrs)+1][std::get<0>(entity->chrs)] != MAP::Map::O) { //Make ghost to go till it hits obstacle
                std::get<1>(entity->chrs)++; 
                if (map.map_structs[std::get<1>(entity->chrs)+1][std::get<0>(entity->chrs)] == map.O) {
                    std::get<4>(entity->chrs) = ' '; // if hits, reset the direction
                }
            }
            std::get<4>(entity->chrs) = '0'; //Keep the ghost facing 
            break;
                                    
        case '1': // Same as above
            if (std::get<0>(entity->chrs) == 0 && std::get<1>(entity->chrs) == map.ySize/2) {
                std::get<0>(entity->chrs) = map.xSize-1;
            }
            if (map.map_structs[std::get<1>(entity->chrs)][std::get<0>(entity->chrs)-1] != MAP::Map::O) {
                std::get<0>(entity->chrs)--;
                if (map.map_structs[std::get<1>(entity->chrs)][std::get<0>(entity->chrs)-1] == map.O) {
                    std::get<4>(entity->chrs) = ' ';
                }
            }
            std::get<4>(entity->chrs) = '1';
            break;
                                    
        case '2':
            if (std::get<0>(entity->chrs) == map.xSize/2 && std::get<1>(entity->chrs) == 0) {
                std::get<1>(entity->chrs) = map.ySize-1;
            } else if (map.map_structs[std::get<1>(entity->chrs)-1][std::get<0>(entity->chrs)] != MAP::Map::O) {
                std::get<1>(entity->chrs)--;
                if (map.map_structs[std::get<1>(entity->chrs)-1][std::get<0>(entity->chrs)] == map.O) {
                    std::get<4>(entity->chrs) = ' ';
                }
            }
            std::get<4>(entity->chrs) = '2';
            break;  
                                    
        case '3':
            if (std::get<0>(entity->chrs) == map.xSize-1 && std::get<1>(entity->chrs) == map.ySize/2) {
                std::get<0>(entity->chrs) = 0;
            } else if (map.map_structs[std::get<1>(entity->chrs)][std::get<0>(entity->chrs)+1] != MAP::Map::O) {
                std::get<0>(entity->chrs)++;
                if (map.map_structs[std::get<1>(entity->chrs)][std::get<0>(entity->chrs)+1] == map.O) {
                    std::get<4>(entity->chrs) = ' ';
                }
            }
            std::get<4>(entity->chrs) = '3';
            break;
    }

}

namespace ENTITY {
    USI difficulty = 0;
    USI immunity_clock = 10;
    USI collided = false;

    void ENTITY::Pacman::Move(MAP::Map & map, std::vector<ENTITY::Entity *> & entities) { //Setting the controls for Pacman
        switch (std::get<2>(entities[0]->chrs)) {
            case 2: // Looks up
                if (std::get<0>(entities[0]->chrs) == map.xSize/2 && std::get<1>(entities[0]->chrs) == 0) { // Immitating the teleportation
                    std::get<1>(entities[0]->chrs) = map.ySize-1;
                } else if (map.map_structs[std::get<1>(entities[0]->chrs)-1][std::get<0>(entities[0]->chrs)] != MAP::Map::O) { // Check if there is no obstacle
                    std::get<1>(entities[0]->chrs)--;
                }
                break; 
                
            case 1: // Looks left
                if (std::get<0>(entities[0]->chrs) == 0 && std::get<1>(entities[0]->chrs) == map.ySize/2) { // Immitating the teleportation
                    std::get<0>(entities[0]->chrs) = map.xSize-1;
                } else if (map.map_structs[std::get<1>(entities[0]->chrs)][std::get<0>(entities[0]->chrs)-1] != MAP::Map::O) { // Check if there is no obstacle
                    std::get<0>(entities[0]->chrs)--;
                }
                break;
                
            case 0: //Looks down
                if (std::get<0>(entities[0]->chrs) == map.xSize/2 && std::get<1>(entities[0]->chrs) == map.ySize-1) { // Immitating the teleportation
                    std::get<1>(entities[0]->chrs) = 0;
                } else if (map.map_structs[std::get<1>(entities[0]->chrs)+1][std::get<0>(entities[0]->chrs)] != MAP::Map::O) { // Check if there is no obstacle
                    std::get<1>(entities[0]->chrs)++;
                }
                break;
                
            case 3: // Looks right
                if (std::get<0>(entities[0]->chrs) == map.xSize-1 && std::get<1>(entities[0]->chrs) == map.ySize/2) { // Immitating the teleportation
                    std::get<0>(entities[0]->chrs) = 0;
                } else if (map.map_structs[std::get<1>(entities[0]->chrs)][std::get<0>(entities[0]->chrs)+1] != MAP::Map::O) { // Check if there is no obstacle
                    std::get<0>(entities[0]->chrs)++;
                }      
                break;
        }
    }

    void ENTITY::Blinky::Move(MAP::Map & map, std::vector<ENTITY::Entity *> & entities) {//Setting the controls for Blinky
        BlinkySearch(map, entities[1]); //Standard Search
    }

    void ENTITY::Inky::Move(MAP::Map & map, std::vector<ENTITY::Entity *> & entities) { //Setting the controls for Inky
        if (ENTITY::difficulty > 0) { // Check the game difficulty
            if (map.on_borderline == false) { // If pacman is not about to teleport
                if (std::get<3>(entities[2]->chrs) == true) { // Check if offensive
                    AlterBFS(std::get<0>(entities[2]->chrs), std::get<1>(entities[2]->chrs), map, true, entities, entities[2]);
                } else { // else defensive
                    AlterBFS(std::get<0>(entities[2]->chrs), std::get<1>(entities[2]->chrs), map, false, entities, entities[2]);
                }  
            }
        } else { // If easy mode - standard search
            StandardSearch(map, entities[2]);
        }
    }

    void ENTITY::Pinky::Move(MAP::Map & map, std::vector<ENTITY::Entity *> & entities) { //Setting the controls for Pinky
        if (map.on_borderline == false) {// If pacman is not about to teleport
            if (std::get<3>(entities[3]->chrs) == true) {
                AlterBFS(std::get<0>(entities[3]->chrs), std::get<1>(entities[3]->chrs), map, true, entities, entities[3]);
            } else {
                AlterBFS(std::get<0>(entities[3]->chrs), std::get<1>(entities[3]->chrs), map, false, entities, entities[3]);
            }
        }
    }

    void ENTITY::Clyde::Move(MAP::Map & map, std::vector<ENTITY::Entity *> & entities) { //Setting the controls for Clyde
        ClydeSearch(map, entities[4]); // StandardSearch
    }
}
