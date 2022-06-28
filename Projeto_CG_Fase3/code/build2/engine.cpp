#include <iostream>
#include <fstream>
#include <vector>
#include "C:\Users\jdtcs\OneDrive\Ambiente de Trabalho\Uni\3ano\CG\tinyxml2.cpp"
#include "C:\Users\jdtcs\OneDrive\Ambiente de Trabalho\Uni\3ano\CG\tinyxml2.h"
#include "C:\Users\jdtcs\OneDrive\Ambiente de Trabalho\Uni\3ano\CG\toolkits\glew\GL\glew.h"
#include "C:\Users\jdtcs\OneDrive\Ambiente de Trabalho\Uni\3ano\CG\toolkits\devil\IL\il.h"
#define Tesselation 100.0


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


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
float* init_translate; //translate inicial
float* init_rotate; //translate inicial
float* vertices;
float** all_vertices;
int N; //numero de vertices 
int num;
float radius, dist; //dist is the same as radius but y coord is 0
float angleAlpha, angleBeta;
int timebase = 0, frame = 0;

int cord_x = -1, cord_y = -1;


//VB0
typedef struct buffer {
	GLuint buffers[1];
	int verticesCount;
	struct buffer* next;
	int pos;
}*Buffer;



typedef struct caracteristicas {
	int ignore;

	int t_time;
	int r_time;
	int align;

	float* translate;
	float* rotate;
	float* scale;
	//int* ordem;

	Buffer b;
	struct caracteristicas** luas;
	int nLuas;
}*Caracteristicas;

Caracteristicas* planetas;  //array com todas as estruturas para desenhar as transformacoes
int nPlanetas;

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

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();
}


void buildRotMatrix(float* x, float* y, float* z, float* m) { // Assuming an initial specification of an Y0 vector, to align the object with the curve, we need to build
															  // a rotation matrix for the object

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void cross(float* a, float* b, float* res) {

	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float* a) {

	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}

void multMatrixVector(float* m, float* v, float* res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}

void getCatmullRomPoint(float t, float* p0, float* p1, float* p2, float* p3, float* pos, float* deriv) {
	// catmull-rom matrix
	float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f} };

	for (int i = 0; i < 3; i++) {	 // i = x, y, z
		float p[4] = { p0[i], p1[i], p2[i], p3[i] };
		float a[4];
		// Compute A = M * P
		multMatrixVector((float*)m, p, a);

		pos[i] = powf(t, 3.0) * a[0] + powf(t, 2.0) * a[1] + t * a[2] + a[3]; // vetor do P(t) = {t^3, t^2, t, 1}
		// Compute pos = T * A

		// compute deriv = T' * A
		deriv[i] = 3 * powf(t, 2.0) * a[0] + 2 * t * a[1] + a[2];

	}
}

void getGlobalCatmullRomPoint(float gt, float* pos, float* deriv, Caracteristicas c) {
	// given  global t, returns the point in the curve
	int size = 4; //tamanho do array com os pontos da transla��o
	//size = tamanho do array com os pontos lidos de cada transla��o
	float t = gt * size; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4];
	indices[0] = (index + size - 1) % size;
	indices[1] = (indices[0] + 1) % size;
	indices[2] = (indices[1] + 1) % size;
	indices[3] = (indices[2] + 1) % size;

	float p[4][3];  //12 coordenadas

	int w = 0;
	for (int j = 0; j < 4; j++) {
		for (int k = 0; k < 3; k++) {
			p[j][k] = c->translate[w++];
		}
	}

	getCatmullRomPoint(t, p[indices[0]], p[indices[1]], p[indices[2]], p[indices[3]], pos, deriv);
}

void renderCatmullRomCurve(Caracteristicas c) {
	float pos[3];  // coordenadas de cada posi��o, ou seja, coordenadas do P0, P1...
	float deriv[3]; // coordenadas de cada posi��o, ou seja, coordenadas do P0, P1... mas quando usamos o vetor das derivadas

	// draw curve using line segments with GL_LINE_LOOP
	glBegin(GL_LINE_LOOP);
	float gt = 0;
	while (gt < 1) {
		getGlobalCatmullRomPoint(gt, pos, deriv, c);
		glColor3f(0.5f, 0.5f, 0.0f);
		glVertex3f(pos[0], pos[1], pos[2]);
		gt += 1 / Tesselation;
	}
	glEnd();
}

void drawCatmullRomCurve(float t, Caracteristicas c) {
	float pos[3];
	float deriv[3];
	float x[3] = { deriv[0], deriv[1], deriv[2] };
	float y[3] = { 0.0f, 1.0f, 0.0f };
	float z[3];

	renderCatmullRomCurve(c);

	getGlobalCatmullRomPoint(t, pos, deriv, c); //t = tempo passado no xml

	glTranslatef(pos[0], pos[1], pos[2]);
	if (c->align == 1) {
		normalize(x);
		cross(x, y, z);
		normalize(z);
		cross(z, x, y);
		normalize(y);
		normalize(deriv);

		float m[16];

		buildRotMatrix(x, y, z, m);
		glMultMatrixf(m);
	}
}


void Transform(Caracteristicas c, int flag) {
	if (init_translate != NULL) {
		glTranslatef(init_translate[0], init_translate[1], init_translate[2]);
	}
	if (init_rotate!=NULL) {
		float angle = 360 / (init_rotate[0] * 1000);
		int time = glutGet(GLUT_ELAPSED_TIME);

		glRotatef(angle * time, init_rotate[0], init_rotate[1], init_rotate[2]);
	}
	if (c->translate != NULL) {
		int t_time = c->t_time;
		float time = glutGet(GLUT_ELAPSED_TIME) % (int)(t_time * 1000);
		float tempo = time / (t_time * 1000.0);
		drawCatmullRomCurve(tempo, c);
	}
	if (c->rotate != NULL) {
		if (c->r_time != 0) {
			float angle = 360 / (c->r_time * 1000);
			int time = glutGet(GLUT_ELAPSED_TIME);
			glRotatef(time * angle, c->rotate[0], c->rotate[1], c->rotate[2]);
		}
		else {
			glRotatef(c->rotate[0], c->rotate[1], c->rotate[2], c->rotate[3]);
		}
	}
	if (c->scale != NULL && flag) //flag serve para escalas relativas as luas, i.e., para nao fazer escala do planeta para a lua mas sim da esfera raio 1 para a lua
		glScalef(c->scale[0], c->scale[1], c->scale[2]);

}



void renderSceneAux(Caracteristicas c, Caracteristicas cOriginal) {
	Buffer b = c->b;
	glPushMatrix();
	//float d = sqrt(pow(c->translate[0], 2) + pow(c->translate[1], 2) + pow(c->translate[2], 2));
	//orbita(d, 60);
	if (cOriginal) Transform(cOriginal, 0);
	Transform(c, 1);



	glBindBuffer(GL_ARRAY_BUFFER, b->buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, b->verticesCount);


	glPopMatrix();

	if (c->luas) for (int k = 0; k < c->nLuas; k++) {
			renderSceneAux(c->luas[k], c);
	}

}




void renderScene(void) {


	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	// set the camera
	glLoadIdentity();

	gluLookAt(cos(angleBeta) * sin(angleAlpha) * radius, sin(angleBeta) * radius, cos(angleBeta) * cos(angleAlpha) * radius,
		lookat[0], lookat[1], lookat[2],
		up[0], up[1], up[2]);

	for (int i = 0; i < nPlanetas; i++) {
		renderSceneAux(planetas[i], NULL);
	}

	Axis();

	float fps;
	int timet;
	char s[64];

	frame++;
	timet = glutGet(GLUT_ELAPSED_TIME);
	if (timet - timebase > 1000) {
		fps = frame * 1000.0 / (timet - timebase);
		timebase = timet;
		frame = 0;
		sprintf(s, "FPS: %f", fps);
		glutSetWindowTitle(s);
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
	case ('+'):
		radius -= 0.3f;
		break;
	case ('-'):
		radius += 0.3f;
		break;
	case ('o'):
		radius -= 0.1f;
		break;
	case ('l'):
		radius += 0.1f;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void special_keyboard(int key_code, int x, int y) {
	switch (key_code) {
	case GLUT_KEY_LEFT:
		angleAlpha -= 0.07f;
		break;
	case GLUT_KEY_RIGHT:
		angleAlpha += 0.07f;
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
void readXMLaux(Caracteristicas c, XMLElement* elem) {

	const char* file3d;
	
	if (elem->FirstChildElement("models") != NULL) {
		file3d = elem->FirstChildElement("models")->FirstChildElement("model")->Attribute("file");
		printf("\nfilename: %s\n", file3d);
		readFile(file3d);
	}


	float* vertexB = (float*)malloc(sizeof(float) * N);
	vertexB = vertices;
	c->b = (Buffer)malloc(sizeof(buffer));

	c->b->verticesCount = N;

	glGenBuffers(1, c->b->buffers); //criar o VB0
	glBindBuffer(GL_ARRAY_BUFFER, c->b->buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * c->b->verticesCount, vertexB, GL_STATIC_DRAW);


	XMLElement* TL = elem->FirstChildElement("transform")->FirstChildElement("translate");
	XMLElement* RT = elem->FirstChildElement("transform")->FirstChildElement("rotate");
	XMLElement* SC = elem->FirstChildElement("transform")->FirstChildElement("scale");
	
	if (TL) {
		const char* t_time1 = TL->Attribute("time");
		c->t_time = atoi(t_time1);

		const char* al = TL->Attribute("align");

		if (strcmp(al, "True")==0){
			c->align = 1;
		}
		else {
			c->align = 0;
		}

		XMLElement* point = TL->FirstChildElement("point");
		if (point) {
			const char* p_X = point->Attribute("x"), * p_Y = point->Attribute("y"), * p_Z = point->Attribute("z");
			c->translate = (float*)malloc(sizeof(float) * 4 * 3);
			c->translate[0] = atof(p_X);
			c->translate[1] = atof(p_Y);
			c->translate[2] = atof(p_Z);
			int w = 3;
			while (point->NextSiblingElement()) {
				point = point->NextSiblingElement();
				const char* p_X1 = point->Attribute("x"), * p_Y1 = point->Attribute("y"), * p_Z1 = point->Attribute("z");
				c->translate[w++] = atof(p_X1);
				//printf("pontos de tranlacao[%d]: %f\n", w-1, t->pontos_t[w-1]);
				c->translate[w++] = atof(p_Y1);
				//printf("pontos de tranlacao[%d]: %f\n", w - 1, t->pontos_t[w - 1]);
				c->translate[w++] = atof(p_Z1);
				//printf("pontos de tranlacao[%d]: %f\n", w - 1, t->pontos_t[w - 1]);
			}
		}
	}

	else {
		c->t_time = 0;
		c->translate = NULL;
	}
	
	if (RT) {

		if (RT->Attribute("time")) {
			const char* r_time1 = RT->Attribute("time"), * axisX = RT->Attribute("x"), * axisY = RT->Attribute("y"), * axisZ = RT->Attribute("z");
			c->rotate = (float*)malloc(sizeof(float) * 4);
			c->rotate[0] = atoi(r_time1);
			c->rotate[1] = atof(axisX);
			c->rotate[2] = atof(axisY);
			c->rotate[3] = atof(axisZ);
		}
		else if (RT->Attribute("angle")) {
			c->r_time = 0;

			c->rotate = (float*)malloc(sizeof(float) * 4);
			c->rotate[0] = atof(RT->Attribute("angle"));
			c->rotate[1] = atof(RT->Attribute("x"));
			c->rotate[2] = atof(RT->Attribute("y"));
			c->rotate[3] = atof(RT->Attribute("z"));
		}
	}

	else {
		c->rotate = NULL;
		c->r_time = 0;
	}
	

	if (SC) {
		c->scale = (float*)malloc(sizeof(float) * 3);
		
		c->scale[0] = atof(SC->Attribute("x"));
		c->scale[1] = atof(SC->Attribute("y"));
		c->scale[2] = atof(SC->Attribute("z"));
		
	}

	else {
		c->scale = NULL;
		
	}

	int i = 0;
	if (elem->FirstChildElement("group")) {

		for (XMLElement* elem2 = elem->FirstChildElement("group"); elem2; elem2 = elem2->NextSiblingElement()) i++;
		c->luas = (Caracteristicas*)malloc(sizeof(struct caracteristicas) * i);
		c->nLuas = i;


		XMLElement* elem3 = elem->FirstChildElement("group");
		for (int k = 0; k < c->nLuas; k++) {
			c->luas[k] = (Caracteristicas)malloc(sizeof(struct caracteristicas));
			readXMLaux(c->luas[k], elem3);
			elem3 = elem3->NextSiblingElement();
		}
	}
	else { c->luas = NULL; c->nLuas = 0; }
}


void readxml(char* filename) {
	XMLDocument doc;
	const char* file3d;
	Caracteristicas c;
	int i = 0, k;


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


		XMLElement* TR = doc.FirstChildElement("world")->FirstChildElement("group")->FirstChildElement("transform");
		if (TR) {
			XMLElement* IT = TR->FirstChildElement("translate");
			XMLElement* IR = TR->FirstChildElement("rotate");
			if (IT) {
				init_translate = (float*)malloc(sizeof(float) * 3);
				init_translate[0] = atof(IT->Attribute("x"));
				init_translate[1] = atof(IT->Attribute("y"));
				init_translate[2] = atof(IT->Attribute("z"));
			}
			else {
				init_translate = NULL;
			}
			if (IR) {
				init_rotate = (float*)malloc(sizeof(float) * 4);
				init_rotate[0] = atof(IR->Attribute("time"));
				init_rotate[1] = atof(IR->Attribute("x"));
				init_rotate[2] = atof(IR->Attribute("y"));
				init_rotate[3] = atof(IR->Attribute("z"));
			}
			else {
				init_rotate = NULL;
			}
		}

		for (XMLElement* elem = doc.FirstChildElement("world")->FirstChildElement("group")->FirstChildElement("group"); elem; elem = elem->NextSiblingElement()) i++;
		planetas = (Caracteristicas*)malloc(sizeof(struct caracteristicas) * i);

		nPlanetas = i;
		XMLElement* elem = doc.FirstChildElement("world")->FirstChildElement("group")->FirstChildElement("group");


		for (k = 0; k < nPlanetas; k++) {
			planetas[k] = (Caracteristicas)malloc(sizeof(struct caracteristicas));
			readXMLaux(planetas[k], elem);
			elem = elem->NextSiblingElement();
		}



		//camera stuff
		radius = sqrt(pow(pos[0], 2) + pow(pos[1], 2) + pow(pos[2], 2)), dist = sqrt(pow(pos[0], 2) + pow(pos[2], 2)); //dist is the same as radius but y coord is 0
		angleAlpha = acos(pos[2] / dist), angleBeta = acos(dist / radius);

	}


	else {
		printf("O ficheiro XML nao foi encontrado.\n");
	}

}



int main(int argc, char** argv) {


	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);


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

	glewInit();

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	//glEnable(GL_RESCALE_NORMAL);

	if (argc == 2) {
		readxml(argv[1]);
	}
	else {
		printf("Número de argumentos introduzido incorreto. Tente novamente.\n");
		return 0;
	}


	// enter GLUT's main cycle
	glutMainLoop();

	free(vertices);

	return 1;
}

