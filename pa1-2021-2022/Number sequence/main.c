#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int main(int argc, const char * argv[]) {
    long int position;
    long int number;
    long int numberHolder1;
    long int numberHolder2;
    long int index;
    int spaceIterations;
    int factor = 1;
    int radix;
    int scnStatus;
    bool status = true;
    bool system = true;
    bool correctStart = false;

    printf("Position and radix:\n");

    while (system) {
        scnStatus = scanf(" %ld %d", &position, &radix);
        if (scnStatus != EOF) {
            if (scnStatus == 2 && position >= 0 && radix >= 2 && radix <= 36) {
                correctStart = true;
                status = true;
                while (status) {
                    if (number >= 10) {
                        numberHolder1 = number;
                        numberHolder2 = number;
                        while (numberHolder1 != 0) {
                            factor *= 10;
                            numberHolder1 /= 10;
                        }
                        while (factor > 1) {
                            factor /= 10;
                            if (index == position) {
                                printf("%ld\n", number);
                                printf("%*s^\n", spaceIterations, "");
                                status = false;
                                break;
                            }
                            numberHolder2 %= factor;
                            spaceIterations++;
                            index++;
                        }
                        spaceIterations = 0;
                    } else {
                        if (position == index) {
                            printf("%ld\n^\n", number);
                            break;
                            status = false;
                        }
                        index++;
                    }
                    number++;
                }
            } else {
                if (!correctStart) {
                    printf("Invalid input.\n");
                }
                return 1;
            }
        } else {
            return 1;
        }
        position = 0;
        radix = 0;
        numberHolder1 = 0;
        numberHolder2 = 0;
        index = 0;
        number = 0;
        spaceIterations = 0;
        factor = 1;
        scnStatus = 0;
    }

    return 0;
}
