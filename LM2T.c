#include <stdio.h>
#include <string.h>

int main() {
    int vars;
    int Form;
    int Anzahl;
    int i;
    char Term[100][64]; // Assuming a maximum of 100 terms

     printf("Enter number of variables:\n");
    scanf_s("%d", &vars);
    printf("Enter number of terms:(max 100)\n");
    scanf_s("%d", &Anzahl);
    for (i = 0; i < Anzahl; i++) {
        do {
            printf("Enter term %d:\n", i);
            scanf_s("%63s", &Term[i][0], 64);
            if(strlen(Term[i]) != vars) {
                printf("Invalid term length. Please enter a term with %d variables.\n", vars);
            }
        } while (strlen(Term[i]) != vars);
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