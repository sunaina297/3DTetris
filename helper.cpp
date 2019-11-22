#include "constants.h"
#include <time.h>


// returns a random color
color4 H_change_color(void) {
	color4 rand_color;
	int num = rand() % 5;

	if (num == 0) {
		rand_color= color4(0.1f, 1.0f, 0.3f,1.0f);
	}
	else if (num == 1) {
		rand_color= color4(1.0f, 0.30f, 0.3f,1.0);
	}
	else if (num == 2) {
		rand_color= color4(0.0f, 0.30f, 0.9f,1.0f);

	}
	else if (num == 3) {
		rand_color= color4(0.7f, 0.70f, 0.3f,1.0f);

	}
	else if (num == 4) {
		rand_color= color4(0.8f, 0.4f, 0.3f,1.0f);

	}

	return rand_color;
}


// return random x position to be used 
int H_setRandTile() {
	srand(time(NULL));

	int randLocation = rand() % 7 + (-5);

	return randLocation;

}


// returns a "matrix" of random tile  
bool** H_set_random_tetris_block(void) {
	srand(time(NULL));
	bool** tetro_block = 0;
	tetro_block = new bool* [4];
	int tetris_random_index[3] = { 0,4,8 };
	int rand_tetris_index = rand() % 3;
	int index = tetris_random_index[rand_tetris_index];

	for (int i = 0; i < 4; i += 1) {
		tetro_block[i] = new bool[4];
		for (int j = 0; j < 4; j += 1) {
			tetro_block[i][j] = tetro[index][j];
			//printf("treo bool %d %d", i, j);
		}
		index += 1;
	}
	return tetro_block;

}


// rotates tetromino block 
bool** H_rotate(bool** tetro_curr) {
	bool** tetro_block = 0;
	tetro_block = new bool* [4];
	//init
	for (int i = 0; i < 4; i += 1) {
		tetro_block[i] = new bool[4];
		for (int j = 0; j < 4; j += 1) {
			tetro_block[i][j] = 0;
			//printf("treo bool %d %d", i, j);
		}
	}



	// reotate tetro block
	for (int i = 1; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tetro_block[j][i] = tetro_curr[i - 1][3 - j];
			//printf("new block: %d\n", tetro_block[j][i]);
		}

	}

	//cant set in loop
	tetro_block[0][0] = tetro_curr[3][3];
	tetro_block[1][0] = tetro_curr[3][2];
	tetro_block[2][0] = tetro_curr[3][1];
	tetro_block[3][0] = tetro_curr[3][0];

	return tetro_block;

}


// fills a color array with gray tone 
//color4(0.3, 0.30, 0.3, 1.0)
void H_fillColor(int len, point4* colr, color4 cor) {
	for (int i = 0; i < len; i++) {
		colr[i] = cor;
	}
}

point4* H_restartTetro(point4* end, int drop) {

	for (int i = 0; i < NumTetroCPts * drop; i++) {
		end[i] = point4(0.0, 0.0, 0.0, 1.0);
	}
	return end;
}

color4* H_restartColor(color4* end, int drop) {
	for (int i = 0; i < NumTetroCPts * drop; i++) {
		end[i] = color4(0.0, 0.0, 0.0, 1.0);
	}
	return end;
}