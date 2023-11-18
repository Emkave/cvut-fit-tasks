#ifndef data_h
#define data_h
#include "kernel.h"
std::tuple<USI, USI> get_player_stats(); // Get player stats
std::tuple<USI, USI, USI, bool, char, std::string> get_entity_stats(std::string entity);  // Get entity stats
void Save(USI level, USI points); // Save data 
#endif //dataWithdraw_h
