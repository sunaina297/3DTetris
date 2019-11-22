#include "constants.h"
#include "helper.h"

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

static point4 points[NumVertices];
static color4 colors[NumVertices];
static GLuint armVAO;
static GLuint armVBO;

// Parameters controlling the size of the Robot's arm



// Shader transformation matrices



static point4 vertices[8] = {
	point4(-0.5, -0.5,  2, 1.0),
	point4(-0.5,  0.5,  2, 1.0),
	point4(0.5,  0.5,  2, 1.0),
	point4(0.5, -0.5,  2, 1.0),
	point4(-0.5, -0.5, 1, 1.0),
	point4(-0.5,  0.5, 1, 1.0),
	point4(0.5,  0.5, 1, 1.0),
	point4(0.5, -0.5, 1, 1.0)
};


//----------------------------------INIT--------------------------------

int Index = 0;

void
quad(int a, int b, int c, int d)
{
	points[Index] = vertices[a]; Index++;
	points[Index] = vertices[b]; Index++;
	points[Index] = vertices[c]; Index++;
	points[Index] = vertices[a]; Index++;
	points[Index] = vertices[c]; Index++;
	points[Index] = vertices[d]; Index++;
}

void
colorcube()
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);

	H_fillColor(NumVertices, colors, color4(0.5, 0, 1, 1.0) );
}
//----------------------------------SETUP--------------------------------
//----------------------------------------------------------------------------

/* Define the three parts */
/* Note use of push/pop to return modelview matrix
to its state before functions were entered and use
rotation, translation, and scaling to create instances
of symbols (cube and cylinder */

void base(int model, mat4 model_view)
{
	mat4 instance = (Translate(-4.0, 0.5 * BASE_HEIGHT, 0.0) *
		Scale(BASE_WIDTH,
			BASE_HEIGHT,
			BASE_WIDTH));

	glUniformMatrix4fv(model, 1, GL_TRUE, model_view * instance);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}


void lower_arm(int model, mat4 model_view)
{
	mat4 instance = (Translate(0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0) *
		Scale(LOWER_ARM_WIDTH,
			LOWER_ARM_HEIGHT,
			LOWER_ARM_WIDTH));

	glUniformMatrix4fv(model, 1, GL_TRUE, model_view * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void upper_arm(int model, mat4 model_view)
{
	mat4 instance = (Translate(0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0) *
		Scale(UPPER_ARM_WIDTH,
			UPPER_ARM_HEIGHT,
			UPPER_ARM_WIDTH));

	glUniformMatrix4fv(model, 1, GL_TRUE, model_view * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}
//----------------------------------RENDER--------------------------------

void arm_init() {
	colorcube();
	
	glGenVertexArrays(1, &armVAO);
	glBindVertexArray(armVAO);
	// Create and initialize a buffer object

	glGenBuffers(1, &armVBO);
	glBindBuffer(GL_ARRAY_BUFFER, armVBO);
	// empty buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
}

void renderArm(int projection, int program) {
	glBindVertexArray(armVAO);
	glBindBuffer(GL_ARRAY_BUFFER, armVBO);

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

}