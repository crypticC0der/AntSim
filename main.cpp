#include <GL/gl.h>
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

float colors[7][3] = {{.1,.1,.1},{0,1,0} ,{0,0,1} ,{0,0,0} ,{1,0,0},{.9,.1,.6},{.95,.8,0}};
int dirs[8][2]={{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

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
//1 -> phermones leading home
//0 -> phermones leading to food
float* attractions[2];

float randFloat(){
	return rand() / (float)(RAND_MAX);
}

class Ant{
	public:
		const float independance=0.0001;
		uint position;
		char direction;
		bool gathering=true;
		float pheramone=1;
		char prevState;
	
		void init(uint start){
			position = start;
			prevState=state[position];
			direction = rand()%8;
		}

		void debug(){
			cout << "pos:"<<endl<<"\tx: " << position%LENGTH<<endl<<"\ty: " << position/LENGTH<<endl;
			cout << "direction: " << (int)direction<<endl;
			cout << "gathering: " << gathering<<endl;
			cout << "pheremone: " << pheramone<<endl;
			cout << "prevState: " << (int)prevState<<endl;
		}

		void Move(){
			//cout << 1<<endl;
			//debug();
			attractions[(int)(gathering)][position]+=pheramone/5;
			pheramone-=1/(float)100;
			if(pheramone<0){
				pheramone=0;
				gathering=false;
			}
			state[position]=prevState;
			position+=dirs[direction][0]*LENGTH + dirs[direction][1];
			position+=LENGTH*HEIGHT;
			position%=LENGTH*HEIGHT;
			prevState=state[position];
			//cout << 2<<endl;
			if(position<0||position>=HEIGHT*LENGTH){
				position-=dirs[direction][0]*LENGTH + dirs[direction][1];
				direction+=4;
			}
			if(state[position]==3){
				position-=dirs[direction][0]*LENGTH + dirs[direction][1];
				direction+=4;
			}
			if((gathering&&state[position]==2)||(!gathering&&state[position]==1)){
				gathering=!gathering;
				pheramone=state[position];
			}
			//state[position]=4;
			//handle direction changing			
			float total=0;
			for(int i=-1;i<=1;i++){
				int p = position + dirs[(i+direction)%8][0]*LENGTH + dirs[(i+direction)%8][1];	
				if(p>=0 && p<LENGTH*HEIGHT){
					total+=attractions[(int)(!gathering)][p]+.01;
				}
			}
			float randVal=randFloat()*total;
			for(int i=-1;i<=1;i++){
				int p = position + dirs[(i+direction)%8][0]*LENGTH + dirs[(i+direction)%8][1];	
				if(p>=0 && p<LENGTH*HEIGHT){
					randVal-=attractions[(int)(!gathering)][p]+0.01;
				}
				if(randVal<=0){
					direction+=i;
					break;
				}
			}
			direction+=8;
			direction %=8;
		}

};

Ant* ants;
const int antcount=100;

void draw(){
	char pState=state[0];
	float yd = ((float)(2))/HEIGHT;
	float xd = ((float)(2))/LENGTH;
	float* gradcol=colors[5];
	float* oGradcol=colors[6];
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<LENGTH;j++){
			float scalar = attractions[0][i*LENGTH+j]/20;
			float otherScalar = attractions[1][i*LENGTH+j]/20; 
			if (scalar+otherScalar>1){
				scalar/=scalar+otherScalar;
				otherScalar/=scalar+otherScalar;
			}
			float nscalar=1-(scalar+otherScalar);
			glColor3f((0.1*nscalar) + scalar*gradcol[0] + otherScalar*oGradcol[0],
					(0.1*nscalar) + scalar*gradcol[1]+ otherScalar*oGradcol[1],
					(0.1*nscalar) + scalar*gradcol[2]+ otherScalar*oGradcol[2]);
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

	for (int i =0;i<antcount;i++){
		float x = ants[i].position%LENGTH;
		float y = ants[i].position/LENGTH;
		x*=2;
		x/=LENGTH;
		x-=1;
		y*=2;
		y/=LENGTH;
		y-=1;
		glColor3f(colors[4][0],colors[4][1],colors[4][1]);
		glBegin(GL_QUADS);
		glVertex2d(x,y);
		glVertex2d(x,y+yd);
		glVertex2d(x+xd,y+yd);
		glVertex2d(x+xd,y);
		glEnd();
	}

}


void disInit(){
	glClearColor(0.1f, 0.1f, 0.1f, 0); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
	draw();
	glFlush();
}

void run(){
	for (int i =0;i<antcount;i++){
		ants[i].Move();
	}
	for(int j=0;j<2;j++){
		for(int i=0;i<HEIGHT*LENGTH;i++){
			if(attractions[j][i]>0){
				attractions[j][i]-=0.0001;
			}
		}
	}
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
	float* fAttr= new float[HEIGHT*LENGTH];
	float* hAttr= new float[HEIGHT*LENGTH];
	attractions[0]=hAttr;
	attractions[1]=fAttr;
	ants = new Ant[antcount];
	float fr[] = {3,7};
	uint fp[] = {0,7431};
	initializeMap(5,HEIGHT*LENGTH/2 + LENGTH/2,fr,fp,2);
	for (int i=0;i<antcount;i++){
		ants[i].init(HEIGHT*LENGTH/2 + LENGTH/2);
	}
	
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
