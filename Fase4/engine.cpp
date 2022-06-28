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
float* normais;  //array que contem as normais para desenhar a primitiva
float* texturas; //array que contem as texturas para desenhar a primitiva
int N; // numero de elementos do array vertices e normais
int T; // numero de elementos do array texturas
int num;
float radius, dist; //dist is the same as radius but y coord is 0
float angleAlpha, angleBeta;
int timebase = 0, frame = 0;

const char* texture;
XMLElement* color;

int cord_x = -1, cord_y = -1;


//VB0
typedef struct buffer {
	GLuint buffers[3];
	GLuint textureID;
	int verticesCount;
	int textVerticesCount;
	struct buffer* next;
	int pos;
}*Buffer;



typedef struct caracteristicas {
	int ignore; // caso o ficheiro 3d só tenha vert de triângulos

	int t_time;
	int r_time;
	int align;

	float* translate;
	float* rotate;
	float* scale;
	
	GLfloat emiss[4];
	GLfloat diff[4];
	GLfloat spec[4];
	GLfloat amb[4];
	GLfloat shiny;

	Buffer b;
	struct caracteristicas** luas;
	int nLuas;
}*Caracteristicas;
Caracteristicas* planetas;  //array com todas as estruturas para desenhar as transformacoes
int nPlanetas;

typedef struct light {
	float pos[4];  //se for ou direcional preencher só este array
	float spot[3];  // so tem isto se for spot
	GLfloat cut;
	struct light* next;
}*Light;

Light l;


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
		if (c->t_time != 0) {
			
			int t_time = c->t_time;
			float time = glutGet(GLUT_ELAPSED_TIME) % (int)(t_time * 1000);
			float tempo = time / (t_time * 1000.0);
			drawCatmullRomCurve(tempo, c);
		}
		else {
			glTranslatef(c->translate[0], c->translate[1], c->translate[2]);
		}
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

	glMaterialfv(GL_FRONT, GL_EMISSION, c->emiss);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c->diff);
	glMaterialfv(GL_FRONT, GL_AMBIENT, c->amb);
	glMaterialfv(GL_FRONT, GL_SPECULAR, c->spec);

	glMaterialf(GL_FRONT, GL_SHININESS, c->shiny);
	
}



void renderSceneAux(Caracteristicas c, Caracteristicas cOriginal) {
	Buffer b = c->b;
	glPushMatrix();
	if (cOriginal) Transform(cOriginal, 0);
	Transform(c, 1);

	glBindBuffer(GL_ARRAY_BUFFER, b->buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	if (!c->ignore) {
		if (color && !texture) {
			glBindBuffer(GL_ARRAY_BUFFER, b->buffers[1]);
			glNormalPointer(GL_FLOAT, 0, 0);
		}
		else if (!color && texture) {
			glBindBuffer(GL_ARRAY_BUFFER, b->buffers[1]);
			glTexCoordPointer(2, GL_FLOAT, 0, 0);
			glBindTexture(GL_TEXTURE_2D, b->textureID); //isto tem de estar antes de desenhar
		}
		else {
			glBindBuffer(GL_ARRAY_BUFFER, b->buffers[1]);
			glNormalPointer(GL_FLOAT, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, b->buffers[2]);
			glTexCoordPointer(2, GL_FLOAT, 0, 0);
			glBindTexture(GL_TEXTURE_2D, b->textureID);
		}
	}
	glDrawArrays(GL_TRIANGLES, 0, b->verticesCount);
	if (!c->ignore && texture) glBindTexture(GL_TEXTURE_2D, 0); //isto tem de estar depois de desenhar

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

	GLfloat dark[4] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat white[4] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, dark);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightf(GL_FRONT, GL_SHININESS, 128);
	
	glLightfv(GL_LIGHT0, GL_POSITION, l->pos);
	if (l->spot != NULL) {

		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l->spot);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, l->cut);
	}


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

int loadTexture(std::string s) {

	unsigned int t, tw, th;
	unsigned char* texData;
	unsigned int texID;
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)s.c_str());
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();

	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

void readFile(const char* filename, Caracteristicas c1) {
	FILE* f;
	f = fopen(filename, "r");
	char* n = (char*)malloc(sizeof(char) * 10);
	n[0] = 0;
	char* novo = (char*)malloc(sizeof(char) * 9);
	novo[0] = 0;
	char c;
	int i = 0;
	int flag = 0;

	if (f) {
		while ((c = fgetc(f)) != EOF) {
			if (flag == 0) {
				if (c == '\n') {
					N = atoi(n);
					vertices = (float*)malloc(sizeof(float) * N);
					flag = 1;
					n[0] = 0;
				}
				else { strncat(n, &c, 1); }
			}
			else if (flag == 1) {
				if (c == ' ') {
					float elem = atof(novo);
					vertices[i++] = elem;
					novo[0] = 0;
				}
				else {
					if (c == 'n') {
						flag = 2;
						i = 0;
						normais = (float*)malloc(sizeof(float) * N);
					}
					else { strncat(novo, &c, 1); }
				}
			}
			else if (flag == 2) {
				if (c == ' ') {
					float elem = atof(novo);
					normais[i++] = elem;
					novo[0] = 0;
				}
				else {
					if (c == 't') {
						flag = 3;
						i = 0;
					}
					else { strncat(novo, &c, 1); }
				}
			}
			else if (flag == 3) {
				if (c == '\n') {
					T = atoi(n);
					texturas = (float*)malloc(sizeof(float) * T);
					flag = 4;
					n[0] = 0;
				}
				else { strncat(n, &c, 1); }
			}
			else if (flag == 4) {
				if (c == ' ') {
					float elem = atof(novo);
					texturas[i++] = elem;
					novo[0] = 0;
				}
				else { strncat(novo, &c, 1); }
			}
		}
		if (flag == 1) {
			c1->ignore = 1;
		}
		else {
			c1->ignore = 0;
		}
	}
	else {
		printf("Erro ao ler o ficheiro.\nTente novamente.\n");
	}
	fclose(f);
}

void readXMLaux(Caracteristicas c, XMLElement* elem) {

	const char* file3d;
	if (elem->FirstChildElement("models") != NULL) {
		file3d = elem->FirstChildElement("models")->FirstChildElement("model")->Attribute("file");
		printf("\nfilename: %s\n", file3d);
	}

	if(elem->FirstChildElement("models")->FirstChildElement("model")->FirstChildElement("texture"))
		texture = elem->FirstChildElement("models")->FirstChildElement("model")->FirstChildElement("texture")->Attribute("file");
	color = elem->FirstChildElement("models")->FirstChildElement("model")->FirstChildElement("color");
	
	XMLElement* DIF = elem->FirstChildElement("models")->FirstChildElement("model")->FirstChildElement("color")->FirstChildElement("diffuse");
	if (DIF) {
		c->diff[0] = atof(DIF->Attribute("R"))/255.0f;
		c->diff[1] = atof(DIF->Attribute("G")) / 255.0f;
		c->diff[2] = atof(DIF->Attribute("B")) / 255.0f;
		c->diff[3] = 1.0;
	}
	else {
		c->diff[0] = NULL;
		c->diff[1] = NULL;
		c->diff[2] = NULL;
		c->diff[3] = NULL;
	}
	

	XMLElement* AMB = elem->FirstChildElement("models")->FirstChildElement("model")->FirstChildElement("color")->FirstChildElement("ambient");
	if (AMB) {
		c->amb[0] = atof(AMB->Attribute("R")) / 255.0f;
		c->amb[1] = atof(AMB->Attribute("G")) / 255.0f;
		c->amb[2] = atof(AMB->Attribute("B")) / 255.0f;
		c->amb[3] = 1.0;
	}
	else {
		c->amb[0] = NULL;
		c->amb[1] = NULL;
		c->amb[2] = NULL;
		c->amb[3] = NULL;
	}

	XMLElement* SPC = elem->FirstChildElement("models")->FirstChildElement("model")->FirstChildElement("color")->FirstChildElement("specular");
	if (SPC) {
		c->spec[0] = atof(SPC->Attribute("R")) / 255.0f;
		c->spec[1] = atof(SPC->Attribute("G")) / 255.0f;
		c->spec[2] = atof(SPC->Attribute("B")) / 255.0f;
		c->spec[3] = 1.0;
	}
	else {
		c->spec[0] = NULL;
		c->spec[1] = NULL;
		c->spec[2] = NULL;
		c->spec[3] = NULL;
	}

	XMLElement* EMIS = elem->FirstChildElement("models")->FirstChildElement("model")->FirstChildElement("color")->FirstChildElement("emissive");
	if (EMIS) {
		c->emiss[0] = atof(EMIS->Attribute("R")) / 255.0f;
		c->emiss[1] = atof(EMIS->Attribute("G")) / 255.0f;
		c->emiss[2] = atof(EMIS->Attribute("B")) / 255.0f;
		c->emiss[3] = 1.0;
	}
	else {
		c->emiss[0] = NULL;
		c->emiss[1] = NULL;
		c->emiss[2] = NULL;
		c->emiss[3] = NULL;
	}

	XMLElement* SNY = elem->FirstChildElement("models")->FirstChildElement("model")->FirstChildElement("color")->FirstChildElement("shininess");
	if (SNY) {
		c->shiny = atof(SNY->Attribute("value"));
	}
	else {
		c->shiny = 0;
	}
	
	readFile(file3d, c);

	float* vertexB = (float*)malloc(sizeof(float) * N);
	vertexB = vertices;
	c->b = (Buffer)malloc(sizeof(buffer));

	float* normals = (float*)malloc(sizeof(float) * N);
	float* textCoord = (float*)malloc(sizeof(float) * T);

	normals = normais;
	textCoord = texturas;


	c->b = (Buffer)malloc(sizeof(buffer));
	c->b->verticesCount = N;
	c->b->textVerticesCount = T;
	if (color && !texture) {
		glGenBuffers(3, c->b->buffers); //criar o VB0
		glBindBuffer(GL_ARRAY_BUFFER, c->b->buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * c->b->verticesCount, vertexB, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, c->b->buffers[1]);
	
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * c->b->verticesCount, normals, GL_STATIC_DRAW);
	}

	else if (!color && texture) {
		glGenBuffers(3, c->b->buffers); //criar o VB0
		glBindBuffer(GL_ARRAY_BUFFER, c->b->buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* c->b->verticesCount, vertexB, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, c->b->buffers[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * c->b->textVerticesCount, textCoord, GL_STATIC_DRAW);

		c->b->textureID = loadTexture(texture);
	}
	else {
		glGenBuffers(3, c->b->buffers); //criar o VB0
		glBindBuffer(GL_ARRAY_BUFFER, c->b->buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * c->b->verticesCount, vertexB, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, c->b->buffers[1]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * c->b->verticesCount, normals, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, c->b->buffers[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * c->b->textVerticesCount, textCoord, GL_STATIC_DRAW);

		c->b->textureID = loadTexture(texture);
	}

	if (elem->FirstChildElement("transform")) {
		XMLElement* TL = elem->FirstChildElement("transform")->FirstChildElement("translate");
		XMLElement* RT = elem->FirstChildElement("transform")->FirstChildElement("rotate");
		XMLElement* SC = elem->FirstChildElement("transform")->FirstChildElement("scale");

		if (TL) {
			if (TL->Attribute("time")){
				const char* t_time1 = TL->Attribute("time");
				c->t_time = atoi(t_time1);

				const char* al = TL->Attribute("align");

				if (strcmp(al, "True") == 0) {
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
						c->translate[w++] = atof(p_Y1);
						c->translate[w++] = atof(p_Z1);
					}
				}
			}
			else {
				c->translate = (float*)malloc(sizeof(float) * 3);
				c->translate[0] = atof(TL->Attribute("x"));
				c->translate[1] = atof(TL->Attribute("y"));
				c->translate[2] = atof(TL->Attribute("z"));
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
	}
	else {
		c->t_time = 0;
		c->translate = NULL;
		c->rotate = NULL;
		c->r_time = 0;
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

	l = (Light)malloc(sizeof(struct light));
	Light novo=l;
	Buffer b = (Buffer)malloc(sizeof(struct buffer));

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


		XMLElement* LGT = doc.FirstChildElement("world")->FirstChildElement("lights");

		if (LGT) {
			for (XMLElement* lightelem = LGT->FirstChildElement("light"); lightelem; lightelem = lightelem->NextSiblingElement()) {
				if (strcmp(lightelem->Attribute("type"), "point") == 0) {
					l->pos[0] = atof(lightelem->Attribute("posx"));
					l->pos[1] = atof(lightelem->Attribute("posy"));
					l->pos[2] = atof(lightelem->Attribute("posz"));
					l->pos[3] = 1.0;
				}
				if (strcmp(lightelem->Attribute("type"), "directional") == 0) {
					l->pos[0] = atof(lightelem->Attribute("dirx"));
					l->pos[1] = atof(lightelem->Attribute("diry"));
					l->pos[2] = atof(lightelem->Attribute("dirz"));
					l->pos[3] = 0.0;
				}
				if (strcmp(lightelem->Attribute("type"), "spot") == 0) {

					l->pos[0] = atof(lightelem->Attribute("posx"));
					l->pos[1] = atof(lightelem->Attribute("posy"));
					l->pos[2] = atof(lightelem->Attribute("posz"));
					l->pos[3] = 1.0;
					l->spot[0] = atof(lightelem->Attribute("dirx"));
					l->spot[1] = atof(lightelem->Attribute("diry"));
					l->spot[2] = atof(lightelem->Attribute("dirz"));
					l->cut = atof(lightelem->Attribute("cutoff"));
				}
				if (lightelem->NextSiblingElement()) {
					l->next = (Light)malloc(sizeof(struct light));
					l = l->next;
				}
			}
		}
		
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
	
	glEnable(GL_RESCALE_NORMAL);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_TEXTURE_2D);

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

