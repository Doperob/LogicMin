#include <stdio.h>
#include <string.h>

typedef struct {
    unsigned int fixedMask;
    unsigned int onesMask;
} Term;

Term parseTerm(const char eingabe[], int vars) {
    Term result;

    result.fixedMask = 0;
    result.onesMask = 0;

    for (int i = 0; i < vars; i++) {
        unsigned int position = (unsigned int)i;
        unsigned int bit = 1U << position;

        if (eingabe[i] == '0' || eingabe[i] == '1') {
            result.fixedMask |= bit;
        }

        if (eingabe[i] == '1') {
            result.onesMask |= bit;
        }
    }

    return result;
}

int main() {
    int vars;
    int Form;
    int Anzahl;
    int i;
    Term terms[100];

    printf("Enter number of variables:\n");
    scanf_s("%d", &vars);
    printf("Enter number of terms:(max 100)\n");
    scanf_s("%d", &Anzahl);
    for (i = 0; i < Anzahl; i++) {
        char eingabe[64];
        int validTerm;

        do {
            printf("Enter term %d:\n", i);
            scanf_s("%63s", eingabe, (unsigned int)sizeof(eingabe));

            if (strlen(eingabe) != (size_t)vars) {
                printf("Invalid term length. Please enter a term with %d variables.\n", vars);
            }

            validTerm = strlen(eingabe) == (size_t)vars;

            if (validTerm) {
                for (int j = 0; j < vars; j++) {
                    if (eingabe[j] != '0' && eingabe[j] != '1') {
                        validTerm = 0;
                        printf("Invalid term. Use only 0 and 1.\n");
                        break;
                    }
                }
            }
        } while (!validTerm);

        terms[i] = parseTerm(eingabe, vars);

        printf(
            "Term %d: fixedMask=%u, onesMask=%u\n",
            i,
            terms[i].fixedMask,
            terms[i].onesMask);
    }
    printf("Enter 0 for KNF or 1 for DNF:\n ");
    scanf_s("%d", &Form);

    if (Form == 0) {
        printf("You selected KNF.\n");
    
    } else if (Form == 1) {
        printf("You selected DNF.\n");
        
    } else {
        printf("Invalid selection. Please enter 0 or 1.\n");
    }
    return 0;
}