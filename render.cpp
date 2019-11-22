#include "constants.h"
#include "grid.h"
#include "tetris.h"
#include "arm.h"
#include "helper.h"
#include "savedTetro.h"

#include <chrono>

//----------------------------VARIABLES----------------------------------------
// for movements 
static GLfloat  left = -12.0, right = 12.0;
static GLfloat  bottom = -2.0, top = 21.0;
static GLfloat  zNear = -10, zFar = 15;

static GLfloat radius = 1.0;
static GLfloat theta = 0.0;
static GLfloat phi = 0.0;

static const GLfloat  dr = 5.0 * DegreesToRadians;
static GLfloat steady = 0.0;

// Array of rotation angles (in degrees) for each rotation axis
enum { Base = 0, LowerArm = 1, UpperArm = 2, Tile = 3, NumAngles = 4 };
static int      Axis = Base;
static GLfloat  Theta[NumAngles] = { 0.0 ,30.0 ,-120.0, 90 };
static mat4  mv;
static bool drop = 0;
static point4 Translated_Tetro_Points[144] = { 0 };
static color4 Translated_Tetro_Color[144] = { 0 };

// tetro 
static int NUM_TILES_DROPPED = 0;
static bool grey = 0;
static point4* savedtetroPts;
static color4* savedTetroColor;
float zVal = 0;
int flip = 1;

// model 
static GLuint program;
static GLuint  ModelView, projection;
static mat4  model_view;


// for timer 
//typedef std::chrono::high_resolution_clock Clock;
static const int time_end = 30;
int timed = 0;
auto start = std::chrono::steady_clock::now();
auto updated = std::chrono::steady_clock::now();
auto elapsed = 10;

// -------------------------------HELPER-------------------------------------

// drop the current tile into grid 
void drop_tile() {
	grey = checkIfSaved(savedtetroPts, Translated_Tetro_Points);
	if (grey == 0) {
		NUM_TILES_DROPPED++;
		// save those points 

		savedtetroPts = AddTetro(Translated_Tetro_Points, savedtetroPts, NUM_TILES_DROPPED);
		savedTetroColor = setSavedColor(savedTetroColor, NUM_TILES_DROPPED);


		// get new tetro
		newTetro();

		//start = Clock::now();

		glutPostRedisplay();
		timed = 0;
	}
}


//----------------------------KEYBOARD-------------------------------------------

// movements and rotations of arm 
static void arm_left_move() {
	Theta[Axis] += 5.0;
	if (Theta[Axis] > 360.0) { Theta[Axis] -= 360.0; }

	glutPostRedisplay();
}

static void arm_right_move() {
	Theta[Axis] -= 5.0;
	if (Theta[Axis] < 0.0) { Theta[Axis] += 360.0; }

	glutPostRedisplay();
}


void
keyboard(int key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case GLUT_KEY_LEFT:
		if (glutGetModifiers() & GLUT_ACTIVE_CTRL) {
			theta += dr;
		}
		break;
	case GLUT_KEY_RIGHT:
		if (glutGetModifiers() & GLUT_ACTIVE_CTRL) {
			theta -= dr;
		}
		break;
	case GLUT_KEY_UP:
		tetris_rotateTetro();
		break;
	case GLUT_KEY_DOWN:
		Axis = Tile;
		arm_left_move();
		break;
}

	glutPostRedisplay();
}


void arm_keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		delete[] savedTetroColor;
		delete[] savedtetroPts;
		exit(EXIT_SUCCESS);
		break;
		// lower arm 
	case 'w': 
		Axis = LowerArm;
		arm_left_move();
		break;
	case 's': 
		Axis = LowerArm;
		arm_right_move();
		break;
		// upper arm
	case 'a':
		steady += 5;
		Axis = UpperArm;
		arm_left_move();
		break;
	case 'd': 
		steady -= 5;
		Axis = UpperArm;
		arm_right_move();
		break;
	case 'r':
		savedtetroPts = H_restartTetro(savedtetroPts, NUM_TILES_DROPPED);
		savedTetroColor = H_restartColor(savedTetroColor, NUM_TILES_DROPPED);
		NUM_TILES_DROPPED = 0;
		left = -12.0, right = 12.0;
		bottom = -2.0, top = 21.0;
		zNear = -10, zFar = 15;
		radius = 1.0;
		theta = 0.0;
		phi = 0.0;
		steady = 0.0;
		Theta[Tile] =  90 ;
		Theta[UpperArm] = -120.0 ;
		Theta[LowerArm] =  30.0  ;
		Theta[Base] =  0.0 ;
		newTetro();
		break;
	case ' ':
		drop_tile();
		break;
		
	}
	

	glutPostRedisplay();
}



// -------------------------HELPER DISPLAY-----------------------------

void display_arm() {
	// ARM
	renderArm(projection, program);

	model_view = RotateY(Theta[Base]);
	base(ModelView, mv);
	mv *= (Translate(-4.0, BASE_HEIGHT, 7.0) * RotateZ(Theta[LowerArm]));
	lower_arm(ModelView, mv);
	mv *= (Translate(0.0, LOWER_ARM_HEIGHT, 0.0) * RotateZ(Theta[UpperArm]));
	upper_arm(ModelView, mv);
}
//----------------------------DISPLAY-------------------------------------


// does not necessarily end game
// if grey tetro and no time then end
// else start new game 
void game_over() {
	if (grey == 1) {
		savedtetroPts = H_restartTetro(savedtetroPts, NUM_TILES_DROPPED);
		savedTetroColor = H_restartColor(savedTetroColor, NUM_TILES_DROPPED); 
		NUM_TILES_DROPPED = 0;
		left = -12.0, right = 12.0;
		bottom = -2.0, top = 21.0;
		zNear = -10, zFar = 15;
		radius = 1.0;
		theta = 0.0;
		phi = 0.0;
		steady = 0.0;
		Theta[Tile] = 90;
		Theta[UpperArm] = -120.0;
		Theta[LowerArm] = 30.0;
		Theta[Base] = 0.0;
		newTetro();
		printf("GAME OVER.\n NEW GAME.");
		timed = 0;

	}
	else {
		drop_tile();
		timed = 0;
	}
	glutPostRedisplay();
}



// initialize display arrays 
void init()
{

	glMatrixMode(GL_MODELVIEW);

	grid_init();
	tetris_init();
	arm_init();
	savedTetris_init();


	// Load shaders and use the resulting shader program
	program = InitShader("vshader81.glsl", "fshader81.glsl");
	glUseProgram(program);
	ModelView = glGetUniformLocation(program, "model_view");
	projection = glGetUniformLocation(program, "projection");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}



// keep track of timer 
// 30 secs to place tetro into grid 
void time_display(int id) {
	glutTimerFunc(1000, time_display, 0);
	timed += 1;
	/*
	updated = Clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(updated - start).count();
	

	int timer = time - elapsed;*/
	std::cout << time_end -timed;
	std::cout << "\n";
	if (time_end -timed <= 0) {
		game_over();
	}
}


void display() {


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4  p = Ortho(left, right, bottom, top, zNear, zFar);
	glUniformMatrix4fv(projection, 1, GL_TRUE, p);

	point4  eye(radius * sin(theta) * cos(phi),0.0f,radius * cos(theta),1.0);
	point4  at(0.0, 0.0, 0.0, 1.0);
	vec4    up(0.0, 1.0, 0.0, 0.0);

	mv = LookAt(eye, at, up);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, mv);

	// GRID
	renderGridLines(projection, program, ModelView);
	renderGridCube(projection, program);

	// ARM
	display_arm();

	// for current tetro
	mv *= (Translate(0.0, UPPER_ARM_HEIGHT-1, 0.0) * RotateZ(Theta[Tile]-steady));
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, mv );




	// possible places to drop
	point4* original = getTetroCubePoints();
	vec4 testNegativeZ =  mv * original[0];
	float newZ = testNegativeZ[2];
	if (newZ < -0.5) {
		flip = -1;
	}

	mv *= (Translate(0.0, 0.0, 0.0) * RotateZ(-(Theta[Tile] - steady)) * RotateZ(-(Theta[UpperArm])) * RotateZ(-(Theta[LowerArm])));
	for (int i = 0; i < NumTetroCPts; i++) {

		Translated_Tetro_Points[i] = original[i];
		zVal = original[i][2];
		Translated_Tetro_Points[i] = mv * Translated_Tetro_Points[i];

		Translated_Tetro_Points[i][0] = floor(Translated_Tetro_Points[i][0])*flip;
		Translated_Tetro_Points[i][1] = floor(Translated_Tetro_Points[i][1]);
		Translated_Tetro_Points[i][2] = zVal;
	}

	// check translated points is inside box
	//if inside box, check if collision
	
	grey = checkIfSaved(savedtetroPts, Translated_Tetro_Points);
	




	// TETRO CURRENT
	renderTetro(projection, program, grey);

	// SAVED TETRO
	mv = LookAt(eye, at, up);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, mv);
	PlaceSavedInBuffer(savedtetroPts, savedTetroColor, projection, program);


	glutSwapBuffers();
	flip = 1;

	
}   