#include "Angel.h"
#include "render.h"



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 600);
	glutInitWindowPosition(400, 20);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Color Cube");
	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutDisplayFunc(display);
	glutSpecialFunc(keyboard);
	glutKeyboardFunc(arm_keyboard);

	glutTimerFunc(0, time_display, 0);
	glutMainLoop();
	return 0;
}