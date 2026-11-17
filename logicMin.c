#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// void expand(int *terms[], int term){
  
// }

// helpers
void printout(const int *terms,int count){
  for (int i=0;i<count;i++){
    printf("%d",terms[i]);
  }
}

int parser(const char *argv, int *terms, int index, int vars){
  int length = strlen(argv);
  if (vars != length){
    printf("wrong number of variables used in specified terms\n");
    return -1;
  }
  for (int i=0;i<length;i++){
    if (argv[i] == '0'){
      terms[index] = 0;
      index++;
    }else if (argv[i] == '1'){
      terms[index] = 1;
      index++;
    }else if (argv[i] == '_'){
      // add 1 to count of missing vars in term then after loop add code
      // to add all missing full terms
      // also set terms at the index to 0 and add index
    }else{
      printf("terms can only have  '1' '0' '_'\n");
      return -1;
    }
  }
  return index;
}

// first param is num of variables
// second param is 1 for DNF and 0 for KNF
// the folowing are the terms
int main(int argc,char *argv[]){
  if (argc<4){
    printf("Please give all Parameters:\n\n");
    printf("\tfirst: number of vaariables\n");
    printf("\tsecond: 1 for DNF or 0 for KNF\n");
    printf("\tlast: all terms with 0 being negation and 1 being normal\n\n");
    printf("\tnotation: if you dont specify all variables in a term use '_' to replace the missing variable so that the length of each terms is the same\n");
    return 1;
  }

  int vars = atoi(argv[1]);
  int type = atoi(argv[2]);
  int terms[100] = {0};
  int index = 0;

  for (int i=3;i<argc;i++) {
    int x = parser(argv[i],terms,index,vars);
    if (x==-1){
      return 1;
    }
    index = x;
  }

  printout(terms,index);



  
  // int vars;
  // int type;
  // printf("How many variables do your terms have: ");
  // scanf("%d",&vars);
  // printf("Do you want to minimalize a KNF(0) or DNF(1): ");
  // scanf("%d",&type);
  // collect the KNF or DNF
  // int terms[] = {};
  // printf("Enter the KNF/DNF in format 01... enter then the next (with 0 is not and 1 is true) enter 2 when you're done:\n");
  // do{
  //   int term;
  //   scanf("%b",&term);
  //   if (term)
  // }
  // int index = pow(2,vars);
  // int terms[20] = {0};
  // char terms_str;
  // printf("Enter the KKNF/KDNF in format 01.../10.../00... (with 0 is not and 1 is true):\n");
  // scanf("%s",&terms_str);
  // parser(&terms_str,terms);
  // printout(terms);
}
