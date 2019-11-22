#include "Angel.h"
#pragma once
#ifndef _helper_h
#define _helper_h


color4 H_change_color(void);

int H_setRandTile(void);

bool** H_set_random_tetris_block(void);

bool** H_rotate(bool** tetro_curr);

void H_fillColor(int len, point4* colr, color4 cor);

color4* H_restartColor(color4* end, int drop);

point4* H_restartTetro(point4* end, int drop);
#endif