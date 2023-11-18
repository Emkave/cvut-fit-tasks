#ifndef __PROGTEST__
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int m_TotalDays;
    int m_WorkDays;
} TResult;
#endif /* __PROGTEST__ */


bool is_leap_year(const int y) {    // Check for leap years
    return ((y % 4 == 0) && ((y % 400 == 0) || (y % 100 != 0)) && (y % 4000 != 0));
}

int day_of_week(int y, int m, int d) {
    if (m < 3) {
        m += 12;
        y--;
    }               // Basically calculating here the day of the week, assuming leap years exist and the week days are shifting every year
    int w = (d + (2 * m) + (3 * (m + 1) / 5) + y + (y / 4) - (y / 100) + (y / 400) - (y / 4000)) % 7;
    return (w + 1) % 7 == 0 ? 7 : (w + 1) % 7;
}


int get_day_num_slower(int y, int m, int d) {
    int total = 0;
    int num_leap = 0;
    int months_add_from_year_start[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int iter = 0;

    for (iter = 1; iter < y; iter++) {  // Calculate number of leap years
        if (is_leap_year(iter)) {
            num_leap++;
        }
    }

    if (is_leap_year(y)) { // If leap, we add extra day
        for (iter = 2; iter < 12; iter++) {
            months_add_from_year_start[iter]++;
        }
    }

    total = (y - 1) * 365 + num_leap + months_add_from_year_start[m - 1] + d;

    return total;
}


int get_day_num(int y, int m, int d) {
    int months_add_from_year_start[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int num_of_year = y - 1;
    int num_of_leap_year = (num_of_year / 4) - (num_of_year / 100) + (num_of_year / 400) - (num_of_year / 4000);
    int num_of_non_leap_year = num_of_year - num_of_leap_year;
    int days = num_of_non_leap_year * 365 + num_of_leap_year * 366;
    days += months_add_from_year_start[m - 1];
    days += d - 1;

    if (is_leap_year(y) && m > 2) {
        days++;
    }

    return days;
}



bool is_holiday(const int m, const int d) {
    switch (m) {
        case 1: {
            switch (d) {        // Jump table to check if holiday
                case 1:
                    return true;
                default:
                    return false;
            }
        }

        case 5: {
            switch (d) {
                case 1:
                case 8:
                    return true;
                default:
                    return false;
            }
        }

        case 7: {
            switch (d) {
                case 5:
                case 6:
                    return true;
                default:
                    return false;
            }
        }

        case 9: {
            switch (d) {
                case 28:
                    return true;
                default:
                    return false;
            }
        }

        case 10: {
            switch (d) {
                case 28:
                    return true;
                default:
                    return false;
            }
        }

        case 11: {
            switch (d) {
                case 17:
                    return true;
                default:
                    return false;
            }
        }

        case 12: {
            switch (d) {
                case 24:
                case 25:
                case 26:
                    return true;
                default:
                    return false;
            }
        }

        default:
            return false;
    }
}


int get_working_days(int y1, int m1, int d1, int y2, int m2, int d2) {
    int working_days = 0;

    // Iterate over each day in the range
    while (y1 < y2 || (y1 == y2 && m1 < m2) || (y1 == y2 && m1 == m2 && d1 <= d2)) {
        int week_day = day_of_week(y1, m1, d1);

        // Check if the day is a working day (not weekend and not a holiday)
        if (week_day != 6 && week_day != 7 && !is_holiday(m1, d1)) {
            working_days++;
        }

        // Move to the next day
        d1++;
        if ((m1 == 2 && (is_leap_year(y1) ? d1 > 29 : d1 > 28)) || ((m1 == 4 || m1 == 6 || m1 == 9 || m1 == 11) && d1 > 30) || d1 > 31) {
            d1 = 1;
            m1++;
            if (m1 > 12) {
                m1 = 1;
                y1++;
            }
        }
    }
    return working_days;
}


bool is_strict_later(const int y1, const int m1, const int d1, const int y2, const int m2, const int d2) {
    if (y1 > y2) {
        return true;
    }                       /// Checking for validity of the date values

    if (y1 == y2 && m1 > m2) {
        return true;
    }

    if (y1 == y2 && m1 == m2 && d1 > d2) {
        return true;
    }

    return false;
}


bool is_valid_date(const int y, const int m, const int d) {
    bool is_leap = false;

    if (y < 2000 || m < 1 || m > 12 || d < 1) {     // Function to check date validity
        return false;
    }

    if (is_leap_year(y)) {
        is_leap = true;
        if (m == 2 && d > 29) {
            return false;
        }
    }

    switch (m) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if (d > 31) {
                return false;
            }
            break;

        case 2:
            if (!is_leap && d > 28) {
                return false;
            }
            break;

        case 4:
        case 6:
        case 9:
        case 11:
            if (d > 30) {
                return false;
            }
            break;
    }

    return true;
}


bool isWorkDay(int y, int m, int d) {   // Check if working
    return (is_valid_date(y, m, d) && (day_of_week(y, m, d) < 6) && !is_holiday(m, d));
}


TResult countDays(int y1, int m1, int d1, int y2, int m2, int d2) {
    TResult result;

    if (!is_valid_date(y1, m1, d1) || !is_valid_date(y2, m2, d2) || is_strict_later(y1, m1, d1, y2, m2, d2)) {
        result.m_TotalDays = -1;
        result.m_WorkDays = -1;
        return result;
    }   // Standard check procedure

    result.m_TotalDays = get_day_num(y2, m2, d2) - get_day_num(y1, m1, d1) + 1;
    result.m_WorkDays = get_working_days(y1, m1, d1, y2, m2, d2);

    return result;
}

#ifndef __PROGTEST__
int main(int argc, char * argv[]) {
    TResult r;

    assert(isWorkDay(2023, 10, 10));

    assert(!isWorkDay(2023, 11, 11));

    assert(!isWorkDay(2023, 11, 17));

    assert(!isWorkDay(2023, 11, 31));

    assert(!isWorkDay(2023, 2, 29));

    assert(!isWorkDay(2004, 2, 29));

    assert(isWorkDay(2008, 2, 29));

    assert(!isWorkDay(2001, 2, 29));

    assert(!isWorkDay(1996, 1, 2));

    r = countDays(2023, 11, 1,
                  2023, 11, 30);
    assert(r.m_TotalDays == 30);
    assert(r.m_WorkDays == 21);

    r = countDays(2023, 11, 1,
                  2023, 11, 17);
    assert(r.m_TotalDays == 17);
    assert(r.m_WorkDays == 12);

    r = countDays(2023, 11, 1,
                  2023, 11, 1);
    assert(r.m_TotalDays == 1);
    assert(r.m_WorkDays == 1);

    r = countDays(2023, 11, 17,
                  2023, 11, 17);
    assert(r.m_TotalDays == 1);
    assert(r.m_WorkDays == 0);

    r = countDays(2023, 1, 1,
                  2023, 12, 31);
    assert(r.m_TotalDays == 365);
    assert(r.m_WorkDays == 252);

    r = countDays(2024, 1, 1,
                  2024, 12, 31);
    assert(r.m_TotalDays == 366);
    assert(r.m_WorkDays == 254);

    r = countDays(2000, 1, 1,
                  2023, 12, 31);
    assert(r.m_TotalDays == 8766);
    assert(r.m_WorkDays == 6072);

    r = countDays(2001, 2, 3,
                  2023, 7, 18);
    assert(r.m_TotalDays == 8201);
    assert(r.m_WorkDays == 5682);

    r = countDays(2021, 3, 31,
                  2023, 11, 12);
    assert(r.m_TotalDays == 957);
    assert(r.m_WorkDays == 666);

    r = countDays(2001, 1, 1,
                  2000, 1, 1);
    assert(r.m_TotalDays == -1);
    assert(r.m_WorkDays == -1);

    r = countDays(2001, 1, 1,
                  2023, 2, 29);
    assert(r.m_TotalDays == -1);
    assert(r.m_WorkDays == -1);


    r = countDays(2428, 10, 9, 3335, 10, 13);
    assert(r.m_TotalDays == 331279);
    assert(r.m_WorkDays == 229503);


    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
