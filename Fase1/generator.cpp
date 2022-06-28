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
        float var_x = -length/2;
        float var_z = -length/2;
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
            var_x = -length/2;
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


    if (f){
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
    return 0;
}