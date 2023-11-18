#include "kernel.h"

void SCENE::Manual::Load(void) { // Loading the manual scene
    setlocale(LC_CTYPE, ""); // Standart initialization
    initscr();
    noecho();
    clear();
    curs_set(0);
    nodelay(stdscr, TRUE);

    USI manual_height = 0;
    USI manual_width = 0;

    getmaxyx(stdscr, manual_height, manual_width); // Getting the window size
    WINDOW * manual_window = newwin(0, 0, 0, 0); // Creating the window
    box(manual_window, 0, 0);
    refresh();
    wrefresh(manual_window);

    while (true) {
        mvprintw(manual_height/manual_height, 2, "Pacman: CVUT Edition Manuals"); // Printing the manuals...
        mvprintw(4, 2, "♦︎ 'W' or 'w' to select upper or move up.");
        mvprintw(6, 2, "♦︎ 'S' or 's' to select lower or move down.");
        mvprintw(8, 2, "♦︎ 'D' or 'd' to move right.");
        mvprintw(10, 2, "♦︎ 'A' or 'a' to move left.");
        mvprintw(12, 2, "♦︎ 'I' or 'i' to enter [behaves as ENTER key].");
        mvprintw(14, 2, "♦︎ 'E' or 'e' to escape [behaves as ESC key].");
        mvprintw(16, 2, "♦︎ Selecting New Game will reset the player's stats!");
        mvprintw(manual_height-2, 2, "Press 'e' to close the manuals.");

        switch(getch()) { // untill escape key is pressed
            case 'e':
            case 'E':
                SCENE::Manual::Exit(manual_window, false);
                SCENE::MainMenu menu;
                menu.Load();
                break;
        }
    }
}
