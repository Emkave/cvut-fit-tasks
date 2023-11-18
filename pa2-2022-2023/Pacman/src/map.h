#ifndef map_h
#define map_h
#include "kernel.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <deque>
#include <ncurses.h>
#include <curses.h>
#include <vector>

typedef unsigned short int USI;
typedef unsigned int UI;

namespace MAP { // Wrapping the class and its attributes into a namespace
    extern unsigned short int level;
    extern unsigned short int gained_points;
    class Map {
        public:
            enum map_cell_state {O, U, C, B, E, L}; // Enum used to render features on the map
            unsigned short int total_points = 0; // Total points to collect
            unsigned short int xSize = 0; // Size of map in x-axis
            unsigned short int ySize = 0;   // Size of map in y-axis
            bool on_borderline = false; //Pacman on the map borderline
            std::deque<std::deque<bool>> map_skeleton; // Boolean map (required for BFS search)
            std::deque<std::deque<MAP::Map::map_cell_state>> map_structs; // Enum map
            std::deque<std::pair<USI, USI>> free_poses; // Needs for random generator of bonuses 
            std::vector<std::vector<bool>> bmap; // Needs for BFS search
            std::string available = "0123";

        public:
            Map() { // Initialization of the map and its properties
                std::fstream file("map.txt", std::fstream::in); // Open the file responsible for the map
                if (!file.is_open()) { // Check if the file has opened successfully
                    throw std::invalid_argument("Could not open the file."); //If true, throw an exception
                } else {
                    std::deque<MAP::Map::map_cell_state> subS; // Needs to emplace into the vector of vector of types
                    std::deque<bool> subB;
                    std::string line;
                    USI x = 0;
                    USI y = 0;
                    char c;

                    while (file.get(c)) { // Reading the character from file and fulfilling the map
                        if (c == 'O') {
                            subB.emplace_back(true);
                            subS.emplace_back(O);
                            x++;
                        } else if (c == '\n') {
                            map_skeleton.emplace_back(subB);
                            subB.clear();
                            map_structs.emplace_back(subS);
                            subS.clear();
                            y++;
                            x=0;
                        } else if (c == 'U' || c == 'C' || c == 'B') {
                            free_poses.emplace_back(y, x);
                            subB.emplace_back(false);
                            x++;
                            switch (c) {
                                case 'U':
                                    subS.emplace_back(U);
                                    this->total_points++; // Number of points to collect when U is encountered
                                    break;
                                
                                case 'C':
                                    subS.emplace_back(C);//
                                    break;
                                
                                case 'B':
                                    subS.emplace_back(B);
                                    break;
                            }
                        }
                    }
                    subS.clear();
                    subB.clear();

                    this->xSize = map_skeleton[0].size();  // Filling the gaps for BFS
                    this->ySize = map_skeleton.size();
                    this->map_skeleton[(int)ySize/2][0] = 1;
                    this->map_skeleton[(int)ySize/2][xSize-1] = 1;
                    this->map_skeleton[0][(int)xSize/2] = 1;
                    this->map_skeleton[ySize-1][(int)xSize/2] = 1;   
                    
                    file.close(); //Close the file after finishing the reading     

                    std::vector<bool> bline; // Needs for BFS

                    for (UI y=0; y<this->ySize*this->ySize; y++) { // Expanding the field in square 
                        for (UI x=0; x<this->xSize*this->xSize; x++) {
                            bline.emplace_back(false);
                        }
                        bmap.emplace_back(bline);
                        bline.clear();
                    }

                    // Here we use the tactic of graph adjacency matrix
                    // We fill the matrix of x^2 to y^2 size representing the relationship between an one cell with all the other cells
                    // We create the following relationship for not obstacle.
                    // Here we have to check if there is not an obstacle in the cell and the destination cell.
                    for (UI y1=0; y1<this->ySize; y1++) { // Filling the field for BFS with true or false
                        for (UI x1=0; x1<this->xSize; x1++) {
                            if (this->map_skeleton[y1][x1] != true) {
                                if (y1 > 0 && this->map_skeleton[y1-1][x1] != true) { // Up
                                    UI skeleton_cell1 = y1*this->map_skeleton[0].size()+x1; // Source cell
                                    UI skeleton_cell2 = (y1-1)*this->map_skeleton[0].size()+x1; // Destination cell
                                    this->bmap[skeleton_cell1][skeleton_cell2] = true;
                                    this->bmap[skeleton_cell2][skeleton_cell1] = true;
                                }

                                if (x1+1 < map_skeleton[0].size() && this->map_skeleton[y1][x1+1] != true) { // Right
                                    UI skeleton_cell1 = y1*this->map_skeleton[0].size()+x1; // Source cell
                                    UI skeleton_cell2 = y1*this->map_skeleton[0].size()+x1+1; // destination cell
                                    this->bmap[skeleton_cell1][skeleton_cell2] = true;
                                    this->bmap[skeleton_cell2][skeleton_cell1] = true;
                                }

                                if (y1+1 < map_skeleton.size() && this->map_skeleton[y1+1][x1] != true) { // Down
                                    UI skeleton_cell1 = y1*this->map_skeleton[0].size()+x1; // Source cell
                                    UI skeleton_cell2 = (y1+1)*this->map_skeleton[0].size()+x1; // Destination cell
                                    this->bmap[skeleton_cell1][skeleton_cell2] = true;
                                    this->bmap[skeleton_cell2][skeleton_cell1] = true;
                                }

                                if (x1 > 0 && this->map_skeleton[y1][x1-1] != true) { // Left
                                    UI skeleton_cell1 = y1*this->map_skeleton[0].size()+x1; // Source cell
                                    UI skeleton_cell2 = y1*this->map_skeleton[0].size()+(x1-1); // destination cell
                                    this->bmap[skeleton_cell1][skeleton_cell2] = true;
                                    this->bmap[skeleton_cell2][skeleton_cell1] = true;
                                }
                            }
                        }
                    }
                }
            }

            ~Map() { //Destroying the object
                map_structs.clear();
                map_skeleton.clear();
                free_poses.clear();
            }
    };
 
}

#endif // map_h