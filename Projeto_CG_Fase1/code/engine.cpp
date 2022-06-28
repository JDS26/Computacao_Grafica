#include <iostream>
#include <fstream>
#include <vector>
#include "C:\Users\jdtcs\OneDrive\Ambiente de Trabalho\Uni\3ano\CG\tinyxml2.cpp"
#include "C:\Users\jdtcs\OneDrive\Ambiente de Trabalho\Uni\3ano\CG\tinyxml2.h"
#include "C:\Users\jdtcs\OneDrive\Ambiente de Trabalho\Uni\3ano\CG\toolkits\glut\GL\glut.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>


using namespace tinyxml2;

//valores da camara
float pos[3];
float lookat[3];
float up[3];
float proj[3];

int n_primitivas;
float* vertices;
float** all_vertices;
int N; //numero de vertices 
int num;
float radius, dist; //dist is the same as radius but y coord is 0
float angleAlpha, angleBeta;

int cord_x = -1, cord_y = -1;


void changeSize(int w, int h) {
	
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(proj[0], ratio, proj[1], proj[2]);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void Axis() {
	glBegin(GL_LINES);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
}

void renderScene(void) {
	

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	


	// set the camera
	glLoadIdentity();

	gluLookAt(cos(angleBeta) * sin(angleAlpha) * radius, sin(angleBeta) * radius, cos(angleBeta) * cos(angleAlpha) * radius,
		lookat[0], lookat[1], lookat[2],
		up[0], up[1], up[2]);

	Axis();

	// put the geometric transformations here
	

	for (int k = 0; k < n_primitivas; k++) {
	
		for (int i = 1; i < (int)all_vertices[k][0]; i += 9) {
			glColor3f(0.3f, 0.3f, 0.5f);
			glBegin(GL_TRIANGLES);
			glVertex3f(all_vertices[k][i], all_vertices[k][i + 1], all_vertices[k][i + 2]);
			glVertex3f(all_vertices[k][i + 3], all_vertices[k][i + 4], all_vertices[k][i + 5]);
			glVertex3f(all_vertices[k][i + 6], all_vertices[k][i + 7], all_vertices[k][i + 8]);
			glEnd();

			i += 9;
			glColor3f(0.3f, 0.2f, 0.6f);
			glBegin(GL_TRIANGLES);
			glVertex3f(all_vertices[k][i], all_vertices[k][i + 1], all_vertices[k][i + 2]);
			glVertex3f(all_vertices[k][i + 3], all_vertices[k][i + 4], all_vertices[k][i + 5]);
			glVertex3f(all_vertices[k][i + 6], all_vertices[k][i + 7], all_vertices[k][i + 8]);
			glEnd();
		}
	}

	// End of frame
	glutSwapBuffers();
}

void menu(int value) {
	switch (value) {
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case 2:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 3:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}


// write function to process keyboard events
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case ('w'):
		radius -= 0.3f;
		break;
	case ('s'):
		radius += 0.3f;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void special_keyboard(int key_code, int x, int y) {
	switch (key_code) {
	case GLUT_KEY_LEFT:
		angleAlpha -= 0.1f;
		break;
	case GLUT_KEY_RIGHT:
		angleAlpha += 0.1f;
		break;
	case GLUT_KEY_UP:
		if (angleBeta < M_PI / 2)
			angleBeta += 0.1f;
		break;
	case GLUT_KEY_DOWN:
		if (angleBeta > -M_PI / 2)
			angleBeta -= 0.1f;
		break;
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			cord_x = x;
			cord_y = y;
		}
		else {
			cord_x = -1.0f;
			cord_y = -1.0f;
		}
	}
}

void motion_mouse(int x, int y) {
	if (cord_x >= 0) {
		angleAlpha = (x + cord_x) * 0.01f;
		if (angleBeta < M_PI / 2) {
			angleBeta = (y - cord_y) * 0.01f;
		}
		if (angleBeta > -M_PI / 2) {
			angleBeta = (y - cord_y) * 0.01f;
		}
	}

	glutPostRedisplay();
}

void readFile(const char* filename) {
	FILE* f;
	f = fopen(filename, "r");
	char* n = (char*)malloc(sizeof(char) * 10);
	n[0] = 0;
	char* novo = (char*)malloc(sizeof(char) * 9);
	novo[0] = 0;
	char c;
	int i = 0;
	int flag = 1;

	if (f) {
		while ((c = fgetc(f)) != EOF) {
			if (flag) {
				if (c == '\n') {
					num = atoi(n);
					vertices = (float*)malloc(sizeof(float) * num);
					flag = 0;
				}
				else strncat(n, &c, 1);
			}
			else {
				if (c == ' ') {
					float elem = atof(novo);
					vertices[i++] = elem;
					novo[0] = 0;
				}
				else strncat(novo, &c, 1);
			}
		}
		N = i;
	}
	else {
		printf("Erro ao criar o ficheiro.\nTente novamente.\n");
	}
	fclose(f);

	
}


void readxml(char* filename) {
	XMLDocument doc;
	const char* file3d;
	int i = 0, j = 1, k = 1;// j � o numero de primitivas
	
	if (!(doc.LoadFile(filename))) {

		XMLElement* position = doc.FirstChildElement("world")->FirstChildElement("camera")->FirstChildElement("position");

		pos[0] = atof(position->Attribute("x"));
		pos[1] = atof(position->Attribute("y"));
		pos[2] = atof(position->Attribute("z"));


		XMLElement* LA = doc.FirstChildElement("world")->FirstChildElement("camera")->FirstChildElement("lookAt");

		lookat[0] = atof(LA->Attribute("x"));
		lookat[1] = atof(LA->Attribute("y"));
		lookat[2] = atof(LA->Attribute("z"));

		XMLElement* UP = doc.FirstChildElement("world")->FirstChildElement("camera")->FirstChildElement("up");

		up[0] = atof(UP->Attribute("x"));
		up[1] = atof(UP->Attribute("y"));
		up[2] = atof(UP->Attribute("z"));

		XMLElement* PJ = doc.FirstChildElement("world")->FirstChildElement("camera")->FirstChildElement("projection");

		proj[0] = atof(PJ->Attribute("fov"));
		proj[1] = atof(PJ->Attribute("near"));
		proj[2] = atof(PJ->Attribute("far"));

		for (XMLElement* mod = doc.FirstChildElement("world")->FirstChildElement("group")->FirstChildElement("models")->FirstChildElement("model"); mod; mod = mod->NextSiblingElement()) {
			if (j = k) {
				k = k * 2;
				all_vertices = (float**)malloc(sizeof(float) * k);
			}
			j++;

		}
		

		for (XMLElement* mod = doc.FirstChildElement("world")->FirstChildElement("group")->FirstChildElement("models")->FirstChildElement("model"); mod; mod = mod->NextSiblingElement()) {

			file3d = mod->Attribute("file");

			readFile(file3d);
			all_vertices[i] = (float*)malloc(sizeof(float) * (N + 1));
			all_vertices[i][0] = (float)N;
			for (int w = 1; w <= N; w++) {
				all_vertices[i][w] = vertices[w - 1];
			}
			i++;
		}

		n_primitivas = i;

		//camera stuff
		radius = sqrt(pow(pos[0], 2) + pow(pos[1], 2) + pow(pos[2], 2)), dist = sqrt(pow(pos[0], 2) + pow(pos[2], 2)); //dist is the same as radius but y coord is 0
		angleAlpha = acos(pos[2] / dist), angleBeta = acos(dist / radius);
	}

	else {
		printf("O ficheiro XML nao foi encontrado.\n");
	}
	
}



int main(int argc, char** argv) {

	if (argc == 2) {
		readxml(argv[1]);
	}
	else {
		printf("N�mero de argumentos introduzido incorreto. Tente novamente.\n");
	}

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);


	// put here the registration of the keyboard callbacks
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion_mouse);

	//menu with drawing options
	glutCreateMenu(menu);
	glutAddMenuEntry("Pontos", 1);
	glutAddMenuEntry("Linhas", 2);
	glutAddMenuEntry("Preenchido", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// enter GLUT's main cycle
	glutMainLoop();
	
	free(vertices);
	free(all_vertices);

	return 1;
}
