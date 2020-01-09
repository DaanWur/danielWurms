#include "vertex.h"
#include "face.h"
typedef struct {
	int numberOfVertexes;
	struct Vertex *vertexes;
	int numberOfFaces;
	struct Face *faces;
} Object;
