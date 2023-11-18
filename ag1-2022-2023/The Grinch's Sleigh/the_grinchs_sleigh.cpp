#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>

using Place = size_t;

struct Map {
    size_t places; // Number of neighbor nodes
    Place start, end; // Id of start and finish
    std::vector<std::pair<Place, Place>> connections; // Set pairs of nodes - set of edges
    std::vector<std::vector<Place>> items; // 13
};

template <typename F, typename S> struct std::hash<std::pair<F, S>> {
    std::size_t operator () (const std::pair<F, S> &p) const noexcept {
        // something like boost::combine would be much better
        return std::hash<F>()(p.first) ^ (std::hash<S>()(p.second) << 1);
    }
};

#endif

using Item = std::int16_t;
#define adjacent_room_index adjacent_room_index

typedef struct back_track { 
    Place room_number = 0; // Room number
    Item bitconfig = 0; // Bit version of an item (idk why I put it here but let it be)
    std::shared_ptr<back_track> parent = nullptr; // pointer to dump parent node
    back_track(std::shared_ptr<back_track> parent, Place room_number, Item bitconfig) :  room_number(room_number), bitconfig(bitconfig), parent(parent) {} //frckn constructor kill me already
} back_track;

std::unordered_map<Place, std::vector<Place>> make_adjacency(const Map & map) {
    std::unordered_map<Place, std::vector<Place>> to_return = {};

    for (size_t i=0; i<map.connections.size(); i++) {               
        to_return[map.connections[i].first].push_back(map.connections[i].second); // init-fill key       
        to_return[map.connections[i].second].push_back(map.connections[i].first); // init-fill key
    }
    
    return to_return;
}

bool check_all(const Map & map) noexcept {
    if (map.connections.size() == 0) // check if we have any connections
        return true;
    else
        for (const auto & elem : map.items) // check ifwe have isolated item
            if (elem.size() == 0) 
                return true;
    return false;
}

bool check_all(const Map & map, const std::unordered_map<Place, std::vector<Place>> & adj_list) noexcept {
    if (map.start != map.end && adj_list.find(map.end) == adj_list.end()) 
        return true;          // searching if end is isolated
    else 
        if (map.start != map.end && adj_list.find(map.start) == adj_list.end())
            return true; // Checking if start is isolated
    return false;
}

std::vector<Item> make_combo_items(const Map & map) {
    std::vector<Item> to_return (map.places);
    for (size_t i=0; i<map.items.size(); i++) {
        for (size_t j=0; j<map.items[i].size(); j++) {
            to_return[map.items[i][j]] |= (1 << i);
        }
    }
    return to_return;
}

std::list<Place> BFS(const std::unordered_map<Place, std::vector<Place>> & adj_list, std::vector<Item> items_r, const Map & map) {    // BFS
    if (check_all(map, adj_list)) 
        return std::list<Place>{};
    
    std::list<Place> to_return = std::list<Place>{};         // to return
    std::queue<std::shared_ptr<back_track>> queue; // idk need for the bfs
    std::deque<std::deque<Item>> combinations (map.places); // combinations
    
    combinations[map.start].push_back(items_r[map.start]);
    queue.push(std::make_shared<back_track>(nullptr, map.start, items_r[map.start])); // starting point        
    
    if (queue.front()->bitconfig == (1 << (map.items.size()-1)) && map.start == map.end) return std::list<Place>{map.start}; // check if we found all items already
    
    int final_item = (1 << map.items.size())-1;
    
    while (!queue.empty()) {
        std::shared_ptr<back_track> room = queue.front();
        queue.pop();
        
        for (size_t adjacent_room_index = 0; adjacent_room_index < adj_list.at(room->room_number).size(); adjacent_room_index++) {
            Place adjacent_room = adj_list.at(room->room_number)[adjacent_room_index];
            
            if (std::find(combinations.at(adjacent_room).begin(),combinations.at(adjacent_room).end(), \
                  (items_r[adjacent_room] | room->bitconfig)) == combinations.at(adjacent_room).end()) {
                
                combinations.at(adjacent_room).push_back((room->bitconfig | items_r[adjacent_room]));
                std::shared_ptr<back_track> new_room = std::make_shared<back_track>(room, adjacent_room, (room->bitconfig | items_r[adjacent_room]));
             
                queue.push(new_room);
                
                if (new_room->bitconfig == final_item && new_room->room_number == map.end) {
                    for (std::shared_ptr<back_track> ptr = new_room; ptr!=nullptr; ptr=ptr->parent) {
                        to_return.push_front(ptr->room_number);
                    }
                    return to_return;
                }
            }
        }
    }
        
    return to_return;
} //=====================================================================//


std::list<Place> find_path(const Map & map) {
    if (check_all(map))
        return std::list<Place>{};
    
    return BFS(make_adjacency(map), make_combo_items(map), map);
} //=====================================================================//


#ifndef __PROGTEST__

using TestCase = std::pair<size_t, Map>;

// Class template argument deduction exists since C++17 :-)
const std::array examples = {
    TestCase{1, Map{2,0,0, {{0,1}},                     {{0}}       }},
    TestCase{3, Map{2,0,0, {{0,1}},                     {{1}}       }},
    TestCase{3, Map{4,0,1, {{0,2},{2,3},{0,3},{3,1}},   {}          }},
    TestCase{4, Map{4,0,1, {{0,2},{2,3},{0,3},{3,1}},   {{2}}       }},
    TestCase{0, Map{4,0,1, {{0,2},{2,3},{0,3},{3,1}},   {{2},{}}    }},
};

int main() {    
    int fail = 0;
    for (size_t i = 0; i < examples.size(); i++) {
        auto sol = find_path(examples[i].second);
        if (sol.size() != examples[i].first) {
            std::cout << "Wrong anwer for map " << i << std::endl;
            fail++;
        }
    }
    
    if (fail) std::cout << "Failed " << fail << " tests" << std::endl;
    else std::cout << "All tests completed" << std::endl;
    return 0;
}

#endif

