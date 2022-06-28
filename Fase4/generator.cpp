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
        //normais
        fprintf(f, "n\n");
        for (i = 0; i < divisions; i++) {
            for (j = 0; j < divisions; j++) { 
                fprintf(f, "%f %f %f \n", 0.0, 1.0, 0.0);
                fprintf(f, "%f %f %f \n", 0.0, 1.0, 0.0);
                fprintf(f, "%f %f %f \n", 0.0, 1.0, 0.0);


                fprintf(f, "%f %f %f \n", 0.0, 1.0, 0.0);
                fprintf(f, "%f %f %f \n", 0.0, 1.0, 0.0);
                fprintf(f, "%f %f %f \n", 0.0, 1.0, 0.0);
                
            }
        }

        //texturas

        var_x = 0;
        var_z = 0;
        divSize = 1.0f / divisions;

        fprintf(f, "t%d\n", numVertices*2); //num de coordenadas total das texturas

        for (i = 0; i < divisions; i++) {
            for (j = 0; j < divisions; j++) {
                fprintf(f, "%f %f \n", var_x,  var_z);
                fprintf(f, "%f %f \n", var_x,  var_z + divSize);
                fprintf(f, "%f %f \n", var_x + divSize, var_z + divSize);

                fprintf(f, "%f %f \n", var_x, var_z);
                fprintf(f, "%f %f \n", var_x + divSize, var_z + divSize);
                fprintf(f, "%f %f \n", var_x + divSize, var_z);

                var_x += divSize;
            }
            var_z += divSize;
            var_x = 0;
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

            }
            incr2 += divSize;

            incr1 = -length / 2;
        }

        //normais
        fprintf(f, "n\n");
        for (int i = 1; i <= divisions; i++) {
            for (int j = 1; j <= divisions; j++) {
                //bottom
                fprintf(f, "%f %f %f \n", 0.0, -1.0, 0.0);
                fprintf(f, "%f %f %f \n", 0.0, -1.0, 0.0);
                fprintf(f, "%f %f %f \n", 0.0, -1.0, 0.0);

                fprintf(f, "%f %f %f \n", 0.0, -1.0, 0.0);
                fprintf(f, "%f %f %f \n", 0.0, -1.0, 0.0);
                fprintf(f, "%f %f %f \n", 0.0, -1.0, 0.0);

                //top
                fprintf(f, "%f %f %f \n", 0.0, 1.0, 0.0);
                fprintf(f, "%f %f %f \n", 0.0, 1.0, 0.0);
                fprintf(f, "%f %f %f \n", 0.0, 1.0, 0.0);

                fprintf(f, "%f %f %f \n", 0.0, 1.0, 0.0);
                fprintf(f, "%f %f %f \n", 0.0, 1.0, 0.0);
                fprintf(f, "%f %f %f \n", 0.0, 1.0, 0.0);

                //front
                fprintf(f, "%f %f %f \n", 0.0, 0.0, 1.0);
                fprintf(f, "%f %f %f \n", 0.0, 0.0, 1.0);
                fprintf(f, "%f %f %f \n", 0.0, 0.0, 1.0);

                fprintf(f, "%f %f %f \n", 0.0, 0.0, 1.0);
                fprintf(f, "%f %f %f \n", 0.0, 0.0, 1.0);
                fprintf(f, "%f %f %f \n", 0.0, 0.0, 1.0);

                //back
                fprintf(f, "%f %f %f \n", 0.0, 0.0, -1.0);
                fprintf(f, "%f %f %f \n", 0.0, 0.0, -1.0);
                fprintf(f, "%f %f %f \n", 0.0, 0.0, -1.0);

                fprintf(f, "%f %f %f \n", 0.0, 0.0, -1.0);
                fprintf(f, "%f %f %f \n", 0.0, 0.0, -1.0);
                fprintf(f, "%f %f %f \n", 0.0, 0.0, -1.0);

                //left
                fprintf(f, "%f %f %f \n", -1.0, 0.0, 0.0);
                fprintf(f, "%f %f %f \n", -1.0, 0.0, 0.0);
                fprintf(f, "%f %f %f \n", -1.0, 0.0, 0.0);

                fprintf(f, "%f %f %f \n", -1.0, 0.0, 0.0);
                fprintf(f, "%f %f %f \n", -1.0, 0.0, 0.0);
                fprintf(f, "%f %f %f \n", -1.0, 0.0, 0.0);

                //right
                fprintf(f, "%f %f %f \n", 1.0, 0.0, 0.0);
                fprintf(f, "%f %f %f \n", 1.0, 0.0, 0.0);
                fprintf(f, "%f %f %f \n", 1.0, 0.0, 0.0);

                fprintf(f, "%f %f %f \n", 1.0, 0.0, 0.0);
                fprintf(f, "%f %f %f \n", 1.0, 0.0, 0.0);
                fprintf(f, "%f %f %f \n", 1.0, 0.0, 0.0);
            }
        }

        //texturas
        fprintf(f, "t%d\n", numVertices * 2);

        incr1 = 0;
        incr2 = 0;
        divSize = 1.0f / divisions;

        for (int i = 1; i <= divisions; i++) {
            for (int j = 1; j <= divisions; j++) {

                //bottom
                fprintf(f, "%f %f \n", incr1, incr2);
                fprintf(f, "%f %f \n", incr1 + divSize, incr2 + divSize);
                fprintf(f, "%f %f \n", incr1, incr2 + divSize);

                fprintf(f, "%f %f \n", incr1, incr2);
                fprintf(f, "%f %f \n", incr1 + divSize, incr2);
                fprintf(f, "%f %f \n", incr1 + divSize, incr2 + divSize);

                //top
                fprintf(f, "%f %f \n", incr1, incr2);
                fprintf(f, "%f %f \n", incr1,  incr2 + divSize);
                fprintf(f, "%f %f \n", incr1 + divSize,  incr2 + divSize);

                fprintf(f, "%f %f \n", incr1, incr2);
                fprintf(f, "%f %f \n", incr1 + divSize,  incr2 + divSize);
                fprintf(f, "%f %f \n", incr1 + divSize, incr2);

                //front
                fprintf(f, "%f %f \n", incr1, incr2 + divSize);
                fprintf(f, "%f %f \n", incr1, incr2);
                fprintf(f, "%f %f \n", incr1 + divSize, incr2);

                fprintf(f, "%f %f \n", incr1, incr2 + divSize);
                fprintf(f, "%f %f \n", incr1 + divSize, incr2);
                fprintf(f, "%f %f \n", incr1 + divSize, incr2 + divSize);

                //back
                fprintf(f, "%f %f \n", incr1, incr2 + divSize);
                fprintf(f, "%f %f \n", incr1 + divSize, incr2);
                fprintf(f, "%f %f \n", incr1, incr2);

                fprintf(f, "%f %f \n", incr1, incr2 + divSize);
                fprintf(f, "%f %f \n", incr1 + divSize, incr2 + divSize);
                fprintf(f, "%f %f \n", incr1 + divSize, incr2);

                //left
                fprintf(f, "%f %f \n", incr2 + divSize, incr1 + divSize);
                fprintf(f, "%f %f \n", incr2 + divSize, incr1);
                fprintf(f, "%f %f \n", incr2, incr1);

                fprintf(f, "%f %f \n", incr2 + divSize, incr1 + divSize);
                fprintf(f, "%f %f \n", incr2, incr1);
                fprintf(f, "%f %f \n", incr2, incr1 + divSize);

                //right
                fprintf(f, "%f %f \n", incr2 + divSize, incr1 + divSize);
                fprintf(f, "%f %f \n", incr2, incr1);
                fprintf(f, "%f %f \n", incr2 + divSize, incr1);

                fprintf(f, "%f %f \n", incr2 + divSize, incr1 + divSize);
                fprintf(f, "%f %f \n", incr2, incr1 + divSize);
                fprintf(f, "%f %f \n", incr2, incr1);

                incr1 += divSize;

            }
            incr2 += divSize;

            incr1 = 0;
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

    float texts = 1.0 / slices;
    float textst = 1.0 / stacks;

    if (f) {
        fprintf(f, "%d \n", 3 * numVertices);

        float* normais = (float*)malloc(sizeof(float) * 3 * numVertices);
        float* texturas = (float*)malloc(sizeof(float) * 2 * numVertices);

        int k = 0, w = 0;

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

                normais[k++] = x1 / radius; normais[k++] = y1 / radius; normais[k++] = z1 / radius;
                normais[k++] = x2 / radius; normais[k++] = y2 / radius; normais[k++] = z2 / radius;
                normais[k++] = x3 / radius; normais[k++] = y2 / radius; normais[k++] = z3 / radius;

                texturas[w++] = j * texts; texturas[w++] = i * textst;  //(0,0)
                texturas[w++] = j * texts + texts; texturas[w++] = i * textst;  //(1,0)
                texturas[w++] = j * texts; texturas[w++] = i * textst + textst;  //(0,1)

                fprintf(f, "%f %f %f \n", x1, y1, z1);
                fprintf(f, "%f %f %f \n", x3, y2, z3);
                fprintf(f, "%f %f %f \n", x4, y1, z4);

                normais[k++] = x1 / radius; normais[k++] = y1 / radius; normais[k++] = z1 / radius;
                normais[k++] = x3 / radius; normais[k++] = y2 / radius; normais[k++] = z3 / radius;
                normais[k++] = x4 / radius; normais[k++] = y1 / radius; normais[k++] = z4 / radius;

                texturas[w++] = j * texts + texts; texturas[w++] = i * textst;  //(1,0)
                texturas[w++] = j * texts + texts; texturas[w++] = i * textst + textst; //(1,1)
                texturas[w++] = j * texts; texturas[w++] = i * textst + textst; //(0,1)
            }
        }
        //normais
        fprintf(f, "n\n");
        for (int i = 0; i < 3 * 6 * stacks * slices; i += 3) {
            fprintf(f, "%f %f %f \n", normais[i], normais[i + 1], normais[i + 2]);
        }

        //texturas
        fprintf(f, "t%d \n", 2 * 6 * stacks * slices);
        for (int i = 0; i < 2 * 6 * stacks * slices; i += 2) {
            fprintf(f, "%f %f \n", texturas[i], texturas[i + 1]);
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
    int k = 0;
    int w = 0;

    float texts = 1.0 / slices;
    float textst = 1.0 / stacks;

    if (f) {
        fprintf(f, "%d \n", 3 * 3 * slices + 3 * 6 * stacks * slices);
        float* normais = (float*)malloc(sizeof(float) * (3 * 3 * slices + 3 * 6 * stacks * slices));
        float* texturas = (float*)malloc(sizeof(float) * (2 * 3 * slices + 2 * 6 * stacks * slices));

        
        //base
        for (int i = 0; i < slices; i++) {
            float angleBaseTemp = i * angleBase;
            fprintf(f, "%f %f %f \n", radius * sin(angleBaseTemp), 0.0f, radius * cos(angleBaseTemp));
            fprintf(f, "%f %f %f \n", 0.0f, 0.0, 0.0f);
            fprintf(f, "%f %f %f \n", radius * sin(angleBaseTemp + angleBase), 0.0f, radius * cos(angleBaseTemp + angleBase));

            normais[k++] = 0; normais[k++] = -1; normais[k++] = 0;
            normais[k++] = 0; normais[k++] = -1; normais[k++] = 0;
            normais[k++] = 0; normais[k++] = -1; normais[k++] = 0;

            texturas[w++] = 0; texturas[w++] = 0;
            texturas[w++] = sin(angleBaseTemp + angleBase); texturas[w++] = cos(angleBaseTemp + angleBase);
            texturas[w++] = sin(angleBaseTemp); texturas[w++] = cos(angleBaseTemp);
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

                normais[k++] = sin(angleBaseTemp2); normais[k++] = height / stacks; normais[k++] = cos(angleBaseTemp2);
                normais[k++] = sin(angleBaseTemp2 + angleBase); normais[k++] = height / stacks; normais[k++] = cos(angleBaseTemp2 + angleBase);
                normais[k++] = sin(angleBaseTemp2); normais[k++] = height / stacks; normais[k++] = cos(angleBaseTemp2);

                texturas[w++] = j * texts; texturas[w++] = i * textst;  //(0,0)
                texturas[w++] = j * texts + texts; texturas[w++] = i * textst;  //(1,0)
                texturas[w++] = j * texts; texturas[w++] = i * textst + textst;  //(0,1)

                fprintf(f, "%f %f %f \n", x1, y1, z1);
                fprintf(f, "%f %f %f \n", x4, y1, z4);
                fprintf(f, "%f %f %f \n", x2, y2, z2);

                normais[k++] = sin(angleBaseTemp2); normais[k++] = height / stacks; normais[k++] = cos(angleBaseTemp2);
                normais[k++] = sin(angleBaseTemp2 + angleBase); normais[k++] = height / stacks; normais[k++] = cos(angleBaseTemp2 + angleBase);
                normais[k++] = sin(angleBaseTemp2 + angleBase); normais[k++] = height / stacks; normais[k++] = cos(angleBaseTemp2 + angleBase);

                texturas[w++] = j * texts + texts; texturas[w++] = i * textst;  //(1,0)
                texturas[w++] = j * texts + texts; texturas[w++] = i * textst + textst; //(1,1)
                texturas[w++] = j * texts; texturas[w++] = i * textst + textst; //(0,1)
            }
        }
        //normais
        fprintf(f, "n\n");
        for (int i = 0; i < 3 * 3 * slices + 3 * 6 * stacks * slices; i += 3) {
            fprintf(f, "%f %f %f \n", normais[i], normais[i + 1], normais[i + 2]);
        }

        //texturas
        fprintf(f, "t%d\n", 2 * 3 * slices + 2 * 6 * stacks * slices);
        for (int i = 0; i < 2 * 3 * slices + 2 * 6 * stacks * slices; i += 2) {
            fprintf(f, "%f %f \n", texturas[i], texturas[i + 1]);
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

    float angle_alfa = 2 * M_PI / sides;  //w -> rotação em volta do referencial principal
    float angle_beta = 2 * M_PI / rings;  //v -> rotação em volta do corpo do torus
    float v = 0.0f;
    float w = 0.0f;
    
    float texts = 1.0 / sides;
    float textst = 1.0 / rings;

    int k = 0, s = 0;

    if (f) {
        fprintf(f, "%d \n", 3 * 6 * rings * sides); // num de coordenadas
        float* normais = (float*)malloc(sizeof(float) * 3 * 6 * rings * sides);
        float* texturas = (float*)malloc(sizeof(float) * 2 * 6 * rings * sides);

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

                normais[k++] = cos(v) * cos(w); normais[k++] = cos(v) * sin(w); normais[k++] = sin(v);
                normais[k++] = cos(v) * cos(w + angle_alfa); normais[k++] = cos(v) * sin(w + angle_alfa); normais[k++] = sin(v);
                normais[k++] = cos(v + angle_beta) * cos(w + angle_alfa); normais[k++] = cos(v + angle_beta) * sin(w + angle_alfa); normais[k++] = sin(v + angle_beta);

                texturas[s++] = i * texts; texturas[s++] = j * textst;  //(0,0)
                texturas[s++] = i * texts + texts; texturas[s++] = j * textst;  //(1,0)
                texturas[s++] = i * texts; texturas[s++] = j * textst + textst;   //(0,1)

                fprintf(f, "%f %f %f \n", x2, y2, z1);
                fprintf(f, "%f %f %f \n", x3, y3, z1);
                fprintf(f, "%f %f %f \n", x, y, z);

                normais[k++] = cos(v + angle_beta) * cos(w + angle_alfa); normais[k++] = cos(v + angle_beta) * sin(w + angle_alfa); normais[k++] = sin(v + angle_beta);
                normais[k++] = cos(v + angle_beta) * cos(w); normais[k++] = cos(v + angle_beta) * sin(w); normais[k++] = sin(v + angle_beta);
                normais[k++] = cos(v) * cos(w); normais[k++] = cos(v) * sin(w); normais[k++] = sin(v);

                texturas[s++] = i * texts + texts; texturas[s++] = j * textst;  //(1,0)
                texturas[s++] = i * texts + texts; texturas[s++] = j * textst + textst;  //(1,1)
                texturas[s++] = i * texts; texturas[s++] = j * textst + textst;  //(0,1)

                v = angle_beta * (j + 1);
            }
            w = angle_alfa * (i + 1);
        }
        //normais
        fprintf(f, "n\n");
        for (int i = 0; i < 3 * 6 * rings * sides; i += 3) {
            fprintf(f, "%f %f %f \n", normais[i], normais[i + 1], normais[i + 2]);
        }

        //texturas
        fprintf(f, "t%d \n", 2 * 6 * rings * sides); // num de coordenadas
        for (int i = 0; i < 2 * 6 * rings * sides; i += 2) {
            fprintf(f, "%f %f \n", texturas[i], texturas[i + 1]);
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
        while ((c = fgetc(fr)) != EOF) {
            if (flag == 1) {
                if (c == '\n') {
                    number_patches = atoi(n);
                    indices = new int* [number_patches];
                    indices[j] = (int*)malloc(sizeof(int) * 78);
                    indices[j][0] = 0;
                    n[0] = 0;
                    flag = 0;
                }
                else {
                    strncat(n, &c, 1);
                }
            }
            else if (flag == 0) {
                if (c == ',') {
                    int elem = atoi(numero);
                    indices[j][i++] = elem;
                    numero[0] = 0;
                }
                else if (c == '\n') {
                    int elem = atoi(numero);
                    indices[j][i++] = elem;
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
                    
                    n_ctrlpoints = atoi(n);
                    ctrlpoints = new float* [n_ctrlpoints];
                    ctrlpoints[j] = (float*)malloc(sizeof(float) * 3);
                    ctrlpoints[j][0] = 0;
                    flag = 3;
                }
                else {
                    strncat(n, &c, 1);
                }
            }
            else if (flag == 3) {
                if (c == ',') {
                    float elem = atof(numero);
                    ctrlpoints[j][i++] = elem;
                    numero[0] = 0;
                }
                else if (c == '\n') {
                    float elem = atof(numero);
                    ctrlpoints[j][i++] = elem;
                    numero[0] = 0;
                    j++;
                    i = 0;

                    ctrlpoints[j] = (float*)malloc(sizeof(float) * 3);
                    ctrlpoints[j][0] = 0;
                }
                else if (c != ' ') {
                    strncat(numero, &c, 1);
                }
            }
        }
        float elem = atof(numero);
        ctrlpoints[j][i++] = elem;
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

                        //vai calcular coordenadas x,y,z dos 4 pontos que vão formar as 4 curvas (ou seja, dos 16 pontos q estao no array dos indices):
                        for (int l = 0; l < 16; l++) {//percorre cada ponto de controlo de cada lista de indices
                            pontos[l][0] = ctrlpoints[indices[k][l]][0];//coordenada x de cada ponto da curva
                            pontos[l][1] = ctrlpoints[indices[k][l]][1];//coordenada y de cada ponto da curva
                            pontos[l][2] = ctrlpoints[indices[k][l]][2];//coordenada z de cada ponto da curva
                        }

                        //----------------------------------------------//
                        N = 0;
                        for (int j = 0; j < 16; j += 4) {
                            point = getBezierCubicCurves(x1, pontos[j], pontos[j + 1], pontos[j + 2], pontos[j + 3]);//vai calcular ponto da curva formada pelos 4 vertices guardados
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
        }
        else {
            printf("Erro ao criar o ficheiro.\nTente novamente.\n");
        }

    }
    else {
        printf("Erro ao ler o ficheiro.\nTente novamente.\n");
    }

    printf("Ficheiro .3d criado com sucesso.\n");

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


