#pragma once

#ifndef _grid_h
#define _grid_h

void grid_init();

void renderGridCube(int projection, int program);

void renderGridLines(int projection, int program, int ModelView);

#endif