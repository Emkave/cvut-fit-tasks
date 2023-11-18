## Pacman: CVUT Edition

_**Pac-Man**_ is a 1980 maze action video game developed and released by Namco for arcades. The original Japanese title of _**Puck Man**_ was changed to _Pac-Man_ for international releases as a preventative measure against the defacement of the arcade machines by changing the P to an F. In North America, the game was released by Midway Manufacturing as part of its licensing agreement with Namco America. The player controls Pac-Man, who must eat all the dots inside an enclosed maze while avoiding four colored ghosts. Eating large flashing dots called "Power Pellets" causes the ghosts to temporarily turn blue, allowing Pac-Man to eat them for bonus points.

Below you will see the description of the project realization. 

---

## Description

The project is a game. And every game has windows. 

**The game has:**

*   Main Menu
*   Options
*   Manuals
*   Game

These windows are in close interaction with each other. Therefore, a graph is required to illustrate the relationships between the windows. 

![](https://33333.cdn.cke-cs.com/kSW7V9NHUXugvhoQeFaf/images/1676ccc2031e6203c25403b76ef32cd95152fbc99cb29bd0.png)

The user starts the program and gets immediately into the **main menu** window, from where the user is able to select multiple windows, such as Options, Manual, and Game, or close the game.

---

## Main Menu

The user starts the program and gets immediately into the **main menu** window, from where the user is able to select multiple windows, such as **Options**, **Manual**, and **Game**, or **Exit** the game.

---

## Game

If the user selects to play the game, a window with the **map**, entities with **preferences,** and user **stats** will appear on the window. 

---

## Pause

At any point, the user is able to pop up the pause menu to continue, start a new game, or exit the current game to the **main menu**. 

---

## Options

From the **main menu**, the user is also able to go to **Options** to switch the game **difficulty** on his own. The difficulty chosen will not only affect **entities** but the user too. 

---

## Manuals

The **main menu** also provides the user with the **manuals** of the game. The user is able to access them also by pressing the ‘m’ button. ‘m’ stands for **manual**. 

---

## In-Game Process

Depending on the difficulty, the user will face different obstacles while playing to handle. The game is also provided with the RNG of bonuses that the user is able to use in order to survive. The entities are also provided with good AI. The difficulty of the game decides the number of enemies using AI. However, it also affects the number of lives the user has. 

*   **Easy** mode provides the user with 1 enemy with high AI and 5 lives. 
*   **Medium** mode provides the user with 2 enemies having high AI and 3 lives.
*   **Hard** mode provides the user with 2 enemies having high AI and 1 live.

Enemies that have no high **AI** will use a **standard movement algorithm**. 

The RNG will generate random bonuses that the user can receive. 

*   A **leaf** will give a user bonus of 200 points.
*   A **heart** will give a user +1 life.

There are, however, bonuses that cannot be retrieved again. For example, a **star,** that puts enemies in defensive mode and is edible by the user. For killing one of the enemies, the user is given a bonus of 600 points. 

The enemy is then teleported to the center of the map, where it becomes in offensive mode (chases the user). That is not easy though, because, in defense mode, enemies will run at the center of the map to re-state themselves back to the offensive mode. 

After collecting all the points in the game, the pause menu will pop up, the level will increase, and new user data will be saved.

### Continue

If the user decides to **continue,** the **new game** appears with all **scores and levels saved**. 

### New Game

If the user decides to **play again**, the **new game** appears with all **scores and levels reset as the player never passed this level.**

### Main Menu

If the user decides to **return to the main menu**, the game will close, the **data will be saved**, the **menu will open** and the cycle may start again. 

---

## Polymorphism

The project has a polymorphism. This solution was decided because it helps to re-use the same code multiple times, i.e., classes once written, tested, and implemented can be reused as required. Saves a lot of time. A single variable can be used to store multiple data types. It is easy to debug the code too.

Entities and their classes support the polymorphism. 

```cpp
    class Entity {
        public:
            std::tuple<USI, USI, USI, USI, char, std::string> chrs; 
            virtual void Move(WINDOW * window, USI & x, USI & y, USI & dir, MAP::Map & map, std::vector<ENTITY::Entity *> & entities) = 0;
            virtual ~Entity() = default;
    };

    class Pacman : public Entity {
        public:
            void Move(WINDOW * window, USI & x, USI & y, USI & dir, MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Pacman() override = default;
    };

    class Blinky : public Entity {
        public:
            void Move(WINDOW * window, USI & x, USI & y, USI & dir, MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Blinky() override = default;
    };

    class Pinky : public Entity {
        public:
            void Move(WINDOW * window, USI & x, USI & y, USI & dir, MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Pinky() override = default;
    };

    class Inky : public Entity {
        public:
            void Move(WINDOW * window, USI & x, USI & y, USI & dir, MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Inky() override = default;
    };

    class Clyde : public Entity {
        public:
            void Move(WINDOW * window, USI & x, USI & y, USI & dir, MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Clyde() override = default;
    };
```

---

## BFS

```cpp
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
        for (int i=0; i<map.bmap[v].size(); i++) {
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
```

---

## Standard Algorithm

*   **Standard**

```cpp
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
```

*   **Standard Blinky Algorithm**

```cpp
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
```

*   **Standard Clyde Algorithm**

```cpp
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
```

---

## Map Generation

### Warning!!! How to configure the map…

The map is taken from file map.txt.

In order to configure the map correctly, follow these rules please:

*   The map has to be exactly in square shape and in odd number sizes, i.e. 5x5, 7x7, 9x9!
*   There should be a way to the center of the map!
*   The teleportation gaps have to be at the center of each polygon of the map!
*   Use ‘U’ to mark a way with a score!
*   Use ‘C’ to mark a way without a score!
*   Use ‘B’ to mark a bonus on the map!
*   Use ‘O’ to mark an obstacle on the map!
*   There are only 4 teleportation gaps on the map! Extra gaps may lead to corruption!
*   After changing the map, please, change the preferences of entities to spawn in preferences.txt file!
    *   The configuration template is: (PLEASE, DO NOT ADD ANY WHITESPACES!!!!!!) 
        *   Name (std::string)
        *   x position (unsigned short int)
        *   y position (unsigned short int)
        *   is offensive (bool)
        *   $END (to end its preferences list)

#### Preferences would look like:

```plaintext
pacman
8
9
2
true
$END

blinky
7
5
2
true
$END

inky
9
5
2
true
$END

pinky
7
10
1
true
$END

clyde
9
10
3
true
$END
```

The text below is the representation of the standard map. The size of the map is 17x17.

You are able only modify the inner part of the map, not the outer layer!!! 

Do not add any white spaces!!!

Please, keep the newline at the bottom of txt file!

```plaintext
OOOOOOOOUOOOOOOOO
OBUUUUOUUUOUUUUBO
OUOOOUOUUUOUOOOUO
OUOUUUUOUOUUUUOUO
OUOUOUUUUUUUOUOUO
OUUUUUOUOUOUUUUUO
OUOUUUUOCOUUUUOUO
OUOUOUOOCOOUOUOUO
UUUUOUUUCUUUOUUUU
OUOUOUOOCOOUOUOUO
OUOUUUUUOUUUUUOUO
OUUUUOOUUUOOUUUUO
OUOUUUUOUOUUUUOUO
OUOUUUUUUUUUUUOUO
OUOOOUOOOOOUOOOUO
OBUUUUUUUUUUUUUBO
OOOOOOOOUOOOOOOOO
```

The map below is 7x7 is also available. 

```plaintext
OOOUOOO
OUUUUUO
OUUUUUO
UUUUUUU
OUUUUUO
OUUUUUO
OOOUOOO
```

The image below is the representation of the map.txt in the game with the entity preferences from preferences.txt.

![](https://33333.cdn.cke-cs.com/kSW7V9NHUXugvhoQeFaf/images/f1808ec068b6fb97cc435d76d0dd899518366059aa0fde95.png)

The code below is how the map is read and rendered. 

```cpp
namespace MAP { // Wrapping the class and its attributes into a namespace
    extern unsigned short int level;
    extern unsigned short int gained_points;
    class Map {
        public:
            enum map_cell_state {O, U, C, B, E, L}; // Enum used to render features on the map
            unsigned short int total_points = 0; // Total points to collect
            unsigned short int xSize = 0; // Size of map in x-axis
            unsigned short int ySize = 0;   // Size of map in y-axis
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
```

---

## Kernel

Kernel is a reference to the kernel.h file that handles most of the program's job in terms of libraries and window renderings. 

```cpp
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

typedef unsigned int UI;    //Simplification for fingers 
typedef unsigned short int USI;
typedef short int SI;

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
```

---

## Data

#### Preferences

*   The system gets the entity and the user's preferences in order to render them accordingly on the game map.

```cpp
#include "kernel.h"

std::tuple<USI, USI> get_player_stats() {
    USI level; USI score;
    std::fstream stats("stats.txt", std::fstream::in);
    if (!stats.is_open()) { // Check if the file has opened successfully
        throw std::invalid_argument("Could not open the file 'stats.txt'."); //If true, throw an exception
    } else {
        std::string line;
        USI i=0;
        while (std::getline(stats, line)) { //Reading and spreading the data correctly
            if (i==0) {
                level = std::stoi(line); // Level
            } else if (i==1) {
                score = std::stoi(line); // Score
            }
            i++;
        }
        stats.close();
        return std::make_tuple(level, score); // Return data received
    }
}

std::tuple<USI, USI, USI, bool, char, std::string> get_entity_stats(std::string entity) {
    USI x; USI y; USI dir; bool off; int dir_c; std::string name; // Chars to return 
    std::fstream preferences("preferences.txt", std::fstream::in);
    if (!preferences.is_open()) { // Check if the file has opened successfully
        throw std::invalid_argument("Could not open the file 'preferences.txt'"); //If true, throw an exception
    } else {
        std::string line;
        USI i=0;
        bool todo = false;
        while (std::getline(preferences, line)) {
            if (line == entity || todo) {
                todo = true;
                switch (i) {
                    case 0:
                        name = line; // Get name
                        break;
                    case 1:
                        x = std::stoi(line); // Get x coordinate
                        break;
                    case 2:
                        y = std::stoi(line); // Get y coordinate
                        break;
                    case 3:
                        dir = std::stoi(line); // Get direction
                        break;
                    case 4:
                        if (line == "true") off = true; // Get offensive 
                        else off = false;
                        break;
                }
                if (line == "$END") {
                    break;
                }
                i++;
            }
        }
        preferences.close();
        dir_c = dir + '0';
        return std::make_tuple(x, y, dir, off, dir_c, name);
    }
}
```

#### Saving

*   The system is able to save the user's data after each game won.

The code below is the continuation of the code avove. 

```cpp
void Save(USI level, USI points) {
    std::ofstream stats("stats.txt");
    if (!stats.is_open()) { // Check if the file has opened successfully
        throw std::invalid_argument("Could not open the file 'stats.txt'."); //If true, throw an exception
    } else {
        stats << level << std::endl << points;
        stats.close();
    }
}
```

---

## Entities

An entity has its x,y coordinates, its direction, offensive, defensive state and its name.

Each entity has one virtual function that executes different tasks depending on the game difficulty and the entity.

```cpp
#ifndef entity_h
#define entity_h
#include "kernel.h"

typedef unsigned short int USI;

namespace ENTITY {
    extern USI difficulty;
    extern USI immunity_clock;
    extern USI entity_clock_tick;
    extern USI collided;
    extern USI lives;
    extern bool paused;
    extern bool lost; //
    extern bool iterated;
    extern bool won;

    class Entity {
        public:
            std::tuple<USI, USI, USI, USI, char, std::string> chrs; 
            virtual void Move(WINDOW * window, USI & x, USI & y, USI & dir, MAP::Map & map, std::vector<ENTITY::Entity *> & entities) = 0;
            virtual ~Entity() = default;
    };

    class Pacman : public Entity {
        public:
            void Move(WINDOW * window, USI & x, USI & y, USI & dir, MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Pacman() override = default;
    };

    class Blinky : public Entity {
        public:
            void Move(WINDOW * window, USI & x, USI & y, USI & dir, MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Blinky() override = default;
    };

    class Pinky : public Entity {
        public:
            void Move(WINDOW * window, USI & x, USI & y, USI & dir, MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Pinky() override = default;
    };

    class Inky : public Entity {
        public:
            void Move(WINDOW * window, USI & x, USI & y, USI & dir, MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Inky() override = default;
    };

    class Clyde : public Entity {
        public:
            void Move(WINDOW * window, USI & x, USI & y, USI & dir, MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Clyde() override = default;
    };
}


#endif // entity_h
```