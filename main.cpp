#include <iostream>
#include <GL/glut.h>
#include <time.h> 
#include <stdlib.h>
#include <unistd.h>
using namespace std;
/*
void nextGenerationSquare(int x1, int y1,float* rgb)
{
    glColor3f(rgb[0],rgb[1],rgb[2]);
    glBegin(GL_QUADS);
	float xd = 1.0 / (LENGTH+BUFFER);
	float yd = 1.0 / (HEIGHT+BUFFER);
	float xf=x1- (LENGTH+BUFFER/2.0)/2.0;
	float yf=y1- (HEIGHT+BUFFER/2.0)/2.0;
	xf = xf/(LENGTH/2.0 + BUFFER/2.0) +xd*(BUFFER)/2.0;
	yf = yf/(HEIGHT/2.0 + BUFFER/2.0) +yd*(BUFFER)/2.0;
	xf += xd;
	yf += yd;
	xd=xd*SCALE;
	yd=yd*SCALE;
    glVertex2d(xf + xd, yf + yd);
    glVertex2d(xf + xd, yf - yd);
    glVertex2d(xf - xd, yf - yd);
    glVertex2d(xf - xd, yf + yd);

    glEnd();
}
*/
const int oneLen=6;
const int children=4;

float* triangles;
unsigned int length;

void renderTriangle(int num){
	glColor3f(0,1,0);
	glBegin(GL_LINE_LOOP);
	for (int i =0;i<oneLen/2;i++){
		glVertex2d(triangles[num*oneLen + i*2],triangles[num*oneLen+i*2+ 1]);
	}
	glEnd();
}

void disInit(){
	glClearColor(0.1f, 0.1f, 0.1f, 0); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
	//nextGeneration();
	for(int i =0;i<length/oneLen;i++){
		renderTriangle(i);
	}
	glFlush();
}



void run(){
	float *newTriangle = new float[length*children];
	for (int i =0;i<length/oneLen;i++){
		for(int j=0;j<oneLen/2;j++){
			if(children>=2){
				newTriangle[(i*children+j)*oneLen ]=triangles[i*oneLen+j*2];
				newTriangle[(i*children+j)*oneLen +1]=triangles[i*oneLen+j*2+1];
			}
			for(int k=0;k<children-2;k++){
				newTriangle[((i*children + j)*oneLen) + (k + 1)*2] = (triangles[i*oneLen + (j)*2] + triangles[i*oneLen +((j+k+1)%(oneLen/2))*2])/2;
				newTriangle[((i*children + j)*oneLen) + (k + 1)*2 +1] = (triangles[i*oneLen + (j)*2 +1] + triangles[i*oneLen +((j+k+1)%(oneLen/2))*2 +1])/2;
			}
			newTriangle[(i*children+ children-1)*oneLen +j*2]=(triangles[i*oneLen +j*2] +triangles[i*oneLen+((j+1)%(oneLen/2))*2])/2;
			newTriangle[(i*children+ children-1)*oneLen +j*2 +1]=(triangles[i*oneLen +j*2 +1] +triangles[i*oneLen+((j+1)%(oneLen/2))*2 +1])/2;
		}
	}
	free(triangles);
	triangles=newTriangle;
	length*=children;
	for(int i=0;i<length/oneLen;i++){
		renderTriangle(i);
	}
	glFlush();
	sleep(1);
}


int main(int argc, char** argv) {
	length = oneLen;
	triangles = new float[oneLen];
	triangles[0]=-1;
	triangles[1]=-1;
	triangles[2]=0;
	triangles[3]=1;
	triangles[4]=1;
	triangles[5]=-1;
	srand (time(NULL));
	glutInit(&argc, argv);		// Initialize GLUT
    glutInitWindowSize(600,800);   // Set the window's initial width & height
    glutCreateWindow("Lines"); // Create a window with the given title
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutDisplayFunc(disInit); // Register display callback handler for window re-paint
	glutIdleFunc(run);
	glutMainLoop();
	return 0;
}
