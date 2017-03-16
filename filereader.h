#ifndef FILEREADER_H
#define FILEREADER_H

#include <stdio.h>
#include <stdlib.h>
#include "bodies.h"

void readFile(char* file, int numBodies, body* addr);
int getNumBodies( char* file );
double getTimeStep( char* file );
double getDisplayScale( char* file );

#endif //FILEREADER_H
