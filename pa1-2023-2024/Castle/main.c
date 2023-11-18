#ifndef __PROGTEST__
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAP_MAX 200
#endif /* __PROGTEST__ */

#define min (int)(INT_MIN / 100000)
#define max (int)(INT_MAX / 100000)


void make_influence_map(int ** const __restrict influence_map, int altitude[][MAP_MAX], const int size, const int x, const int y) {
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            if (altitude[i][j] < altitude[y][x]) {
                influence_map[i][j] = 1;
            } else {
                influence_map[i][j] = min;
            }
        }
    }
    influence_map[y][x] = max;
}


int kadane(const int * const __restrict arr, int * const __restrict start, int * const __restrict finish, const int size) {
    int sum = 0;
    int max_sum = INT_MIN;
    int i = 0;

    *finish = -1;
    int local_start = 0;

    for (i = 0; i < size; i++) {
        sum += arr[i];

        if (sum < 0) {
            sum = 0;
            local_start = i + 1;
        } else if (sum > max_sum) {
            max_sum = sum;
            *start = local_start;
            *finish = i;
        }
    }

    if (*finish != -1) {
        return max_sum;
    }

    max_sum = arr[0];
    *start = *finish = 0;

    for (i = 1; i < size; i++) {
        if (arr[i] > max_sum) {
            max_sum = arr[i];
            *start = *finish = i;
        }
    }

    return max_sum;
}


int findMaxSum(int ** const __restrict influence_map, const int size) {
    int max_sum = INT_MIN;
    int left = 0;
    int right = 0;
    int i = 0;
    int * temp = (int*)calloc(size, sizeof(int ));
    int sum = 0;
    int start = 0;
    int finish = 0;

    for (left = 0; left < size; left++) {
        free(temp);
        temp = (int*)calloc(size, sizeof(int ));

        for (right = left; right < size; right++) {
            for (i = 0; i < size; i++) {
                temp[i] += influence_map[i][right];
            }

            sum = kadane(temp, &start, &finish, size);

            if (sum > max_sum) {
                max_sum = sum;
            }
        }
    }

    free(temp);

    return max_sum - max + 1;
}


void castleArea(int altitude[][200], const int size, int area[][200]) {
    int ** influence_map = (int**)malloc(sizeof(int*) * size);

    for (int i__ = 0; i__<size; i__++) {
        influence_map[i__] = (int*)malloc(size * sizeof(int));
    }

    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            make_influence_map(influence_map, altitude, size, j, i);

            area[i][j] = findMaxSum(influence_map, size);
        }
    }

    for (int i=0; i<size; i++) {
        free(influence_map[i]);
    }
    free(influence_map);
}


#ifndef __PROGTEST__
bool identicalMap(const int a[][MAP_MAX], const int b[][MAP_MAX], int n) {
    for (unsigned i=0; i<n; i++) {
        for (unsigned j=0; j<n; j++) {
            if (a[i][j] != b[i][j]) {
                return false;
            }
        }
    }

    return true;
}

int main(int argc, char *argv[]) {
    static int result[MAP_MAX][MAP_MAX];

    static int alt0[MAP_MAX][MAP_MAX] =
            {
                    {1, 2},
                    {3, 4}};
    static int area0[MAP_MAX][MAP_MAX] =
            {
                    {1, 2},
                    {2, 4}};
    castleArea(alt0, 2, result);
    assert(identicalMap(result, area0, 2));
    static int alt1[MAP_MAX][MAP_MAX] =
            {
                    {2, 7, 1, 9},
                    {3, 5, 0, 2},
                    {1, 6, 3, 5},
                    {1, 2, 2, 8}};
    static int area1[MAP_MAX][MAP_MAX] =
            {
                    {1, 12, 2, 16},
                    {4, 4, 1, 2},
                    {1, 9, 4, 4},
                    {1, 2, 1, 12}};
    castleArea(alt1, 4, result);
    assert(identicalMap(result, area1, 4));
    static int alt2[MAP_MAX][MAP_MAX] =
            {
                    {1, 2, 3, 4},
                    {2, 3, 4, 5},
                    {3, 4, 5, 6},
                    {4, 5, 6, 7}};
    static int area2[MAP_MAX][MAP_MAX] =
            {
                    {1, 2, 3, 4},
                    {2, 4, 6, 8},
                    {3, 6, 9, 12},
                    {4, 8, 12, 16}};
    castleArea(alt2, 4, result);
    assert(identicalMap(result, area2, 4));
    static int alt3[MAP_MAX][MAP_MAX] =
            {
                    {7, 6, 5, 4},
                    {6, 5, 4, 5},
                    {5, 4, 5, 6},
                    {4, 5, 6, 7}};
    static int area3[MAP_MAX][MAP_MAX] =
            {
                    {12, 6, 2, 1},
                    {6, 2, 1, 2},
                    {2, 1, 2, 6},
                    {1, 2, 6, 12}};
    castleArea(alt3, 4, result);
    assert(identicalMap(result, area3, 4));
    static int alt4[MAP_MAX][MAP_MAX] =
            {
                    {1, 1, 1, 1, 1},
                    {1, 1, 1, 1, 1},
                    {1, 1, 2, 1, 1},
                    {1, 1, 1, 1, 1},
                    {1, 1, 1, 1, 1}};
    static int area4[MAP_MAX][MAP_MAX] =
            {
                    {1, 1, 1, 1, 1},
                    {1, 1, 1, 1, 1},
                    {1, 1, 25, 1, 1},
                    {1, 1, 1, 1, 1},
                    {1, 1, 1, 1, 1}};
    castleArea(alt4, 5, result);
    assert(identicalMap(result, area4, 5));
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
