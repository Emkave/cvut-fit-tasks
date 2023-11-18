#ifndef kernel_h
#define kernel_h
#include "map.h"    // Libraries added
#include "entity.h"
#include "data.h"
#include <string>
#include <ncurses.h>
#include <curses.h>
#include <iomanip>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <deque>
#include <set>
#include <map>
#include <vector>
#include <queue>
#include <memory>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include <utility>
#include <sstream>
#include <fstream>
#include <random>
#include <tuple>

typedef unsigned int UI;    //Simplification for fingers 
typedef unsigned short int USI;
typedef short int SI;
typedef unsigned long int ULI;

#define CHECKPOINT {std::cout << "CHECKPOINT." << std::endl;} // Used as a debugger (mine is not working because of M1 architecture)

namespace SCENE {           // Wrapping the scenes 
    class Scene {
        protected:
            const std::string windowSelection[5] = {
                "❏ Continue", "❏ New Game", "❏ Options", "❏ Manual", "❏ Exit"  //Main Menu selections
            };

            const std::string game_pause_selection[3] = {
                "➤ Continue", "➤ New Game", "➤ Back to Menu"         // Pause Menu selections
            };

            const std::string selectDiff[4] = {     // Difficulty selection
                "♠︎ Easy", "♣︎ Medium", "♦︎ Hard", "❏ Back"
            };

        public:
            void Exit(WINDOW * window, bool em);   //EXIT
            void Load();        //Load a window
    };

    class Game : public Scene {     // Class of the Game Scene
        public: void Load();
    }; 

    class Options : public Scene {  // Class of the Options Scene
        public:
            void Load();
            void Back(WINDOW * window);
    };

    class MainMenu : public Scene {  // Class of the Main Menu Scene
        public: void Load();
    };

    class Manual : public Scene {   // Class of the Manual Scene
        public: void Load();
    };
}

#endif /* kernel_h */