#include <iostream>
#include <GL/glut.h>
#include <time.h> 
#include <stdlib.h>
#include <unistd.h>



using namespace std;
/*
void drawSquare(int x1, int y1,float* rgb)
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

float* triangles;
unsigned int length;

void renderTriangle(int num){
	glColor3f(0,1,0);
	glBegin(GL_LINE_LOOP);
	glVertex2d(triangles[num*6],triangles[num*6+ 1]);
	glVertex2d(triangles[num*6+2],triangles[num*6+ 3]);
	glVertex2d(triangles[num*6+4],triangles[num*6+ 5]);
	glEnd();
}

void disInit(){
	glClearColor(0.1f, 0.1f, 0.1f, 0); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
	//draw();
	for(int i =0;i<length/6;i++){
		renderTriangle(i);
	}
	glFlush();
}



void run(){
	//if(length>2000) return;
	float *newTriangle = new float[length*4];
	for (int i =0;i<length/6;i++){
		for(int j=0;j<3;j++){
			newTriangle[(i*4+j)*6 ]=triangles[i*6+j*2];
			newTriangle[(i*4+j)*6 +1]=triangles[i*6+j*2+1];
			for(int k=0;k<2;k++){
				newTriangle[((i*4 + j)*6) + (k + 1)*2] = (triangles[i*6 + (j)*2] + triangles[i*6 +((j+k+1)%3)*2])/2;
				newTriangle[((i*4 + j)*6) + (k + 1)*2 +1] = (triangles[i*6 + (j)*2 +1] + triangles[i*6 +((j+k+1)%3)*2 +1])/2;
			}
			newTriangle[(i*4+3)*6 +j*2]=(triangles[i*6 +j*2] +triangles[i*6+((j+1)%3)*2])/2;
			newTriangle[(i*4+3)*6 +j*2 +1]=(triangles[i*6 +j*2 +1] +triangles[i*6+((j+1)%3)*2 +1])/2;
		}
	}
	free(triangles);
	triangles=newTriangle;
	length*=4;
	for(int i=0;i<length/6;i++){
		renderTriangle(i);
	}
	glFlush();
	sleep(1);
}


int main(int argc, char** argv) {
	length = 1*3*2;
	triangles = new float[1*3*2];
	triangles[0]=-1;
	triangles[1]=-1;
	triangles[2]=0;
	triangles[3]=1;
	triangles[4]=1;
	triangles[5]=-1;
	srand (time(NULL));
	glutInit(&argc, argv);		// Initialize GLUT
    glutInitWindowSize(600,800);   // Set the window's initial width & height
    glutCreateWindow("Tetris"); // Create a window with the given title
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutDisplayFunc(disInit); // Register display callback handler for window re-paint
	glutIdleFunc(run);
	glutMainLoop();
	return 0;
}
