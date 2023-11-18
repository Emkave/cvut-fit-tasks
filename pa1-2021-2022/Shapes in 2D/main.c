#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define epsilon __DBL_EPSILON__

int main(int argc, const char * argv[]) {

    char figureIdentifier;
    char activatedFigures[2] = {};

    double biggestP;
    double biggestA;
    double halfPerimeter;
 
    typedef struct FigureData {
        double firstSide;
        double secondSide;
        double thirdSide;
        double area;
        double perimeter;
        
    } figurePrefs;

    figurePrefs firstFigure;
    figurePrefs secondFigure;

    printf("Shape #1\n");
    scanf(" %s", &figureIdentifier);

    switch (figureIdentifier) {
        case 'S':
            if (scanf(" %lg", &firstFigure.firstSide) == 1)
                if (firstFigure.firstSide > 0) {
                    firstFigure.area = firstFigure.firstSide * firstFigure.firstSide;
                    firstFigure.perimeter = firstFigure.firstSide * 4;
                    activatedFigures[0] = 's';
                    break;
                }
            printf("Invalid input.\n");
            return 1;
            break;
        
        case 'R':
            if (scanf(" %lg %lg", &firstFigure.firstSide, &firstFigure.secondSide) == 2)
                if ((firstFigure.firstSide > 0 && firstFigure.secondSide > 0) && (fabs(firstFigure.firstSide - firstFigure.secondSide) > __DBL_EPSILON__ * 1000 * firstFigure.firstSide)) {
                    firstFigure.area = firstFigure.firstSide * firstFigure.secondSide;
                    firstFigure.perimeter = (firstFigure.firstSide + firstFigure.secondSide) * 2;
                    activatedFigures[0] = 'r';
                    break;
                }
            printf("Invalid input.\n");
            return 1;
            break;
        
        case 'T':
            if (scanf(" %lg %lg %lg", &firstFigure.firstSide, &firstFigure.secondSide, &firstFigure.thirdSide) == 3)
                if (firstFigure.firstSide > 0 && firstFigure.secondSide > 0 && firstFigure.thirdSide > 0)
                    if (((1 - epsilon)*(firstFigure.firstSide + firstFigure.secondSide) > firstFigure.thirdSide) && ((1 - epsilon)*(firstFigure.firstSide + firstFigure.thirdSide) > firstFigure.secondSide) && ((1 - epsilon)*(firstFigure.secondSide + firstFigure.thirdSide) > firstFigure.firstSide)){
                        firstFigure.perimeter = firstFigure.firstSide + firstFigure.secondSide + firstFigure.thirdSide;
                        halfPerimeter = firstFigure.perimeter / 2;
                        firstFigure.area = sqrt(halfPerimeter*(halfPerimeter-firstFigure.firstSide)*(halfPerimeter-firstFigure.secondSide)*(halfPerimeter-firstFigure.thirdSide));
                        activatedFigures[0] = 't';
                        break;
                    }
            printf("Invalid input.\n");
            return 1;
            break;
        
        default:
            printf("Invalid input.\n");
            return 1;
            break;
    }

    printf("Shape #2\n");
    scanf(" %s", &figureIdentifier);

    switch (figureIdentifier) {
        case 'S':
            if (scanf(" %lg", &secondFigure.firstSide) == 1)
                if (secondFigure.firstSide > 0) {
                    secondFigure.area = secondFigure.firstSide * secondFigure.firstSide;
                    secondFigure.perimeter = secondFigure.firstSide * 4;
                    activatedFigures[1] = 's';
                    break;
                }
            printf("Invalid input.\n");
            return 1;
            break;
        
        case 'R':
            if (scanf(" %lg %lg", &secondFigure.firstSide, &secondFigure.secondSide) == 2)
                if ((secondFigure.firstSide > 0 && secondFigure.secondSide > 0) && (fabs(secondFigure.firstSide - secondFigure.secondSide) > __DBL_EPSILON__ * 1000 * secondFigure.firstSide)) {
                    secondFigure.area = secondFigure.firstSide * secondFigure.secondSide;
                    secondFigure.perimeter = (secondFigure.firstSide + secondFigure.secondSide) * 2;
                    activatedFigures[1] = 'r';
                    break;
                }
            printf("Invalid input.\n");
            return 1;
            break;
        
        case 'T':
            if (scanf(" %lg %lg %lg", &secondFigure.firstSide, &secondFigure.secondSide, &secondFigure.thirdSide) == 3)
                if (secondFigure.firstSide > 0 && secondFigure.secondSide > 0 && secondFigure.thirdSide > 0)
                    if (((1 - epsilon)*(secondFigure.firstSide + secondFigure.secondSide) > secondFigure.thirdSide) && ((1 - epsilon)*(secondFigure.firstSide + secondFigure.thirdSide) > secondFigure.secondSide) && ((1 - epsilon)*(secondFigure.secondSide + secondFigure.thirdSide) > secondFigure.firstSide)) {
                        secondFigure.perimeter = secondFigure.firstSide + secondFigure.secondSide + secondFigure.thirdSide;
                        halfPerimeter = secondFigure.perimeter / 2;
                        secondFigure.area = sqrt(halfPerimeter*(halfPerimeter-secondFigure.firstSide)*(halfPerimeter-secondFigure.secondSide)*(halfPerimeter-secondFigure.thirdSide));
                        activatedFigures[1] = 't';
                        break;
                    }
            printf("Invalid input.\n");
            return 1;
            break;
        
        default:
            printf("Invalid input.\n");
            return 1;
            break;
    }

    printf("Perimeter: ");

    switch (activatedFigures[0]) {
        case 's':
            printf("square #1 ");
            break;

        case 'r':
            printf("rectangle #1 ");
            break;
        
        case 't':
            printf("triangle #1 ");
            break;
    }

    if (firstFigure.perimeter > secondFigure.perimeter) {
        biggestP = firstFigure.perimeter;
    } else {
        biggestP = secondFigure.perimeter;
    }
    if (firstFigure.area > secondFigure.area) {
        biggestA = firstFigure.area;
    } else {
        biggestA = secondFigure.area;
    }

    if (fabs(firstFigure.perimeter - secondFigure.perimeter) <= __DBL_EPSILON__ * 1000 * biggestP) {
        printf("= ");
    } else if (firstFigure.perimeter - secondFigure.perimeter >  __DBL_EPSILON__ * 1000 * biggestP) {
        printf("> ");
    } else {
        printf("< ");
    }

    switch (activatedFigures[1]) {
        case 's':
            printf("square #2\n");
            break;

        case 'r':
            printf("rectangle #2\n");
            break;
        
        case 't':
            printf("triangle #2\n");
            break;
    }

    printf("Area: ");

    switch (activatedFigures[0]) {
        case 's':
            printf("square #1 ");
            break;

        case 'r':
            printf("rectangle #1 ");
            break;
        
        case 't':
            printf("triangle #1 ");
            break;
    }

    
    if (fabs(firstFigure.area - secondFigure.area) <= __DBL_EPSILON__ * 1000 * biggestA) {
        printf("= ");
    } else if (firstFigure.area - secondFigure.area > __DBL_EPSILON__ * 1000 * biggestA) {
        printf("> ");
    } else {
        printf("< ");
    }

    switch (activatedFigures[1]) {
        case 's':
            printf("square #2\n");
            break;

        case 'r':
            printf("rectangle #2\n");
            break;
        
        case 't':
            printf("triangle #2\n");
            break;
    }
    return 0;
}
