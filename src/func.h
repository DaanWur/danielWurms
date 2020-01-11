#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "scene.h"

void copyAndModify(FILE *original, FILE *deformed);

void transformObject(char *originalObjectFileName, char *deformedObjectFileName);

Object* createObject(char *fileName);

Vertex* createVertex(char *line, Vertex *v);

Face* createFace(char *line, Face *face);

Object* objectFromFile(FILE *file);

char* currentFile(const char *fileName);

Scene* createScene(char *fileName, ...);

void printFaces(Object *ptr, void *numberOfTriangularFaces);

void printVertexes(Object *ptr, void *numberOfVertexes);

void getTotalArea(Object *ptr, void *totalAreaOfTriangularFaces);

void perform(Scene *scene, void (*funcToChoose)(Object*, void*),
		char *howToPrint, char *strPtr);

