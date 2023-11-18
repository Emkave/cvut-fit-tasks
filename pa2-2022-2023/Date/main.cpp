#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */

//=================================================================================================
// a dummy exception class, keep this implementation
class InvalidDateException : public invalid_argument {
public:
    InvalidDateException() : invalid_argument("invalid date or format") {
    }
};

//=================================================================================================
// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
ios_base &( *date_format(const char *fmt))(ios_base &x) {
    return [](ios_base &ios) -> ios_base & { return ios; };
}
//=================================================================================================

typedef unsigned int UI; //Optimization
typedef unsigned short int USI;
typedef const unsigned short int CUSI;
typedef const short int CSI;

class CDate {
public:
    CDate(USI year = 1900, USI month = 1, USI day = 1) { // COMPLETED!!!!!!
        if (!CheckDate(year, month, day)) {
            throw InvalidDateException(); 
        }

        
        this->calendar.tm_year = year-1900;
        this->calendar.tm_mon = month-1;
        this->calendar.tm_mday = day;
        this->cYear = std::to_string(this->calendar.tm_year + 1900);
        this->cMonth = std::to_string(this->calendar.tm_mon + 1); //Increase (1-12) as official month number
        this->cDay = std::to_string(this->calendar.tm_mday);

        if (cMonth.length() == 1) {
            this->cMonth = '0' + this->cMonth; 
        } //Convert one digit number to two digit
        
        if (cDay.length() == 1) {
            this->cDay = '0' + this->cDay; 
        }

        this->YMD = this->cYear + this->cMonth + this->cDay; //yyyymmdd string

        for (USI i=1; i<=8; i++) {
            switch (i) {
                case 5: 
                case 7: 
                case 9: 
                    this->YMD_M += '-'; 
                    break;
            }
            
            this->YMD_M += YMD[i-1];
        }
    }

    inline friend std::ostream & operator << (std::ostream & oss, CDate & obj) { // COMPLETED
        oss << obj.YMD_M; return oss; //return yyyy-mm-dd string
    }

    inline friend std::istream & operator >> (std::istream & iss, CDate & obj) {
        std::string input(std::istreambuf_iterator<char>(iss), {}); //Assigning string from 'iss' to 'input'
        std::istringstream date(input);
        std::string token; // <-- needs for separating // 2015-09-03
        USI tk1 = 0, tk2 = 0, tk3 = 0;

        for (USI i=1; std::getline(date, token, '-'); i++) {
            switch (i) {
                case 1: tk1 = stoi(token);
                    if (tk1 < 1900) {
                        iss.setstate(std::ios::failbit); 
                        return iss;
                    } 
                    break;

                case 2: tk2 = stoi(token);
                    if (tk2-1 < 0 || tk2-1 > 11) {
                        iss.setstate(std::ios::failbit); 
                        return iss;
                    } 
                    break;

                case 3: tk3 = stoi(token);
                    if (tk3 < 1 || tk3 > 31) {
                        iss.setstate(std::ios::failbit); 
                        return iss;
                    }
                    
                    if (tk2 == 2) {
                        if (obj.IsLeap(tk1)) {
                            if (tk3 > 29) {
                                iss.setstate(std::ios::failbit); 
                                return iss;
                            }
                        } else {
                            if (tk3 > 28) {
                                iss.setstate(std::ios::failbit); 
                                return iss;
                            }
                        }
                    }
                    
                    switch (tk2-1) {
                        case 3: 
                        case 5:
                        case 8: 
                        case 10: 
                            if (tk3 > 30) {
                                iss.setstate(std::ios::failbit); 
                                return iss;
                            }
                        }

                        break;
            }
        }
        
        obj.calendar.tm_year = tk1-1900;
        obj.calendar.tm_mon = tk2-1;
        obj.calendar.tm_mday = tk3;
        
        std::string c1 = std::to_string(tk1);
        std::string c2 = std::to_string(tk2);
        std::string c3 = std::to_string(tk3);
        
        if (c2.length() == 1) {
            c2 = '0' + c2;
        }

        if (c3.length() == 1) {
            c3 = '0' + c3;
        }
        
        obj.YMD_M = c1 + '-' + c2 + '-' + c3;
        
        return iss;
    }

    inline CDate operator ++ (int) { // Post-increment
        CDate obj = * this;
        this->calendar.tm_mday++; 
        mktime(&this->calendar);

        std::string mY = std::to_string(this->calendar.tm_year+1900);
        std::string mM = std::to_string(this->calendar.tm_mon+1);
        std::string mD = std::to_string(this->calendar.tm_mday);

        if (mM.length() == 1) {
            mM = '0' + mM;
        } //Convert one digit number to two digit

        if (mD.length() == 1) {
            mD = '0' + mD;
        }

        this->YMD_M = mY + '-' + mM + '-' + mD;

        return obj;
    }

    inline CDate operator -- (int) { // Post-decrement
        CDate obj = *this;
        this->calendar.tm_mday--;
        mktime(&this->calendar);

        std::string mY = std::to_string(this->calendar.tm_year+1900);
        std::string mM = std::to_string(this->calendar.tm_mon+1);
        std::string mD = std::to_string(this->calendar.tm_mday);
        
        if (mM.length() == 1) {
            mM = '0' + mM; 
        } //Convert one digit number to two digit

        if (mD.length() == 1) {
            mD = '0' + mD;
        }

        this->YMD_M = mY + '-' + mM + '-' + mD;

        return obj;
    }

    inline friend CDate & operator ++ (CDate & obj) { // Pre-increment COMPLETED!!!!
        ++obj.calendar.tm_mday;
        mktime(&obj.calendar);

        std::string mY = std::to_string(obj.calendar.tm_year+1900);
        std::string mM = std::to_string(obj.calendar.tm_mon+1);
        std::string mD = std::to_string(obj.calendar.tm_mday);

        if (mM.length() == 1) {
            mM = '0' + mM; 
        } //Convert one digit number to two digit

        if (mD.length() == 1) {
            mD = '0' + mD;
        }

        obj.YMD_M = mY + '-' + mM + '-' + mD;

        return obj;
    }

    inline friend CDate & operator -- (CDate & obj) { // Pre-decrement COMPLETED!!!!
        --obj.calendar.tm_mday;
        mktime(&obj.calendar);

        std::string mY = std::to_string(obj.calendar.tm_year+1900);
        std::string mM = std::to_string(obj.calendar.tm_mon+1);
        std::string mD = std::to_string(obj.calendar.tm_mday);

        if (mM.length() == 1) {
            mM = '0' + mM;
        } //Convert one digit number to two digit

        if (mD.length() == 1) {
            mD = '0' + mD;
        }

        obj.YMD_M = mY + '-' + mM + '-' + mD;

        return obj;
    }

    inline friend CDate & operator + (CDate & obj, CSI days) { // Addition COMPLETED!!!!
        obj.calendar.tm_mday += days;
        mktime(&obj.calendar);

        std::string mY = std::to_string(obj.calendar.tm_year+1900);
        std::string mM = std::to_string(obj.calendar.tm_mon+1);
        std::string mD = std::to_string(obj.calendar.tm_mday);

        if (mM.length() == 1) {
            mM = '0' + mM;
        } //Convert one digit number to two digit

        if (mD.length() == 1) {
            mD = '0' + mD;
        }

        obj.YMD_M = mY + '-' + mM + '-' + mD;

        return obj;
    }

    inline friend CDate & operator - (CDate & obj, CSI &days) { // Substraction COMPLETED!!!!
        obj.calendar.tm_mday -= days;
        mktime(&obj.calendar);

        std::string mY = std::to_string(obj.calendar.tm_year+1900);
        std::string mM = std::to_string(obj.calendar.tm_mon+1);
        std::string mD = std::to_string(obj.calendar.tm_mday);

        if (mM.length() == 1) {
            mM = '0' + mM;
        } //Convert one digit number to two digit

        if (mD.length() == 1) {
            mD = '0' + mD;
        }

        obj.YMD_M = mY + '-' + mM + '-' + mD;

        return obj;
    }

    inline friend UI operator - (CDate & obj1, CDate & obj2) { // Difference COMPLETED!!!!
        std::time_t a = std::mktime(&obj1.calendar);
        std::time_t b = std::mktime(&obj2.calendar);
        int diff = 0;

        if (a != (std::time_t)(-1) && b != (std::time_t)(-1))
            diff = std::difftime(b, a) / 86400;

        return std::abs(diff);
    }

    inline friend bool operator == (CDate & obj1, CDate & obj2) { // Equality COMPLETED!!!!!
        std::time_t a = std::mktime(&obj1.calendar);
        std::time_t b = std::mktime(&obj2.calendar);

        if (a != (std::time_t)(-1) && b != (std::time_t)(-1))
            if (std::difftime(a, b) / 86400 == 0)
                return true;

        return false;
    }

    inline friend bool operator != (CDate & obj1, CDate & obj2) { // Inequality COMPLETED!!!!!
        std::time_t a = std::mktime(&obj1.calendar);
        std::time_t b = std::mktime(&obj2.calendar);

        int diff = 0;

        if (a != (std::time_t)(-1) && b != (std::time_t)(-1))
            diff = std::difftime(a, b) / 86400;

        if (diff != 0) {
            return true;
        }

        return false;
    }

    inline friend bool operator < (CDate & obj1, CDate & obj2) { // Less COMPLETED!!!!!
        std::time_t a = std::mktime(&obj1.calendar);
        std::time_t b = std::mktime(&obj2.calendar);

        int diff = 0;
        
        if (a != (std::time_t)(-1) && b != (std::time_t)(-1))
            diff = std::difftime(a, b) / 86400;

        if (diff < 0) {
            return true;
        }

        return false;
    }

    inline friend bool operator <= (CDate & obj1, CDate & obj2) { // Less-equal COMPLETED!!!!!
        std::time_t a = std::mktime(&obj1.calendar);
        std::time_t b = std::mktime(&obj2.calendar);

        int diff = 0;

        if (a != (std::time_t)(-1) && b != (std::time_t)(-1))
            diff = std::difftime(a, b) / 86400;

        if (diff <= 0) {
            return true;
        }

        return false;
    }

    inline friend bool operator > (CDate & obj1, CDate & obj2) { // More COMPLETED!!!!!
        std::time_t a = std::mktime(&obj1.calendar);
        std::time_t b = std::mktime(&obj2.calendar);

        int diff = 0;

        if (a != (std::time_t)(-1) && b != (std::time_t)(-1))
            diff = std::difftime(a, b) / 86400;

        if (diff > 0) {
            return true;
        }

        return false;
    }

    inline friend bool operator >= (CDate & obj1, CDate & obj2) { // More-equal COMPLETED!!!!!
        std::time_t a = std::mktime(&obj1.calendar);
        std::time_t b = std::mktime(&obj2.calendar);

        int diff = 0;

        if (a != (std::time_t)(-1) && b != (std::time_t)(-1))
            diff = std::difftime(a, b) / 86400;

        if (diff >= 0) {
            return true;
        }

        return false;
    }

private:
    std::string cYear = "";
    std::string cMonth = ""; //Variables needed for date formating
    std::string cDay = "";
    std::string YMD = "";
    std::string YMD_M = "";
    struct std::tm calendar = {0}; // Calendar

    inline bool CheckDate(USI & year, USI & month, USI & day) { // Date checker COMPLETED!!!
        if (year < 1900) {
            return false;
        }

        if (month < 0 || month > 11) {
            return false;
        }

        if (day < 1 || day > 31) {
            return false;
        }

        if (month == 2) {
            if (IsLeap(year)) {
                if (day > 29) {
                    return false;
                }
            } else {
                if (day > 28) {
                    return false;
                }
            }
        } 
        
        switch (month) {
            case 3: 
            case 5: 
            case 8:
            case 10: 
                if (day > 30) {
                    return false; 
                };
            }

        return true;
    }

    // Leap year checked  |
    //                    V
    inline bool IsLeap(CUSI & year) {
        return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)); 
    }

};

#ifndef __PROGTEST__

int main(void) {
    ostringstream oss;
    istringstream iss;

    CDate a(2000, 1, 2);
    CDate b(2010, 2, 3);
    CDate c(2004, 2, 10);
    oss.str("");
    oss << a;
    assert(oss.str() == "2000-01-02");
    oss.str("");
    oss << b;
    assert(oss.str() == "2010-02-03");
    oss.str("");
    oss << c;
    assert(oss.str() == "2004-02-10");
    a = a + 1500;
    oss.str("");
    oss << a;
    assert(oss.str() == "2004-02-10");
    b = b - 2000;
    oss.str("");
    oss << b;
    assert(oss.str() == "2004-08-13");
    assert(b - a == 185);
    assert((b == a) == false);
    assert((b != a) == true);
    assert((b <= a) == false);
    assert((b < a) == false);
    assert((b >= a) == true);
    assert((b > a) == true);
    assert((c == a) == true);
    assert((c != a) == false);
    assert((c <= a) == true);
    assert((c < a) == false);
    assert((c >= a) == true);
    assert((c > a) == false);
    a = ++c;
    oss.str("");
    oss << a << " " << c;
    assert(oss.str() == "2004-02-11 2004-02-11");
    a = --c;
    oss.str("");
    oss << a << " " << c;
    assert(oss.str() == "2004-02-10 2004-02-10");
    a = c++;
    oss.str("");
    oss << a << " " << c;
    assert(oss.str() == "2004-02-10 2004-02-11");
    a = c--;
    oss.str("");
    oss << a << " " << c;
    assert(oss.str() == "2004-02-11 2004-02-10");
    iss.clear();
    iss.str("2015-09-03");
    assert((iss >> a));
    oss.str("");
    oss << a;
    assert(oss.str() == "2015-09-03");
    a = a + 70;
    oss.str("");
    oss << a;
    assert (oss.str() == "2015-11-12");

    CDate d(2000, 1, 1);
    try {
        CDate e(2000, 32, 1);
        assert("No exception thrown!" == nullptr);
    }
    catch (...) {
    }
    iss.clear();
    iss.str("2000-12-33");
    assert(!(iss >> d));
    oss.str("");
    oss << d;
    assert (oss.str() == "2000-01-01");
    iss.clear();
    iss.str("2000-11-31");
    assert (!(iss >> d));
    oss.str("");
    oss << d;
    assert (oss.str() == "2000-01-01");
    iss.clear();
    iss.str("2000-02-29");
    assert ((iss >> d));
    oss.str("");
    oss << d;
    assert (oss.str() == "2000-02-29");
    iss.clear();
    iss.str("2001-02-29");
    assert (!(iss >> d));
    oss.str("");
    oss << d;
    assert (oss.str() == "2000-02-29");
/*
    //-----------------------------------------------------------------------------
    // bonus test examples
    //-----------------------------------------------------------------------------
    CDate f(2000, 5, 12);
    oss.str("");
    oss << f;
    assert (oss.str() == "2000-05-12");
    oss.str("");
    oss << date_format("%Y/%m/%d") << f;
    assert (oss.str() == "2000/05/12");
    oss.str("");
    oss << date_format("%d.%m.%Y") << f;
    assert (oss.str() == "12.05.2000");
    oss.str("");
    oss << date_format("%m/%d/%Y") << f;
    assert (oss.str() == "05/12/2000");
    oss.str("");
    oss << date_format("%Y%m%d") << f;
    assert (oss.str() == "20000512");
    oss.str("");
    oss << date_format("hello kitty") << f;
    assert (oss.str() == "hello kitty");
    oss.str("");
    oss << date_format("%d%d%d%d%d%d%m%m%m%Y%Y%Y%%%%%%%%%%") << f;
    assert (oss.str() == "121212121212050505200020002000%%%%%");
    oss.str("");
    oss << date_format("%Y-%m-%d") << f;
    assert (oss.str() == "2000-05-12");
    iss.clear();
    iss.str("2001-01-1");
    assert (!(iss >> f));
    oss.str("");
    oss << f;
    assert (oss.str() == "2000-05-12");
    iss.clear();
    iss.str("2001-1-01");
    assert (!(iss >> f));
    oss.str("");
    oss << f;
    assert (oss.str() == "2000-05-12");
    iss.clear();
    iss.str("2001-001-01");
    assert (!(iss >> f));
    oss.str("");
    oss << f;
    assert (oss.str() == "2000-05-12");
    iss.clear();
    iss.str("2001-01-02");
    assert ((iss >> date_format("%Y-%m-%d") >> f));
    oss.str("");
    oss << f;
    assert (oss.str() == "2001-01-02");
    iss.clear();
    iss.str("05.06.2003");
    assert ((iss >> date_format("%d.%m.%Y") >> f));
    oss.str("");
    oss << f;
    assert (oss.str() == "2003-06-05");
    iss.clear();
    iss.str("07/08/2004");
    assert ((iss >> date_format("%m/%d/%Y") >> f));
    oss.str("");
    oss << f;
    assert (oss.str() == "2004-07-08");
    iss.clear();
    iss.str("2002*03*04");
    assert ((iss >> date_format("%Y*%m*%d") >> f));
    oss.str("");
    oss << f;
    assert (oss.str() == "2002-03-04");
    iss.clear();
    iss.str("C++09format10PA22006rulez");
    assert ((iss >> date_format("C++%mformat%dPA2%Yrulez") >> f));
    oss.str("");
    oss << f;
    assert (oss.str() == "2006-09-10");
    iss.clear();
    iss.str("%12%13%2010%");
    assert ((iss >> date_format("%%%m%%%d%%%Y%%") >> f));
    oss.str("");
    oss << f;
    assert (oss.str() == "2010-12-13");

    CDate g(2000, 6, 8);
    iss.clear();
    iss.str("2001-11-33");
    assert (!(iss >> date_format("%Y-%m-%d") >> g));
    oss.str("");
    oss << g;
    assert (oss.str() == "2000-06-08");
    iss.clear();
    iss.str("29.02.2003");
    assert (!(iss >> date_format("%d.%m.%Y") >> g));
    oss.str("");
    oss << g;
    assert (oss.str() == "2000-06-08");
    iss.clear();
    iss.str("14/02/2004");
    assert (!(iss >> date_format("%m/%d/%Y") >> g));
    oss.str("");
    oss << g;
    assert (oss.str() == "2000-06-08");
    iss.clear();
    iss.str("2002-03");
    assert (!(iss >> date_format("%Y-%m") >> g));
    oss.str("");
    oss << g;
    assert (oss.str() == "2000-06-08");
    iss.clear();
    iss.str("hello kitty");
    assert (!(iss >> date_format("hello kitty") >> g));
    oss.str("");
    oss << g;
    assert (oss.str() == "2000-06-08");
    iss.clear();
    iss.str("2005-07-12-07");
    assert (!(iss >> date_format("%Y-%m-%d-%m") >> g));
    oss.str("");
    oss << g;
    assert (oss.str() == "2000-06-08");
    iss.clear();
    iss.str("20000101");
    assert ((iss >> date_format("%Y%m%d") >> g));
    oss.str("");
    oss << g;
    assert (oss.str() == "2000-01-01");*/


    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
