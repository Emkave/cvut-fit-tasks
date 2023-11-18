#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int main(int argc, const char * argv[]) {

    typedef struct UserLog {
        int id; 
    } userLog;

    typedef struct UserLoginHistory {
        int visits;
    } userloghistory;

    userloghistory userLogHist[100000];
    userLog user[1000000];

    int start;
    int end;
    int uniques;
    char requestType;
    int totalVisits = 0;
    int index = 0;

    for (int i=0; i<100000; i++) {
        userLogHist[i].visits = 0;
    }
    
    printf("Requests:\n");

    while (scanf(" %c", &requestType) != EOF) {
        switch (requestType) {
            case '+':
                if (scanf(" %d", &user[index].id) != 1 || user[index].id < 0 || user[index].id > 99999 || totalVisits >= 1000000) {
                    printf("Invalid input.\n");
                    return 0;
                } else {
                    userLogHist[user[index].id].visits++;
                    if (userLogHist[user[index].id].visits == 1) {
                        printf("> first visit\n");
                    } else {
                        printf("> visit #%d\n", userLogHist[user[index].id].visits);
                    }
                    index++;
                    totalVisits++;
                }
                break;
        
            case '?':
                    if (scanf(" %d %d", &start, &end) != 2 || start < 0 || start > totalVisits - 1 || end < start || end < 0 || end > totalVisits - 1) {
                        printf("Invalid input.\n");
                        return 0;
                        break;
                    } else {
                        uniques = end - start + 1;
                        for (int i=start; i<=end; i++) {
                            for (int j=i; j<=end; j++) {
                                if (user[i].id == user[j].id && i != j) {
                                    uniques--;
                                    break;
                                }
                            }
                        }
                        printf("> %d / %d\n", uniques, end - start + 1);
                    }
                break;
            
            default:
                printf("Invalid input.\n");
                return 0;
        }
    }
    return 0;
}

