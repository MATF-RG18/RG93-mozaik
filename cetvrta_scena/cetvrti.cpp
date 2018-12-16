#include<iostream>
#include<GL/glut.h>
#include<math.h>
using namespace std;
/*globalne promenljive*/
char boja; //boja odredjuje koja je boja izabrana i na osnovu nje biramo prelaze boja koji ce da se dese
int kx=0,ky=1,kz=0; //za koliko transliramo nasu kockicu u odnosu na ravan u kojoj se nalazi
int ind=0, pomera_se=0;// da li je izabrana opcija 1 ili 2, u zavisnosti od opcije i nasa Kockica ce imati drugu poziciju
int cn, p1=0, p2=0;//koliko imamo cetvorouglova

static float angle=0, fi=0;
static float lx=0, lz=0, ly=0, r=30;
static float x=0, y=0, z=0, xOrigin=0;

/*struktura cetvorouglova*/
static void nadji_w();
static void nadji_s();
static void nadji_a();
static void nadji_d();

void processSpecialKeys();
struct Quads{
    int x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;
    float boje[3];
    int state, total;
    char boja;
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
        glColor3f(Q[i].boje[0], Q[i].boje[1], Q[i].boje[2]);
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
void releaseKey(int key, int x, int y) { 	
//menjanje
        switch (key) {
             case 'o':
              pomera_se = 0;break;
        }
}
// void mouseMove(int x, int y) { 	
//     
//     if(pomera_se){
//        if(xOrigin>=0){ 
//             fi-=0.05f;
//         
//             lx = lx+sin(angle)*sin(fi);
//             ly=ly+cos(fi);
//             lz =lz-cos(angle)*sin(fi);
//        }else{
//             fi+=0.05f;
//             if(fi>=6.2)
//                  fi=0;
//             lx =lx+ sin(angle)*sin(fi);
//             ly=ly+cos(fi);
//             lz = lz-cos(angle)*sin(fi);
//         }
//     }
//           
// }
 
 void mouseButton(int button, int state, int x, int y) {
    
        if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
                    xOrigin++;
        }else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
                    xOrigin--;
                    //y++;
            }
     
    
 }
/*inicijalizacija prve scene i podesavanje pogleda na nju*/
void init(){
	//specijalna inicijalizacija scena
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
        gluLookAt(x+r*lx, 10-xOrigin,z+r*lz,
            0,-5+fi,0,
            0,1,0
        );
        glPushMatrix();
        glRotatef(-90,1,0,0);
        //draw_axes();
	glTranslatef(-19/2, -10, -19/2);
//   	glRotatef(30,1,1,0);
        drawGrid();
        
//         glScalef(1,-1,-1);
         glRotatef(-90,1,0,0);
       //  glTranslatef(0,0,0);
         drawGrid();
         glRotatef(-90,0,0,1);
         drawGrid();
         
         glRotatef(90,0,0,-1);
         glRotatef(180,0,-1,0);
         //draw_axes();
         glTranslatef(0,0,-19);
       //draw_axes();
        drawQuads();
        Kockica();
        glPopMatrix();
        //draw_axes();
        
        
	glutSwapBuffers();
}

void processSpecialKeys(int key, int xx, int yy) {
        
            switch (key) {
                    case GLUT_KEY_LEFT :
                            angle -= 0.05f;
                         if(angle<=-6)
                            angle=0;
//                          if(ind==0){
                            lx = sin(angle);
                            lz =-cos(angle);
                           
//                          }else{
//                              fi-=0.05f;
//                              
//                                 if(fi<=-1.2)
//                                     fi=0;
//                             lx = sin(angle)*cos(fi);
//                             ly=sin(fi);
//                             lz = -cos(angle)*cos(fi);
//                              
//                         }
                            break;
                    case GLUT_KEY_RIGHT :
                            angle += 0.05f;
                            if(angle>=6)
                                angle=0;
//                             if(ind==0){
                                
                                lx = sin(angle);
                                lz =-cos(angle);
                                
//                             }else{
//                                 fi+=0.05f;
//                                 if(fi>=1.2)
//                                     fi=0;
//                                 lx = sin(angle)*cos(fi);
//                                  ly=sin(fi);
//                                 lz = -cos(angle)*cos(fi);
//                             }
                            break;
                    case GLUT_KEY_UP :
                            //x += lx * fraction;
                            //z += lz * fraction;
                            if(r-0.5<=0)
                                r=0;
                            r -= 0.5;
                            fi=cos(r)*sin(0.5);
                          
                            break;
                    case GLUT_KEY_DOWN :
                            //x -= lx * fraction;
                            //z -= lz * fraction;
                            if(r+0.5>60)
                                r=60;
                            r += 0.5;
                            
                            fi=-cos(r)*sin(0.5);
                            
                        break;
            }
        
            
	glutPostRedisplay();
}
void draw_axes() {
    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(5,0,0);
        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,10,0);
        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,10);
    glEnd();
}
/*funkcija za prikaz opcije 1*/
void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	init();
	glLoadIdentity();
        gluLookAt(r*lx,10,r*lz,
                  0,0,0,
                  0,1,0
            );
	glPushMatrix();
            glTranslatef(-19/2+2, 0, -19/2+2);
//             glRotatef(40,1,1,0);
            drawGrid();
            drawQuads();
            Kockica();
        glPopMatrix();
        //draw_axes();
	glutSwapBuffers();
}
void pomeraj(unsigned char tast, int x,int y){
    /*Funkcija koja nadgleda sta je korisnik pritisnuo na tastaturi*/
    /*Kockica se krece gore*/
    if(tast=='w'){
        if(ind==0){
           nadji_w();
        }else{
            nadji_w();
        }
    }
    /*Kockica se krece dole*/
    if(tast=='s'){
        if(ind==0){
            nadji_s();
        }else{
           nadji_s();
        }
    }
    /*Kockica se krece ulevo*/
    if(tast=='a'){
        if(ind==0){
            nadji_a();
        }else{
            nadji_a();
        }
        
    }
    /*Kockica se krece udesno*/
    if(tast=='d'){
        if(ind==0){
            nadji_d();
        }else{
           nadji_d();
        }
        
    }
    /*samo opcija 2 ima ove tastere i Kockica se krece vertikalno nagore*/
    if(tast=='q' && ind==1){
        if (ky+1>19)
            ky=19;
        else
            ky++;
    }
    /*samo opcija 2 ima ove tastere i Kockica se krece vertikalno nadole*/
    if(tast=='e' && ind==1){
        if(ky!=0)
            ky--;
    }
     /*scena 1*/
    if (tast=='1') {
        kx=0, ky=1, kz=0, ind=0;
        
        angle=0, fi=0;
        lx=0, lz=0, ly=0, r=30;
        x=0, y=0, z=0, xOrigin=0;
        deleteQuads();
        glutDisplayFunc(display);
        
    }
     /*scena 2*/
    if (tast=='2') {
        kx=0, ky=0, kz=0, ind=1;
        angle=0, fi=0;
        lx=0, lz=0, ly=0, r=30;
        x=0, y=0, z=0, xOrigin=0;
         deleteQuads();
        glutDisplayFunc(display1);
    }
    if(tast==27)
        exit(0);
    if(tast==32) {
         
        DodajQuads();
    }
    if(tast=='r') {Q[cn].boje[0]=1; Q[cn].boje[1]=0; Q[cn].boje[2]=0; Q[cn].boja='c';}
    if(tast==45) {
        if(Q[cn].boja=='c')
            Q[cn].boje[0]-=0.05;
        else if(Q[cn].boja=='z')
            Q[cn].boje[1]-=0.05;
        else if(Q[cn].boja=='p')
            Q[cn].boje[2]-=0.05;
        else if(Q[cn].boja=='y'){
            Q[cn].boje[1]-=0.05;
             Q[cn].boje[0]-=0.05;
        }else{
            for(int i=0;i<3;i++)
                Q[cn].boje[i]-=0.05;
        }
    }
    if(tast==43) {
        if(Q[cn].boja=='c')
            Q[cn].boje[0]+=0.02;
        else if(Q[cn].boja=='z')
            Q[cn].boje[1]+=0.02;
        else if(Q[cn].boja=='p')
            Q[cn].boje[2]+=0.02;
        else if(Q[cn].boja=='y'){
            Q[cn].boje[1]+=0.02;
             Q[cn].boje[0]+=0.02;
        }else{
            for(int i=0;i<3;i++)
                Q[cn].boje[i]+=0.02;
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
    if(tast=='g') {Q[cn].boje[0]=0; Q[cn].boje[1]=1; Q[cn].boje[2]=0;Q[cn].boja='z';}
    if(tast=='b') {Q[cn].boje[0]=0; Q[cn].boje[1]=0; Q[cn].boje[2]=1;Q[cn].boja='p';}
    if(tast=='y') {Q[cn].boje[0]=1; Q[cn].boje[1]=1; Q[cn].boje[2]=0;Q[cn].boja='y';}
    /*tasteri za menjanje boja kvadrata, svako slovo dodaje ili oduzima nijansu neke od boja*/
    if(tast=='x'){
        int element=0;
        switch(Q[cn].boja){
            case 'c':
                element=1;
                break;
            case 'z':
                element=2;
                break;
            case 'p':
                element=3;
                break;
            case 'y':
                element=0;
                break;
        }
        if(element==3){
            Q[cn].boje[1]-=0.05;
            Q[cn].boje[0]-=0.05;
        }else
            Q[cn].boje[element]-=0.05;
            
    }
    if(tast=='m'){
         int element=0;
        switch(Q[cn].boja){
            case 'c':
                element=1;
                break;
            case 'z':
                element=2;
                break;
            case 'p':
                element=3;
                break;
            case 'y':
                element=0;
                break;
        }
        if(element==3){
            Q[cn].boje[1]+=0.02;
            Q[cn].boje[0]+=0.02;
        }else
            Q[cn].boje[element]+=0.02;
            
            
    }
    
    if(tast=='c'){
        int element=0;
        switch(Q[cn].boja){
            case 'c':
                element=2;
                break;
            case 'z':
                element=3;
                break;
            case 'b':
                element=0;
                break;
            case 'y':
                element=1;
                break;
        }
        if(element==3){
            Q[cn].boje[1]-=0.05;
            Q[cn].boje[0]-=0.05;
        }else
            Q[cn].boje[element]-=0.05;
          
    }
    if(tast=='n'){
        int element=0;
        switch(Q[cn].boja){
            case 'c':
                element=2;
                break;
            case 'z':
                element=3;
                break;
            case 'b':
                element=0;
                break;
            case 'y':
                element=1;
                break;
        }
        if(element==3){
            Q[cn].boje[1]+=0.02;
            Q[cn].boje[0]+=0.02;
        }else
            Q[cn].boje[element]+=0.02;
          
    }
    
    if(tast=='v'){
        int element=0;
        switch(Q[cn].boja){
            case 'c':
                element=3;
                break;
            case 'z':
                element=0;
                break;
            case 'p':
               element=1;
                break;
            case 'y':
                element=2;
                break;
        }
         if(element==3){
            Q[cn].boje[1]-=0.05;
            Q[cn].boje[0]-=0.05;
        }else
            Q[cn].boje[element]-=0.05;
            
    }
    if(tast=='t'){
       int element=0;
        switch(Q[cn].boja){
            case 'c':
                element=3;
                break;
            case 'z':
                element=0;
                break;
            case 'p':
               element=1;
                break;
            case 'y':
                element=2;
                break;
        }
         if(element==3){
            Q[cn].boje[1]+=0.02;
            Q[cn].boje[0]+=0.02;
        }else
            Q[cn].boje[element]+=0.02;
          
            
    }
  
    glutPostRedisplay();
}
static void pocetni(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.6,0.6,0.6,0);
    glutSwapBuffers();
}
void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

        // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(810, 610);
	glutCreateWindow("pokusaj");
        
        glutKeyboardFunc(pomeraj);
        glutReshapeFunc(changeSize);
	glutSpecialFunc(processSpecialKeys);
        glutDisplayFunc(pocetni);
        glutMouseFunc(mouseButton);
        glutSpecialUpFunc(releaseKey);
	glutMainLoop();
	return 0;
}
void povecaj_z(){
        if(kz+1>19)/*ne sme da izadje iz kvadrata*/
            kz=19;
        else kz++;
}
void smanji_z(){
    if(kz-1<0)/*ne sme da izadje iz kvadrata*/
        kz=0;
    else kz--;
}
void povecaj_x(){
        if(kx+1>19)/*ne sme da izadje iz kvadrata*/
            kx=19;
        else kx++;
}
void smanji_x(){
    if(kx-1<0)/*ne sme da izadje iz kvadrata*/
        kx=0;
    else kx--;
}
void povecaj_y(){
        if(ky+1>19)/*ne sme da izadje iz kvadrata*/
            ky=19;
        else ky++;
}
void smanji_y(){
    if(ky-1<0)/*ne sme da izadje iz kvadrata*/
        ky=0;
    else ky--;
}
void nadji_w(){
    if((angle>=0 && angle<0.95) || (angle<0 && angle>=-0.95)){
        povecaj_z();
    }
    else if((angle>=0.95 && angle<2.6) || (angle<-3.95 && angle>=-4.7)){
        smanji_x();
    }
    else if((angle>=2.6 && angle<3.95) || (angle<-2.4 && angle>=-3.95))
        smanji_z();
    else if((angle>=3.95 && angle<4.7) ||(angle<-0.95 && angle>=-2.4))
        povecaj_x();
    else if((angle>=4.7 && angle<6) || (angle<-4.7 && angle>=-6))
        povecaj_z();
}
void nadji_s(){
    if((angle>=0 && angle<0.95) || (angle<0 && angle>=-0.95))
        smanji_z();
    else if((angle>=0.95 && angle<2.6) || (angle<-3.95 && angle>=-4.7))
        povecaj_x();
    else if((angle>=2.6 && angle<3.95) || (angle<-2.4 && angle>=-3.95))
        povecaj_z();
    else if((angle>=3.95 && angle<4.7) ||(angle<-0.95 && angle>=-2.4))
        smanji_x();
    else if((angle>=4.7 && angle<6) || (angle<-4.7 && angle>=-6))
        smanji_z();
}
void nadji_a(){
    if((angle>=0 && angle<0.95) || (angle<0 && angle>=-0.95))
        povecaj_x();
    else if((angle>=0.95 && angle<2.6) || (angle<-3.95 && angle>=-4.7))
        povecaj_z();
    else if((angle>=2.6 && angle<3.95) || (angle<-2.4 && angle>=-3.95))
        smanji_x();
    else if((angle>=3.95 && angle<4.7) ||(angle<-0.95 && angle>=-2.4))
        smanji_z();
    else if((angle>=4.7 && angle<6) || (angle<-4.7 && angle>=-6))
        povecaj_x();
}
void nadji_d(){
    if((angle>=0 && angle<0.95) || (angle<0 && angle>=-0.95))
        smanji_x();
    else if((angle>=0.95 && angle<2.6) || (angle<-3.95 && angle>=-4.7))
        smanji_z();
    else if((angle>=2.6 && angle<3.95) || (angle<-2.4 && angle>=-3.95))
        povecaj_x();
    else if((angle>=3.95 && angle<4.7) ||(angle<-0.95 && angle>=-2.4))
        povecaj_z();
    else if((angle>=4.7 && angle<6) || (angle<-4.7 && angle>=-6))
        smanji_x();
}
