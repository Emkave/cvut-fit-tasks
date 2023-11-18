#include "kernel.h"

void SCENE::MainMenu::Load(void) { // Main Menu loading
    ENTITY::lost = false;
    ENTITY::paused = false;
    ENTITY::iterated = true;
    setlocale(LC_CTYPE, ""); // Standard initialization
    initscr();//
    noecho();
    clear();
    curs_set(0);
    nodelay(stdscr, TRUE);

    start_color(); // Initialization of colors 

    init_pair(1, COLOR_GREEN, COLOR_BLACK); // Color palettes // Easy
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // Medium
    init_pair(3, COLOR_RED, COLOR_BLACK); // Hardd

    unsigned short int MENU_WIDTH = 0;
    unsigned short int MENU_HEIGHT = 0; // Window preferences
    int highlight = 0;
    double MENU_WIDTH_SHIFT = 0;
    double MENU_HEIGHT_SHIFT = 0;
    MAP::level = std::get<0>(get_player_stats());
    MAP::gained_points = std::get<1>(get_player_stats());

    getmaxyx(stdscr, MENU_HEIGHT, MENU_WIDTH); // Get window size
    if (MENU_HEIGHT < 27 || MENU_WIDTH < 100) { // Check the window size
        endwin();
        std::cout << std::endl << std::endl << "[✘] Invalid Window Size, should be at least 100x28!" << std::endl;
        exit(true);
    }

    WINDOW * MAIN_MENU = newwin(MENU_HEIGHT/2, MENU_WIDTH/2, MENU_HEIGHT/4, MENU_WIDTH/4); // Create the window
    box(MAIN_MENU, 0, 0); // Initialization of the window
    refresh();
    wrefresh(MAIN_MENU);

    MENU_HEIGHT_SHIFT = MENU_HEIGHT/5.3;
    MENU_WIDTH_SHIFT = MENU_WIDTH/5.3;

    switch (ENTITY::difficulty) { // COlor the difficulty text accroding its value
        case 0:
            wattron(MAIN_MENU, COLOR_PAIR(1)); // green
            break;
        
        case 1:
            wattron(MAIN_MENU, COLOR_PAIR(2)); // yellow
            break;

        case 2:
            wattron(MAIN_MENU, COLOR_PAIR(3)); // red
            break;
    }

    mvwprintw(MAIN_MENU, 3, 13, SCENE::Scene::selectDiff[ENTITY::difficulty].c_str());
    switch (ENTITY::difficulty) {
        case 0:
            wattroff(MAIN_MENU, COLOR_PAIR(1));
            break;
        
        case 1:
            wattroff(MAIN_MENU, COLOR_PAIR(2));
            break;

        case 2:
            wattroff(MAIN_MENU, COLOR_PAIR(3));
            break;
    }

    USI i=1;

    if (MAP::level > 1) { // If not new game, add Continue button
        i = 0;
        highlight = 0;
    } else if (MAP::level == 1) {
        i = 1;
        highlight = 1;
    }

    USI s = sizeof(SCENE::Scene::windowSelection)/sizeof(SCENE::Scene::windowSelection[0]);

    while (true) { // Render the main menu attributes
        mvwprintw(MAIN_MENU, 0, 1, "Pacman: CVUT Edition"); // print game chars
        mvwprintw(MAIN_MENU, 1, 1, "High score:");
        mvwprintw(MAIN_MENU, 1, 13, std::to_string(MAP::gained_points).c_str());
        mvwprintw(MAIN_MENU, 2, 1, "High level:");
        mvwprintw(MAIN_MENU, 4, 1, "Press M for manual.");
        if (MAP::level == 256) 
            mvwprintw(MAIN_MENU, 2, 13, "0   ");
        else mvwprintw(MAIN_MENU, 2, 13, std::to_string(MAP::level).c_str());
        mvwprintw(MAIN_MENU, 3, 1, "Difficulty:");
        if (MAP::level > 1) {
            i = 0;
        } else if (MAP::level == 1) {
            i = 1;
        }
        while (i<s) { // Highlight algorithm
            if (i == highlight) {
                wattron(MAIN_MENU, A_REVERSE);
            }
            mvwprintw(MAIN_MENU, MENU_HEIGHT_SHIFT+i, MENU_WIDTH_SHIFT, SCENE::Scene::windowSelection[i].c_str());
            wattroff(MAIN_MENU, A_REVERSE);
            i++;
        }

        switch (wgetch(MAIN_MENU)) { // COntrols 
            case 'w':
            case 'W':
                highlight--; // Select upper
                if (MAP::level == 1) {
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
                if (MAP::level == 1) {
                    if (highlight == s) {
                        highlight = 1;
                    }
                } else {
                    if (highlight == s) {
                        highlight = 0;
                    }
                }
                break;
            
            case 'm':
            case 'M': {
                SCENE::Manual manual_engine;
                SCENE::Scene::Exit(MAIN_MENU, false);
                manual_engine.Load();
                break;
            }
            
            case '=':
                MAP::level = 256;
                break;

            case 'i':
            case 'I':
                switch (highlight) { // Press I as Enter_key and check what has been pressed
                    case 0: { // Continue selected
                        SCENE::Scene::Exit(MAIN_MENU, false);
                        SCENE::Game game;
                        game.Load();
                        break;
                    }

                    case 1: { // New Game selected
                        MAP::gained_points = 0;
                        MAP::level = 1;
                        Save(MAP::level, MAP::gained_points);
                        SCENE::Scene::Exit(MAIN_MENU, false);
                        SCENE::Game game;
                        game.Load();
                        break;
                    }

                    case 2: { // Options
                        SCENE::Options options_engine;
                        SCENE::Scene::Exit(MAIN_MENU, false);
                        options_engine.Load();
                        break;
                    }

                    case 3: { // Manuals selected 
                        SCENE::Manual manual_engine;
                        SCENE::Scene::Exit(MAIN_MENU, false);
                        manual_engine.Load();
                        break;
                    }

                    case 4: // Exit selected 
                        SCENE::Scene::Exit(MAIN_MENU, true);
                        break;
                }
                break;
        }
    }
}
