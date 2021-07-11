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

typedef unsigned int uint;

float colors[6][3] = {{.1,.1,.1},{0,1,0} ,{0,0,1} ,{1,0,0} ,{0,0,0},{.9,.1,.6}};

const uint HEIGHT=100;
const uint LENGTH=100;

/*	states meanings:
 *	0: empty free area;
 *	1: nest;
 *	2: food;
 *	3: object;
 *	4: ant;
*/
char* state;
float* attraction;
uint* antPos; 

void draw(){
	char pState=state[0];
	float yd = ((float)(2))/HEIGHT;
	float xd = ((float)(2))/LENGTH;
	float* gradcol=colors[5];
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<LENGTH;j++){
			float scalar = attraction[i*LENGTH+j]/5;
			if(scalar>1){scalar=1;}
			float nscalar=1-scalar;
			glColor3f((0.1*nscalar) + scalar*gradcol[0],
					(0.1*nscalar) + scalar*gradcol[1],
					(0.1*nscalar) + scalar*gradcol[2]);
			glBegin(GL_QUADS);
			float x = (float)(j*2 );
			x/=LENGTH;
			x-=1;
			float y = (float)(i*2);
			y/=HEIGHT;
			y-=1;
			glVertex2d(x,y);
			glVertex2d(x,y+yd);
			glVertex2d(x+xd,y+yd);
			glVertex2d(x+xd,y);
			glEnd();
		}
	}

	glColor3f(colors[state[0]][0],colors[state[0]][1],colors[state[0]][2]);
	glBegin(GL_QUADS);
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<LENGTH;j++){
			if(state[i*LENGTH+j]==0){
				continue;
			}
			if(state[i*LENGTH+j]!=pState){
				glEnd();
				float* color = colors[state[i*LENGTH+j]];
				glColor3f(color[0],color[1],color[2]);
				glBegin(GL_QUADS);
			}
			pState=state[i*LENGTH+j];
			float x = (float)(j*2 );
			x/=LENGTH;
			x-=1;
			float y = (float)(i*2);
			y/=HEIGHT;
			y-=1;
			glVertex2d(x,y);
			glVertex2d(x,y+yd);
			glVertex2d(x+xd,y+yd);
			glVertex2d(x+xd,y);
		}
	}
	glEnd();

}


void disInit(){
	glClearColor(0.1f, 0.1f, 0.1f, 0); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
	draw();
	glFlush();
}

void run(){
	attraction[1720]+=0.001f;
	draw();
	glFlush();
}

void fillState(int x,int y, int a, int b,int v){
	if(x+a<LENGTH && y+b<HEIGHT){state[(y+b)*LENGTH + (x+a)]=v;}
	if(x+a<LENGTH && y-b>=0){state[(y-b)*LENGTH + (x+a)]=v;}
	if(x-a>=0 && y-b>=0){state[(y-b)*LENGTH + (x-a)]=v;}
	if(x-a>=0 && y+b<HEIGHT){state[(y+b)*LENGTH + (x-a)]=v;}
}

void genCircle(float r, uint p, char s){

	int rInt = (int)(r);
	int x = p%LENGTH;
	int y = p/LENGTH;
	int rSq = (int)(r*r);
	for (int i =0;i<rInt;i++){
		for(int j=0;j<=i && (i*i +j*j)<rSq;j++){
			fillState(x,y,i,j,s);	
			fillState(x,y,j,i,s);	
		}
	}
}

void initializeMap(float nestRad, uint nestPos, float* foodRads,uint* foodPos, uint len){
	genCircle(nestRad,nestPos,1);
	for(int i =0;i<len;i++){
		genCircle(foodRads[i],foodPos[i],2);
	}
}

int main(int argc, char** argv) {

	state = new char[HEIGHT*LENGTH];
	attraction = new float[HEIGHT*LENGTH];
	antPos = new uint[HEIGHT*LENGTH];
	float fr[] = {3,7};
	uint fp[] = {0,7431};
	initializeMap(5,HEIGHT*LENGTH/2 + LENGTH/2,fr,fp,2);
	
	srand (time(NULL));
	glutInit(&argc, argv);		// Initialize GLUT
    glutInitWindowSize(800,800);   // Set the window's initial width & height
    glutCreateWindow("Lines"); // Create a window with the given title
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutDisplayFunc(disInit); // Register display callback handler for window re-paint
	glutIdleFunc(run);
	glutMainLoop();
	return 0;
}
