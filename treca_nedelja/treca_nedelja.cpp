#include<iostream>
#include<GL/glut.h>
using namespace std;
/*globalne promenljive*/
char boja; //boja odredjuje koja je boja izabrana i na osnovu nje biramo prelaze boja koji ce da se dese
int kx=0,ky=1,kz=0; //za koliko transliramo nasu kockicu u odnosu na ravan u kojoj se nalazi
int ind=0;// da li je izabrana opcija 1 ili 2, u zavisnosti od opcije i nasa Kockica ce imati drugu poziciju
int cn;//koliko imamo cetvorouglova
/*struktura cetvorouglova*/
struct Quads{
    int x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;
    float r,g,b;
    int state, total;
};
Quads Q[100];
/**funkcija koja pravi cetvorouglove*/
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
/**funkcija koja crta cetvorouglove*/
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

/**funkcija koja brise cetvorouglove za prelaz sa jedne opcije na drugu*/
void deleteQuads(){
    for(int i=0;i<Q[0].total+1;i++){
        glColor3f(1, 1, 1);
        Q[i].x1=0, Q[i].y1=0, Q[i].z1=0;
        Q[i].x2=0, Q[i].y2=0, Q[i].z2=0;
        Q[i].x3=0, Q[i].y3=0, Q[i].z3=0;
        Q[i].x4=0, Q[i].y4=0, Q[i].z4=0;
    }
    cn=0;
    Q[0].total=0;
    Q[0].state=0;
    
}
void Kockica(){
    /*crtamo kockicu*/
    glPushMatrix();
    glColor3f(0.3,0.3,0.6);
    glTranslatef(kx,ky,kz);
    glutSolidCube(0.4);
    glPopMatrix();
    
}
/*inicijalizacija prve scene i podesavanje pogleda na nju*/
void init(){
        glClearColor(0.3, 0.2, 0.1, 0.5);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(32,1.2f,2.1f,1000);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	
}
/*inicijalizacija druge scene i podesavanje pogleda na nju*/
void init1(){
        glClearColor(0.5, 0.6, 0.3, 0.5);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(42,1.5f,1.0f,1000);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	
}
/*crta se postolje za mozaik*/
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
/*funkcija za prikaz opcije 2*/
void display1(){
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	init1();
	glLoadIdentity();
	glTranslatef(-13, -4, -45);
	glRotatef(30,1,1,0);
        drawGrid();
        
        glScalef(1,-1,-1);
        glRotatef(90,1,0,0);
//         glTranslatef(0,0,1);
        drawGrid();
        
        glTranslatef(19,0,0);
        glRotatef(90,0,0,-1);
        drawGrid();
        drawQuads();
        Kockica();
	glutSwapBuffers();
}
/*funkcija za prikaz opcije 1*/
void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	init();
	glLoadIdentity();
	glTranslatef(-13, 0, -45);
	glRotatef(40,1,1,0);
        drawGrid();
        drawQuads();
        Kockica();
	glutSwapBuffers();
}
void pomeraj(unsigned char tast, int x,int y){
    /*Funkcija koja nadgleda sta je korisnik pritisnuo na tastaturi*/
    /*Kockica se krece gore*/
    if(tast=='w'){
        if(ind==0){
            if(kz-1<0)/*ne sme da izadje iz kvadrata*/
                kz=0;
            else kz--;
        }else{
            if(kx-1<0)/*ne sme da izadje iz kvadrata*/
                kx=0;
            else kx--;
        }
    }
    /*Kockica se krece dole*/
    if(tast=='s'){
        if(ind==0){
            if(kz+1>19/*ne sme da izadje iz kvadrata*/)
                kz=19;
            else kz++;
        }else{
            if(kx+1>19)/*ne sme da izadje iz kvadrata*/
                kx=19;
            else kx++;
        }
    }
    /*Kockica se krece ulevo*/
    if(tast=='a'){
        if(ind==0){
            if(kx-1<0)/*ne sme da izadje iz kvadrata*/
                kx=0;
            else kx--;
        }else{
            if(ky-1<-19)
                ky=-19;
            else
                ky--;
        }
        
    }
    /*Kockica se krece udesno*/
    if(tast=='d'){
        if(ind==0){
            if(kx+1>19)/*ne sme da izadje iz kvadrata*/
                kx=19;
            else kx++;
        }else{
           if(ky<0)
                ky++;
        }
        
    }
    /*samo opcija 2 ima ove tastere i Kockica se krece vertikalno nagore*/
    if(tast=='q' && ind==1){
        if (kz+1>19)
            kz=19;
        else
            kz++;
    }
    /*samo opcija 2 ima ove tastere i Kockica se krece vertikalno nadole*/
    if(tast=='e' && ind==1){
        if(kz!=0)
            kz--;
    }
     /*scena 1*/
    if (tast=='1') {
        kx=0, ky=1, kz=0, ind=0;
        deleteQuads();
        glutDisplayFunc(display);
        
    }
     /*scena 2*/
    if (tast=='2') {
         kx=0, ky=0, kz=0, ind=1;
         deleteQuads();
        glutDisplayFunc(display1);
    }
    if(tast==32) {
         
        DodajQuads();
    }
    if(tast=='r') {Q[cn].r=1; Q[cn].g=0; Q[cn].b=0; boja='c';}
    if(tast==45) {
        if(Q[cn].r!=0 && Q[cn].g==0 && Q[cn].b==0)
            Q[cn].r-=0.05;
        else if(Q[cn].r==0 && Q[cn].g!=0 && Q[cn].b==0)
            Q[cn].g-=0.05;
        else if(Q[cn].r==0 && Q[cn].g==0 && Q[cn].b!=0)
            Q[cn].b-=0.05;
        else if(Q[cn].r!=0 && Q[cn].g!=0 && Q[cn].b==0){
            Q[cn].g-=0.05;
             Q[cn].r-=0.05;
        }else{
            Q[cn].g-=0.05;
             Q[cn].r-=0.05;
             Q[cn].b-=0.05;
        }
    }
    if(tast==43) {
        if(Q[cn].r!=0 && Q[cn].g==0 && Q[cn].b==0)
            Q[cn].r+=0.02;
        else if(Q[cn].r==0 && Q[cn].g!=0 && Q[cn].b==0)
            Q[cn].g+=0.02;
        else if(Q[cn].r==0 && Q[cn].g==0 && Q[cn].b!=0)
            Q[cn].b+=0.02;
        else if(Q[cn].r!=0 && Q[cn].g!=0 && Q[cn].b==0){
            Q[cn].g+=0.02;
             Q[cn].r+=0.02;
        }else{
            
            Q[cn].g+=0.02;
             Q[cn].r+=0.02;
             Q[cn].b+=0.02;
        }
        
    }
    /*vazi samo za opciju 1 i spusta kvadrat na polje*/
   if (tast=='o'){
//         x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4
        if(Q[cn].y1-1==0){
            Q[cn].y1--;
            Q[cn].y2--;
            Q[cn].y3--;
            Q[cn].y4--;
        }
    }
    if(tast=='g') {Q[cn].r=0; Q[cn].g=1; Q[cn].b=0;boja='z';}
    if(tast=='b') {Q[cn].r=0; Q[cn].g=0; Q[cn].b=1;boja='p';}
    if(tast=='y') {Q[cn].r=1; Q[cn].g=1; Q[cn].b=0;boja='y';}
    /*tasteri za menjanje boja kvadrata, svako slovo dodaje ili oduzima nijansu neke od boja*/
    if(tast=='x'){
        switch(boja){
            case 'c':
                Q[cn].g-=0.05;
                break;
            case 'z':
                Q[cn].b-=0.05;
                break;
            case 'p':
                Q[cn].g-=0.05;
                Q[cn].r-=0.05;
                break;
            case 'y':
                Q[cn].r-=0.05;
                break;
        }
            
    }
    if(tast=='m'){
    
        switch(boja){
            case 'c':
                Q[cn].g+=0.02;
                break;
            case 'z':
                Q[cn].b+=0.02;
                break;
            case 'p':
                Q[cn].g+=0.02;
                Q[cn].r+=0.02;
                break;
            case 'y':
                Q[cn].r+=0.02;
                break;
        }
            
    }
    
    if(tast=='c'){
        switch(boja){
            case 'c':
                Q[cn].b-=0.05;
                break;
            case 'z':
                Q[cn].g-=0.05;
                Q[cn].r-=0.05;
                break;
            case 'p':
                Q[cn].r-=0.05;
                break;
            case 'y':
                Q[cn].g-=0.05;
                break;
        }
            
    }
    if(tast=='n'){
        switch(boja){
            case 'c':
                Q[cn].b+=0.02;
                break;
            case 'z':
                Q[cn].g+=0.02;
                Q[cn].r+=0.02;
                break;
            case 'b':
                Q[cn].r+=0.02;
                break;
            case 'y':
                Q[cn].g+=0.02;
                break;
        }
            
    }
    
    if(tast=='v'){
        switch(boja){
            case 'c':
                Q[cn].g-=0.05;
                Q[cn].r-=0.05;
                break;
            case 'z':
                Q[cn].r-=0.05;
                break;
            case 'p':
                Q[cn].g-=0.05;
                break;
            case 'y':
                Q[cn].b-=0.05;
                break;
        }
            
    }
    if(tast=='t'){
        switch(boja){
            case 'c':
                Q[cn].g+=0.02;
                Q[cn].r+=0.02;
                break;
            case 'z':
                Q[cn].r+=0.02;
                break;
            case 'p':
                Q[cn].g+=0.02;
                break;
            case 'y':
                Q[cn].b+=0.02;
                break;
        }
            
    }

    glutPostRedisplay();
}
static void pocetni(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.6,0.6,0.6,0);
    glutSwapBuffers();
}
int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(810, 610);
	glutCreateWindow("pokusaj");
        glutKeyboardFunc(pomeraj);
        glutDisplayFunc(pocetni);
	glutMainLoop();
	return 0;
}