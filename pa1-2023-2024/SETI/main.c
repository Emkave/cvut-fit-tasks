#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define checkpoint printf("Checkpoint\n");
#define ll long long

typedef struct Properties {
    ll string_size;
    ll prefix_size;
} Properties;

int status = 0;

ll extended_gcd(ll a, ll b, ll *x, ll *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }

    ll x1, y1;
    ll g = extended_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

void solve_equation(ll a, ll b, ll c, ll *x, ll *y) {
    ll g = extended_gcd(a, b, x, y);

    if (c % g != 0) {
        status = 0;
        return;
    }

    c /= g;
    *x = *x * c;
    *y = *x * c;

    status = 1;
}

ll gcd(ll a, ll b) {
    if (b == 0) {
        return a;
    }

    return gcd(b, a % b);
}

int main(void) {
    char ch;
    unsigned short bar_num = 0;

    ll time_unit_alphabet[26];
    time_unit_alphabet[0] = 1;

    for (unsigned short i = 1; i < 26; i++) {
        time_unit_alphabet[i] = time_unit_alphabet[i - 1] * 2;
    }

    Properties properties[2];

    ll i = 0;
    printf("Messages:\n");
    while (i < 2) {
        bar_num = 0;
        properties[i].string_size = 0;
        properties[i].prefix_size = 0;

        while ((ch = getchar()) && ch != EOF) {
            if (feof(stdin) || ch == '\n') {
                break;
            }

            if ((ch < 'a' || ch > 'z') && ch != '|') {
                printf("Invalid input.\n");
                return 1;
            }

            if (ch == '|') {
                if (++bar_num != 1) {
                    printf("Invalid input.\n");
                    return 1;
                }
                properties[i].prefix_size = properties[i].string_size;
            } else {
                properties[i].string_size += time_unit_alphabet[ch - 'a'];
            }
        }

        if (feof(stdin)) {
            break;
        }

        if (bar_num == 0) {
            printf("Invalid input.\n");
            return 1;
        }
        i++;
    }

    if (i < 2 || properties[0].string_size == 0 || properties[1].string_size == 0) {
        printf("Invalid input.\n");
        return 1;
    }

    if (properties[0].string_size != 0 && properties[1].string_size != 0 && properties[0].prefix_size == 0 && properties[1].prefix_size == 0) {
        printf("Synchronized in: 0\n");
        return 0;
    }

    if (properties[0].string_size == properties[0].prefix_size && properties[1].string_size == properties[1].prefix_size) {
        printf("Synchronized in: 0\n");
        return 0;
    }

    if (properties[0].string_size == properties[1].string_size && properties[0].prefix_size != 0 && properties[1].prefix_size == 0) {
        printf("Synchronized in: 0\n");
        return 0;
    }

    // The equation
    ll x;
    ll y;
    ll a = properties[0].string_size;
    ll b = properties[1].string_size * -1;
    ll c = properties[1].prefix_size - properties[0].prefix_size;

    solve_equation(a, b, c, &x, &y);

    if (!status) {
        printf("Never synchronizes.\n");
        return 0;
    }

    ll g = gcd(a, b);
    ll add = b / g;
    ll num = 0;

    if (add < 0) {
        num = (ll)floor(1.0 * (double)-x / (double)add);
    } else {
        num = (ll)ceil(1.0 * (double)-x / (double)add);
    }

    x = x + b / g * num;
    y = y - a / g * num;

    printf("Synchronized in: %lld\n", a * x + properties[0].prefix_size);

    return 0;
}
