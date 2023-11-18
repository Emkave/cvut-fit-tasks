#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

typedef unsigned int UI; //Optimization
typedef unsigned short int USI;
typedef unsigned long int ULI;
typedef unsigned long long int ULLI;

typedef struct Company { //Structure of a company
    std::string name;
    std::string addr;
    std::string taxID;
    UI amount = 0;
    Company(const std::string name, const std::string addr, const std::string taxID)
            : name(name), addr(addr), taxID(taxID) {} //Used for better initiliziation
} Company;

//static UI pos = 1; // <- Important tool that makes firstCompany and nextCompany work in O(1) DO NOT MODIFY IT!

class CVATRegister {
public:
    inline bool newCompany(const string & name, const string & addr, const string & taxID) {
        if (emptySector == true) {
            sector.emplace_back((Company(name, addr, taxID))); 
            emptySector = false;
            sectorID.emplace_back((Company(name, addr, taxID))); 
            return true;
        } else { 
            for (UI i=0; i<sectorID.size(); i++) {
                if (taxID.compare(sectorID[i].taxID) == 0) {
                    if (i == sectorID.size()) {
                        break;
                    }
                    return false;
                }
            } 
            
            auto napp = std::lower_bound(sector.begin(), sector.end(), (Company(name, addr, taxID)), [this](const Company & a, const Company & b) {
                return (NAComparator(a, b) < 0);});
            
            if (napp != sector.end() && NAComparator(*napp, (Company(name, addr, taxID))) == 0) {
                return false;
            }

            sector.insert(napp, (Company(name, addr, taxID)));
            for (UI i=0; i<sectorID.size(); i++) {
                if (taxID.compare(sectorID[i].taxID) < 0) {
                    sectorID.insert(sectorID.begin()+i, (Company(name, addr, taxID))); 
                    break;
                } else if (i+1 == sectorID.size()) {
                    sectorID.push_back((Company(name, addr, taxID))); 
                    break;
                }
            } return true;
        }
    }

    bool cancelCompany(const string & name, const string & addr) {
        if (sector.size() == 0) {
            return false;
        }

        int posNA = CompanySearch(sector, name, addr);

        if (posNA == -1) {
            return false;
        }

        int posID = CompanySearch(sectorID, sector[posNA].taxID);
        
        if (posID == -1) {
            return false;
        }

        sector.erase(sector.begin()+posNA);
        sectorID.erase(sectorID.begin()+posID);

        if (sector.size() == 0 || sectorID.size() == 0) {
            emptySector = true; 
            transactions.clear();
        } 
        return true;
    }

    bool cancelCompany(const string & taxID) {
        if (sector.size() == 0) {
            return false;
        }

        int posID = CompanySearch(sectorID, taxID);

        if (posID == -1) {
            return false;
        }

        int posNA = CompanySearch(sector, sectorID[posID].name, sectorID[posID].addr);

        if (posNA == -1) {
            return false;
        }

        sector.erase(sector.begin()+posNA);
        sectorID.erase(sectorID.begin()+posID);

        if (sector.size() == 0 || sectorID.size() == 0) {
            emptySector = true; 
            transactions.clear();
        } 
        return true;
    }

    inline bool invoice(const string & taxID, unsigned int amount) {
        if (sector.size() == 0) {
            return false;
        }

        int posID = CompanySearch(sectorID, taxID);

        if (posID == -1) {
            return false;
        }

        int posNA = CompanySearch(sector, sectorID[posID].name, sectorID[posID].addr);

        if (posNA == -1) {
            return false;
        }

        sector[posNA].amount += amount;
        sectorID[posID].amount += amount;
        transactions.push_back(amount);

        return true;
    }

    inline bool invoice(const string & name, const string & addr, unsigned int amount) {
        if (sector.size() == 0) {
            return false;
        }

        int posNA = CompanySearch(sector, name, addr);

        if (posNA == -1) {
            return false;
        }

        int posID = CompanySearch(sectorID, sector[posNA].taxID);

        if (posID == -1) {
            return false;
        }

        sector[posNA].amount += amount;
        sectorID[posID].amount += amount;
        transactions.push_back(amount);

        return true;
    }

    inline bool audit(const string & name, const string & addr, unsigned int & sumIncome) const {
        if (sector.size() == 0) {
            return false;
        }

        UI mid = 0; UI left = 0;
        UI right = sector.size();

        while (left < right) {
            mid = left + (right - left) / 2;
            int nameGateWay = strcasecmp(name.c_str(), sector[mid].name.c_str());

            if (nameGateWay < 0) {
                right = mid;
            } else if (nameGateWay > 0) {
                left = mid + 1;
            } else if (nameGateWay == 0) {
                int addrGateWay = strcasecmp(addr.c_str(), sector[mid].addr.c_str());

                if (addrGateWay < 0) {
                    right = mid;
                } else if (addrGateWay > 0) {
                    left = mid + 1;
                } else if (addrGateWay == 0) {
                    break;
                }
            }
        } 
        
        if (left >= right) {
            return false;
        }

        sumIncome = sector[mid].amount;

        return true;
    }

    inline bool audit(const string & taxID, unsigned int & sumIncome) const {
        if (sectorID.size() == 0) {
            return false;
        }

        UI mid = 0; UI left = 0;
        UI right = sectorID.size();

        while (left < right) {
            mid = left + (right - left) / 2;

            if (taxID > sectorID[mid].taxID) {
                left = mid + 1;
            } else if (taxID < sectorID[mid].taxID) {
                right = mid;
            } else if (taxID == sectorID[mid].taxID) {
                break;
            }
        } 
        
        if (left >= right) {
            return false;
        }

        sumIncome = sectorID[mid].amount;

        return true;
    }

    bool firstCompany(string & name, string & addr) const {
        if (sector.size() == 0) {
            return false;
        } else {
            name = sector[0].name;
            addr = sector[0].addr; pos = 1;
            return true;
        } 
        return false;
    }

    bool nextCompany(string & name, string & addr) const {
        if (sector.size() == 0 || sector.size() == 1 || pos == sector.size()) {
            return false;
        } else {
            name = sector[pos].name;
            addr = sector[pos].addr; pos++;
            return true;
        } 
        return false;
    }
    
    unsigned int medianInvoice(void) const {
        std::vector<UI> flow (transactions);

        if (flow.size() == 0) {
            return 0;
        }

        std::nth_element(flow.begin(), flow.begin()+flow.size()/2, flow.end());

        return flow[flow.size()/2];
    }

private:
    std::vector<Company> sector;
    std::vector<Company> sectorID;
    std::vector<UI> transactions;   
    bool emptySector = true;
    bool emptyTransaction = true;
    
    static UI pos; // <- Important tool that makes firstCompany and nextCompany work in O(1) DO NOT MODIFY IT!
    
    inline UI CompanySearch(std::vector<Company> const & vector, std::string const & id) {
        UI mid = 0; UI left = 0;
        UI right = vector.size();

        while (left < right) {
            mid = left + (right - left) / 2;
            if (id > vector[mid].taxID) {
                left = mid + 1;
            } else if (id < vector[mid].taxID) {
                right = mid;
            } else {
                return mid;
            }
        } 
        return -1;
    }
    
    inline UI CompanySearch(std::vector<Company> const & vector, std::string const & name, std::string const & addr) {
        UI mid = 0; UI left = 0;
        UI right = vector.size();

        while (left < right) {
            mid = left + (right - left) / 2;
            int nameGateWay = strcasecmp(name.c_str(), vector[mid].name.c_str());

            if (nameGateWay < 0) {
                right = mid;
            } else if (nameGateWay > 0) {
                left = mid + 1;
            } else if (nameGateWay == 0) {
                int addrGateWay = strcasecmp(addr.c_str(), vector[mid].addr.c_str());

                if (addrGateWay < 0) {
                    right = mid;
                } else if (addrGateWay > 0) {
                    left = mid + 1;
                } else if (addrGateWay == 0) {
                    return mid;
                }
            }
        } 
        return -1;
    }
    
    inline int NAComparator(const Company & a, const Company & b) {
        int r1 = strcasecmp(a.name.c_str(), b.name.c_str());
        switch (r1) {
            case 0: 
                r1=strcasecmp(a.addr.c_str(), b.addr.c_str());
                switch (r1) {
                    case 0: 
                        return false; 

                    default: 
                        break;
                } 
            break;
        } 
        return r1;
    }
};

UI CVATRegister::pos = 1;

#ifndef __PROGTEST__
int main(void) {
    string name, addr;
    unsigned int sumIncome;
    
    CVATRegister b1;
    assert(b1.newCompany("ACME", "Thakurova", "666/666"));
    assert(b1.newCompany("ACME", "Kolejni", "666/666/666"));
    assert(b1.newCompany("Dummy", "Thakurova", "123456"));
    assert(b1.invoice("666/666", 2000));
    assert(b1.medianInvoice() == 2000);
    assert(b1.invoice("666/666/666", 3000));
    assert(b1.medianInvoice() == 3000);
    assert(b1.invoice("123456", 4000));
    assert(b1.medianInvoice() == 3000);
    assert(b1.invoice("aCmE", "Kolejni", 5000));
    assert(b1.medianInvoice() == 4000);
    assert(b1.audit("ACME", "Kolejni", sumIncome) && sumIncome == 8000);
    assert(b1.audit("123456", sumIncome) && sumIncome == 4000);
    assert(b1.firstCompany(name, addr) && name == "ACME" && addr == "Kolejni");
    assert(b1.nextCompany(name, addr) && name == "ACME" && addr == "Thakurova");
    assert(b1.nextCompany(name, addr) && name == "Dummy" && addr == "Thakurova");
    assert(!b1.nextCompany(name, addr));
    assert(b1.cancelCompany("ACME", "KoLeJnI"));
    assert(b1.medianInvoice() == 4000);
    assert(b1.cancelCompany("666/666"));
    assert(b1.medianInvoice() == 4000);
    assert(b1.invoice("123456", 100));
    assert(b1.medianInvoice() == 3000);
    assert(b1.invoice("123456", 300));
    assert(b1.medianInvoice() == 3000);
    assert(b1.invoice("123456", 200));
    assert(b1.medianInvoice() == 2000);
    assert(b1.invoice("123456", 230));
    assert(b1.medianInvoice() == 2000);
    assert(b1.invoice("123456", 830));
    assert(b1.medianInvoice() == 830);
    assert(b1.invoice("123456", 1830));
    assert(b1.medianInvoice() == 1830);
    assert(b1.invoice("123456", 2830));
    assert(b1.medianInvoice() == 1830);
    assert(b1.invoice("123456", 2830));
    assert(b1.medianInvoice() == 2000);
    assert(b1.invoice("123456", 3200));
    assert(b1.medianInvoice() == 2000);
    assert(b1.firstCompany(name, addr) && name == "Dummy" && addr == "Thakurova");
    assert(!b1.nextCompany(name, addr));
    assert(b1.cancelCompany("123456"));
    assert(!b1.firstCompany(name, addr));


    CVATRegister b2;
    assert(b2.newCompany("ACME", "Kolejni", "abcdef"));
    assert(b2.newCompany("Dummy", "Kolejni", "123456"));
    assert(!b2.newCompany("AcMe", "kOlEjNi", "1234"));
    assert(b2.newCompany("Dummy", "Thakurova", "ABCDEF"));
    assert(b2.medianInvoice() == 0);
    assert(b2.invoice("ABCDEF", 1000));
    assert(b2.medianInvoice() == 1000);
    assert(b2.invoice("abcdef", 2000));
    assert(b2.medianInvoice() == 2000);
    assert(b2.invoice("aCMe", "kOlEjNi", 3000));
    assert(b2.medianInvoice () == 2000);
    assert(!b2.invoice("1234567", 100));
    assert(!b2.invoice("ACE", "Kolejni", 100));
    assert(!b2.invoice("ACME", "Thakurova", 100));
    assert(!b2.audit("1234567", sumIncome));
    assert(!b2.audit("ACE", "Kolejni", sumIncome));
    assert(!b2.audit("ACME", "Thakurova", sumIncome));
    assert(!b2.cancelCompany("1234567"));
    assert(!b2.cancelCompany("ACE", "Kolejni"));
    assert(!b2.cancelCompany("ACME", "Thakurova"));
    assert(b2.cancelCompany("abcdef"));
    assert(b2.medianInvoice() == 2000);
    assert(!b2.cancelCompany("abcdef"));
    assert(b2.newCompany("ACME", "Kolejni", "abcdef"));
    assert(b2.cancelCompany("ACME", "Kolejni"));
    assert(!b2.cancelCompany("ACME", "Kolejni"));

     return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */


