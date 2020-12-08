#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include <GL/glut.h>

#define window_Width 800
#define window_Height 900

#define body_Cnt 16
GLUquadricObj *pObj[body_Cnt];

/*
	  [ Body Modeling ]

			  B
			  |
	  C - s - A - s - D
			|   |
			s   s
			|   |
			E   F

  part s : joint
  part A : torso
  part B : head, neck
  part C : left_upper_arm, left_lower_arm, left_hand
  part D : right_upper_arm, right_lower_arm, right_hand
  part E : left_upper_leg, left_lower_leg, left_foot
  part F : right_upper_leg, right_lower_leg, right_foot
*/

#define torso_Radius 0.15
#define torso_Height 0.6
#define torso_dist 0.025
#define head_Radius 0.16
#define neck_Radius 0.05
#define neck_Height 0.1
#define joint_Radius 0.04
#define left_upper_arm_Radius 0.05
#define left_upper_arm_Height 0.25
#define left_lower_arm_Radius 0.04
#define left_lower_arm_Height 0.22
#define left_hand_Radius 0.04
#define right_upper_arm_Radius 0.05
#define right_upper_arm_Height 0.25
#define right_lower_arm_Radius 0.04
#define right_lower_arm_Height 0.22
#define right_hand_Radius 0.04
#define left_upper_leg_Radius 0.065
#define left_upper_leg_Height 0.37
#define left_lower_leg_Radius 0.055
#define left_lower_leg_Height 0.33
#define left_foot_Radius 0.05
#define right_upper_leg_Radius 0.065
#define right_upper_leg_Height 0.37
#define right_lower_leg_Radius 0.055
#define right_lower_leg_Height 0.33
#define right_foot_Radius 0.05

enum __Action
{
	Not = -1,
	Init = 0,
	Hurray = 1,
	Jump = 2,
	Sit = 3,
	Walk = 4,
	Run = 5
} Action;

double cv[9];
bool initFlag;
double hurray_X, hurray_Y, jump_Y, walk_Y, run_arm_Y, run_leg_Y;
int hurray_Angle, jump_Angle, walk_Angle, run_arm_Angle, run_leg_Angle, run_Angle;
int cnt, run_cnt, walk_cnt;

void varInit() {
	initFlag = true;
	Action = Not;
	hurray_Y = hurray_Angle = jump_Y = 0;
	walk_Angle = walk_Y = run_arm_Y = run_leg_Y = 0;
	jump_Angle = run_arm_Angle = run_leg_Angle = run_Angle = 0;
	cnt = walk_cnt = run_cnt = 0;
}

void initial() {
	for (int i = 0; i < body_Cnt; ++i) {
		pObj[i] = gluNewQuadric();
		gluQuadricDrawStyle(pObj[i], GLU_FILL);
	}

	for (int i = 0; i < 9; ++i) {
		if (i == 5) cv[i] = -1;
		else if (i == 7) cv[i] = 1;
		else cv[i] = 0;
	}

	varInit();
	initFlag = false;

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
}

void InitLight() {
	GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_position[] = { -3, 2, 3.0, 0.0 };

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

///////// Body Draw Function /////////
void torso() {
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	gluCylinder(pObj[0], torso_Radius, torso_Radius, torso_Height, 10, 10);
	glPopMatrix();
}

void head() {
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluSphere(pObj[1], head_Radius, 10, 10);
	glPopMatrix();
}

void neck() {
	glPushMatrix();
	glRotatef(-90, 1.0, 0, 0);
	gluCylinder(pObj[2], neck_Radius, neck_Radius, neck_Height, 10, 10);
	glPopMatrix();
}

void joint() {
	glPushMatrix();
	glRotatef(-90, 1.0, 0, 0);
	gluSphere(pObj[3], joint_Radius, 10, 10);
	glPopMatrix();
}

void left_upper_arm() {
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glRotatef(-35, 0, 1, 0);
	gluCylinder(pObj[4], left_upper_arm_Radius, left_upper_arm_Radius - 0.01,
		left_upper_arm_Height, 10, 10);
	glPopMatrix();
}

void left_lower_arm() {
	glPushMatrix();
	glRotatef(90, 1.0, 0, 0);
	gluCylinder(pObj[5], left_lower_arm_Radius, left_lower_arm_Radius - 0.01,
		left_lower_arm_Height, 10, 10);
	glPopMatrix();
}

void left_hand() {
	glPushMatrix();
	glRotatef(-90, 1.0, 0, 0);
	gluSphere(pObj[6], left_hand_Radius, 10, 10);
	glPopMatrix();
}

void right_upper_arm() {
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glRotatef(35, 0, 1, 0);
	gluCylinder(pObj[7], right_upper_arm_Radius, right_upper_arm_Radius - 0.01,
		right_upper_arm_Height, 10, 10);
	glPopMatrix();
}

void right_lower_arm() {
	glPushMatrix();
	glRotatef(90, 1.0, 0, 0);
	gluCylinder(pObj[8], right_lower_arm_Radius, right_lower_arm_Radius - 0.01,
		right_lower_arm_Height, 10, 10);
	glPopMatrix();
}

void right_hand() {
	glPushMatrix();
	glRotatef(-90, 1.0, 0, 0);
	gluSphere(pObj[9], right_hand_Radius, 10, 10);
	glPopMatrix();
}

void left_upper_leg() {
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	gluCylinder(pObj[10], left_upper_leg_Radius, left_upper_leg_Radius - 0.01,
		left_upper_leg_Height, 10, 10);
	glPopMatrix();
}

void left_lower_leg() {
	glPushMatrix();
	glRotatef(90, 1.0, 0, 0);
	gluCylinder(pObj[11], left_lower_leg_Radius, left_lower_leg_Radius - 0.01,
		left_lower_leg_Height, 10, 10);
	glPopMatrix();
}

void left_foot() {
	glPushMatrix();
	glRotatef(-90, 1.0, 0, 0);
	gluSphere(pObj[12], left_foot_Radius, 10, 10);
	glPopMatrix();
}

void right_upper_leg() {
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	gluCylinder(pObj[13], right_upper_leg_Radius, right_upper_leg_Radius - 0.01,
		right_upper_leg_Height, 10, 10);
	glPopMatrix();
}

void right_lower_leg() {
	glPushMatrix();
	glRotatef(90, 1.0, 0, 0);
	gluCylinder(pObj[14], right_lower_leg_Radius, right_lower_leg_Radius - 0.01,
		right_lower_leg_Height, 10, 10);
	glPopMatrix();
}

void right_foot() {
	glPushMatrix();
	glRotatef(-90, 1.0, 0, 0);
	gluSphere(pObj[15], right_foot_Radius, 10, 10);
	glPopMatrix();
}

void create_chair() {
	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	glutSolidCube(0.38);
	glPopMatrix();
}

double calc(double *k, int angle) {
	double rad = abs(angle) * (3.14 / 180);
	k[0] = sin(rad) * k[0];
	k[1] = cos(rad) * k[1];
	return *k;
}

void MyDisplay() {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(cv[0], cv[1], cv[2], cv[3], cv[4], cv[5], cv[6], cv[7], cv[8]);
	glColor3f(0, 0, 0);

	double k[] = { right_upper_arm_Height, right_upper_arm_Height };
	calc(k, 35);
	glPushMatrix();
	if (Action == Jump || Action == Sit) {
		glPushMatrix();
		if (cnt < 10) {
			glTranslatef(0, jump_Y, 0);
		}
		else {
			glTranslatef(0, jump_Y, 0);
		}
	}

	// part A
	if (Action == Run) {
		glRotatef(run_Angle, 1, 0, 0);
	}
	torso();

	// part B
	glPushMatrix();
	glTranslatef(0, torso_Height, 0);
	neck();
	glTranslatef(0, neck_Height + head_Radius, 0);
	head();
	glPopMatrix();

	// part C
	glPushMatrix();
	glTranslatef(-torso_Radius - torso_dist, torso_Height - torso_dist, 0);
	joint();
	glTranslatef(-joint_Radius / 2, -joint_Radius / 2 + hurray_Y + walk_Y, 0);
	if (Action == Hurray) {
		glRotatef(hurray_Angle, 1, 0, 0);
	}
	else if (Action == Walk) {
		glRotatef(walk_Angle, 1, 0, 0);
	}
	else if (Action == Run) {
		glRotatef(run_arm_Angle, 1, 0, 0);
	}
	left_upper_arm();
	k[0] = k[1] = left_upper_arm_Height;
	calc(k, 35);
	glTranslatef(-k[0], -k[1], 0);
	glTranslatef(-joint_Radius / 2, -joint_Radius / 2, 0);
	joint();
	glTranslatef(0, -joint_Radius, 0);
	if (Action == Run) {
		glRotatef(-30, 1, 0, 0);
	}
	left_lower_arm();
	glTranslatef(0, -joint_Radius - k[1], 0);
	left_hand();
	glPopMatrix();

	// part D
	glPushMatrix();
	glTranslatef(torso_Radius + torso_dist, torso_Height - torso_dist, 0);
	joint();
	glTranslatef(joint_Radius / 2, -joint_Radius / 2 + hurray_Y + walk_Y + run_arm_Y, 0);
	if (Action == Hurray) {
		glRotatef(hurray_Angle, 1, 0, 0);
	}
	else if (Action == Walk) {
		glRotatef(-walk_Angle, 1, 0, 0);
	}
	else if (Action == Run) {
		glRotatef(-run_arm_Angle, 1, 0, 0);
	}
	right_upper_arm();
	glTranslatef(k[0], -k[1], 0);
	glTranslatef(joint_Radius / 2, -joint_Radius / 2, 0);
	joint();
	glTranslatef(0, -joint_Radius, 0);
	if (Action == Run) {
		glRotatef(-30, 1, 0, 0);
	}
	right_lower_arm();
	glTranslatef(0, -joint_Radius - k[1], 0);
	right_hand();
	glPopMatrix();

	// part E
	glPushMatrix();
	glTranslatef(-torso_Radius - torso_dist, -torso_dist, 0);
	joint();
	glTranslatef(0, -joint_Radius / 2, 0);
	if (Action == Jump || Action == Sit) {
		glPushMatrix();
		glRotatef(jump_Angle, 1, 0, 0);
	}
	else if (Action == Walk) {
		glRotatef(-walk_Angle, 1, 0, 0);
	}
	else if (Action == Run) {
		glRotatef(-run_arm_Angle, 1, 0, 0);
	}
	left_upper_leg();
	glTranslatef(0, -left_upper_leg_Height, 0);
	glTranslatef(0, -joint_Radius / 2, 0);
	joint();
	glTranslatef(0, -joint_Radius, 0);
	if (Action == Jump || Action == Sit) {
		if (Action == Jump) {
			glRotatef(-jump_Angle * 1.85, 1, 0, 0);
		}
		else {
			glRotatef(-jump_Angle * 1.3, 1, 0, 0);
		}
	}
	if (Action == Run) {
		glRotatef(20, 1, 0, 0);
	}
	left_lower_leg();
	glTranslatef(0, -left_upper_leg_Height, 0);
	glTranslatef(0, -joint_Radius / 2, 0);
	left_foot();
	if (Action == Jump || Action == Sit) {
		glPopMatrix();
	}
	glPopMatrix();

	// part F
	glPushMatrix();
	glTranslatef(torso_Radius + torso_dist, -torso_dist, 0);
	joint();
	glTranslatef(0, -joint_Radius / 2, 0);
	if (Action == Jump || Action == Sit) {
		glPushMatrix();
		glRotatef(jump_Angle, 1, 0, 0);
	}
	else if (Action == Walk) {
		glRotatef(walk_Angle, 1, 0, 0);
	}
	else if (Action == Run) {
		glRotatef(run_arm_Angle, 1, 0, 0);
	}
	right_upper_leg();
	glTranslatef(0, -right_upper_leg_Height, 0);
	glTranslatef(0, -joint_Radius / 2, 0);
	joint();
	glTranslatef(0, -joint_Radius, 0);
	if (Action == Jump || Action == Sit) {
		if (Action == Jump) {
			glRotatef(-jump_Angle * 1.85, 1, 0, 0);
		}
		else {
			glRotatef(-jump_Angle * 1.3, 1, 0, 0);
		}
	}
	if (Action == Run) {
		glRotatef(20, 1, 0, 0);
	}
	right_lower_leg();
	glTranslatef(0, -right_upper_leg_Height, 0);
	glTranslatef(0, -joint_Radius / 2, 0);
	right_foot();
	if (Action == Jump || Action == Sit) {
		glPopMatrix();
	}
	glPopMatrix();
	if (Action == Jump || Action == Sit) {
		glPopMatrix();
	}
	glPopMatrix();

	// Action에 따른 제어
	switch (Action) {
	case -1:
		initFlag = false;
		break;
	case 0:
		if (!initFlag) {
			for (int i = 0; i < 9; ++i) {
				if (i == 5) cv[i] = -1;
				else if (i == 7) cv[i] = 1;
				else cv[i] = 0;
			}
			varInit();
		}
		break;
	case 1:
		if (cnt++ < 18) {
			hurray_Y += 0.003;
			hurray_Angle -= 10;
			Sleep(100);
		}
		break;
	case 2:
		if (cnt++ < 10) {
			jump_Y -= 0.02;
			jump_Angle -= 6;
			Sleep(100);
		}
		else if (cnt++ < 24) {
			jump_Y += 0.1;
			jump_Angle += 8;
			Sleep(100);
		}
		else if (cnt++ < 40) {
			jump_Y -= 0.1;
			Sleep(150);
		}
		break;
	case 3:
		if (cnt++ < 10) {
			jump_Y -= 0.02;
			jump_Angle -= 8;
			Sleep(150);
		}
		else {
			create_chair();
		}
		break;
	case 4:
		if (cnt++ < 5) {
			walk_Y += 0.003;
			walk_Angle += 8;
			Sleep(100);
		}
		else if (cnt < 15) {
			walk_Y -= 0.003;
			walk_Angle -= 8;
			Sleep(100);
		}
		else if (cnt < 18) {
			walk_Y += 0.003;
			walk_Angle += 8;
			Sleep(100);
		}
		else {
			walk_cnt++;
			cnt = walk_Y = walk_Angle = 0;
			Sleep(100);
			if (walk_cnt == 5) {
				Action = Not;
			}
		}
		break;
	case 5:
		if (cnt++ < 5) {
			run_arm_Y -= 0.003;
			run_leg_Y += 0.003;
			run_arm_Angle -= 12;
			run_leg_Angle -= 15;
			run_Angle += 1;
			Sleep(100);
		}
		else if (cnt < 15) {
			run_arm_Y += 0.003;
			run_leg_Y -= 0.003;
			run_arm_Angle += 12;
			run_leg_Angle += 20;
			run_Angle -= 0.5;
			Sleep(100);
		}
		else if (cnt < 18) {
			run_arm_Y -= 0.003;
			run_leg_Y += 0.003;
			run_arm_Angle -= 12;
			run_leg_Angle -= 15;
			run_Angle += 1;
			Sleep(100);
		}
		else {
			run_cnt++;
			cnt = run_arm_Y = run_leg_Y = run_arm_Angle = run_leg_Angle = 0;
			Sleep(100);
			if (run_cnt == 5) {
				Action = Not;
			}
		}
		break;
	default:
		break;
	}

	glFlush();
	glutSwapBuffers();
}

void MyMainMenu(int entryID)
{
	if (entryID == 0)
	{
		Action = Init;
	}
	else if (entryID == 1)
	{
		Action = Hurray;
	}
	else if (entryID == 2)
	{
		Action = Jump;
	}
	else if (entryID == 3)
	{
		Action = Sit;
	}
	else if (entryID == 4)
	{
		Action = Walk;
	}
	else if (entryID == 5)
	{
		Action = Run;
	}
	else exit(0);
}

void MyResahpe(int w, int h) {
	glViewport(0, 0, w, h);

	float widthfactor = (float)w / 600;
	float heightfactor = (float)h / 600;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0 * widthfactor, 1.0 * widthfactor, -1.0 * heightfactor, 1.0 * heightfactor, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

/*
 * Camera View Convert *
	← : 좌회전
	→ : 우회전
	↑ : 위로 회전
	↓ : 아래로 회전
 */
void MySpecialKeyboard(int pressedkey, int x, int y) {
	switch (pressedkey) {
	case GLUT_KEY_LEFT:
		cv[3] -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		cv[3] += 0.1;
		break;
	case GLUT_KEY_UP:
		cv[4] -= 0.1;
		break;
	case GLUT_KEY_DOWN:
		cv[4] += 0.1;
		break;
	}
}

/*
	Spacebar : 초기화
		   q : 종료
*/
void MyKeyboard(unsigned char pressedkey, int x, int y) {
	switch (pressedkey) {
	case 32:
		initial();
		break;
	case 'q':
		exit(0);
		break;
	}
}

int main(int argc, char** argv) {

	// GLUT Window Create
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(window_Width, window_Height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Human Body Modeling");

	// Set up
	initial();
	InitLight();

	// Event - Callback Connect
	glutReshapeFunc(MyResahpe);
	glutDisplayFunc(MyDisplay);
	glutIdleFunc(MyDisplay);
	glutKeyboardFunc(MyKeyboard);
	glutSpecialFunc(MySpecialKeyboard);

	// Menu Create
	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAddMenuEntry("Init", 0);
	glutAddMenuEntry("Hurray", 1);
	glutAddMenuEntry("Jump", 2);
	glutAddMenuEntry("Sit", 3);
	glutAddMenuEntry("Walk", 4);
	glutAddMenuEntry("Run", 5);
	glutAddMenuEntry("Exit", 6);
	glutAttachMenu(GLUT_LEFT_BUTTON);

	glutMainLoop();

	return 0;
}