#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define TERMS 8192

// implicant / implicat stuct to save the minimized terms 
// (2 for don't care)
typedef struct {
    u_int8_t term[16];
    bool essential;
} Im;
// solution memory for recursive optimisation
typedef struct {
    int count;
    bool selection[TERMS];
} Solution;

// helpers
// prints the terms array
void printout(const u_int8_t *terms, int count){
  for (int i=0;i<count;i++){
    printf("%d",terms[i]);
  }
   printf("\n");
}

// nicely prints the finished terms for DNF and KNF
void printdone(const Im *ims, int imsCount, int type, const char *varNames, int vars){
    bool firstTerm = true;
    for (int i = 0; i < imsCount; i++){
        if (!ims[i].essential) continue;
        if (!type) {
            printf("(");
        }
        if (!i && !type) printf("Your KMF is: ");
        if (!i && type) printf("Your DMF is: ");
        bool firstLiteral = true;
        
        bool allTwos = true;
        for (int x = 0; x < vars; x++) {
            if (ims[i].term[x] != 2) {
                allTwos = false;
                break;
            }
        }
        if (allTwos) {
            printf("1");
        }
        
        for (int x = 0; x < vars; x++){
            if (ims[i].term[x] != 2) {
                if (!firstLiteral) {
                    if (!type) {
                        printf("+");
                    }
                }
                firstLiteral = false;
                
                char var = varNames[x];
                if (type == 1) {
                    // DNF: 1 = normal, 0 = negated (capitalized)
                    if (ims[i].term[x] == 1) {
                        printf("%c", var);
                    } else {
                        printf("%c", toupper(var));
                    }
                } else {
                    // KNF: 0 = normal, 1 = negated (capitalized)
                    if (ims[i].term[x] == 0) {
                        printf("%c", var);
                    } else {
                        printf("%c", toupper(var));
                    }
                }
            }
        }

        if (!type) {
            printf(")");
        }

        bool last = true;
        for (int x=i+1;x<imsCount;x++){
            if (ims[x].essential){
                last = false;
            }
        }

        if (!last) {
            if (type) {
                printf(" + ");
            } else {
                printf(" · ");
            }
        }
    }
    printf("\n");
}

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

// check for difference between two terms by one
bool diff(const uint8_t *term1, const uint8_t *term2, int vars, int *diffIndex){
    int diffCount = 0;
    for (int i = 0; i < vars; i++){
        if (term1[i] != term2[i]){
            diffCount++;
            *diffIndex = i;
        }
    }
    if (diffCount != 1) return false;
    // ensure diff is a 0/1 flip (not don't care flip)
    uint8_t a = term1[*diffIndex];
    uint8_t b = term2[*diffIndex];
    if ((a==0&&b==1)||(a==1&&b==0)) {
        return true;
    }
    return false;
}

// Quine-McCluskey minimization
int quineMcCluskey(const u_int8_t *terms, int termsCount, Im *ims, int vars){
    int imsCount = termsCount;
    for (int i=0;i<imsCount;i++){
        for (int x=0;x<vars;x++){
            ims[i].term[x] = terms[i*vars + x];
        }
    }
    Im primes[TERMS];
    int primesCount = 0;
    bool done = false;
    while (!done){
        done = true;
        Im new[TERMS];
        int newCount = 0;
        bool combined[TERMS] = {0};
        for (int i=0;i<imsCount;i++){
            for (int x=0;x<imsCount;x++){
                int diffIndex = -1;
                if (diff(ims[i].term,ims[x].term,vars,&diffIndex)){
                    done = false;
                    combined[i] = true;
                    combined[x] = true;
                    // when found two with only one diff create new Im with 2 for the irelevant one
                    Im merged;
                    for (int y=0;y<vars;y++){
                        merged.term[y] = (y==diffIndex) ? 2 : ims[i].term[y];
                    }
                    // remove duplicates from next itaration
                    bool exists = false;
                    for (int y=0;y<newCount;y++){
                        if (memcmp(merged.term, new[y].term, sizeof(merged.term)) == 0){
                            exists = true;
                            break;
                        }
                    }
                    if (!exists){
                        new[newCount++] = merged;
                    }
                }
            }
        }
        // Collect uncombined terms as prime implicants
        for (int i=0; i<imsCount; i++) {
            if (!combined[i]) {
                bool exists = false;
                for (int y=0; y<primesCount; y++) {
                    if (memcmp(ims[i].term, primes[y].term, sizeof(primes[y].term)) == 0) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    primes[primesCount++] = ims[i];
                }
            }
        }
        // check if there are still terms being minimized
        if (!done){
            imsCount = newCount;
            for (int i=0;i<newCount;i++){
                ims[i] = new[i];
            }
        }
    }
    // Copy collected prime implicants back to ims
    for (int i=0; i<primesCount; i++) {
        ims[i] = primes[i];
    }
    return primesCount;
}

// check for cover of Im by term
bool covers(const uint8_t *im, const uint8_t *term, int vars) {
    for (int i = 0; i < vars; i++) {
        if (im[i] != 2 && im[i] != term[i]) {
            return false;
        }
    }
    return true;
}

void findMinCover(const u_int8_t chart[TERMS][TERMS], int imsCount, int termsCount, int currIm, int currCount, bool currSelection[TERMS],bool covTerms[TERMS], Solution *sol){
    // if the current is greater than the best found return
    if (currCount >= sol->count) return;
    // coverance check
    bool covered = true;
    for (int i=0;i<termsCount;i++){
        if (!covTerms[i]){
            covered = false;
            break;
        }
    }
    if (covered){
        sol->count = currCount;
        memcpy(sol->selection,currSelection,sizeof(bool)*imsCount);
        return;
    }
    if (currIm >= imsCount) return;
    // try include of currIm if it covers at least one uncovered Term
    bool better = false;
    bool newCovTerms[termsCount];
    memcpy(newCovTerms,covTerms,sizeof(bool)*termsCount);
    for (int i=0;i<termsCount;i++){
        if (chart[currIm][i] && !covTerms[i]){
            newCovTerms[i] = true;
            better = true;
        }
    }
    if (better){
        currSelection[currIm] = true;
        findMinCover(chart,imsCount,termsCount,currIm+1,currCount+1,currSelection,newCovTerms,sol);
        currSelection[currIm] = false;
    }
    // try exclude of currIm
    findMinCover(chart,imsCount,termsCount,currIm+1,currCount,currSelection,covTerms,sol);
}

int selectTerms(const u_int8_t *terms, int termsCount, Im *ims, int imsCount, int vars){
    // allocate engough memory to solve
    u_int8_t (*chart)[TERMS] = malloc(sizeof(u_int8_t) * TERMS * TERMS);
    if (!chart) return 0;
    memset(chart, 0, sizeof(u_int8_t) * TERMS * TERMS);
    
    // build initial coverance chart
    for (int i=0;i<imsCount;i++){
        for (int x=0;x<termsCount;x++){
            if (covers(ims[i].term,&terms[x*vars],vars)){
                chart[i][x] = 1;
            }
        }
    }
    // track selected ims and terms covered
    bool currSelection[TERMS] = {false};
    int currCount = 0;
    bool covTerms[TERMS] = {false};
    // find the ims that are not covered by others (Kernal Im)
    bool foundKI = true;
    while (foundKI){
        foundKI = false;
        for (int i=0; i<termsCount;i++){
            if (covTerms[i]) continue;
            int covIm = -1;
            int count = 0; // how ims cover the term
            for (int x=0;x<imsCount;x++){
                if (chart[x][i]){
                    count++;
                    covIm = x;
                }
            }
            // count is one then it is a KI
            if (count==1 && !currSelection[covIm]){
                foundKI = true;
                currSelection[covIm] = true;
                currCount++;
                // mark all terms covered by KI
                for (int x=0;x<termsCount;x++){
                    if (chart[covIm][x]){
                        covTerms[x] = true;
                    }
                }
            }
        }
    }
    // reduce the remaining ims to only the ones needed
    Solution sol;
    sol.count = 999; // large value needed so that the algo start properly
    memset(sol.selection,0,sizeof(sol.selection));
    findMinCover(chart,imsCount,termsCount,0,currCount,currSelection,covTerms,&sol);
    for (int i=0;i<imsCount;i++){
        ims[i].essential = sol.selection[i];
    }
    free(chart); // free up allocated memory
    return sol.count;
}
// first param is num of variables
// second param is 1 for DNF and 0 for KNF
// the folowing are the terms
int main(int argc,char *argv[]){
  if (argc<4){
    printf("Please give all Parameters!\n");
    return 1;
  }

  int vars = atoi(argv[1]);
  int type = atoi(argv[3]);
  if (type!=0 && type!=1){
      printf("Invalid type of output (Parameter 3)");
      return 1;
  }
  char varNames[vars];
  for (int i=0;i<vars;i++){
      varNames[i] = argv[2][i];
  }
  u_int8_t terms[TERMS] = {0};
  int index = 0;

  for (int i=4;i<argc;i++) {
    index = parse(argv[i],terms,vars,index);
    if (index == -1){
      return -1;
    }
  }

  //printout(terms,index);

  int termsCount = index/vars;
  Im ims[TERMS];
  int imsCount = quineMcCluskey(terms,termsCount,ims,vars);
  imsCount = selectTerms(terms,termsCount,ims,imsCount,vars);
  
  printdone(ims,imsCount,type,varNames,vars);
}
