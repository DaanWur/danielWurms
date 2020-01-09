#include "vertex.h"
#include "face.h"
typedef struct {
	int numberOfVertexes;
	Vertex *vertexes;
	int numberOfFaces;
	Face *faces;
} Object;
