#include "parser.h"

main(int argc, char* argv[]){
  if(argc!=2){printf("Usage: %s filename\n",argv[0]); exit(2);}
  processP test = parser(argv[1]);
  printP(test);
}
