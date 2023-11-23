#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct Comment {   
    char comment[4098];
    size_t rating;
    struct Comment * next;
} Comment;

typedef struct Review {    
    int year;              
    int month;
    int day;

    Comment * head; 
    Comment * tail;
} Review;

void emplace_back_comment(Review * review, char * comment, const size_t rating) { 
    Comment * new__ = (Comment*)malloc(sizeof(Comment));   
    new__->next = NULL;
    new__->rating = rating;
    memcpy(new__->comment, comment, sizeof(review->head->comment));

    if (review->head == NULL) {
        review->head = review->tail = new__;
    } else {
        review->tail->next = new__;
        review->tail = review->tail->next;
    }
}

void clear_comment_linked_list(Review * review) {
    Comment * temp = review->head;
    Comment * next;
    while (temp != NULL) {
        next = temp->next;
        free(temp);
        temp = next;
    }
    review->head = NULL;
    review->tail = NULL;
}

typedef Review type;        // Some kind of template class

typedef struct Vector {
    size_t size;            // Size of the vector
    size_t capacity;        // Total capacity of the vector
    size_t num_of_revs;
    type * arr;             // The values it stores

    // The functions below are self describing.
    void (*emplace_back)     (struct Vector * const __restrict self, const type * const __restrict val, char * comment, const size_t rating);
    int (*clear)            (struct Vector * const __restrict self);
    type*(*at)              (struct Vector * const __restrict self, const size_t index);
} Vector;


int clear_vector(Vector * const __restrict self) {  // Also clears the vector, just naming matters
    if (self->arr != NULL) {
        for (size_t i=0; i<self->size; i++) {
            clear_comment_linked_list(&self->arr[i]);   // We clear comments under each review
        }
        free(self->arr);            // We clear the vector
        self->capacity = 0;
        self->size = 0;
        self->num_of_revs = 0;
    }
    return 0;
}


type * at_vector(Vector * const __restrict self, const size_t index) {  // Returning pointer to the value at index
    return &self->arr[index];
}


void emplace_back_vector(Vector * const __restrict self, const type * const __restrict val, char * comment, const size_t rating) {
    if (!self->arr) {
        self->arr = (type *)malloc(sizeof(type));       
        self->capacity = 1;
    } else if (self->size + 1 > self->capacity) {
        self->arr = (type *)realloc(self->arr, sizeof(type) * self->capacity * 2);  
        self->capacity *= 2;
    }

    if (self->size) {        
        type * last_review = &self->arr[self->size - 1];
        if (last_review->year == val->year && last_review->month == val->month && last_review->day == val->day) {
            emplace_back_comment(last_review, comment, rating);
            self->num_of_revs++;
            return;
        }
    }

    self->arr[self->size] = *val;
    emplace_back_comment(&self->arr[self->size], comment, rating);
    self->size++;       // Increment values
    self->num_of_revs++;
}


void init_vector(Vector * const __restrict self) { 
    self->at = at_vector;
    self->clear = clear_vector;
    self->emplace_back = emplace_back_vector;
    self->arr = NULL;
    self->size = 0;
    self->num_of_revs = 0;
    self->capacity = 0;
}

void swap(size_t * const a, size_t * const b) {   
    size_t temp = *a;
    *a = *b;
    *b = temp;
}

size_t partition(size_t arr[], size_t low, size_t high) {
    size_t pivot = arr[high];
    size_t i = (low - 1);

    for (size_t j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]); 
    return i + 1; 
}


size_t quick_select(size_t arr[], size_t low, size_t high, size_t k) {
    if (low == high) {
        return arr[low];
    }

    size_t pivot_index = partition(arr, low, high);

    if (k == pivot_index) { 
        return arr[k];
    } else if (k < pivot_index) {  
        return quick_select(arr, low, pivot_index - 1, k);
    } else {
        return quick_select(arr, pivot_index + 1, high, k);
    }
}


size_t get_median(size_t arr[], size_t n) {
    return quick_select(arr, 0, n - 1, (n - 1) / 2);
}


int isequal(const double a, const double b) {
    return fabs(a - b) <= 1e-9 * (fabs(a) + fabs(b));
}


bool is_strict_later(const int y1, const int m1, const int d1, const int y2, const int m2, const int d2) {
    if (y1 > y2) {
        return true;
    }                       

    if (y1 == y2 && m1 > m2) {
        return true;
    }

    if (y1 == y2 && m1 == m2 && d1 > d2) {
        return true;
    }

    return false;
}


bool is_leap_year(const int y) {
    return ((y % 4 == 0) && ((y % 400 == 0) || (y % 100 != 0)) && (y % 4000 != 0));
}


bool is_valid_date(const int y, const int m, const int d) {
    bool is_leap = is_leap_year(y);

    if (y < 1800 || y > 3000 || m < 1 || m > 12 || d < 1) {
        return false;
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
            } else if (is_leap && d > 29) {
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


void search(Vector * reviews, const char operation, const size_t interval) {
    Review * largest_from = &reviews->arr[0];
    Review * largest_to = &reviews->arr[0];
    double largest_average = 0;
    size_t largest_medium = 0;
    size_t * ratings = (size_t *)malloc(sizeof(size_t) * reviews->num_of_revs);
    size_t largest_rating = 0;
    double diff = 0;

    for (size_t r=0; r<reviews->size; r++) {
        size_t reviews_counted = 0;
        size_t total_rating = 0;
        size_t medium;
        double average;

        for (size_t i=r; i<reviews->size; i++) {
            for (Comment * ptr = reviews->arr[i].head; ptr != NULL; ptr = ptr->next) { 
                ratings[reviews_counted] = ptr->rating; 
                reviews_counted++;
                total_rating += ptr->rating;
            }

            if (reviews_counted >= interval) {
                size_t * actual_ratings = (size_t *)malloc(sizeof(size_t)*reviews_counted);
                memcpy(actual_ratings, ratings, sizeof(size_t)*reviews_counted);
                medium = get_median(actual_ratings, reviews_counted);
                average = (double)total_rating / (double)reviews_counted;

                double l_diff = fabs((double)medium - average);         

                if (l_diff > diff) {
                    diff = l_diff;      
                    largest_to = &reviews->arr[i];
                    largest_from = &reviews->arr[r];
                    largest_medium = medium;
                    largest_average = average;
                    largest_rating = total_rating;
                } else if (isequal(l_diff, diff)) { 
                    if (&reviews->arr[i] > largest_to) { 
                        largest_to = &reviews->arr[i];  
                        largest_from = &reviews->arr[r];
                        largest_medium = medium;
                        largest_rating = total_rating;
                        largest_average = average;
                    } else if (&reviews->arr[i] == largest_to && total_rating > largest_rating) {
                        largest_rating = total_rating;  
                        largest_average = average;  
                        largest_medium = medium;
                    }
                }

                free(actual_ratings); 
            }
        }
    }

    free(ratings); 

    printf("%d-%02d-%02d - %d-%02d-%02d: %.6lf %zu\n", largest_from->year, largest_from->month, largest_from->day, largest_to->year, largest_to->month, largest_to->day, largest_average, largest_medium);

    if (operation == '?') {
        for (Review * ptr = largest_from; ptr <= largest_to; ptr++) {
            for (Comment * ptr__ = ptr->head; ptr__ != NULL; ptr__ = ptr__->next) {
                printf("  %zu: %s\n", ptr__->rating, ptr__->comment);
            }
        }
    }
}


int main(void) {
    Vector reviews;         
    init_vector(&reviews);

    char operation;
    int y, d, m; long long c, r;
    char txt[4098];

    printf("Reviews:\n");

    while (true) {
        if (scanf(" %c", &operation) != 1) {
            if (feof(stdin)) {
                goto ending;
            }
            printf("Invalid input.\n");
            reviews.clear(&reviews);
            return EXIT_FAILURE;
        }

        switch (operation) {
            case '+': {
                if (scanf(" %d-%d-%d %lld", &y, &m, &d, &r) != 4 || r < 1 || !is_valid_date(y, m, d)) {
                    if (feof(stdin)) {
                        goto ending;
                    }
                    printf("Invalid input.\n");
                    reviews.clear(&reviews);
                    return EXIT_FAILURE;
                }

                if (scanf("%4097s", txt) != 1) {
                    if (feof(stdin)) {
                        goto ending;
                    }
                    printf("Invalid input.\n");
                    reviews.clear(&reviews);
                    return EXIT_FAILURE;
                }

                if (reviews.size > 0) {
                    Review * __rev = reviews.at(&reviews, reviews.size-1);
                    if (is_strict_later(__rev->year, __rev->month, __rev->day, y, m, d)) {
                        printf("Invalid input.\n");
                        reviews.clear(&reviews);
                        return EXIT_FAILURE;
                    }
                }

                {
                    Review __rev;
                    __rev.day = d;
                    __rev.month = m;
                    __rev.year = y;
                    __rev.tail = NULL;
                    __rev.head = NULL;
                    reviews.emplace_back(&reviews, &__rev, txt, r);
                }

                break;
            }

            case '#':
            case '?': {
                if (scanf(" %lld", &c) != 1 || c < 1 || reviews.num_of_revs == 0) {
                    if (feof(stdin)) {
                        goto ending;
                    }
                    printf("Invalid input.\n");
                    reviews.clear(&reviews);
                    return EXIT_FAILURE;
                }

                if ((size_t)c > reviews.num_of_revs) {
                    printf("No interval.\n");
                    break;
                }

                search(&reviews, operation, c);
                break;
            }

            default: {
                printf("Invalid input.\n");
                reviews.clear(&reviews);
                return EXIT_FAILURE;
            }
        }
    }

    ending:             // The end
    reviews.clear(&reviews);
    return EXIT_SUCCESS;
}