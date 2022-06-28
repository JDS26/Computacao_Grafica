#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define _USE_MATH_DEFINES
#include <math.h>


void plane(float length, int divisions, char* filename) {
    FILE* f;
    f = fopen(filename, "w");

    float divSize = length / divisions;
    int numVertices = 3 * 2 * pow(divisions, 2.0f); //tem divisions * divisions quadrados num plano, logo tem o dobro disso de triangulos


    if (f) {

        fprintf(f, "%d \n", numVertices * 3); //numero de vertices necessarios * 3 (cada vertice tem 3 coordenadas)
        int i;
        int j;
        float var_x = -length / 2;
        float var_z = -length / 2;
        for (i = 0; i < divisions; i++) { //percorrer as "stacks" 
            for (j = 0; j < divisions; j++) { //percorrer as "slices"
                fprintf(f, "%f %f %f \n", var_x, 0.0f, var_z);
                fprintf(f, "%f %f %f \n", var_x, 0.0f, var_z + divSize);
                fprintf(f, "%f %f %f \n", var_x + divSize, 0.0f, var_z + divSize);

                fprintf(f, "%f %f %f \n", var_x, 0.0f, var_z);
                fprintf(f, "%f %f %f \n", var_x + divSize, 0.0f, var_z + divSize);
                fprintf(f, "%f %f %f \n", var_x + divSize, 0.0f, var_z);


                var_x += divSize;
            }
            var_z += divSize;
            var_x = -length / 2;
        }
        printf("Ficheiro .3d criado com sucesso.\n");
    }

    else {
        printf("Erro ao criar o ficheiro.\nTente novamente.\n");
    }
    fclose(f);


}

void box(float length, int divisions, char* filename) {

    FILE* f;
    f = fopen(filename, "w");

    float divSize = length / divisions;
    int numVertices = 3 * 2 * pow(divisions, 2.0f) * 6;

    if (f) {
        fprintf(f, "%d \n", numVertices * 3);

        float incr1 = -length / 2; //usada para nao repetir variaveis
        float incr2 = -length / 2;//idem

        //float var_xBT = -length / 2; //BT -> Bottom Top
        //float var_zBT = -length / 2; 
        //float var_yFB = -length / 2; //FB -> Front Back
        //float var_xFB = -length / 2; 
        //float var_yLR = -length / 2; //LR -> Left Right
        //float var_zLR = -length / 2; 

        for (int i = 1; i <= divisions; i++) {
            for (int j = 1; j <= divisions; j++) {

                //bottom
                fprintf(f, "%f %f %f \n", incr1, -length / 2, incr2);
                fprintf(f, "%f %f %f \n", incr1 + divSize, -length / 2, incr2 + divSize);
                fprintf(f, "%f %f %f \n", incr1, -length / 2, incr2 + divSize);

                fprintf(f, "%f %f %f \n", incr1, -length / 2, incr2);
                fprintf(f, "%f %f %f \n", incr1 + divSize, -length / 2, incr2);
                fprintf(f, "%f %f %f \n", incr1 + divSize, -length / 2, incr2 + divSize);

                //top
                fprintf(f, "%f %f %f \n", incr1, length / 2, incr2);
                fprintf(f, "%f %f %f \n", incr1, length / 2, incr2 + divSize);
                fprintf(f, "%f %f %f \n", incr1 + divSize, length / 2, incr2 + divSize);

                fprintf(f, "%f %f %f \n", incr1, length / 2, incr2);
                fprintf(f, "%f %f %f \n", incr1 + divSize, length / 2, incr2 + divSize);
                fprintf(f, "%f %f %f \n", incr1 + divSize, length / 2, incr2);

                //front
                fprintf(f, "%f %f %f \n", incr1, incr2 + divSize, length / 2);
                fprintf(f, "%f %f %f \n", incr1, incr2, length / 2);
                fprintf(f, "%f %f %f \n", incr1 + divSize, incr2, length / 2);

                fprintf(f, "%f %f %f \n", incr1, incr2 + divSize, length / 2);
                fprintf(f, "%f %f %f \n", incr1 + divSize, incr2, length / 2);
                fprintf(f, "%f %f %f \n", incr1 + divSize, incr2 + divSize, length / 2);

                //back
                fprintf(f, "%f %f %f \n", incr1, incr2 + divSize, -length / 2);
                fprintf(f, "%f %f %f \n", incr1 + divSize, incr2, -length / 2);
                fprintf(f, "%f %f %f \n", incr1, incr2, -length / 2);

                fprintf(f, "%f %f %f \n", incr1, incr2 + divSize, -length / 2);
                fprintf(f, "%f %f %f \n", incr1 + divSize, incr2 + divSize, -length / 2);
                fprintf(f, "%f %f %f \n", incr1 + divSize, incr2, -length / 2);

                //left
                fprintf(f, "%f %f %f \n", -length / 2, incr2 + divSize, incr1 + divSize);
                fprintf(f, "%f %f %f \n", -length / 2, incr2 + divSize, incr1);
                fprintf(f, "%f %f %f \n", -length / 2, incr2, incr1);

                fprintf(f, "%f %f %f \n", -length / 2, incr2 + divSize, incr1 + divSize);
                fprintf(f, "%f %f %f \n", -length / 2, incr2, incr1);
                fprintf(f, "%f %f %f \n", -length / 2, incr2, incr1 + divSize);

                //right
                fprintf(f, "%f %f %f \n", length / 2, incr2 + divSize, incr1 + divSize);
                fprintf(f, "%f %f %f \n", length / 2, incr2, incr1);
                fprintf(f, "%f %f %f \n", length / 2, incr2 + divSize, incr1);

                fprintf(f, "%f %f %f \n", length / 2, incr2 + divSize, incr1 + divSize);
                fprintf(f, "%f %f %f \n", length / 2, incr2, incr1 + divSize);
                fprintf(f, "%f %f %f \n", length / 2, incr2, incr1);

                incr1 += divSize;

                //var_xBT += divSize;
                //var_xFB += divSize;
                //var_zLR += divSize;

            }
            incr2 += divSize;

            //var_zBT += divSize;
            //var_yFB += divSize;
            //var_yLR += divSize;

            incr1 = -length / 2;

            //var_xBT = -length / 2;
            //var_xFB = -length / 2;
            //var_zLR = -length / 2;
        }
        printf("Ficheiro .3d criado com sucesso.\n");
    }
    fclose(f);

}

void sphere(float radius, int slices, int stacks, char* filename) {
    FILE* f;
    f = fopen(filename, "w");
    float angleCirc = 2 * M_PI / slices;
    float angleSides = M_PI / stacks;
    int numVertices = 6 * stacks * slices;

    if (f) {
        fprintf(f, "%d \n", 3 * numVertices);

        for (int i = 0; i < stacks; i++) {
            float angleSidesTemp = -(M_PI / 2) + i * angleSides;
            float y1 = radius * sin(angleSidesTemp + angleSides);
            float y2 = radius * sin(angleSidesTemp);
            for (int j = 0; j < slices; j++) {
                float angleCircTemp = j * angleCirc;
                float x1 = radius * cos(angleSidesTemp + angleSides) * sin(angleCircTemp);
                float x2 = radius * cos(angleSidesTemp) * sin(angleCircTemp);
                float x3 = radius * cos(angleSidesTemp) * sin(angleCircTemp + angleCirc);
                float x4 = radius * cos(angleSidesTemp + angleSides) * sin(angleCircTemp + angleCirc);
                float z1 = radius * cos(angleSidesTemp + angleSides) * cos(angleCircTemp);
                float z2 = radius * cos(angleSidesTemp) * cos(angleCircTemp);
                float z3 = radius * cos(angleSidesTemp) * cos(angleCircTemp + angleCirc);
                float z4 = radius * cos(angleSidesTemp + angleSides) * cos(angleCircTemp + angleCirc);

                fprintf(f, "%f %f %f \n", x1, y1, z1);
                fprintf(f, "%f %f %f \n", x2, y2, z2);
                fprintf(f, "%f %f %f \n", x3, y2, z3);

                fprintf(f, "%f %f %f \n", x1, y1, z1);
                fprintf(f, "%f %f %f \n", x3, y2, z3);
                fprintf(f, "%f %f %f \n", x4, y1, z4);
            }
        }
        printf("Ficheiro .3d criado com sucesso.\n");
    }
    else {
        printf("Erro ao criar o ficheiro.\nTente novamente.\n");
    }
    fclose(f);
}



void cone(float radius, float height, int slices, int stacks, char* filename) {
    FILE* f;
    f = fopen(filename, "w");


    float angleBase = 2 * M_PI / slices;
    float angleSides = height / stacks;


    if (f) {
        fprintf(f, "%d \n", 3 * 3 * slices + 3 * 6 * stacks * slices);
        //base
        for (int i = 0; i < slices; i++) {
            float angleBaseTemp = i * angleBase;
            fprintf(f, "%f %f %f \n", radius * sin(angleBaseTemp), 0.0f, radius * cos(angleBaseTemp));
            fprintf(f, "%f %f %f \n", 0.0f, 0.0, 0.0f);
            fprintf(f, "%f %f %f \n", radius * sin(angleBaseTemp + angleBase), 0.0f, radius * cos(angleBaseTemp + angleBase));
        }

        //sides
        for (int i = 0; i < stacks; i++) {
            float sides1 = i * angleSides;
            float sides2 = (i + 1) * angleSides;
            float rTemp1 = radius - ((radius / stacks) * i);
            float rTemp2 = radius - ((radius / stacks) * (i + 1));
            for (int j = 0; j < slices; j++) {
                float angleBaseTemp2 = j * angleBase;

                float x1 = rTemp1 * sin(angleBaseTemp2);
                float x2 = rTemp2 * sin(angleBaseTemp2 + angleBase);
                float x3 = rTemp2 * sin(angleBaseTemp2);
                float x4 = rTemp1 * sin(angleBaseTemp2 + angleBase);
                float y1 = sides1;
                float y2 = sides2;
                float z1 = rTemp1 * cos(angleBaseTemp2);
                float z2 = rTemp2 * cos(angleBaseTemp2 + angleBase);
                float z3 = rTemp2 * cos(angleBaseTemp2);
                float z4 = rTemp1 * cos(angleBaseTemp2 + angleBase);

                fprintf(f, "%f %f %f \n", x1, y1, z1);
                fprintf(f, "%f %f %f \n", x2, y2, z2);
                fprintf(f, "%f %f %f \n", x3, y2, z3);

                fprintf(f, "%f %f %f \n", x1, y1, z1);
                fprintf(f, "%f %f %f \n", x4, y1, z4);
                fprintf(f, "%f %f %f \n", x2, y2, z2);
            }
        }
        printf("Ficheiro .3d criado com sucesso.\n");
    }
    else {
        printf("Erro ao criar o ficheiro.\nTente novamente.\n");
    }

    fclose(f);

}


void torus(float innerRadius, float outterRadius, int sides, int rings, char* filename) {
    FILE* f;
    f = fopen(filename, "w");

    float angle_alfa = 2 * M_PI / sides;  //w -> rotação � volta do referencial principal
    float angle_beta = 2 * M_PI / rings;  //v -> rotação � volta do corpo do torus
    float v = 0.0f;
    float w = 0.0f;

    if (f) {
        fprintf(f, "%d \n", 3 * 6 * rings * sides); // num de coordenadas

        for (int i = 0; i < sides; i++) {
            for (int j = 0; j < rings; j++) {
                float x = (outterRadius + innerRadius * cos(v)) * cos(w);
                float x1 = (outterRadius + innerRadius * cos(v)) * cos(w + angle_alfa);
                float x2 = (outterRadius + innerRadius * cos(v + angle_beta)) * cos(w + angle_alfa);
                float x3 = (outterRadius + innerRadius * cos(v + angle_beta)) * cos(w);
                float y = (outterRadius + innerRadius * cos(v)) * sin(w);
                float y1 = (outterRadius + innerRadius * cos(v)) * sin(w + angle_alfa);
                float y2 = (outterRadius + innerRadius * cos(v + angle_beta)) * sin(w + angle_alfa);
                float y3 = (outterRadius + innerRadius * cos(v + angle_beta)) * sin(w);
                float z = innerRadius * sin(v);
                float z1 = innerRadius * sin(v + angle_beta);

                fprintf(f, "%f %f %f \n", x, y, z);
                fprintf(f, "%f %f %f \n", x1, y1, z);
                fprintf(f, "%f %f %f \n", x2, y2, z1);

                fprintf(f, "%f %f %f \n", x2, y2, z1);
                fprintf(f, "%f %f %f \n", x3, y3, z1);
                fprintf(f, "%f %f %f \n", x, y, z);

                v = angle_beta * (j + 1);
            }
            w = angle_alfa * (i + 1);
        }
        printf("Ficheiro .3d criado com sucesso.\n");
    }
    else {
        printf("Erro ao criar o ficheiro.\nTente novamente.\n");
    }
    fclose(f);
}


void multMatrixVector(float* m, float* v, float* res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }
}

float* getBezierCubicCurves(float t, float* p0, float* p1, float* p2, float* p3) {
    float* pos = new float[3];

    //bezier matrix
    float m[4][4] = { {-1.0f,  3.0f, -3.0f,  1.0f},
                      { 3.0f, -6.0f,  2.0f,  0.0f},
                      {-3.0f,  3.0f,  0.0f,  0.0f},
                      { 1.0f,  0.0f,  0.0f,  0.0f} };

    for (int i = 0; i < 3; i++) {	 // i = x, y, z
        float p[4] = { p0[i], p1[i], p2[i], p3[i] };
        float a[4];

        // Compute A = M * P
        multMatrixVector((float*)m, p, a);

        // Compute pos = T * A
        pos[i] = powf(t, 3.0) * a[0] + powf(t, 2.0) * a[1] + t * a[2] + a[3];
    }

    return pos;
}

void teapot(char* patchname, int tessellation, char* filename) {
    FILE* fw;
    FILE* fr;
    fr = fopen(patchname, "r");
    fw = fopen(filename, "w");

    int number_patches; //numero de patches
    int n_ctrlpoints; //numero de pontos de controlo
    char* n = (char*)malloc(sizeof(char) * 2);
    n[0] = 0;
    char* numero = (char*)malloc(sizeof(char) * 3);
    numero[0] = 0;
    char c;
    int i = 0;
    int j = 0;
    int flag = 1;
    float** ctrlpoints;
    int** indices;
    float gt = 1.0 / tessellation;
    if (fr) {
        //number_patches = atoi(fgets(n_patches, 3, fr));

        //printf("ola\n");
        while ((c = fgetc(fr)) != EOF) {
            if (flag == 1) {
                if (c == '\n') {
                    //printf("string:%s\n", n);
                    number_patches = atoi(n);
                    indices = new int* [number_patches];
                    indices[j] = (int*)malloc(sizeof(int) * 78);
                    indices[j][0] = 0;
                    n[0] = 0;
                    flag = 0;
                }
                else {
                    strncat(n, &c, 1);
                    //printf("string:%s\n", n);
                }
            }
            else if (flag == 0) {
                if (c == ',') {
                    //printf("numero:%s\n", numero);
                    int elem = atoi(numero);
                    indices[j][i++] = elem;
                    //printf("indices[%d][%d]: %d\n", j, i-1, indices[j][i-1]);
                    numero[0] = 0;
                }
                else if (c == '\n') {
                    int elem = atoi(numero);
                    indices[j][i++] = elem;
                    //printf("indices[%d][%d]: %d\n", j, i-1, indices[j][i - 1]);
                    numero[0] = 0;
                    j++;
                    i = 0;
                    if (j == number_patches) {
                        flag = 2;
                        j = 0;
                    }
                    else {
                        indices[j] = (int*)malloc(sizeof(int) * 78);
                        indices[j][0] = 0;
                    }
                }
                else if (c != ' ') {
                    strncat(numero, &c, 1);
                }
            }
            else if (flag == 2) {
                if (c == '\n') {
                    //printf("string:%s\n", n);
                    n_ctrlpoints = atoi(n);
                    ctrlpoints = new float* [n_ctrlpoints];
                    ctrlpoints[j] = (float*)malloc(sizeof(float) * 3);
                    ctrlpoints[j][0] = 0;
                    flag = 3;
                }
                else {
                    strncat(n, &c, 1);
                    //printf("string:%s\n", n);
                }
            }
            else if (flag == 3) {
                if (c == ',') {
                    float elem = atof(numero);
                    //printf("ctrlpoints[%d][%d]: %d\n", j, i, ctrlpoints[j][i]);
                    ctrlpoints[j][i++] = elem;
                    //printf("ctrlpoints[%d][%d]: %f\n", j, i - 1, ctrlpoints[j][i - 1]);
                    numero[0] = 0;
                }
                else if (c == '\n') {
                    float elem = atof(numero);
                    ctrlpoints[j][i++] = elem;
                    //printf("ctrlpoints[%d][%d]: %f\n", j, i - 1, ctrlpoints[j][i - 1]);
                    numero[0] = 0;
                    j++;
                    i = 0;

                    ctrlpoints[j] = (float*)malloc(sizeof(float) * 3);
                    ctrlpoints[j][0] = 0;
                }
                else if (c != ' ') {
                    strncat(numero, &c, 1);
                    //printf("string:%s\n", numero);
                }
            }
        }
        float elem = atof(numero);
        ctrlpoints[j][i++] = elem;
        //printf("ctrlpoints[%d][%d]: %d\n", j, i - 1, ctrlpoints[j][i - 1]);
        numero[0] = 0;

        if (fw) {
            fprintf(fw, "%d \n", number_patches * (tessellation) * 2 * (tessellation) * 3 * 3);
            float*** resultPoints = new float** [number_patches];
            float pontosTeapot[4][3];
            int N;
            float pontos[16][3];
            float* point = new float[3];
            for (int k = 0; k < number_patches; k++) {
                resultPoints[k] = new float* [4];
                for (int v = 0; v < tessellation; v++) {
                    for (int u = 0; u < tessellation; u++) {

                        float x1 = gt * v;
                        float y1 = gt * u;
                        float x2 = gt * (v + 1);
                        float y2 = gt * (u + 1);

                        //vai calcular coordenadas x,y,z dos 4 pontos que v�o formar as 4 curvas (ou seja, dos 16 pontos q estao no array dos indices):
                        for (int l = 0; l < 16; l++) {//percorre cada ponto de controlo de cada lista de indices
                            pontos[l][0] = ctrlpoints[indices[k][l]][0];//coordenada x de cada ponto da curva
                            pontos[l][1] = ctrlpoints[indices[k][l]][1];//coordenada y de cada ponto da curva
                            pontos[l][2] = ctrlpoints[indices[k][l]][2];//coordenada z de cada ponto da curva
                        }

                        //----------------------------------------------//
                        N = 0;
                        for (int j = 0; j < 16; j += 4) {
                            point = getBezierCubicCurves(x1, pontos[j], pontos[j + 1], pontos[j + 2], pontos[j + 3]);//vai calcular ponto da curva formada pelos 4 vertices guardados
                            //printf("RESULT: %f , %f, %f\n", point[0], point[1], point[2]);
                            //guardamos de seguida as coordenadas x,y,z dos 4 pontos das 4 curvas
                            pontosTeapot[N][0] = point[0];
                            pontosTeapot[N][1] = point[1];
                            pontosTeapot[N][2] = point[2];
                            N++;
                        }
                        resultPoints[k][0] = getBezierCubicCurves(y1, pontosTeapot[0], pontosTeapot[1], pontosTeapot[2], pontosTeapot[3]);
                        //-----------------------------------------//
                        N = 0;
                        for (int j = 0; j < 16; j += 4) {
                            point = getBezierCubicCurves(x1, pontos[j], pontos[j + 1], pontos[j + 2], pontos[j + 3]);//vai calcular ponto da curva formada pelos 4 vertices guardados
                            //printf("RESULT: %f , %f, %f\n", point[0], point[1], point[2]);
                            //guardamos de seguida as coordenadas x,y,z dos 4 pontos das 4 curvas
                            pontosTeapot[N][0] = point[0];
                            pontosTeapot[N][1] = point[1];
                            pontosTeapot[N][2] = point[2];
                            N++;
                        }
                        resultPoints[k][1] = getBezierCubicCurves(y2, pontosTeapot[0], pontosTeapot[1], pontosTeapot[2], pontosTeapot[3]);
                        //----------------------------------------------//
                        N = 0;
                        for (int j = 0; j < 16; j += 4) {
                            point = getBezierCubicCurves(x2, pontos[j], pontos[j + 1], pontos[j + 2], pontos[j + 3]);//vai calcular ponto da curva formada pelos 4 vertices guardados
                            //printf("RESULT: %f , %f, %f\n", point[0], point[1], point[2]);
                            //guardamos de seguida as coordenadas x,y,z dos 4 pontos das 4 curvas
                            pontosTeapot[N][0] = point[0];
                            pontosTeapot[N][1] = point[1];
                            pontosTeapot[N][2] = point[2];
                            N++;
                        }
                        resultPoints[k][2] = getBezierCubicCurves(y1, pontosTeapot[0], pontosTeapot[1], pontosTeapot[2], pontosTeapot[3]);
                        //--------------------------------------------------//
                        N = 0;
                        for (int j = 0; j < 16; j += 4) {
                            point = getBezierCubicCurves(x2, pontos[j], pontos[j + 1], pontos[j + 2], pontos[j + 3]);//vai calcular ponto da curva formada pelos 4 vertices guardados
                            //printf("RESULT: %f , %f, %f\n", point[0], point[1], point[2]);
                            //guardamos de seguida as coordenadas x,y,z dos 4 pontos das 4 curvas
                            pontosTeapot[N][0] = point[0];
                            pontosTeapot[N][1] = point[1];
                            pontosTeapot[N][2] = point[2];
                            N++;
                        }
                        resultPoints[k][3] = getBezierCubicCurves(y2, pontosTeapot[0], pontosTeapot[1], pontosTeapot[2], pontosTeapot[3]);

                        fprintf(fw, "%f %f %f \n", resultPoints[k][0][0], resultPoints[k][0][1], resultPoints[k][0][2]);
                        fprintf(fw, "%f %f %f \n", resultPoints[k][2][0], resultPoints[k][2][1], resultPoints[k][2][2]);
                        fprintf(fw, "%f %f %f \n", resultPoints[k][3][0], resultPoints[k][3][1], resultPoints[k][3][2]);

                        fprintf(fw, "%f %f %f \n", resultPoints[k][0][0], resultPoints[k][0][1], resultPoints[k][0][2]);
                        fprintf(fw, "%f %f %f \n", resultPoints[k][3][0], resultPoints[k][3][1], resultPoints[k][3][2]);
                        fprintf(fw, "%f %f %f \n", resultPoints[k][1][0], resultPoints[k][1][1], resultPoints[k][1][2]);

                    }
                }
            }
            printf("Ficheiro .3d criado com sucesso.\n");
        }

        else {
            printf("Erro ao criar o ficheiro.\nTente novamente.\n");
        }

    }
    else {
        printf("Erro ao ler o ficheiro.\nTente novamente.\n");
    }

    free(indices);
    free(ctrlpoints);
    fclose(fw);
    fclose(fr);
}


int main(int argc, char* argv[])
{
    if (argc < 1) {
        printf("Faltam Argumentos.\nTente novamente.");
        return 1;
    }
    else if (strcmp(argv[1], "plane") == 0) {
        plane(atof(argv[2]), atoi(argv[3]), argv[4]);
    }
    else if (strcmp(argv[1], "box") == 0) {
        box(atof(argv[2]), atoi(argv[3]), argv[4]);
    }
    else if (strcmp(argv[1], "sphere") == 0) {
        sphere(atof(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
    }
    else if (strcmp(argv[1], "cone") == 0) {
        cone(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
    }

    else if (strcmp(argv[1], "torus") == 0) {
        torus(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
    }
    else if (strcmp(argv[1], "teapot.patch") == 0) {  //teapot.patch "valor para a tesselagem" teapot.3d
        teapot(argv[1], atof(argv[2]), argv[3]);
    }

    return 0;
}


