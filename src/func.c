#include "func.h"
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
Object* objectFromFile(FILE *file1, FILE *file2, Object *obj);

Object* createObject(char *fileName) {
	FILE *file1 = fopen(fileName, "r");
	FILE *file2 = fopen(fileName, "r");
	Object *obj = (Object*) malloc(sizeof(Object));
	if (file1 == NULL) {
		printf("Failed opening the file , Exiting !\n ");
		return NULL;

	} else if (file2 == NULL) {
		printf("Failed opening the file , Exiting !\n ");
		return NULL;
	}
	return objectFromFile(file1, file2, obj);
}
//creates vertex
Vertex* createVertex(char *line) {
	Vertex *v = (Vertex*) malloc(sizeof(Vertex));
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
	char *delimeters = " f";
	char *temp = NULL;
	temp = strtok(line, delimeters);
	face->vertex = (int*) calloc(1, sizeof(int));
	face->size = 0;
//	temp = strtok(NULL, delimeters);

	while (temp != NULL && strcmp(temp, "\n") != 0) {
		int size = face->size + 1;
		face->vertex = (int*) realloc(face->vertex, size * sizeof(int));

		if (face->vertex == NULL) {
			printf("Failed to reallocate memory \n");
			return NULL;
		}
		face->vertex[face->size++] = atoi(temp);
		temp = strtok(NULL, delimeters);
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
Object* objectFromFile(FILE *file1, FILE *file2, Object *obj) {
	obj->vertexes = malloc(sizeof(int));
	if (obj == NULL) {
		printf("Failed allocating memory for object");
		return NULL;
	}
	obj->faces = malloc(sizeof(Face));
	if (obj->faces == NULL) {
		printf("Failed allocating memory for face");
		return NULL;
	}

	obj->faces->vertex = calloc(3, sizeof(Vertex));
	if (obj->vertexes == NULL) {
		printf("Failed allocating memory for vertex");
		return NULL;
	}
	char line[MAX_SIZE];
	Vertex *vertexes = obj->vertexes;
	Face *faces = obj->faces;
	obj->numberOfVertexes = 0;
	obj->numberOfFaces = 0;
	while (fgets(line, MAX_SIZE, file1)) {
		if (line[0] == 'v' && line[1] == ' ') {
			obj->vertexes = realloc(vertexes,
					(obj->numberOfVertexes + 1) * sizeof(Vertex));
			if (obj->vertexes == NULL) {
				printf("Failed allocating vertexes");
				return NULL;
			}
			vertexes = obj->vertexes;
			obj->vertexes[obj->numberOfVertexes++] = *createVertex(line);
		}
	}
	while (fgets(line, MAX_SIZE, file2)) {
		if (line[0] == 'f' && line[1] == ' ') {
			obj->faces = realloc(faces,
					(obj->numberOfFaces + 1) * sizeof(Face));
			if (obj->faces == NULL) {
				printf("Failed allocating faces");
				return NULL;
			}
			faces = obj->faces;
			createFace(line, &obj->faces[obj->numberOfFaces]);
			obj->numberOfFaces++;
		}
	}

	fclose(file1);
	fclose(file2);
	return obj;
}

//char* currentFile(const char *fileName) {
//	char *temp = (char*) calloc(1, sizeof(char));
//	int counter = 0;
//	while (temp[counter] != '\0') {
//		temp[counter] = fileName[counter];
//		counter++;
//	}
//	return temp;
//}
Lnode* insertLast(Lnode *head, Object *obj) {
	Lnode *tempPtr = head;
	if (head->object == NULL) {
		head->object = obj;
		return head;
	}
	Lnode *newNode = (Lnode*) malloc(sizeof(Lnode));
	newNode->object = obj;
	newNode->next = NULL;
	while (tempPtr->next != NULL) {
		tempPtr = tempPtr->next;

	}
	tempPtr->next = newNode;
	return head;
}
Scene* createScene(char *fileName, ...) {
	va_list files;
	va_start(files, fileName);
	Scene *scene = (Scene*)malloc(sizeof(Scene));
	if (scene == NULL) {
		printf("Can't allocate memory , canceling");
		return NULL;
	}
	FILE *file = fopen(fileName, "r");
	if (file == NULL) {
		printf("Cannot access file, canceling !\n");
		return NULL;
	}
	char *fileRead = fileName;
	scene->head = (Lnode*) calloc(1, sizeof(Lnode));
	while (fileRead != NULL) {
		Object *object = createObject(fileRead);

		scene->numofobjects++;
		scene->head = insertLast(scene->head, object);
		fileRead = va_arg(files, char*);
	}
	va_end(files);
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
			int firstV = ptr->faces[i].vertex[0]-1;
			int secondV = ptr->faces[i].vertex[1]-1 ;
			int thirdV = ptr->faces[i].vertex[2] -1;
			double firstTRib = sqrt(pow(ptr->vertexes[firstV].x - ptr->vertexes[secondV].x, 2)
							+ (pow(ptr->vertexes[firstV].y- ptr->vertexes[secondV].y, 2))
							+ (pow(ptr->vertexes[firstV].z- ptr->vertexes[secondV].z, 2))); //first and second
			double secondTRib = sqrt(pow(ptr->vertexes[firstV].x - ptr->vertexes[thirdV].x, 2)
							+ pow(ptr->vertexes[firstV].y- ptr->vertexes[thirdV].y, 2)
							+ pow(ptr->vertexes[firstV].z- ptr->vertexes[thirdV].z, 2)); //first and third
			double thirdTRib = sqrt(pow(ptr->vertexes[secondV].x - ptr->vertexes[thirdV].x, 2)
							+ pow(ptr->vertexes[secondV].y- ptr->vertexes[thirdV].y, 2)
							+ pow(ptr->vertexes[secondV].z- ptr->vertexes[thirdV].z, 2)); //second and third
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

void perform(Scene *scene, void (*funcToChoose)(Object*, void*),char *howToPrint, char *strPtr) {
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
