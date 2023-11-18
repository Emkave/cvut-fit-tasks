#ifndef entity_h
#define entity_h
#include "kernel.h"
#include <tuple>

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
            virtual void Move(MAP::Map & map, std::vector<ENTITY::Entity *> & entities) = 0;
            virtual ~Entity() = default;
    };

    class Pacman : public Entity {
        public:
            void Move(MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Pacman() override = default;
    };

    class Blinky : public Entity {
        public:
            void Move(MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Blinky() override = default;
    };

    class Pinky : public Entity {
        public:
            void Move(MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Pinky() override = default;
    };

    class Inky : public Entity {
        public:
            void Move(MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Inky() override = default;
    };

    class Clyde : public Entity {
        public:
            void Move(MAP::Map & map, std::vector<ENTITY::Entity *> & entities) override;
            ~Clyde() override = default;
    };
}


#endif // entity_h