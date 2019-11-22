#include "constants.h"
#include "tetris.h"
#include "helper.h"

static GLuint savedTetroVAO;
static GLuint savedTetroBuffer;


// 4 squares * 6 sides * 2 triangles * 3 vertices = 144 points
//const int NumTetroCPts = 144;
static color4* savedTetroColor;
static point4* savedtetroPts;
int num_tetroPTS_so_far = 0;


//---------------------------CHECK FOR GREY-------------------------
bool checkIfOutside(point4* saved, point4* translated) {
	int index = 0;
	bool grey = 0;
	int x = 0, y = 0;
	while (index < NumTetroCPts && grey == 0) {
		x = translated[index][0];
		y = translated[index][1];

		if (x > RightEdge || x < LeftEdge || y > Top || y < Bottom){
			grey = 1;
		}



		index++;
	}

	return grey;
}
// once a Tetro is dropped, saved it into the savedTetro array 
//and remove from currentTetro




bool checkIfSaved(point4* saved, point4* translated) {
	
	bool grey = 0;
	int ind = 0;
	const int offset = 36;
	int i = 0;
	
	grey = checkIfOutside(saved, translated);

	while (grey == 0 && ind < num_tetroPTS_so_far) {

		//printf(" ind value %d\n", i);
		for (int j = 0; j < 4; j++) {
			//printf(" i value %d\n", i);
			// check front face of every saved cube for match with current possible move;
			if ((saved[ind][0] == translated[i][0] && saved[ind][1] == translated[i][1]) &&
				(saved[ind + 1][0] == translated[i + 1][0] && saved[ind + 1][1] == translated[i + 1][1]) &&
				(saved[ind + 2][0] == translated[i + 2][0] && saved[ind + 2][1] == translated[i + 2][1]) &&
				(saved[ind + 3][0] == translated[i + 3][0] && saved[ind + 3][1] == translated[i + 3][1]) &&
				(saved[ind + 4][0] == translated[i + 4][0] && saved[ind + 4][1] == translated[i + 4][1]) &&
				(saved[ind + 5][0] == translated[i + 5][0] && saved[ind + 5][1] == translated[i + 5][1]))
			{
				grey = 1;
				//printf("collided\n");
			}

			i += offset;
			// if we checked all 4 cubes, then we're checing next saved tetromino
			if (i > NumTetroCPts) {
				i = 0;
			}
		}


		ind += offset;
	}

	return grey;

}


// -------------------------SAVED------------------------------------
 point4* AddTetro(point4* translatedTetro, point4* savedtetroPts, int dropped) {
	//printf("here");
	num_tetroPTS_so_far = NumTetroCPts* dropped;  //144
	point4* saved_tetro_add = new point4[num_tetroPTS_so_far];

	int index_translated = 0;
 
		for (int i = 0; i < num_tetroPTS_so_far; i++) {

			if (i < num_tetroPTS_so_far- NumTetroCPts) {
 				saved_tetro_add[i] = savedtetroPts[i];
				//printf("old stuff\n");
			}

			else {
				saved_tetro_add[i] = translatedTetro[index_translated];
				index_translated++;
				//printf("test: %f %f %f\n", saved_tetro_add[i][0], saved_tetro_add[i][1], saved_tetro_add[i][2]);

			}
		}

		delete[] savedtetroPts;
		return saved_tetro_add;
}


color4* setSavedColor( point4* savedTetroColor, int dropped) {
	color4* currentColr = getTetroCubeColor();
	point4* saved_tetro_colr = new point4[num_tetroPTS_so_far];
	int index_translated = 0;

	// can only drop once into grid
	// zval is looking at position of z axis 
	for (int i = 0; i < num_tetroPTS_so_far; i++) {

		if (i < num_tetroPTS_so_far - NumTetroCPts) {
			saved_tetro_colr[i] = savedTetroColor[i];
		}
		else {
			saved_tetro_colr[i] = currentColr[index_translated];
			index_translated++;
			//printf("color : %f %f %f\n", saved_tetro_colr[i][0], saved_tetro_colr[i][1], saved_tetro_colr[i][2]);

		}
	}

	delete[] savedTetroColor;
	return saved_tetro_colr;
}


// --------------------------RENDER------------------------

void savedTetris_init() {

	glGenVertexArrays(1, &savedTetroVAO);
	glBindVertexArray(savedTetroVAO);
	// Create and initialize a buffer object

	glGenBuffers(1, &savedTetroBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, savedTetroBuffer);

	//empty
	glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 0, savedtetroPts);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 0, savedTetroColor);

}

// get new set of vertex points after rotation 
// create new set of points 


color4* FillSavedColor(color4* colr) {

	savedTetroColor = new point4[num_tetroPTS_so_far];
	H_fillColor(144, savedTetroColor, color4(0.3, 0.30, 0.3, 1.0));

	return savedTetroColor;
}



void PlaceSavedInBuffer( point4* pts, color4* colr, int projection, int program) {

	int size = sizeof(pts) * num_tetroPTS_so_far* 4;
	//point4* original = getTetroCubePoints();
	//printf("saved sz %d\n", size);

	/*if (pts != NULL) {
		for (int i = 0; i < num_tetroPTS_so_far; i++) {
			
			printf("tetro points %f %f  %f at - ind %d\n", pts[i][0], pts[i][1], pts[i][2], i);
			printf("color points %f %f  %f at - ind %d\n", colr[i][0], colr[i][1], colr[i][2], i);
			printf("\n");

		}
	}*/
	

	//glGenVertexArrays(1, &savedTetroVAO);
	glBindVertexArray(savedTetroVAO);

	//glGenBuffers(1, &savedTetroBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, savedTetroBuffer);

	glBufferData(GL_ARRAY_BUFFER, size *2, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, pts);
	glBufferSubData(GL_ARRAY_BUFFER, size, size, colr);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));


	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(size));

	projection = glGetUniformLocation(program, "projection");

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, num_tetroPTS_so_far);
}