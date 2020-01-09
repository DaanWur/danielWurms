#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "scene.h"

#define MAX_SIZE 100

enum FileType {
	TextFormat, BinaryFormat
};
// helper which copies the text and deforming the x value
void copyAndModify(FILE *original, FILE *deformed) {
	float x, y, z;
	char *line = (char*) calloc(1, sizeof(char));
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

Object* createObject(char *f) {
	FILE *fileToObject = fopen(f, "r");
	if (fileToObject == NULL) {
		printf("Failed opening the file , Exiting !\n ");
		return NULL;

	}
	return objectFromFile(&f);
}
//creates vertex
Vertex* createVertex(char *line, Vertex *v) {
	float x, y, z;
	sscanf(line, "%*c %f %f %f", &x, &y, &z);
	v->x = x;
	v->y = y;
	v->z = z;
	return v;
}
//creates face
Face* createFace(char *line, Face *face) {
	char *delimeters = " ";
	char *temp = (char*) calloc(1, sizeof(char));
	temp = strtok(line, delimeters);

	while (temp != NULL) {
		temp = strtok(NULL, delimeters);
		*face->vertex = atoi(temp);
	}
	return face;
}
//creates object
Object* objectFromFile(FILE *file) {
	Object *obj = (Object*) malloc(sizeof(Object));
	if (obj == NULL) {
		printf("Failed allocating memory for object");
		return NULL;
	}
	Vertex *vertexes = (Vertex*) malloc(sizeof(Vertex));
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
	while (fgets(line, MAX_SIZE, file)) {
		if (line[0] == 'v' && line[1] == ' ') {
			vertexes = (Vertex*) realloc(vertexes,
					(obj->numberOfVertexes + 1) * sizeof(Vertex));
			if (vertexes == NULL) {
				printf("Failed allocating vertexes");
				return NULL;
			}
			vertexes[obj->numberOfVertexes++] = *createVertex(line, vertexes);
		} else if (line[0] == 'f' && line[1] == ' ') {
			faces = (Face*) realloc(faces,
					(obj->numberOfFaces + 1) * sizeof(Face));
			if (faces == NULL) {
				printf("Faild allocating faces");
				return NULL;
			}
			createFace(line, faces);
			faces[obj->numberOfFaces++] = *createFace(line, faces);
		}
	}
	obj->vertexes = vertexes;
	obj->faces = faces;
	free(line);
	fclose(file);
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
	nodePtr = calloc(1, sizeof(scene->head));
	while (fileRead != NULL) {
		Object *object = createObject(fileName);
		scene->numofobjects++;
		nodePtr->next = object;
		fileRead = va_arg(files, char*);
	}
	va_end(files);
	scene->head = nodePtr;

	return scene;
}

void printFaces(Object *ptr, void *numberOfTriangularFaces) {
	int counter = 0;
	Face *fPtr = ptr->faces;

	for (int i = 0; i < ptr->numberOfFaces; ++i) {
	}

}

/**void printVertexes(Object *ptr, void *numberOfVertexes);
 void getTotalArea (Object *ptr, void *totalAreaOfTriangularFaces);
 void saveScene(Scene *scene, char *fileName, enum FileType type);
 Scene * loadScene(char *fileName, enum FileType type);
 void freeScene(Scene *scene);*/
