#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

typedef unsigned int UI;
typedef short int SI;
typedef const unsigned int CUI;
typedef unsigned short int USI; // in
typedef const unsigned short int CUSI;
typedef const int CI;


class CDate {
public:
    USI date[3] {};
    CDate(CUSI & year, CUSI & month, CUSI & day) {
        this->date[0] = year;
        this->date[1] = month;
        this->date[2] = day;
    }
};

class CSupermarket {
public:
    typedef struct Storage {
        std::string cargo_des;
        int count = 0;
        struct std::tm vDate {0};
        time_t time = 0;
        inline Storage (std::string cargo_des, CI count, struct std::tm vDate, time_t time)
                : cargo_des(std::move(cargo_des)), count(count), vDate(vDate), time(time) {}
    } Storage;

private:
    std::map<std::string, std::deque<Storage>> storageList;

public:
    inline CSupermarket & store(const std::string & name, CDate oDate, int count) {   // Total complexity: Log(n) + Log(n)
        struct std::tm date = {0};
        date.tm_year = oDate.date[0];
        date.tm_mon = oDate.date[1]-1;
        date.tm_mday = oDate.date[2];
        
        auto storageList_finder = this->storageList.find(name);

        if (storageList_finder != this->storageList.end()) {
            auto pos = std::lower_bound(this->storageList[name].begin(), this->storageList[name].end(), Storage(name, count, date, mktime(&date)), [this](const Storage & a, const Storage & b) {
                return a.time < b.time; // Log(n)
            });

            this->storageList[name].insert(pos, (Storage(name, count, date, mktime(&date)))); // Log(n)
        } else 
            this->storageList[name].emplace_front(Storage(name, count, date, mktime(&date))); // Log(n)

        return * this;
    }

    inline std::list<std::pair<std::string, int>> expired(CDate oDate) const {     //n ::: nLog(n) === O(nLog(n)) [✔︎]
        struct std::tm d = {0};
        d.tm_year = oDate.date[0];
        d.tm_mon = oDate.date[1]-1;
        d.tm_mday = oDate.date[2];
        time_t date = mktime(&d);
        
        std::list<std::pair<std::string, int>> expired;
        std::multimap<int, std::string, std::greater<int>> expired_map;
        
        for (auto storageList_scanner=this->storageList.begin(); storageList_scanner!=this->storageList.end(); storageList_scanner++) {
            bool found = false;
            UI total = 0;

            for (auto storageList_deque_scanner=storageList_scanner->second.begin(); storageList_deque_scanner!=storageList_scanner->second.end(); storageList_deque_scanner++) {
                if (difftime(date, storageList_deque_scanner->time) > 0) {
                    total += storageList_deque_scanner->count;
                    found = true;
                }
            }
            if (found) {
                expired_map.emplace(total, storageList_scanner->first);
            }

            total = 0;
        }
        
        for (auto ptr=expired_map.begin(); ptr!=expired_map.end(); ptr++) {
            expired.emplace_back(ptr->second, ptr->first);
        }
        
        return expired;
    }

    inline void sell(std::list<std::pair<std::string, int>> & listToSell) {
        std::deque<std::string> storageList_products_toErase;
        
        std::string product_toSell;
        USI nonMatch = 0;
        USI typo_dups = 0;
        bool found = false;
        
        for (auto listToSell_scanner=listToSell.begin(); listToSell_scanner!=listToSell.end(); listToSell_scanner++) {
            auto storageList_finder = this->storageList.find(listToSell_scanner->first);

            if (storageList_finder != this->storageList.end()) {
                product_toSell = storageList_finder->first;
                found = true;
            } else {
                for (auto & storageList_product : this->storageList) {
                    if (listToSell_scanner->first.size() == storageList_product.first.size()) {
                        for (USI i=0; nonMatch < 2 && i<listToSell_scanner->first.size(); i++) {
                            if (listToSell_scanner->first[i] != storageList_product.first[i]) {
                                nonMatch++;
                            }
                        }

                        if (nonMatch == 1) {
                            product_toSell = storageList_product.first;
                            typo_dups++;
                            found = true;
                        }
                        
                        if (typo_dups == 2 || nonMatch == 0) {
                            typo_dups = 0;
                            nonMatch = 0;
                            found = false;
                            break;
                        }
                        nonMatch = 0;
                    }
                }
                typo_dups = 0;
            }
            if (found) {
                if (!this->storageList[product_toSell].empty()) {
                    while (listToSell_scanner->second != 0 && !this->storageList[product_toSell].empty()) {
                        if (listToSell_scanner->second > this->storageList[product_toSell][0].count) {
                            listToSell_scanner->second -= this->storageList[product_toSell][0].count;
                            this->storageList[product_toSell].pop_front();

                            if (this->storageList[product_toSell].empty()) {
                                storageList_products_toErase.emplace_back(product_toSell);
                            }
                            
                        } else if (listToSell_scanner->second < this->storageList[product_toSell][0].count) {
                            this->storageList[product_toSell][0].count -= listToSell_scanner->second;
                            auto ptr = listToSell_scanner;
                            listToSell_scanner--;
                            listToSell.erase(ptr);
                            break;

                        } else if (listToSell_scanner->second == this->storageList[product_toSell][0].count) {
                            this->storageList[product_toSell].pop_front();
                            auto ptr = listToSell_scanner;
                            listToSell_scanner--;
                            listToSell.erase(ptr);
                            break;
                        }
                    }
                    found = false;
                } else {
                    storageList_products_toErase.emplace_back(product_toSell);
                }
            }
        }

        if (!storageList_products_toErase.empty()) {
            for (auto & ptr : storageList_products_toErase) {
                this->storageList.erase(this->storageList.find(ptr));
            }
        }
    }
};



#ifndef __PROGTEST__

int main(void) {
    CSupermarket s;
    
    
    s.store("bread",  CDate(2016, 4, 30), 100);
    s.store("butter", CDate(2016, 5, 10), 10);
    s.store("beer",   CDate(2016, 8, 10), 50);
    s.store("bread",  CDate(2016, 4, 25), 100);
    s.store("okey",   CDate(2016, 7, 18), 5);


    list<pair<string, int>> l0 = s.expired(CDate(2018, 4, 30));
    assert (l0.size() == 4);
    assert ((l0 == list<pair<string, int>> {{"bread",  200},
                                            {"beer",   50},
                                            {"butter", 10},
                                            {"okey",   5}}));

    list<pair<string, int>> l1 {{"bread",  2},
                                {"Coke",   5},
                                {"butter", 20}};
    s.sell(l1);
    assert (l1.size() == 2);
    assert ((l1 == list<pair<string, int>> {{"Coke",   5},
                                            {"butter", 10}}));

    list<pair<string, int> > l2 = s.expired(CDate(2016, 4, 30));
    assert (l2.size() == 1);
    assert ((l2 == list<pair<string, int>> {{"bread", 98}}));

    list<pair<string, int> > l3 = s.expired(CDate(2016, 5, 20));
    assert (l3.size() == 1);
    assert ((l3 == list<pair<string, int>> {{"bread", 198}}));

    list<pair<string, int> > l4{{"bread", 105}};
    s.sell(l4);
    assert (l4.size() == 0);
    assert ((l4 == list<pair<string, int>> {}));

    list<pair<string, int> > l5 = s.expired(CDate(2017, 1, 1));
    assert (l5.size() == 3);
    assert ((l5 == list<pair<string, int>> {{"bread", 93},
                                            {"beer",  50},
                                            {"okey",  5}}));

    s.store("Coke", CDate(2016, 12, 31), 10);

    list<pair<string, int> > l6{{"Cake",  1},
                                {"Coke",  1},
                                {"cake",  1},
                                {"coke",  1},
                                {"cuke",  1},
                                {"Cokes", 1}};
    s.sell(l6);
    assert (l6.size() == 3);
    assert ((l6 == list<pair<string, int> >{{"cake",  1},
                                            {"cuke",  1},
                                            {"Cokes", 1}}));

    list<pair<string, int> > l7 = s.expired(CDate(2017, 1, 1));
    assert (l7.size() == 4);
    assert ((l7 == list<pair<string, int> >{{"bread", 93},
                                            {"beer",  50},
                                            {"Coke",  7},
                                            {"okey",  5}}));

    s.store("cake", CDate(2016, 11, 1), 5);

    list<pair<string, int> > l8{{"Cake", 1},
                                {"Coke", 1},
                                {"cake", 1},
                                {"coke", 1},
                                {"cuke", 1}};
    s.sell(l8);
    assert (l8.size() == 2);
    assert ((l8 == list<pair<string, int> >{{"Cake", 1},
                                            {"coke", 1}}));

    list<pair<string, int> > l9 = s.expired(CDate(2017, 1, 1));
    assert (l9.size() == 5);
    assert ((l9 == list<pair<string, int> >{{"bread", 93},
                                            {"beer",  50},
                                            {"Coke",  6},
                                            {"okey",  5},
                                            {"cake",  3}}));

    list<pair<string, int> > l10{{"cake", 15},
                                 {"Cake", 2}};
    s.sell(l10);
    assert (l10.size() == 2);
    assert ((l10 == list<pair<string, int> >{{"cake", 12},
                                             {"Cake", 2}}));

    list<pair<string, int> > l11 = s.expired(CDate(2017, 1, 1));
    assert (l11.size() == 4);
    assert ((l11 == list<pair<string, int> >{{"bread", 93},
                                             {"beer",  50},
                                             {"Coke",  6},
                                             {"okey",  5}}));

    list<pair<string, int> > l12{{"Cake", 4}};
    s.sell(l12);
    assert (l12.size() == 0);
    assert ((l12 == list<pair<string, int> >{}));

    list<pair<string, int> > l13 = s.expired(CDate(2017, 1, 1));
    assert (l13.size() == 4);
    assert ((l13 == list<pair<string, int> >{{"bread", 93},
                                             {"beer",  50},
                                             {"okey",  5},
                                             {"Coke",  2}}));

    list<pair<string, int> > l14{{"Beer", 20},
                                 {"Coke", 1},
                                 {"bear", 25},
                                 {"beer", 10}};
    s.sell(l14);
    assert (l14.size() == 1);
    assert ((l14 == list<pair<string, int> >{{"beer", 5}}));

    s.store("ccccb", CDate(2019, 3, 11), 100);
    s.store("ccccd", CDate(2019, 6, 9), 100);
    s.store("dcccc", CDate(2019, 2, 14), 100);

    list<pair<string, int> > l15{{"ccccc", 10}};
    s.sell(l15);
    assert (l15.size() == 1);
    assert ((l15 == list<pair<string, int> >{{"ccccc", 10}}));

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */


