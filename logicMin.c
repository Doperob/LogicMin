#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// void expand(int *terms[], int term){
  
// }

// helpers
void printout(const int *terms){
  for (int i=0;i<20;i++){
    printf("%d",terms[i]);
  }
}

void parser(const char *terms_str, int *terms){
  int length = strlen(terms_str);
  int index = 0;
  for (int i=0;i<length;i++){
    if (terms_str[i] == '0'){
      terms[index] = 0;
      index++;
    }else if (terms_str[i] == '1'){
      terms[index] = 1;
      index++;
    }else{
      printf("%s\n",&terms_str[i]);
    }
  }
}

// first param is num of variables the folowing are the min or max terms
int main(){
  int vars;
  int type;
  printf("How many variables do your terms have: ");
  scanf("%d",&vars);
  printf("Do you want to minimalize a KNF(0) or DNF(1): ");
  scanf("%d",&type);
  // collect the KNF or DNF
  // int terms[] = {};
  // printf("Enter the KNF/DNF in format 01... enter then the next (with 0 is not and 1 is true) enter 2 when you're done:\n");
  // do{
  //   int term;
  //   scanf("%b",&term);
  //   if (term)
  // }
  // int index = pow(2,vars);
  int terms[20] = {0};
  char terms_str;
  printf("Enter the KKNF/KDNF in format 01.../10.../00... (with 0 is not and 1 is true):\n");
  scanf("%s",&terms_str);
  parser(&terms_str,terms);
  printout(terms);
}
