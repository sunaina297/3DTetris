#pragma once


void base(int model, mat4 model_view);


void arm_init();

void renderArm(int projection, int program);

void upper_arm(int model, mat4 model_view);

void lower_arm(int model, mat4 model_view);