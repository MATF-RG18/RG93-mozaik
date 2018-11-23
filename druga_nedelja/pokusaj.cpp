#include<iostream>
#include<GL/glut.h>
using namespace std;

int kx=0,ky=0,kz=0;
int cn;
/*struktura cetvorouglova*/
struct Quads{
    int x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;
    float r,g,b;
    int state, total;
};
Quads Q[100];
void DodajQuads(){
    /*dodajemo novi cetvorougao*/
    Q[0].state++;
    if(Q[0].state>4) Q[0].state=1;
    int st=Q[0].state;
    if(st==1){
        Q[0].total++;
        cn=Q[0].total;
    }
    if(st==1){
        Q[cn].x1=kx;
        Q[cn].y1=ky;
        Q[cn].z1=kz;
    }
    
    if(st==1 || st==2){
        Q[cn].x2=kx;
        Q[cn].y2=ky;
        Q[cn].z2=kz;
    }
    
    if(st==1 || st==2 || st==3){
        Q[cn].x3=kx;
        Q[cn].y3=ky;
        Q[cn].z3=kz;
    }
    
    if(st==1 || st==2 || st==3 || st==4){
        Q[cn].x4=kx;
        Q[cn].y4=ky;
        Q[cn].z4=kz;
    }
}
void drawQuads(){
    /*funkcija koja crta cetvorouglove*/
    int i;
    for(i=1;i<Q[0].total+1;i++){
        glBegin(GL_QUADS);
        glColor3f(Q[i].r, Q[i].g, Q[i].b);
        glVertex3f(Q[i].x1, Q[i].y1, Q[i].z1);
        glVertex3f(Q[i].x2, Q[i].y2, Q[i].z2);
        glVertex3f(Q[i].x3, Q[i].y3, Q[i].z3);
        glVertex3f(Q[i].x4, Q[i].y4, Q[i].z4);
        glEnd();
    }
    
}
void Kockica(){
    /*crtamo kockicu*/
    glPushMatrix();
    glColor3f(0.3,0.3,0.6);
    glTranslatef(kx,ky,kz);
    glutSolidCube(0.4);
    glPopMatrix();
    
}

void init(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35,1.0f,2.3f,1100);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3, 0.2, 0.1, 0.5);
}
void pomeraj(unsigned char tast, int x,int y){
    /*Funkcija koja nadgleda sta je korisnik pritisnuo na tastaturi*/
    if(tast=='w'){
        if(kz-1<0)/*ne sme da izadje iz kvadrata*/
            kz=0;
        else kz--;
    }
    if(tast=='s')
        {
        if(kz+1>19/*ne sme da izadje iz kvadrata*/)
            kz=19;
        else kz++;
    }
    if(tast=='a'){
        if(kx-1<0)/*ne sme da izadje iz kvadrata*/
            kx=0;
        else kx--;
    }
    if(tast=='d'){
        if(kx+1>19)/*ne sme da izadje iz kvadrata*/
            kx=19;
        else kx++;
    }
    if(tast==32) DodajQuads();
    if(tast=='r') {Q[cn].r=1; Q[cn].g=0; Q[cn].b=0;}
    if(tast=='g') {Q[cn].r=0; Q[cn].g=1; Q[cn].b=0;}
    if(tast=='b') {Q[cn].r=0; Q[cn].g=0; Q[cn].b=1;}
    if(tast=='y') {Q[cn].r=1; Q[cn].g=1; Q[cn].b=0;}
    glutPostRedisplay();
}
void drawGrid(){
	int i;
	for(i=0;i<40;i++){
		glPushMatrix();
		if(i<20) glTranslatef(0, 0, i);
		if(i>=20){
			glTranslatef(i-20, 0, 0);
			glRotatef(-90, 0, 1, 0);
		}
		glColor3f(1,1,1);
		glBegin(GL_LINES);
		glLineWidth(1);
		glVertex3f(0,-0.1,0);
		glVertex3f(19, -0.1, 0);
		glEnd();
		glPopMatrix();
	}
}
void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glTranslatef(-13, 0, -45);
	glRotatef(40,1,1,0);
 	drawGrid();
        drawQuads();
        Kockica();
	glutSwapBuffers();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("pokusaj");
	glutDisplayFunc(display);
        glutKeyboardFunc(pomeraj);
        init();
	glutMainLoop();
	return 0;
}
