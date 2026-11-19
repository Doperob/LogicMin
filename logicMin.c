#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// expands all terms to min/max terms
// then saves into array as int
int parse(const char *term, u_int8_t *terms, int vars, int index) {
    if (strlen(term)!=vars){
        printf("term size does not fit specified variable count\n");
        return -1;
    }
    int i;
    for (i = 0; i < vars; i++) {
        if (term[i] == '_') break;
        if (term[i] != '_' && term[i] != '1' && term[i] != '0'){
            printf("unknown char\n");
            return -1;
        }
    }
    if (i == vars) {
        for (int x=0;x<vars;x++) {
            terms[index++] = term[x] - '0';
        }
        return index;
    }
    
    char modified[32];
    strcpy(modified, term);
    modified[i] = '0';
    index = parse(modified, terms, vars, index);
    modified[i] = '1';
    index = parse(modified, terms, vars, index);
    return index;
}

// helpers
void printout(const u_int8_t *terms,int count){
  for (int i=0;i<count;i++){
    printf("%d",terms[i]);
  }
   printf("\n");
}

// implicant / implicat stuct to save the minimized terms 
// (-1 for don't care)
struct {
    int term[32];
    bool minimized;
    bool minimizing;
} Im;

// check for difference between two terms by one
bool diff(const u_int8_t *term1, const u_int8_t *term2, int vars, int *diffIndex){
    int diffCount = 0;
    for (int i=0;i<vars;i++){
        if (term1[i] == term2[i]){
            diffCount++;
            *diffIndex = i;
        }
    }
    return (diffCount==1);
}

// Quine-McCluskey minimization
int quineMcCluskey(const u_int8_t *terms, int count, int vars){
    int numMinTerms = count/vars;
    
    return 0;
}

// first param is num of variables
// second param is 1 for DNF and 0 for KNF
// the folowing are the terms
int main(int argc,char *argv[]){
  if (argc<4){
    printf("Please give all Parameters:\n\n");
    printf("\tfirst: number of variables\n");
    printf("\tsecond: 1 for DNF or 0 for KNF\n");
    printf("\tlast: all terms with 0 being negation and 1 being normal\n\n");
    printf("\tnotation: if you dont specify all variables in a term use '_' to replace the missing variable so that the length of each terms is the same\n");
    return 1;
  }

  int vars = atoi(argv[1]);
  int type = atoi(argv[2]);
  u_int8_t terms[8192] = {0};
  int index = 0;

  for (int i=3;i<argc;i++) {
    index = parse(argv[i],terms,vars,index);
    if (index == -1){
      return -1;
    }
  }

  printout(terms,index);
}
