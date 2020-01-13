/*
 ============================================================================
 Name        : danielWurms.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "func.h"
#include <stdio.h>

void main() {
	Scene *scene;
	Scene *scene1, *scene2;

	// Create a Scene of Objects received
// from files GoingMerry.obj, Donald.obj, Pony_cartoon.obj
	scene = createScene("GoingMerry.obj", "Donald.obj", "Pony_cartoon.obj", NULL);

	// Print out all Vertexes
	perform(scene, printVertexes, "INT" ,"The number of vertexes of all objects is");

	// Count the number of all Triangular faces of all objects in Scene
	perform(scene, printFaces, "INT", "The number of Triangular faces of all objects is");

	// Count the total area of all Triangular faces of all objects in Scene
	perform(scene, getTotalArea, "DOUBLE", "The total area of Triangular faces of all objects is");

}
