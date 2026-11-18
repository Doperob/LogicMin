#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int parse(const char *term, int *terms,int _count, int *mem, int spot, int vars, int index, int start, int replace){
  int length = strlen(term);
  if (vars != length){
    printf("wrong number of variables used in specified terms\n");
    return -1;
  }
  

  for (int i=0;i<spot;i++){
    if (i==spot-1){
      terms[index++] = replace;
    }else{
      terms[index++] = mem[i];
    }
  }
  
  for (int i=start; i<vars; i++){
    if (term[i] == '0'){
      terms[index++] = 0;
      mem[i] = 0;
    }else if (term[i] == '1'){
      terms[index++] = 1;
      mem[i] = 1;
    }else if (term[i] == '_'){
      _count-=1;
      terms[index++] = replace;
      mem[i] = replace;
      if (_count == 0){
        spot = i+1;
        index = parse(term,terms,_count,mem,spot,vars,index,i+1,1);
      }else{
        index = parse(term,terms,_count,mem,spot,vars,index,i+1,0);
        index = parse(term,terms,_count,mem,spot,vars,index,i+1,1);
      }
    }
  }
  return index;
}

// helpers
void printout(const int *terms,int count){
  for (int i=0;i<count;i++){
    printf("%d",terms[i]);
  }
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
  int terms[100] = {0};
  int mem[vars];
  int index = 0;

  for (int i=3;i<argc;i++) {
    int _count = 0;
    for (int x=0;x<vars;x++){
      if (argv[i][x] == '_'){
        _count++;
      }
    }
    index = parse(argv[i],terms,_count,mem,0,vars,index,0,0);
    if (index == -1){
      return -1;
    }
  }

  printout(terms,index);
}
