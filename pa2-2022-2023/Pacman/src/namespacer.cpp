#include "kernel.h" // Including the core

namespace ENTITY {  // Variables responsible for appropriate render and game contorols 
    USI lives; // Lives available
    bool lost = false; // Is lost
    bool paused = false; // Is paused
    bool iterated = true; // Is iterated
    bool won = false; // Is won
    USI entity_clock_tick = 1; // Ingame tick clock
}

namespace MAP { //Variables responsible for player stats 
    unsigned short int level = std::get<0>(get_player_stats()); // Get level from file
    unsigned short int gained_points = std::get<1>(get_player_stats()); // get points from file
}


