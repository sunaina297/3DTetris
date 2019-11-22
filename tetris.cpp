#include "constants.h"
#include "helper.h"

// may need to be changed
//const int Num = 7200;
//const int cube_vertices = 1600; // 10 cube vertices

static GLuint tetroVAO;
static GLuint tetroBuffer;


// 4 tiles per tetromino; 8 vertices per cube 
static const int NumNewTetroVertices = 32;
static point4 tetrominoPts[NumNewTetroVertices];
static int IndexTetro = 0;

// 4 squares * 6 sides * 2 triangles * 3 vertices = 144 points
//static const int NumTetroCPts = 144;
static color4 tetroColr[NumTetroCPts] = { 0 };
static color4 greyColor[NumTetroCPts] = { 0 };
static point4 tetroCubePts[NumTetroCPts] = { 0 };
static int IndexTCube = 0;


// --------------------------RETRIEVE------------------------

bool** curr_tetro_block = H_set_random_tetris_block();
color4 tile_colors[4] = { 0 };


point4* getTetroCubePoints() {
	point4* tet = tetroCubePts;
	return tet;
}

color4* getTetroCubeColor() {
	point4* tet = tetroColr;
	return tet;
}


// --------------------------INIT------------------------


// NEW tetro block -- sets color too
// just vertices --> 8 POINTS * 4 CUBES = 32 POINTS
static void tetris_NewVertices() {

	//int new_tetro_x = H_setRandTile();
	int new_tetro_x = 0;
	//curr_tetro_block = H_set_random_tetris_block();
	int curr_tetro_x = 0;
	int Top = 4;
	// vertices of new tetro;
	for (int x = new_tetro_x; x < new_tetro_x+4; x++) {
		for (int y = 0; y < 4; y++) {
			if (curr_tetro_block[curr_tetro_x][y]) {

				 // front cube
				tetrominoPts[IndexTetro] = point4(x, Top - y-1, 0.5, 1.0); IndexTetro++;
				tetrominoPts[IndexTetro] = point4(x, Top - y , 0.5, 1.0); IndexTetro++;
				tetrominoPts[IndexTetro] = point4(x+1, Top - y, 0.5, 1.0); IndexTetro++;
				tetrominoPts[IndexTetro] = point4(x+1, Top - y - 1, 0.5, 1.0); IndexTetro++;

				//back cube 
				tetrominoPts[IndexTetro] = point4(x, Top - y - 1, -0.5, 1.0); IndexTetro++;
				tetrominoPts[IndexTetro] = point4(x, Top - y, -0.5, 1.0); IndexTetro++;
				tetrominoPts[IndexTetro] = point4(x+1, Top - y, -0.5, 1.0); IndexTetro++;
				tetrominoPts[IndexTetro] = point4(x+1, Top - y - 1, -0.5, 1.0); IndexTetro++;

			}
		}
		curr_tetro_x++;
	}

	/*for (int i = 0; i < 32; i++) {
		printf("tetro points %f %f  %f\n", tetrominoPts[i][0], tetrominoPts[i][1], tetrominoPts[i][2]);
	}*/
	//printf("number of vertices %d\n", IndexTetro);
	IndexTetro = 0;
}



// --------------------VERTECES TO CUBE----------------------

static void quad(int a, int b, int c, int d, point4 color) {
	tetroCubePts[IndexTCube] = tetrominoPts[a]; 
	//printf("tetro points %f %f  %f at ind %d\n", tetroCubePts[IndexTCube][0], tetroCubePts[IndexTCube][1], tetroCubePts[IndexTCube][2], IndexTCube);
	tetroColr[IndexTCube] = color; IndexTCube++;
	tetroCubePts[IndexTCube] = tetrominoPts[b];
	//printf("tetro points %f %f  %f at ind %d\n", tetroCubePts[IndexTCube][0], tetroCubePts[IndexTCube][1], tetroCubePts[IndexTCube][2], IndexTCube);
	tetroColr[IndexTCube] = color; IndexTCube++;
	tetroCubePts[IndexTCube] = tetrominoPts[c];
	//printf("tetro points %f %f  %f at ind %d\n", tetroCubePts[IndexTCube][0], tetroCubePts[IndexTCube][1], tetroCubePts[IndexTCube][2], IndexTCube);
	tetroColr[IndexTCube] = color; IndexTCube++;
	tetroCubePts[IndexTCube] = tetrominoPts[a];
	//printf("tetro points %f %f  %f at ind %d\n", tetroCubePts[IndexTCube][0], tetroCubePts[IndexTCube][1], tetroCubePts[IndexTCube][2], IndexTCube);
	tetroColr[IndexTCube] = color; IndexTCube++;
	tetroCubePts[IndexTCube] = tetrominoPts[c];
	//printf("tetro points %f %f  %f at ind %d\n", tetroCubePts[IndexTCube][0], tetroCubePts[IndexTCube][1], tetroCubePts[IndexTCube][2], IndexTCube);
	tetroColr[IndexTCube] = color; IndexTCube++;
	tetroCubePts[IndexTCube] = tetrominoPts[d];
	//printf("tetro points %f %f  %f at ind %d\n", tetroCubePts[IndexTCube][0], tetroCubePts[IndexTCube][1], tetroCubePts[IndexTCube][2], IndexTCube);
	tetroColr[IndexTCube] = color; IndexTCube++;
	
}


void tetris_newTetro() {
	tetris_NewVertices();

	tile_colors[0] = H_change_color();
	tile_colors[1] = H_change_color();
	tile_colors[2] = H_change_color();
	tile_colors[3] = H_change_color();
	int tile_ind = 0;

	int ind = 0;
	color4 color;
	int colrInd = 0;
	while (ind < NumNewTetroVertices) {
		color = tile_colors[tile_ind];
		tile_ind++;
		// 6 sides of one cube 
		quad(ind+1, ind, ind+3, ind + 2, color);
		quad(ind + 2, ind + 3, ind + 7, ind + 6, color);
		quad(ind + 3, ind , ind + 4, ind + 7, color);
		quad(ind + 6, ind + 5, ind + 1, ind + 2, color);
		quad(ind + 4, ind + 5, ind + 6, ind + 7, color);
		quad(ind + 5, ind + 4, ind , ind + 1, color);
		ind+=8;
		//printf("ind %d\n", ind);
		//printf("tetro points %f %f  %f \n", color[0], color[1], color[2]);
	}

	/*for (int i = 0; i < 4; i++) {
		printf("tetro points %f %f  %f at ind %d\n", tetroCubePts[i][0], tetroCubePts[i][1], tetroCubePts[i][2], i);
	}*/
	IndexTCube = 0;

}



// --------------------CHANGED VERTICES OR POINTS----------------------
int Zval = 6;
void tetris_rotateTetro() {

	curr_tetro_block = H_rotate(curr_tetro_block);
	tetris_NewVertices();

	int tile_ind = 0;
	int ind = 0;
	color4 color;

	while (ind < NumNewTetroVertices) {
		color = tile_colors[tile_ind];
		tile_ind++;
		//color4 color = color4(0.3, 0.30, 0.3, 1.0);  //test
		// 6 sides of one cube 
		quad(ind + 1, ind, ind + 3, ind + 2, color);
		quad(ind + 2, ind + 3, ind + 7, ind + 6, color);
		quad(ind + 3, ind, ind + 4, ind + 7, color);
		quad(ind + 6, ind + 5, ind + 1, ind + 2, color);
		quad(ind + 4, ind + 5, ind + 6, ind + 7, color);
		quad(ind + 5, ind + 4, ind, ind + 1, color);
		ind += 8;
		//printf("ind %d\n", ind);
		//printf("tetro points %f %f  %f \n", color[0], color[1], color[2]);
	}
	IndexTCube = 0;

	printf(" tetro pts %d and colr %d", sizeof(tetroCubePts), sizeof(tetroColr));
	glBindVertexArray(tetroVAO);
	glBindBuffer(GL_ARRAY_BUFFER, tetroBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tetroCubePts), tetroCubePts);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(tetroCubePts), sizeof(tetroColr), tetroColr);

}

void newTetro() {
	curr_tetro_block = H_set_random_tetris_block();
	tetris_newTetro();

	glBindVertexArray(tetroVAO);
	glBindBuffer(GL_ARRAY_BUFFER, tetroBuffer);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tetroCubePts), tetroCubePts);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(tetroCubePts), sizeof(tetroColr), tetroColr);
}
// --------------------------RENDER------------------------

void tetris_init() {

	tetris_newTetro();
	H_fillColor(144, greyColor, color4(0.3, 0.30, 0.3, 1.0));

	glGenVertexArrays(1, &tetroVAO);
	glBindVertexArray(tetroVAO);
	// Create and initialize a buffer object

	glGenBuffers(1, &tetroBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tetroBuffer);
	// empty buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetroCubePts) + sizeof(tetroColr), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tetroCubePts), tetroCubePts);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(tetroCubePts), sizeof(tetroColr), tetroColr);


}

// get new set of vertex points after rotation 
// create new set of points 

void renderTetro(int projection, int program, bool grey) {

	// vertices will "drop"
	//tetris_NewTetro();

	glBindVertexArray(tetroVAO);
	glBindBuffer(GL_ARRAY_BUFFER, tetroBuffer);

	if (grey == 1) {
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(tetroCubePts), sizeof(tetroColr), greyColor);
	}
	else
	{
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(tetroCubePts), sizeof(tetroColr), tetroColr);
	}
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));


	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(tetroCubePts)));

	projection = glGetUniformLocation(program, "projection");

	//printf("sz color %d and points %d\n", sizeof(savedTetroColor), sizeof(savedtetroPts));
	for (int i = 0; i < 4; i++) {
		//printf("tetro points %f %f  %f at - ind %d\n", tetroCubePts[i][0], tetroCubePts[i][1], tetroCubePts[i][2], i);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, NumTetroCPts);
}

