#include "kernel.h"

void SCENE::Scene::Exit(WINDOW * window, bool em) { //Function used to close the game/window
    wrefresh(window);
    delwin(window);
    endwin();
    if (em == true) { // Closes the game entirely if bool is true
        exit(true);
    }
}