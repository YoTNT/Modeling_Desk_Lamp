/********* Instruction***************/
/*
	Press f to fasten the animation.
	Press s to slow down the animation.
	Press t to turn on or off the lamp.
*/

#include <GL/glut.h>

GLfloat pos[] = { -2, 4, 5, 1 },
		pos2[] = {0, 0, 999, 0},
		amb[] = { 0.3, 0.3, 0.3, 1.0 };
GLfloat front_amb_diff_off[] = { 0.6, 0.3, 0.2, 1.0 };
GLfloat front_amb_diff_on[] = { 1.0, 1.0, 0, 0};
GLfloat front_amb_diff2[] = { 0.9, 0.9, 0.9, 1.0 };
GLfloat front_amb_diff3[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat front_amb_diff4[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat back_amb_diff[] = { 0.4, 0.7, 0.1, 1.0 };
GLfloat spe[] = { 0.25, 0.25, 0.25, 1 };
GLfloat theta = 0, dt = 0.1, axes[3][3] = { {1,0,0}, {0,1,0}, {0,0,1} };
int axis = 0;
bool turn_on = false;
bool flag = true;

void draw_cover(void) {
	glPushMatrix();
	if (!turn_on) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_amb_diff_off);
		glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, front_amb_diff_off);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_amb_diff_on);
		glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, front_amb_diff_on);
	}
	GLUquadricObj *quadObj;
	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_LINE);
	gluCylinder(quadObj, 1.0, 0.5, 1.2, 360, 180);

	glPopMatrix();
}

void draw_bulb(void) {
	glPushMatrix();
	if(!turn_on)
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_amb_diff2);
	else
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_amb_diff_on);
	GLUquadricObj *quadObj;
	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	glTranslated(0, 0, 0.5);
	gluSphere(quadObj, 0.35, 36, 30);

	glPopMatrix();
}

void draw_cylinder(void) {
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_amb_diff3);
	glTranslated(0, 0, -0.55);
	glScaled(1, 1, 6);
	glutSolidCube(0.3);
	glPopMatrix();
}

void draw_base(void) {
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_amb_diff4);
	glTranslated(0, 0, -1.5);
	glutSolidTorus(0.1, 0.6, 48, 96);
	glutSolidCone(0.65, 0.3, 40, 20);

	glPopMatrix();
}

void draw_lamp(void) {
	draw_cover();
	draw_bulb();
	draw_cylinder();
	draw_base();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	if (axis < 3)
		glRotated(theta, axes[axis][0], axes[axis][1], axes[axis][2]);
	else {
		if (!turn_on) {
			glPushMatrix();
			glRotated(theta, 0, 0, 1);
			glLightfv(GL_LIGHT0, GL_POSITION, pos);
			glPopMatrix();
		}
	}
	if (turn_on) {
		glPushMatrix();
		glLightfv(GL_LIGHT0, GL_POSITION, pos2);
		glPopMatrix();
	}
	draw_lamp();
	glPopMatrix();
	glutSwapBuffers();
}
void idle(void) {
	if (theta >= 360) axis = (axis + 1) % 4;
	theta = (theta < 360) ? theta + dt : dt;
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 't':
			if (turn_on == false)
				turn_on = true;
			else
				turn_on = false;
			break;
		case 'f':
			if (dt >= 0.4)
				break;
			else
				dt += 0.1;
			break;
		case 's':
			if (dt <= 0.1)
				break;
			else
				dt -= 0.1;
			break;
	}
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("GLUT Objects");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1.0, 2, 8);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 75);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0, 0, -5);
	glRotated(-55, 1, 0, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}