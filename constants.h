#include "Angel.h"
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#pragma once 

#ifndef _const_h
#define _const_h

const float LeftEdge = -5;
const float RightEdge = 5;
const float Top = 20;
const float Bottom = 0;

// for grid 
const int NumLineVer = 120;
const int NumGrid = 24;


const bool tetro[12][4] = {
	{0, 0, 0, 0},
	{1 ,1 ,1 ,1},
	{0, 0, 0, 0},
	{0, 0, 0, 0},

	{0, 0, 0, 0},
	{0 ,0 ,1 ,1},
	{0, 1, 1, 0},
	{0, 0, 0, 0},

	{0, 0, 0, 0},
	{0 ,1 ,1 ,1},
	{0, 1, 0, 0},
	{0, 0, 0, 0},

};

const GLfloat BASE_HEIGHT = 2.0;
const GLfloat BASE_WIDTH = 5.0;
const GLfloat LOWER_ARM_HEIGHT = 10.0;
const GLfloat LOWER_ARM_WIDTH = 0.5;
const GLfloat UPPER_ARM_HEIGHT = 9.0;
const GLfloat UPPER_ARM_WIDTH = 0.5;

const int NumTetroCPts = 144;
#endif