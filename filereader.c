#include <stdio.h>
#include <stdlib.h>
#include "bodies.h"
#include "filereader.h"

void readFile(char* file, int numBodies, body* addr ){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(file, "r");
    
    // Ignore blank lines
    for (int i=0; i<6; ++i)
      getline(&line, &len, fp);

    for (int i=0; i<numBodies; ++i) {

        getline(&line, &len, fp);
        getline(&line, &len, fp); sscanf(line,"%lf",&(addr[i].pos.x));
	getline(&line, &len, fp); sscanf(line,"%lf",&(addr[i].pos.y));
	getline(&line, &len, fp); sscanf(line,"%lf",&(addr[i].pos.z));
	getline(&line, &len, fp); sscanf(line,"%lf",&(addr[i].vel.x));
	getline(&line, &len, fp); sscanf(line,"%lf",&(addr[i].vel.y));
	getline(&line, &len, fp); sscanf(line,"%lf",&(addr[i].vel.z));
	getline(&line, &len, fp); sscanf(line,"%Lf",&(addr[i].mass));
	getline(&line, &len, fp); sscanf(line,"%lf",&(addr[i].radius));
	getline(&line, &len, fp); sscanf(line,"%lf",&(addr[i].disp_radius));
	getline(&line, &len, fp); (addr[i].color.r) = atoi(line);
	getline(&line, &len, fp); (addr[i].color.g) = atoi(line);
	getline(&line, &len, fp); (addr[i].color.b) = atoi(line);

    }

    fclose(fp);
}

int getNumBodies( char* file ){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int numBodies;

    fp = fopen(file, "r");
    getline(&line, &len, fp);
    getline(&line, &len, fp); numBodies = atoi(line);
    fclose(fp);
    return numBodies;
}

double getTimeStep( char* file ){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    double timeStep;

    fp = fopen(file, "r");
    // Ignore blank lines
    for (int i=0; i<3; ++i)
      getline(&line, &len, fp);
    getline(&line, &len, fp); sscanf(line,"%lf",&timeStep);
    fclose(fp);
    return timeStep;
}

double getDisplayScale( char* file ){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    double scale;

    fp = fopen(file, "r");
    // Ignore blank lines
    for (int i=0; i<5; ++i)
      getline(&line, &len, fp);
    getline(&line, &len, fp); sscanf(line,"%lf",&scale);
    fclose(fp);
    return scale;
}




