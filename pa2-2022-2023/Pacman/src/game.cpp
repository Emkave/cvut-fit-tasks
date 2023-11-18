#include "kernel.h"

inline void Resetter(std::vector<ENTITY::Entity *> & entities) {    // Resetting entities stats when the new game is called 
    std::tuple<USI, USI, USI, bool, char, std::string> to_imply;
    to_imply = get_entity_stats("pacman");
    std::get<0>(entities[0]->chrs) = std::get<0>(to_imply);
    std::get<1>(entities[0]->chrs) = std::get<1>(to_imply);
    std::get<2>(entities[0]->chrs) = std::get<2>(to_imply);
    to_imply = get_entity_stats("blinky");
    std::get<0>(entities[1]->chrs) = std::get<0>(to_imply);
    std::get<1>(entities[1]->chrs) = std::get<1>(to_imply);
    std::get<2>(entities[1]->chrs) = std::get<2>(to_imply);
    std::get<3>(entities[1]->chrs) = std::get<3>(to_imply);
    to_imply = get_entity_stats("inky");
    std::get<0>(entities[2]->chrs) = std::get<0>(to_imply);
    std::get<1>(entities[2]->chrs) = std::get<1>(to_imply);
    std::get<2>(entities[2]->chrs) = std::get<2>(to_imply);
    std::get<3>(entities[2]->chrs) = std::get<3>(to_imply);
    to_imply = get_entity_stats("pinky");
    std::get<0>(entities[3]->chrs) = std::get<0>(to_imply);
    std::get<1>(entities[3]->chrs) = std::get<1>(to_imply);
    std::get<2>(entities[3]->chrs) = std::get<2>(to_imply);
    std::get<3>(entities[3]->chrs) = std::get<3>(to_imply);
    to_imply = get_entity_stats("clyde");
    std::get<0>(entities[4]->chrs) = std::get<0>(to_imply);
    std::get<1>(entities[4]->chrs) = std::get<1>(to_imply);
    std::get<2>(entities[4]->chrs) = std::get<2>(to_imply);
    std::get<3>(entities[4]->chrs) = std::get<3>(to_imply);
}

inline void Resetter(std::vector<ENTITY::Entity *> & entities, int sep) {    // Resetting the entity stats separately, when the new game is called 
    std::tuple<USI, USI, USI, bool, char, std::string> to_imply;
    if (sep == -1) {
        to_imply = get_entity_stats("pacman");
        std::get<0>(entities[0]->chrs) = std::get<0>(to_imply);
        std::get<1>(entities[0]->chrs) = std::get<1>(to_imply);
        std::get<2>(entities[0]->chrs) = std::get<2>(to_imply);
    } else if (sep == 2) {
        to_imply = get_entity_stats("blinky");
        std::get<0>(entities[1]->chrs) = std::get<0>(to_imply);
        std::get<1>(entities[1]->chrs) = std::get<1>(to_imply);
        std::get<2>(entities[1]->chrs) = std::get<2>(to_imply);
        std::get<3>(entities[1]->chrs) = std::get<3>(to_imply);
    } else if (sep == 3) {
        to_imply = get_entity_stats("inky");
        std::get<0>(entities[2]->chrs) = std::get<0>(to_imply);
        std::get<1>(entities[2]->chrs) = std::get<1>(to_imply);
        std::get<2>(entities[2]->chrs) = std::get<2>(to_imply);
        std::get<3>(entities[2]->chrs) = std::get<3>(to_imply);
    } else if (sep == 4) {
        to_imply = get_entity_stats("pinky");
        std::get<0>(entities[3]->chrs) = std::get<0>(to_imply);
        std::get<1>(entities[3]->chrs) = std::get<1>(to_imply);
        std::get<2>(entities[3]->chrs) = std::get<2>(to_imply);
        std::get<3>(entities[3]->chrs) = std::get<3>(to_imply);
    } else if (sep == 5) {
        to_imply = get_entity_stats("clyde");
        std::get<0>(entities[4]->chrs) = std::get<0>(to_imply);
        std::get<1>(entities[4]->chrs) = std::get<1>(to_imply);
        std::get<2>(entities[4]->chrs) = std::get<2>(to_imply);
        std::get<3>(entities[4]->chrs) = std::get<3>(to_imply);
    }
}

void SCENE::Game::Load(void) { // Loading the game
    setlocale(LC_CTYPE, "");
    noecho();
    clear();
    curs_set(0);
    nodelay(stdscr, TRUE); //Standard intialization
    initscr();
    cbreak();

    start_color(); // Color palettes
    init_pair(1, COLOR_BLUE, COLOR_BLUE);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);

    init_pair(10, COLOR_YELLOW, COLOR_BLACK);
    init_pair(11, COLOR_CYAN, COLOR_BLACK);
    init_pair(12, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(13, COLOR_RED, COLOR_BLACK);
    init_pair(14, COLOR_WHITE, COLOR_BLACK);

    init_pair(15, COLOR_GREEN, COLOR_GREEN);
    init_pair(16, COLOR_RED, COLOR_RED);
    
    USI GAME_WIDTH = 0;
    USI points = 0;
    int lives = 0;
    double GAME_HEIGHT_SHIFT = 0;
    bool iterated = false;
    int highlight = 0;
    bool highlighted = false;
    unsigned short int s = sizeof(SCENE::Scene::game_pause_selection)/sizeof(SCENE::Scene::game_pause_selection[0]); // size of pause selectoions

    MAP::Map game_map;  // Initialization of the game map
    
    std::vector<ENTITY::Entity *> entities; // Vector of entities
    ENTITY::Pacman * pacman = new ENTITY::Pacman(); // Creating Pacman
    ENTITY::Inky * inky = new ENTITY::Inky();   // Creating Inky
    ENTITY::Clyde * clyde = new ENTITY::Clyde(); // Creating Clyde
    ENTITY::Pinky * pinky = new ENTITY::Pinky(); // Creating Pinky
    ENTITY::Blinky * blinky = new ENTITY::Blinky(); // Creating Blinky

    entities.emplace_back(pacman); // Store the entities in the vector
    entities.emplace_back(blinky);
    entities.emplace_back(inky);
    entities.emplace_back(pinky);
    entities.emplace_back(clyde);
    
    // Structure of entities:           x  y dir offens dir  name
    std::tuple<USI, USI, USI, bool, char, std::string> to_imply; // Will be used to withdraw information about entities

    to_imply = get_entity_stats("pacman");
    entities[0]->chrs = std::make_tuple(std::get<0>(to_imply), std::get<1>(to_imply), std::get<2>(to_imply), std::get<3>(to_imply), std::get<4>(to_imply), std::get<5>(to_imply));
    to_imply = get_entity_stats("blinky");
    entities[1]->chrs = std::make_tuple(std::get<0>(to_imply), std::get<1>(to_imply), std::get<2>(to_imply), std::get<3>(to_imply), std::get<4>(to_imply), std::get<5>(to_imply)); //Default prefernces of entities
    to_imply = get_entity_stats("inky");
    entities[2]->chrs = std::make_tuple(std::get<0>(to_imply), std::get<1>(to_imply), std::get<2>(to_imply), std::get<3>(to_imply), std::get<4>(to_imply), std::get<5>(to_imply));
    to_imply = get_entity_stats("pinky");
    entities[3]->chrs = std::make_tuple(std::get<0>(to_imply), std::get<1>(to_imply), std::get<2>(to_imply), std::get<3>(to_imply), std::get<4>(to_imply), std::get<5>(to_imply));
    to_imply = get_entity_stats("clyde");
    entities[4]->chrs = std::make_tuple(std::get<0>(to_imply), std::get<1>(to_imply), std::get<2>(to_imply), std::get<3>(to_imply), std::get<4>(to_imply), std::get<5>(to_imply));

    GAME_WIDTH = getmaxx(stdscr); // Getting the size of the screen

    WINDOW * game_window = newwin(0, 0, 0, 0); // Creating the window 
    box(game_window, 0, 0); // Setting the window
    refresh();
    wrefresh(game_window);

    mvprintw(0, GAME_HEIGHT_SHIFT-5, "Pacman: CVUT Edition"); // Title

    while (true) { // Game starts here
        if (MAP::level != 256) { // The easter-egg (switch the level value to 256 to activate the easter-egg in namespacer.cpp, or pass all 255 levels)
            for (USI i=0; i<s; i++) { // Highlight choice
                if (i == highlight) {
                    attron(COLOR_PAIR(3));
                }
                mvprintw(5+i, 2, "                 ");
                attroff(COLOR_PAIR(3));
            }   
            mvprintw(1, 1, "Points: "); // Current characteristics of the user
            mvprintw(1, 9, std::to_string(MAP::gained_points).c_str());
            mvprintw(2, 1, "Level: ");
            mvprintw(2, 8, std::to_string(MAP::level).c_str()); 
            mvprintw(3, 1, "Lives:");
            mvprintw(3, 8, std::to_string(ENTITY::lives).c_str());

            if (ENTITY::paused == false) { //Check if the game is paused
                if (ENTITY::iterated) { // First do, then render method
                    switch (ENTITY::difficulty) {
                        case 0:
                            ENTITY::lives = 5;
                            break;
                        
                        case 1:
                            ENTITY::lives = 3;
                            break;
                        
                        case 2:
                            ENTITY::lives = 1;
                            break;
                    }
                }

                ENTITY::iterated = false; 
                if (iterated) {
                    if (!ENTITY::paused) { // I check it every time just to be sure and for the debugging purposes
                        if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[1]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[1]->chrs) && std::get<3>(entities[1]->chrs) == true) { // Check the entity
                            ENTITY::lives--; 
                            if (ENTITY::lives == 0) { // Check if the player is lost
                                ENTITY::lost = true;
                                ENTITY::paused = true;
                            } else {
                                Resetter(entities); // Resetting the entites if the player is dead and has lives
                            }
                        } else if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[1]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[1]->chrs) && std::get<3>(entities[1]->chrs) == false) { // Check if the entity is not in offensive mode and player ate him
                            std::get<0>(entities[1]->chrs) = game_map.xSize/2; // Respawn
                            std::get<1>(entities[1]->chrs) = game_map.ySize/2;
                            std::get<3>(entities[1]->chrs) = true;
                            std::get<4>(entities[1]->chrs) = '1';
                            MAP::gained_points += 600; // Gaining points for 'killing a ghost'
                            points += 600; // The variable is some of RAM. Used to track the number of points in a particular game and if necessary to add/subtract from the overall
                        }
                        entities[1]->Move(game_map, entities); // Move the entity
                        if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[1]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[1]->chrs) && std::get<3>(entities[1]->chrs) == true) { // Check the entity
                            ENTITY::lives--; 
                            if (ENTITY::lives == 0) { // Check if the player is lost
                                ENTITY::lost = true;
                                ENTITY::paused = true;
                            } else {
                                Resetter(entities); // Resetting the entites if the player is dead and has lives
                            }
                        } else if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[1]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[1]->chrs) && std::get<3>(entities[1]->chrs) == false) { // Check if the entity is not in offensive mode and player ate him
                            std::get<0>(entities[1]->chrs) = game_map.xSize/2; // Respawn
                            std::get<1>(entities[1]->chrs) = game_map.ySize/2;
                            std::get<3>(entities[1]->chrs) = true;
                            std::get<4>(entities[1]->chrs) = '1';
                            MAP::gained_points += 600; // Gaining points for 'killing a ghost'
                            points += 600; // The variable is some of RAM. Used to track the number of points in a particular game and if necessary to add/subtract from the overall
                        }
                    }

                    if (!ENTITY::paused) { // I check it every time just to be sure and for the debugging purposes
                        if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[2]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[2]->chrs) && std::get<3>(entities[2]->chrs) == true) { // Check if the entity is not in offensive mode and player ate him
                            ENTITY::lives--;
                            if (ENTITY::lives == 0) { // Check if the player is lost
                                ENTITY::lost = true; 
                                ENTITY::paused = true;
                            } else {
                                Resetter(entities); // Resetting the entites if the player is dead and has lives
                            }
                        } else if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[2]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[2]->chrs) && std::get<3>(entities[2]->chrs) == false) {
                            std::get<0>(entities[2]->chrs) = game_map.xSize/2; // Respawn
                            std::get<1>(entities[2]->chrs) = game_map.ySize/2;
                            std::get<3>(entities[2]->chrs) = true;
                            std::get<4>(entities[2]->chrs) = '1';
                            MAP::gained_points += 600; // Gaining points for 'killing a ghost'
                            points += 600; // The variable is some of RAM. Used to track the number of points in a particular game and if necessary to add/subtract from the overall 
                        }
                        entities[2]->Move(game_map, entities); // Move the entity
                        if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[2]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[2]->chrs) && std::get<3>(entities[2]->chrs) == true) { // Check if the entity is not in offensive mode and player ate him
                            ENTITY::lives--;
                            if (ENTITY::lives == 0) { // Check if the player is lost
                                ENTITY::lost = true; 
                                ENTITY::paused = true;
                            } else {
                                Resetter(entities); // Resetting the entites if the player is dead and has lives
                            }
                        } else if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[2]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[2]->chrs) && std::get<3>(entities[2]->chrs) == false) {
                            std::get<0>(entities[2]->chrs) = game_map.xSize/2; // Respawn
                            std::get<1>(entities[2]->chrs) = game_map.ySize/2;
                            std::get<3>(entities[2]->chrs) = true;
                            std::get<4>(entities[2]->chrs) = '1';
                            MAP::gained_points += 600; // Gaining points for 'killing a ghost'
                            points += 600; // The variable is some of RAM. Used to track the number of points in a particular game and if necessary to add/subtract from the overall 
                        }
                    }

                    if (!ENTITY::paused) { // I check it every time just to be sure and for the debugging purposes
                        if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[3]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[3]->chrs) && std::get<3>(entities[1]->chrs) == true) { // Check if the entity is not in offensive mode and player ate him
                            ENTITY::lives--;
                            lives--;
                            if (ENTITY::lives == 0) { // Check if the player is lost
                                ENTITY::lost = true;
                                ENTITY::paused = true;
                            } else {
                                Resetter(entities); // Resetting the entites if the player is dead and has lives
                            }
                        } else if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[3]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[3]->chrs) && std::get<3>(entities[1]->chrs) == false) {
                            std::get<0>(entities[3]->chrs) = game_map.xSize/2; // Respawn
                            std::get<1>(entities[3]->chrs) = game_map.ySize/2;
                            std::get<3>(entities[3]->chrs) = true;
                            std::get<4>(entities[3]->chrs) = '1';
                            MAP::gained_points += 600; // Gaining points for 'killing a ghost'
                            points += 600; // The variable is some of RAM. Used to track the number of points in a particular game and if necessary to add/subtract from the overall
                        }  
                        entities[3]->Move(game_map, entities);// Move the entity
                        if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[3]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[3]->chrs) && std::get<3>(entities[1]->chrs) == true) { // Check if the entity is not in offensive mode and player ate him
                            ENTITY::lives--;
                            lives--;
                            if (ENTITY::lives == 0) { // Check if the player is lost
                                ENTITY::lost = true;
                                ENTITY::paused = true;
                            } else {
                                Resetter(entities); // Resetting the entites if the player is dead and has lives
                            }
                        } else if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[3]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[3]->chrs) && std::get<3>(entities[1]->chrs) == false) {
                            std::get<0>(entities[3]->chrs) = game_map.xSize/2; // Respawn
                            std::get<1>(entities[3]->chrs) = game_map.ySize/2;
                            std::get<3>(entities[3]->chrs) = true;
                            std::get<4>(entities[3]->chrs) = '1';
                            MAP::gained_points += 600; // Gaining points for 'killing a ghost'
                            points += 600; // The variable is some of RAM. Used to track the number of points in a particular game and if necessary to add/subtract from the overall
                        }   
                    }

                    if (!ENTITY::paused) {// I check it every time just to be sure and for the debugging purposes
                        if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[4]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[4]->chrs) && std::get<3>(entities[4]->chrs) == true) { // Check if the entity is not in offensive mode and player ate him
                            ENTITY::lives--;
                            if (ENTITY::lives == 0) { // Check if the player is lost
                                ENTITY::lost = true;
                                ENTITY::paused = true;
                            } else {
                                Resetter(entities); // Resetting the entites if the player is dead and has lives
                            }
                        } else if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[4]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[4]->chrs) && std::get<3>(entities[4]->chrs) == false) {
                            std::get<0>(entities[4]->chrs) = game_map.xSize/2; // Respawn
                            std::get<1>(entities[4]->chrs) = game_map.ySize/2;
                            std::get<3>(entities[4]->chrs) = true;
                            std::get<4>(entities[4]->chrs) = '1';
                            MAP::gained_points += 600; // Gaining points for 'killing a ghost'
                            points += 600; // The variable is some of RAM. Used to track the number of points in a particular game and if necessary to add/subtract from the overall
                        }
                        entities[4]->Move(game_map, entities);// Move the entity
                        if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[4]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[4]->chrs) && std::get<3>(entities[4]->chrs) == true) { // Check if the entity is not in offensive mode and player ate him
                            ENTITY::lives--;
                            if (ENTITY::lives == 0) { // Check if the player is lost
                                ENTITY::lost = true;
                                ENTITY::paused = true;
                            } else {
                                Resetter(entities); // Resetting the entites if the player is dead and has lives
                            }
                        } else if (std::get<0>(entities[0]->chrs) == std::get<0>(entities[4]->chrs) && std::get<1>(entities[0]->chrs) == std::get<1>(entities[4]->chrs) && std::get<3>(entities[4]->chrs) == false) {
                            std::get<0>(entities[4]->chrs) = game_map.xSize/2; // Respawn
                            std::get<1>(entities[4]->chrs) = game_map.ySize/2;
                            std::get<3>(entities[4]->chrs) = true;
                            std::get<4>(entities[4]->chrs) = '1';
                            MAP::gained_points += 600; // Gaining points for 'killing a ghost'
                            points += 600; // The variable is some of RAM. Used to track the number of points in a particular game and if necessary to add/subtract from the overall
                        }
                    }
                    
                    if (game_map.map_structs[std::get<1>(entities[0]->chrs)][std::get<0>(entities[0]->chrs)] == game_map.U) { // Check if pacman ate the dots
                        game_map.map_structs[std::get<1>(entities[0]->chrs)][std::get<0>(entities[0]->chrs)] = game_map.C;
                        game_map.total_points--;
                        MAP::gained_points++;
                        points++;
                    } else if (game_map.map_structs[std::get<1>(entities[0]->chrs)][std::get<0>(entities[0]->chrs)] == game_map.B) { // Check if pacman ate the bonus
                        game_map.map_structs[std::get<1>(entities[0]->chrs)][std::get<0>(entities[0]->chrs)] = game_map.C;
                        std::get<3>(entities[1]->chrs) = false;
                        std::get<3>(entities[2]->chrs) = false;
                        std::get<3>(entities[3]->chrs) = false;
                        std::get<3>(entities[4]->chrs) = false;
                        ENTITY::immunity_clock = 20; // Immunity clock giving opportunity to kill ghost entities
                    } else if (game_map.map_structs[std::get<1>(entities[0]->chrs)][std::get<0>(entities[0]->chrs)] == game_map.L) {
                        game_map.map_structs[std::get<1>(entities[0]->chrs)][std::get<0>(entities[0]->chrs)] = game_map.C;
                        ENTITY::lives++;
                        lives++;
                    } else if (game_map.map_structs[std::get<1>(entities[0]->chrs)][std::get<0>(entities[0]->chrs)] == game_map.E) {
                        game_map.map_structs[std::get<1>(entities[0]->chrs)][std::get<0>(entities[0]->chrs)] = game_map.C;
                        points += 200;
                        MAP::gained_points += 200;
                    }
                    if (game_map.total_points == 0) { // Check if there are no points to eat more, if so - level +1 
                        MAP::level++;
                        ENTITY::won = true;
                        ENTITY::paused = true;
                    }
                    if (ENTITY::lives == 0) { // If there are no lives end - game over menu scene appears
                        ENTITY::paused = true;
                        ENTITY::lost = true;
                    }

                    if (!ENTITY::paused) { // I check it every time just to be sure and for the debugging purposes
                        entities[0]->Move(game_map, entities);// Move the entity
                        if (std::get<0>(entities[0]->chrs) == (int)game_map.xSize/2 && std::get<1>(entities[0]->chrs) == 0) { // Check if pacman is about to teleport
                            game_map.on_borderline = true; // If pacman on the border of the map
                        } else if (std::get<0>(entities[0]->chrs) == game_map.xSize-1 && std::get<1>(entities[0]->chrs) == (int)game_map.ySize/2) {
                            game_map.on_borderline = true;
                        } else if (std::get<0>(entities[0]->chrs) == (int)game_map.xSize/2 && std::get<1>(entities[0]->chrs) == game_map.ySize-1) {
                            game_map.on_borderline = true;
                        } else if (std::get<0>(entities[0]->chrs) == 0 && std::get<1>(entities[0]->chrs) == (int)game_map.ySize/2) {
                            game_map.on_borderline = true;
                        } else {
                            game_map.on_borderline = false;
                        }
                    }
                }

                /*
                    You will see below this - 

                    attron(smth);
                    mvprintw(smth);
                    attroff(smth);

                    This block of functions are rendering an object on the screen with a particular foreground and background color
                    attron and attroff needs to not render all objects in with the same foreground and background color
                    it is like: render starting from here to there with this foreground and background color
                */

                if (!ENTITY::paused) { 
                    for (USI y=0; y<game_map.ySize; y++) { // Update the window
                        for (USI x=0; x<game_map.xSize; x++) {
                            switch (game_map.map_structs[y][x]) {
                                case game_map.O: // Check if obstacle on the map[y][x]
                                    attron(COLOR_PAIR(1)); // Highlight it 
                                    mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), " ");    
                                    attroff(COLOR_PAIR(1)); // But not the all objects on the screen
                                    break;
                                
                                case game_map.U: // Check if uncleared (dots there) on the map[y][x]
                                    attron(COLOR_PAIR(2));
                                    mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), ".");
                                    attroff(COLOR_PAIR(2));
                                    break;
                                
                                case game_map.C: // Check if clear (dots there eaten) on the map[y][x]
                                    attron(COLOR_PAIR(3));
                                    mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), " ");
                                    attroff(COLOR_PAIR(3));
                                    break;
                                
                                case game_map.B: // Check if bonus on the map[y][x]
                                    attron(COLOR_PAIR(4));
                                    mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "★");
                                    attroff(COLOR_PAIR(4));
                                    break;

                                case game_map.E:
                                    attron(COLOR_PAIR(2));
                                    mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "♣︎");
                                    attroff(COLOR_PAIR(2));
                                    break;
                                
                                case game_map.L:
                                    attron(COLOR_PAIR(13));
                                    mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "❤︎");
                                    attroff(COLOR_PAIR(13));
                                    break;
                            }

                            if (std::get<0>(entities[0]->chrs) == x && std::get<1>(entities[0]->chrs) == y) { // Check Pacman's coordinates and its orientation
                                switch (std::get<2>(entities[0]->chrs)) {
                                    case 0: // Down
                                        attron(COLOR_PAIR(4));
                                        mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "^");
                                        attroff(COLOR_PAIR(4));
                                        break;

                                    case 1: // left
                                        attron(COLOR_PAIR(4));
                                        mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), ">");
                                        attroff(COLOR_PAIR(4));
                                        break;

                                    case 2: // up
                                        attron(COLOR_PAIR(4));
                                        mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "v");
                                        attroff(COLOR_PAIR(4));
                                        break;

                                    case 3: // right
                                        attron(COLOR_PAIR(4));
                                        mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "<");
                                        attroff(COLOR_PAIR(4));
                                        break;
                                }
                            }

                            if (std::get<0>(entities[3]->chrs) == x && std::get<1>(entities[3]->chrs) == y) { // Check Pinky's coordinates and its orientation
                                if (std::get<3>(entities[3]->chrs) == 1) {
                                    attron(COLOR_PAIR(12));
                                    mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "☺︎"); // Render this character if offensive
                                    attroff(COLOR_PAIR(12));
                                } else {
                                    attron(COLOR_PAIR(14));
                                    mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "☹︎"); // Render this character if defensive
                                    attroff(COLOR_PAIR(14));
                                }
                            }

                            if (std::get<0>(entities[2]->chrs) == x && std::get<1>(entities[2]->chrs) == y) { // Check Inky's coordinates and its orientation
                                if (std::get<3>(entities[2]->chrs) == 1) {
                                    attron(COLOR_PAIR(11));
                                    mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "☺︎");  // Render this character if offensive
                                    attroff(COLOR_PAIR(11));
                                } else {
                                    attron(COLOR_PAIR(14));
                                    mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "☹︎"); // Render this character if defensive
                                    attroff(COLOR_PAIR(14));
                                }
                            }

                            if (std::get<0>(entities[4]->chrs) == x && std::get<1>(entities[4]->chrs) == y) { // Check Clyde's coordinates and its orientation
                                if (std::get<3>(entities[4]->chrs) == 1) {
                                    attron(COLOR_PAIR(10));
                                    mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "☺︎"); // Render this character if offensive
                                    attroff(COLOR_PAIR(10));
                                } else {
                                    attron(COLOR_PAIR(14));
                                    mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "☹︎"); // Render this character if defensive
                                    attroff(COLOR_PAIR(14));
                                }
                            }

                            if (std::get<0>(entities[1]->chrs) == x && std::get<1>(entities[1]->chrs) == y) { // Check Blinky's coordinates and its orientation
                                if (std::get<3>(entities[1]->chrs) == 1) {
                                    attron(COLOR_PAIR(13));
                                    mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "☺︎"); // Render this character if offensive
                                    attroff(COLOR_PAIR(13));
                                } else {
                                    attron(COLOR_PAIR(14));
                                    mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "☹︎"); // Render this character if defensive
                                    attroff(COLOR_PAIR(14));
                                }
                            }
                        }
                    }

                    if (!std::get<3>(entities[1]->chrs) || !std::get<3>(entities[2]->chrs) || !std::get<3>(entities[3]->chrs) || !std::get<3>(entities[4]->chrs)) { // If bonus is ate 
                        ENTITY::immunity_clock--; // Immunity clock is deacreasing by each frame
                        if (ENTITY::immunity_clock == 0) { // If the immunity_clock is 0, the ghost are becoming in offensive mode
                            std::get<3>(entities[1]->chrs) = true;
                            std::get<3>(entities[2]->chrs) = true;
                            std::get<3>(entities[3]->chrs) = true;
                            std::get<3>(entities[4]->chrs) = true;
                            ENTITY::immunity_clock = 20;
                        }
                    }
                    
                    cbreak(); // No wait for user input
                    switch (getch()) { // Controller
                        case 'w':
                        case 'W':
                            std::get<2>(entities[0]->chrs) = 2; // Face up
                            break;

                        case 'a':
                        case 'A':
                            std::get<2>(entities[0]->chrs) = 1; // Face left
                            break;

                        case 's':
                        case 'S':
                            std::get<2>(entities[0]->chrs) = 0; // Face down
                            break;

                        case 'd':
                        case 'D':
                            std::get<2>(entities[0]->chrs) = 3; // Face right
                            break;
                        
                        case 'e': // Escape / Pause 
                        case 'E':
                            ENTITY::paused = true;
                            break;
                    }
                        
                    if (ENTITY::entity_clock_tick % 100 == 0) { // Random bonus generator 
                        std::pair<USI, USI> random_pair = game_map.free_poses[rand() % (game_map.free_poses.size()-1)]; // Getting random available position
                        std::random_device rd; // Initialization of random generator
                        std::default_random_engine eng(rd());
                        std::uniform_int_distribution<std::default_random_engine::result_type> distr(game_map.E, game_map.L); // Getting random bonus
                        if (game_map.map_structs[random_pair.first][random_pair.second] == game_map.U) {
                            game_map.total_points--;
                        }
                        game_map.map_structs[random_pair.first][random_pair.second] = static_cast<MAP::Map::map_cell_state>(distr(eng)); // Rendering the bonus
                    }
                }
                    ENTITY::entity_clock_tick++; 
                    usleep(300000); // Set fps (somewhere 10 FPS)
            } else { // If game is paused
                USI i; // Rendreing the pause menu
                if (ENTITY::lost) {
                    i=1;
                    if (!highlighted) {
                        highlight = 1;
                        highlighted= true;
                    }
                } else {
                    i=0;
                }
                for (; i<s; i++) {
                    if (i == highlight) {
                        attron(A_REVERSE);
                    }
                    mvprintw(5+i, 2, SCENE::Scene::game_pause_selection[i].c_str());
                    attroff(A_REVERSE);
                }
                if (ENTITY::lost)
                    mvprintw(9, 2, "You Lost!");
                else if (ENTITY::won) {
                    mvprintw(9, 2, "You Won!");
                }

                switch (getch()) { // Controls the same as for main menu
                    case 'w':
                    case 'W':
                        highlight--; // Select upper 
                        if (ENTITY::lost) {
                            if (highlight == 0) {
                                highlight = s-1;
                            }
                        } else {
                            if (highlight < 0) {
                                highlight = s-1;
                            }
                        }
                        break;

                    case 's':
                    case 'S':
                        highlight++; // Select lower 
                        if (ENTITY::lost) {
                            if (highlight == s) {
                                highlight = 1;
                            }
                        } else {
                            if (highlight == s) {
                                highlight = 0;
                            }
                        }
                        break;

                    case 'i':
                    case 'I': // Choose 
                        switch (highlight) { // Check the button selected 
                            case 0:
                                ENTITY::paused = false; // Unpause the game
                                if (ENTITY::won) { // If won and this selected -> continue with level+1
                                    ENTITY::won = false;
                                    Save(MAP::level, MAP::gained_points);
                                    points = 0;
                                    lives = 0;
                                    for (auto & elem : entities) {
                                        delete elem;
                                    }
                                    SCENE::Scene::Exit(game_window, false);
                                    SCENE::Game game_win;
                                    game_win.Load();
                                }
                                break;
                            
                            case 1: { // If new game is selected...
                                ENTITY::paused = false; // Not paused
                                ENTITY::lost = false; // Not lost
                                if (ENTITY::won) { // If new game is selected when it is won
                                    MAP::level--; // Decrement level
                                    ENTITY::won = false; // Not won
                                }
                                switch (ENTITY::difficulty) { // Num of lives depends on difficulty
                                    case 0:
                                        ENTITY::lives = 5; // Easy 
                                        break;
                                    
                                    case 1:
                                        ENTITY::lives = 3; // Medium 
                                        break;
                                    
                                    case 2:
                                        ENTITY::lives = 1; // Hard
                                        break;
                                }
                                MAP::gained_points -= points; // Decrement the total points 
                                points = 0;
                                lives = 0;
                                for (auto & elem : entities) { // Delete the entities
                                    delete elem;
                                }
                                SCENE::Scene::Exit(game_window, false);
                                SCENE::Game game_win;
                                game_win.Load(); // Reload the game
                                break;
                            }
                            
                            case 2: { // When exit is selected
                                points = 0;  
                                lives = 0;
                                for (auto & elem : entities) { // Delete the entites
                                    delete elem;
                                }
                                if (ENTITY::won)
                                    Save(MAP::level, MAP::gained_points);
                                SCENE::Scene::Exit(game_window, false); // Exit the game
                                SCENE::MainMenu menu;
                                menu.Load();
                                break;
                            }
                        }
                        break;
                }
            }
        } else if (MAP::level == 256) { //Easter-egg activated
            mvprintw(1, 1, "Points:"); 
            mvprintw(1, 9, std::to_string(MAP::gained_points).c_str()); 
            mvprintw(2, 1, "Level: 0   ");

            if (ENTITY::paused == false) { // From here I immitate the famous 256 level bug by...
                for (USI i=0; i<s; i++) {
                    if (i == highlight) {
                        attron(COLOR_PAIR(3));
                    }
                    mvprintw(5+i, 2, "                 ");
                    attroff(COLOR_PAIR(3));
                }
                enum map_cell_state {O, U, C, B, L, E}; 
                USI cell_color[11] = {1, 2, 3, 4, 10, 11, 12, 13, 14, 15, 16};
                std::string sym[10] = {" ", "★", "☹︎", "☺︎", "v", ">", "<", "^", "❤︎", "♣︎"};
                for (USI y=0; y<game_map.ySize; y++) {
                    for (USI x=0; x<game_map.xSize; x++) {
                        USI c = cell_color[rand() % (sizeof(cell_color)/sizeof(cell_color[0]))]; //Randomly selecting, randomly coloring and randomly rendering objects on the field
                        map_cell_state cell = static_cast<map_cell_state>(rand() % E);
                        std::string s = sym[rand() % (sizeof(sym)/sizeof(sym[0]))];
                        attron(COLOR_PAIR(c));
                        switch (cell) {
                            case O:
                                mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), s.c_str());    
                                break;
                            
                            case U:
                                mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), ".");
                                break;
                            
                            case C:
                                mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), " ");
                                break;
                            
                            case B:
                                mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "★");
                                break;
                            case L:
                                mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "❤︎");
                                break;
                            case E:
                                mvprintw(y+3, x+(GAME_WIDTH/2)-(GAME_WIDTH/8), "☘︎");
                                break;
                        }
                        attroff(COLOR_PAIR(c));
                    }
                }

                // No wait for input
                cbreak(); // Controls, the same as above, except no pacman controls (no sense of using it)
                switch (getch()) {
                    case 'e':
                    case 'E':
                        ENTITY::paused = true;
                        break;
                }
                usleep(300000); // FPS set 
            } else { //The same pause menu controls as above
                for (USI i=0; i<s; i++) {
                    if (i == highlight) {
                        attron(A_REVERSE);
                    }
                    mvprintw(5+i, 2, SCENE::Scene::game_pause_selection[i].c_str());
                    attroff(A_REVERSE);
                }

                switch (getch()) { 
                    case 'w': // Upper pause menu selection
                    case 'W':
                        highlight--;
                        if (highlight < 0) {
                            highlight = s-1;
                        }
                        break;

                    case 's':
                    case 'S': // Lower pause menu selection 
                        highlight++;
                        if (highlight == s) {
                            highlight = 0;
                        }
                        break;

                    case 'i':
                    case 'I':
                        switch (highlight) {
                            case 0: // Continue
                                ENTITY::paused = false;
                                break;
                            
                            case 1: { // New Game
                                ENTITY::paused = false;
                                ENTITY::iterated = true;
                                ENTITY::lost = false;
                                for (auto & elem : entities) {
                                    delete elem;
                                }
                                SCENE::Scene::Exit(game_window, false);
                                SCENE::Game game_win;
                                game_win.Load();
                                break;
                            }
                            
                            case 2: { // Exit the scene
                                for (auto & elem : entities) {
                                    delete elem;
                                }
                                SCENE::Scene::Exit(game_window, false);
                                SCENE::MainMenu menu_win;
                                menu_win.Load();
                                break;
                            }
                        }
                        break;
                }
            }
        }
        iterated = true;
        refresh();
    }
}
