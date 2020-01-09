/*
 ============================================================================
 Name        : danielWurms.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdarg.h>
#include "object.h"
#include "scene.h"
#include "node.h"
#include "face.h"
#include "vertex.h"
#define MAX_SIZE 100

enum FileType {
	TextFormat, BinaryFormat
};
// helper which copies the text and deforming the x value
void copyAndModify(FILE *original, FILE *deformed) {
	float x, y, z;
	char *line = malloc(sizeof(char));
	int counter = 0;
	while (fgets(line, MAX_SIZE, original) != NULL || counter != MAX_SIZE) {
		if (line[0] == 'v' && line[1] == ' ') {
			sscanf(line, "%*c %f %f %f", &x, &y, &z);
			x = x * 0.3;
			fprintf(deformed, "v %f %f %f\n", x, y, z);
		} else {
			fprintf(deformed, "%s", line);
		}
		counter++;
	}
	free(line);
	fclose(original);
	fclose(deformed);

}

void transformObject(char *originalObjectFileName, char *deformedObjectFileName) {

	FILE *originalFile = fopen(originalObjectFileName, "r");
	if (originalFile == NULL) {
		printf("Failed opening the file , Exiting !\n ");
		return;

	}
	FILE *deformedFile = fopen(deformedObjectFileName, "w");
	if (deformedFile == NULL) {
		printf("Failed opening the file , Exiting !\n ");
		return;
	}
	copyAndModify(originalFile, deformedFile);

}

Object* createObject(char *filename) {
	FILE *fileToObject = fopen(filename, "r");
	if (fileToObject == NULL) {
		printf("Failed opening the file , Exiting !\n ");
		return NULL;

	}
	return objectFromFileGenerator(filename);
}
//creates vertex
void createVertex(char *line, Vertex *v) {
	float x, y, z;
	sscanf(line, "%*c %f %f %f", &x, &y, &z);
	v->x = x;
	v->y = y;
	v->z = z;

}
//creates face
void createFace(char *line, Face *face) {
	char *delimeters = ' ';
	char *temp = strtok(line, delimeters);
	while (temp != NULL) {
		temp = strtok(NULL, delimeters);
		face->vertex = atoi(temp);
	}

}
//creates object
Object* objectFromFileGenerator(FILE *filename) {
	Object *obj = malloc(sizeof(Object));
	if (obj == NULL) {
		printf("Failed allocating memory for object");
		return NULL;
	}
	Vertex *vertexes = malloc(sizeof(Vertex));
	if (obj == NULL) {
		printf("Failed allocating memory for vertex");
		return NULL;
	}
	Face *faces = malloc(sizeof(Face));
	if (obj == NULL) {
		printf("Failed allocating memory for face");
		return NULL;
	}
	obj->numberOfFaces = 0;
	obj->numberOfVertexes = 0;
	char *line = malloc(sizeof(char));
	while (fgets(line, MAX_SIZE, filename)) {
		if (line[0] == 'v' && line[1] == ' ') {
			vertexes = (Vertex*) realloc(obj->numberOfVertexes + 1,
					sizeof(Vertex));
			if (vertexes == NULL) {
				printf("Failed allocating vertexes");
				return NULL;
			}
			vertexes[obj->numberOfVertexes++] = createVertex(line, vertexes);
		} else if (line[0] == 'f' && line[1] == ' ') {
			faces = realloc(obj->numberOfFaces + 1, sizeof(Face));
			if (faces == NULL) {
				printf("Faild allocating faces");
				return NULL;
			}
			faces[obj->numberOfFaces++] = createFace(line, faces);
		}
	}
	obj->vertexes = vertexes;
	obj->faces = faces;
	free(line);
	fclose(filename);
	return obj;
}

char* currentFile(const char *fileName) {
	char *temp = (char*) calloc(1, sizeof(char));
	int counter = 0;
	while (temp[counter] != '\0') {
		temp[counter] = fileName[counter];
		counter++;
	}
	return temp;
}
Scene* createScene(char *fileName, ...) {
	va_list files;
	va_start(files, fileName);
	Scene *scene = calloc(1, sizeof(Scene));
	if (scene == NULL) {
		printf("Can't allocate memory , canceling");
		return NULL;
	}
	FILE *file = fopen(fileName, "r");
	if (file == NULL) {
		printf("Cannot access file, canceling !\n");
		return NULL;
	}
	scene->head = (Lnode*) calloc(1, sizeof(Lnode));
	char *fileRead = currentFile(fileName);
	Lnode *nodePtr = scene->head;
	while (fileRead != NULL) {
		Object *object = createObject(fileName);
		scene->numofobjects++;
		nodePtr->next = object;
		fileRead = va_arg(files, char*);
	}
	va_end(files);
	scene->head = *nodePtr;

	return scene;
}

void printFaces(Object *ptr, void *numberOfTriangularFaces) {

}

/**void printVertexes(Object *ptr, void *numberOfVertexes);
 void getTotalArea (Object *ptr, void *totalAreaOfTriangularFaces);
 void saveScene(Scene *scene, char *fileName, enum FileType type);
 Scene * loadScene(char *fileName, enum FileType type);
 void freeScene(Scene *scene);*/

void main() {
	transformObject("Donald.obj", "newDonald.obj");

}
