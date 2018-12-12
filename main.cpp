#include <gl/glut.h>
#include <cmath>

GLint vert[100][2];
GLint point[100][3];	// (x, y, in_out_side_mark)
bool poly_finished = false;
GLint vertex1[2], vertex2[2];
int width = 700, height = 700, n = 0, m = 0,
type = GL_LINE_STRIP, v;
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (n == 1 && (type == GL_LINE_STRIP || type == GL_LINE_LOOP))
	{
		glBegin(GL_POINTS);
			glVertex2iv(vert[0]);
		glEnd();
	}
	glBegin(type);
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i < n; i++)
		glVertex2iv(vert[i]);
	glEnd();
	if (poly_finished)
	{
		glBegin(GL_POINTS);
		for (int i = 0; i < m; i++)
		{
			if (point[i][2] == 1)
				glColor3f(0.0, 1.0, 0.0);
			else if (point[i][2] == -1)
				glColor3f(1.0, 0.0, 0.0);
			else
				glColor3f(0.0, 0.0, 1.0);	// Error point will be drew as blue, but it
											// will never happen, and its purpose is to
											// close all the cases.
			glVertex2iv(point[i]);
		}
		glEnd();
	}
	glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'r': n = 0;
				m = 0;
				type = GL_LINE_STRIP;
				poly_finished = false;
				break;
		case 'c': type = GL_LINE_LOOP;
				poly_finished = true;
				break;
	}
	glutPostRedisplay();
}
int findVertex(int x, int y)
{
	int dx, dy;
	for (int i = 0; i < n; i++)
	{
		dx = vert[i][0] - x;
		dy = vert[i][1] - y;
		if (dx*dx + dy * dy < 16)
			return i;
	}
	return -1;
}
void calculate_vertices(int x, int y, int i, int j)
{
	int x1 = vert[i][0];
	int y1 = vert[i][1];
	int x2 = vert[j][0];
	int y2 = vert[j][1];
	vertex1[0] = x1 - x;
	vertex1[1] = y1 - y;
	vertex2[0] = x2 - x;
	vertex2[1] = y2 - y;
}
double angle_value()
{
	int x1 = vertex1[0];
	int y1 = vertex1[1];
	int x2 = vertex2[0];
	int y2 = vertex2[1];
	double v1v2 = x1 * x2 + y1 * y2;	// Int -> Double
	double v1_magnitute = sqrt(x1 * x1 + y1 * y1);
	double v2_magnitute = sqrt(x2 * x2 + y2 * y2);
	double cos = v1v2 / (v1_magnitute * v2_magnitute);
	return acos(cos);
}
double angle_direction()
{
	int x1 = vertex1[0];
	int y1 = vertex1[1];
	int x2 = vertex2[0];
	int y2 = vertex2[1];
	int fact = x1 * y2 - x2 * y1;
	if (fact > 0)
		return 1.0;
	else
		return -1.0;
}
int is_inside(int x, int y)
{
	int front = 0;
	int back = front + 1;
	double temp_angle = 0;
	double radian_sum = 0;
	while (front < n)
	{
		calculate_vertices(x, y, front, back);
		temp_angle = angle_value();
		temp_angle *= angle_direction();
		radian_sum += temp_angle;
		front++;
		back = front + 1;
		if (back == n)
			back = 0;
	}
	radian_sum = abs(radian_sum);
	if (radian_sum > 6.283 && radian_sum < 6.284)	// Tolerance = 0.001
		return 1;	// Inside
	else if (radian_sum < 0.001)
		return -1;	// Outside
	else
		return 0;	// Error mark, indicate the improper result
}
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			if (!poly_finished)
			{
				v = n++;
				vert[v][0] = x;
				vert[v][1] = height - 1 - y;
			}
			else
			{
				v = m++;
				point[v][0] = x;
				point[v][1] = height - 1 - y;
				point[v][2] = is_inside(x, height - 1 - y);	// 1: insde, -1: outside, 0: ERROR
			}
			glutPostRedisplay();
		}
		break;
	}
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 100);
	glutCreateWindow("Project 3");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();
}