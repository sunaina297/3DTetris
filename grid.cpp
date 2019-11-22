
#include "constants.h"
#include "helper.h"



static int Index = 0;
point4 LineVertex[NumLineVer*2] = { 0 };
color4 linecolors[NumLineVer*2] = { 0 };
point4 points[NumGrid];
color4 colors[NumGrid];

static GLuint gridCubeVAO;
static GLuint gridCubeBuffer;

static GLuint gridLineVAO;
static GLuint gridLineBuffer;


static point4 vertices[8] = {

	// front 4 points
	point4(LeftEdge, Bottom, 0.5, 1.0),
	point4(LeftEdge, Top, 0.5, 1.0),
	point4(RightEdge, Top, 0.5, 1.0),
	point4(RightEdge, Bottom, 0.5, 1.0),

	// back 4 points
	point4(LeftEdge, Bottom, -0.5, 1.0),
	point4(LeftEdge, Top, -0.5, 1.0),
	point4(RightEdge, Top, -0.5, 1.0),
	point4(RightEdge, Bottom, -0.5, 1.0),
};

//-----------------INITS-----------------------


// places points into square-like vertices 
static void square(int a, int b, int c, int d) {
	points[Index] = vertices[a];
	colors[Index] = color4(0.3, 0.30, 0.3, 1.0); Index++;
	points[Index] = vertices[b];
	colors[Index] = color4(0.3, 0.30, 0.3, 1.0); Index++;
	points[Index] = vertices[c];
	colors[Index] = color4(0.3, 0.30, 0.3, 1.0); Index++;
	points[Index] = vertices[d];
	colors[Index] = color4(0.3, 0.30, 0.3, 1.0); Index++;
}

//get the grid line vertices 
static void fillLineVertexes() {
	int index = 0;
	//front grid
	for (int col = LeftEdge + 1; col < RightEdge; col += 2) {

		LineVertex[index] = point4(col, Bottom, 0.55, 1.0); index++;
		LineVertex[index] = point4(col, Top, 0.55, 1.0); index++;
		LineVertex[index] = point4(col + 1, Top, 0.55, 1.0); index++;
		LineVertex[index] = point4(col + 1, Bottom, 0.55, 1.0); index++;

		LineVertex[index] = point4(col, Bottom, -0.55, 1.0); index++;
		LineVertex[index] = point4(col, Top, -0.55, 1.0); index++;
		LineVertex[index] = point4(col + 1, Top, -0.55, 1.0); index++;
		LineVertex[index] = point4(col + 1, Bottom, -0.55, 1.0); index++;
	}

	//horizontal 
	for (int row = Bottom + 1; row < Top; row += 2) {

		LineVertex[index] = point4(LeftEdge, row, 0.55, 1.0); index++;
		LineVertex[index] = point4(RightEdge, row, 0.55, 1.0); index++;
		LineVertex[index] = point4(RightEdge, row + 1, 0.55, 1.0); index++;
		LineVertex[index] = point4(LeftEdge, row + 1, 0.55, 1.0); index++;

		LineVertex[index] = point4(LeftEdge, row, -0.55, 1.0); index++;
		LineVertex[index] = point4(RightEdge, row, -0.55, 1.0); index++;
		LineVertex[index] = point4(RightEdge, row + 1, -0.55, 1.0); index++;
		LineVertex[index] = point4(LeftEdge, row + 1, -0.55, 1.0); index++;
	}
	

	/*for (int i = 0; i < 4; i++) {
		printf("points %f %f %f\n", LineVertex[i][0], LineVertex[i][1], LineVertex[i][2]);
	}*/
}


//------------------SETP------------------------

// set-up for grid lines 
static void Grid_lineVertices() {
	fillLineVertexes();
	H_fillColor(NumLineVer, linecolors, color4(0.3, 0.30, 0.3, 1.0));
}

// set-up for grid cube
static void Grid_gridCube() {
	H_fillColor(NumGrid, colors, color4(0.3, 0.30, 0.3, 1.0));
	square(0, 1, 2, 3);
	square(3, 2, 6, 7);
	square(7, 6, 5, 4);
	square(4, 5, 1, 0);
	square(0, 3, 7, 4);
	square(1, 2, 6, 5);
}

//------------------RENDER---------------------

void grid_init() {

	Grid_lineVertices();
	Grid_gridCube();

	glGenVertexArrays(1, &gridCubeVAO);
	glBindVertexArray(gridCubeVAO);
	// Create and initialize a buffer object

	glGenBuffers(1, &gridCubeBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gridCubeBuffer);
	// empty buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	glGenVertexArrays(1, &gridLineVAO);
	glBindVertexArray(gridLineVAO);
	// Create and initialize a buffer object

	glGenBuffers(1, &gridLineBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gridLineBuffer);
	// empty buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineVertex) + sizeof(linecolors), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(LineVertex), LineVertex);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(LineVertex), sizeof(linecolors), linecolors);

}

void renderGridCube(int projection, int program) {

	glBindVertexArray(gridCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gridCubeBuffer);
	//glUseProgram(program);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));


	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	projection = glGetUniformLocation(program, "projection");

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_LINES, 0, NumGrid);
}

void renderGridLines(int projection, int program, int ModelView) {

	glBindVertexArray(gridCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gridLineBuffer);
	glUseProgram(program);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));


	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(LineVertex)));

	ModelView = glGetUniformLocation(program, "model_view");
	projection = glGetUniformLocation(program, "projection");

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_LINES, 0, NumLineVer);
}
