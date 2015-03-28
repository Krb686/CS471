#include "process.h"

#ifndef PARSEROUTPUT_STRUCT
#define PARSEROUTPUT_STRUCT
typedef struct parserOutput{
  processP PID0;
  int memSize, policy, param, count;
} ParserOutputR, *ParserOutputP;
#endif

ParserOutputP parser(char*);
processP insertP(processP,int);
processP lastP(processP);
spaceP insertSpace(spaceP,int);
spaceP lastSpace(spaceP);
