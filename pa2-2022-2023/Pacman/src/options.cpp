#include "kernel.h"

void SCENE::Options::Load(void) {   // Options menu
    setlocale(LC_CTYPE, "");    //Standard intialization of a window
    initscr();
    noecho();
    clear();
    curs_set(0);
    nodelay(stdscr, TRUE);

    start_color(); //Creating color combinations
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_WHITE);

    unsigned short int OPTIONS_WIDTH = 0;
    unsigned short int OPTIONS_HEIGHT = 0;  //Window preferences
    short int highlight = 0;
    double OPTIONS_WIDTH_SHIFT = 0;
    double OPTIONS_HEIGHT_SHIFT = 0;

    getmaxyx(stdscr, OPTIONS_HEIGHT, OPTIONS_WIDTH);    // Getting the size of the screen
    WINDOW * options = newwin(OPTIONS_HEIGHT/2, OPTIONS_WIDTH/2, OPTIONS_HEIGHT/4, OPTIONS_WIDTH/4);   // Creating the window

    wattron(options, COLOR_PAIR(3));
    box(options, 0, 0); //Creating a window
    wattroff(options, COLOR_PAIR(3));
    refresh();
    wrefresh(options);
    
    OPTIONS_HEIGHT_SHIFT = OPTIONS_HEIGHT/5.3; //Trying make it symmetrical 
    OPTIONS_WIDTH_SHIFT = OPTIONS_WIDTH/5.3;

    mvwprintw(options, 0, OPTIONS_HEIGHT_SHIFT-5, "Pacman: CVUT Edition");  //Title
    mvwprintw(options, 1, 1, "Select Difficulty:"); //Purpose of window

    unsigned short int s = sizeof(SCENE::Scene::selectDiff)/sizeof(SCENE::Scene::selectDiff[0]); //Num of choices

    while (true) { // Initialization of the scene
        for (USI i=0; i<s; i++) { //Highlight loop (highlights texts imitating buttons)
            if (i == highlight) {
                wattron(options, COLOR_PAIR(2));
            }
            if (i == 3) {
                mvwprintw(options, OPTIONS_HEIGHT/2-2, OPTIONS_WIDTH_SHIFT-18, SCENE::Scene::selectDiff[i].c_str());
            } else {
                mvwprintw(options, OPTIONS_HEIGHT_SHIFT+(i+2), OPTIONS_WIDTH_SHIFT, SCENE::Scene::selectDiff[i].c_str());
            }
            if (ENTITY::difficulty == i && i == highlight) { // Highligh the button differently, because of double highlight
                wattron(options, COLOR_PAIR(4));
                mvwprintw(options, OPTIONS_HEIGHT_SHIFT+(i+2), OPTIONS_WIDTH_SHIFT, SCENE::Scene::selectDiff[i].c_str());
                wattroff(options, COLOR_PAIR(4));
            } else if (ENTITY::difficulty == i) {
                wattron(options, COLOR_PAIR(1));
                mvwprintw(options, OPTIONS_HEIGHT_SHIFT+(i+2), OPTIONS_WIDTH_SHIFT, SCENE::Scene::selectDiff[i].c_str());
                wattroff(options, COLOR_PAIR(1));
            }
            wattroff(options, COLOR_PAIR(2));
        }

        switch (wgetch(options)) { //Controls 
            case 'w':
            case 'W':
                highlight--; //Select upper (decrement the var to highlight the upper text in the next iterations, the same rule applyes for below cases)
                if (highlight < 0) {
                    highlight = s-1;   
                }
                break;
            
            case 's':
            case 'S':
                highlight++; //Select lower
                if (highlight == s) {
                    highlight = 0;
                }
                break;
            
            case 'i':
            case 'I': // Confirm choice (read as input) (behaves like ENTER key)
                switch (highlight) { 
                    case 0:
                        ENTITY::difficulty = 0; // Easy
                        break;
                    
                    case 1:
                        ENTITY::difficulty = 1; // Medium
                        break;
                    
                    case 2:
                        ENTITY::difficulty = 2; // Hard
                        break;
                    
                    case 3:
                        SCENE::Options::Back(options);
                        break;
                }
                break;
        }
    }
}

void SCENE::Options::Back(WINDOW * window) {    // Switches back to the main menu window
    SCENE::Options::Exit(window, false);
    SCENE::MainMenu menu;
    menu.Load();
}
