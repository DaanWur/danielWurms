#include <math.h>
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
Object* objectFromFile(FILE *file1, FILE *file2);

Object* createObject(char *fileName) {
	FILE *file1 = fopen(fileName, "r");
	FILE *file2 = fopen(fileName, "r");
	if (file1 == NULL) {
		printf("Failed opening the file , Exiting !\n ");
		return NULL;

	} else if (file2 == NULL) {
		printf("Failed opening the file , Exiting !\n ");
		return NULL;
	}
	return objectFromFile(file1, file2);
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
int countVertexes(FILE *file) {
	char *line = (char*) malloc(sizeof(char));
	int counter = 0;
	while (fgets(line, MAX_SIZE, file)) {
		if (line[0] == 'v' && line[1] == ' ')
			counter++;
	}
	free(line);
	return counter;

}

//creates face
Face* createFace(char *line, Face *face) {
	char *delimeters = " ";
	char *temp = NULL;
	temp = (char*) calloc(1, sizeof(char));
	temp = strtok(line, delimeters);
	face->vertex = (int*) calloc(face->size, sizeof(int));
	temp = strtok(NULL, delimeters);
	while (temp != NULL) {
		(face->vertex[face->size]) = atoi(temp);

		temp = strtok(NULL, delimeters);
		face->size++;
	}
	return face;
}
int countFaces(FILE *file) {
	char *line = (char*) malloc(sizeof(char));
	int counter = 0;
	while (fgets(line, MAX_SIZE, file)) {
		if (line[0] == 'f' && line[1] == ' ')
			counter++;
	}
	free(line);

	return counter;

}
//creates object
Object* objectFromFile(FILE *file1, FILE *file2) {
	Object *obj = (Object*) malloc(sizeof(Object));
	if (obj == NULL) {
		printf("Failed allocating memory for object");
		return NULL;
	}

	obj->vertexes = (Vertex*) malloc(sizeof(Vertex));
	if (obj->vertexes == NULL) {
		printf("Failed allocating memory for vertex");
		return NULL;
	}
	obj->faces = malloc(sizeof(Face));
	if (obj->faces == NULL) {
		printf("Failed allocating memory for face");
		return NULL;
	}
	char *line = (char*) calloc(1, sizeof(char));
	obj->numberOfVertexes = countVertexes(file1);
	rewind(file1);
	obj->numberOfFaces = countFaces(file2);
	rewind(file2);
	int i = 0;
	obj->vertexes = (Vertex*) realloc(obj->vertexes,
			(obj->numberOfVertexes + 1) * sizeof(Vertex));
	while (fgets(line, MAX_SIZE, file1)) {
		if (line[0] == 'v' && line[1] == ' ') {
			if (obj->vertexes == NULL) {
				printf("Failed allocating vertexes");
				return NULL;
			}
			(obj->vertexes[i]) = *(createVertex(line, obj->vertexes));
			i++;
		}
	}
	int j = 0;
	obj->faces = (Face*) realloc(obj->faces,
			(obj->numberOfFaces + 1) * sizeof(Face));
	while (fgets(line, MAX_SIZE, file2)) {
		if (line[0] == 'f' && line[1] == ' ') {
			if (obj->faces == NULL) {
				printf("Failed allocating faces");
				return NULL;
			}
			obj->faces[j] = *createFace(line, obj->faces);
			j++;
		}
	}

	free(line);
	fclose(file1);
	fclose(file2);
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
	Scene *scene = (Scene*) calloc(1, sizeof(Scene));
	if (scene == NULL) {
		printf("Can't allocate memory , canceling");
		return NULL;
	}
	FILE *file = fopen(fileName, "r");
	if (file == NULL) {
		printf("Cannot access file, canceling !\n");
		return NULL;
	}
	char *fileRead = currentFile(fileName);
	scene->head = (Lnode*) calloc(1, sizeof(Lnode));
	Lnode *nodePtr = (*scene).head;
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
//prints the triangular faces
void printFaces(Object *ptr, void *numberOfTriangularFaces) {
	int counter = 0;

	for (int i = 0; i < ptr->numberOfFaces; ++i) {
		int size = ptr->faces[i].size;
		if (size == 3) {
			counter += 1;
		}
	}
	*((int*) numberOfTriangularFaces) = counter;
}
//prints the number of all vertexes
void printVertexes(Object *ptr, void *numberOfVertexes) {

	*((int*) numberOfVertexes) = ptr->numberOfVertexes;
}
void getTotalArea(Object *ptr, void *totalAreaOfTriangularFaces) {
	double semiPerimeter = 0, totalArea = 0;
	for (int i = 0; i < ptr->numberOfFaces; ++i) {
		int size = ptr->faces[i].size;
		if (size == 3) {
			int firstV = ptr->faces[i].vertex[0];
			int secondV = ptr->faces[i].vertex[1];
			int thirdV = ptr->faces[i].vertex[2];
			double firstTRib = sqrt(
					pow(ptr->vertexes[firstV].x - ptr->vertexes[secondV].x, 2)
							+ (pow(
									ptr->vertexes[firstV].y
											- ptr->vertexes[secondV].y, 2))
							+ (pow(
									ptr->vertexes[firstV].z
											- ptr->vertexes[secondV].z, 2))); //first and second
			double secondTRib = sqrt(
					pow(ptr->vertexes[firstV].x - ptr->vertexes[thirdV].x, 2)
							+ pow(
									ptr->vertexes[firstV].y
											- ptr->vertexes[thirdV].y, 2)
							+ pow(
									ptr->vertexes[firstV].z
											- ptr->vertexes[thirdV].z, 2)); //first and third
			double thirdTRib = sqrt(
					pow(ptr->vertexes[secondV].x - ptr->vertexes[thirdV].x, 2)
							+ pow(
									ptr->vertexes[secondV].y
											- ptr->vertexes[thirdV].y, 2)
							+ pow(
									ptr->vertexes[secondV].z
											- ptr->vertexes[thirdV].z, 2)); //second and third
			semiPerimeter = (firstTRib + secondTRib + thirdTRib) / 2;
			double localArea = sqrt(
					semiPerimeter * (semiPerimeter - firstTRib)
							* (semiPerimeter - secondTRib)
							* (semiPerimeter - thirdTRib));
			totalArea += localArea;

		}
	}
	*((double*) totalAreaOfTriangularFaces) = totalArea;
}

void perform(Scene *scene, void (*funcToChoose)(Object*, void*),
		char *howToPrint, char *strPtr) {
	Lnode *ptr = scene->head;

	if (strcmp(howToPrint, "INT") == 0) {
		int returnNum = 0;

		while (ptr != NULL) {
			funcToChoose(ptr->object, &returnNum);
			ptr = ptr->next;

		}
		printf("%s : %d \n", strPtr, returnNum);
	}

	else if (strcmp(howToPrint, "DOUBLE") == 0) {
		double returnNum = 0;
		while (ptr != NULL) {
			funcToChoose(ptr->object, &returnNum);
			ptr = ptr->next;
		}
		printf("%s : %f \n", strPtr, returnNum);
	}
}

/** void saveScene(Scene *scene, char *fileName, enum FileType type);
 Scene * loadScene(char *fileName, enum FileType type);
 void freeScene(Scene *scene);*/
