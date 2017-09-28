#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cassert>
#include <cfloat>
#include <stdbool.h>

enum {
    LINEAREQ = -1
    };

const double POISON = tan(M_PI/2);
const double EPS = DBL_EPSILON;
int SquareSolver(double a, double b, double c, double* x1, double* x2);
int IsZero(double littleNum);

//inline void func(); gcc -std.99
//DOXIGEN

int main() {
        printf("Hello, I'm square equilibration's solver.\n"
                    "Enter coffs of ax^2+bx+c=0 this way: 'a b c'\n");

        double a = POISON, b = POISON, c = POISON;
        int nCoffs = scanf("%lg %lg %lg", &a, &b, &c); 
        if (nCoffs != 3) {
                printf("Scanf error\n");
                return -1;
            }

        double root1 = POISON, root2 = POISON;
        int nRoots = SquareSolver(a, b, c, &root1, &root2);
        switch (nRoots) {
            case 0:
               printf("The equilibration has no solutions\n");
               break;
            case 1:
               printf("There is only one root: %lg\n", root1);
               break;
            case 2:
               printf("There are two roots: %lg, %lg\n", root1, root2);
               break;
            case 3:
               printf("The solution is in Real\n");
               break;
            default:
               printf("You ec. has no square member. "
                              "solution: %lg\n", root1);
               break;
        }
        return 0;
}

int SquareSolver(double a, double b, double c, double* x1, double* x2) {
        assert(x1 != NULL);
        assert(x2 != NULL);
        assert(x1 != x2);
        if (IsZero(a)) {
            if (IsZero(b + c)) { // CYKA BLYAT
                return 3;
            }
            *x1 = 0-c/b;
            return LINEAREQ;
        } else {
        double d = b*b - 4*a*c;
        if (IsZero(d)) {
            *x1 = -b/(2*a);
            return 1;
        }
        if (d < 0) {
            return 0;
            }
            *x1 = (-b + sqrt(d))/(2*a);
            *x2 = (-b - sqrt(d))/(2*a);
            return 2; // tabulyatsia poyehala
        }
}

int IsZero(double littleNum) {
        return fabs(littleNum) <= EPS;
}



