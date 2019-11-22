#pragma once



void savedTetris_init();

point4* AddTetro(point4* translatedTetro, point4* savedtetroPts, int dropped);

void PlaceSavedInBuffer(point4* pts, color4* colr, int projection, int program);

color4* FillSavedColor(color4* colr);

color4* setSavedColor(point4* savedTetroColor, int dropped);

bool checkIfOutside(point4* saved, point4* translated);

bool checkIfSaved(point4* saved, point4* translated);

