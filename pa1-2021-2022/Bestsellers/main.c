#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//%99s%c == \n

typedef struct {
    char request[100];
    int amount; 
} Request ;

int Comparisement(Request * first, Request * second) {

    if (first->amount > second->amount) {
        return -1;
    } else if (first->amount == second->amount) {
        return 0;
    } else {
        return 1;
    }
}

int main(int argc, const char * argv[]) {

    char rq[100];
    char testing;
    char requestType;
    int topTier = 0;
    int totalOrders = 0;
    int currentTier = 1;
    int potentialTier = 1;
    int entireOrders = 0;
    int scalar = 10;
    Request * order = (Request*)malloc(sizeof(Request)*scalar);

    printf("Top N:\n");
    if (scanf(" %d", &topTier) != 1 || topTier <= 0) {
        printf("Invalid input.\n");
        free(order);
        return 0;
    } else {
        printf("Requests:\n");
        while (scanf(" %c", &requestType) != EOF) {
            switch(requestType) {
                case '+':
                        if (scanf(" %99s%c", rq, &testing) != 2 || testing != '\n' ) {
                            printf("Invalid input.\n");
                            free(order);
                            return 0;
                        } else {
                            if (totalOrders >= scalar) {
                                scalar *= 2;
                                order = (Request*)realloc(order, sizeof(Request) * scalar);
                            }

                            if (totalOrders == 0) {
                                totalOrders++;
                                order[0].amount = 1;
                                strcpy(order[0].request, rq);
                            } else {
                                for (int i=0; i<totalOrders; i++) {
                                    if (!strcmp(order[i].request, rq)) {
                                        order[i].amount++;
                                        break;
                                    } else if (i+1 == totalOrders) {
                                        strcpy(order[totalOrders].request, rq);
                                        order[totalOrders].amount = 1;
                                        totalOrders++;
                                        break;
                                    }
                                }
                            }
                        }
                        
                    break;
                
                case '#':
                    qsort(order, totalOrders, sizeof(Request), (int(*)(const void *, const void *))Comparisement);
                    
                    for (int i=0; i<totalOrders; i++) {
                        if (i+1 < totalOrders && order[i+1].amount == order[i].amount && i < topTier) {
                            currentTier = i;
                            potentialTier = currentTier;
                            while (i+1 < totalOrders && order[potentialTier].amount == order[potentialTier+1].amount) {
                                potentialTier++;
                            }
                            for (int j=currentTier; j<=potentialTier; j++) {
                                printf("%d.-%d. %s, %dx\n", currentTier+1, potentialTier+1, order[j].request, order[j].amount);
                                entireOrders += order[j].amount;
                            }
                            i = potentialTier;
                        } else {
                            if (i >= topTier) {
                                break;
                            }
                            printf("%d. %s, %dx\n", i+1, order[i].request, order[i].amount);
                            entireOrders += order[i].amount;
                        }
                    }
                    printf("Top sellers: sold %d items\n", entireOrders);

                    entireOrders = 0;
                    currentTier = 1;
                    entireOrders = 0;
                    break;
                
                case '?':
                    qsort(order, totalOrders, sizeof(Request), (int(*)(const void *, const void *))Comparisement);
                    for (int i=0; i<totalOrders; i++) {
                        if (i+1 < totalOrders && order[i].amount == order[i+1].amount && i < topTier) {
                            currentTier = i;
                            potentialTier = currentTier;
                            while (i+1 < totalOrders && order[potentialTier].amount == order[potentialTier+1].amount) {
                                potentialTier++;
                            }
                            for (int j=currentTier; j<=potentialTier; j++) {
                                entireOrders += order[j].amount;
                            }
                            i = potentialTier;
                        } else {
                            if (i >= topTier) {
                                break;
                            }
                            entireOrders += order[i].amount;
                        }
                    }
                    printf("Top sellers: sold %d items\n", entireOrders);

                    entireOrders = 0;
                    currentTier = 1;
                    entireOrders = 0;
                    break;
                
                default:
                    printf("Invalid input.\n");
                    free(order);
                    return 0;
            }
        }
    }

    free(order);
    
    return 0;
}
