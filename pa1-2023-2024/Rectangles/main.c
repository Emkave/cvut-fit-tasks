    #include <stdio.h>
    #include <math.h>

    typedef struct coordinate {
        double x;
        double y;
    } Coordinate;

    typedef struct vector {
        double i;
        double j;
    } Vector;

    Vector create_vector(Coordinate a, Coordinate b) {  //Here we create a vector
        Vector v;
        v.i = b.x - a.x;
        v.j = b.y - a.y;
        return v;
    }

    double dot_product(Vector v1, Vector v2) {      // Calculating cosine angle between vectors
        return v1.i * v2.i + v1.j * v2.j;
    }

    double magnitude(Vector v) {                    // Calculating the lenght of vectors
        return sqrt(v.i * v.i + v.j * v.j);
    }

    double angle_between(Vector v1, Vector v2) {        //Using inverse cosine we calculate the angle between the vectors
        return acos(dot_product(v1, v2) / (magnitude(v1) * magnitude(v2)));
    }

    int isequal(const double a, const double b) {       // Check for equality on relative atomic level
        double diff = fabs(a - b);
        return diff <= 1e-9 * (fabs(a) + fabs(b));
    }

    int main(void) {
        Coordinate points[4];
        char last;
        double lengths[4];
        Vector vectors[4];

        for (unsigned short i = 0; i < 4; i++) {
            printf("Point #%d:\n", i + 1);

            if (scanf(" ( %lf , %lf", &points[i].x, &points[i].y) != 2) {
                printf("Invalid input.\n");
                return 1;
            }

            if (scanf(" %c", &last) != 1 || last != ')') {
                printf("Invalid input.\n");
                return 1;
            }
        }

        for (int i = 0; i < 4; i++) {       // Create vectors and assign their lenght
            vectors[i] = create_vector(points[i], points[(i + 1) % 4]);
            lengths[i] = magnitude(vectors[i]);
        }

        if (!isequal(lengths[0], lengths[2]) || !isequal(lengths[1], lengths[3])) { // Check if any of opposite sides are not equal
            printf("The points do not form a rectangle.\n");
            return 0;
        }

        for (int i = 0; i < 4; i++) {
            if (!isequal(angle_between(vectors[i], vectors[(i + 1) % 4]), M_PI / 2)) {  // Check for the angle between vectors, which should be basically PI/2 (90 degrees)
                printf("The points do not form a rectangle.\n");
                return 0;
            }
        }

        printf("The points form a rectangle.\n");
        return 0;
    }
