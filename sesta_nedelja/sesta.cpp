/*MOZAIK--- igra u kojoj korisnik pravi kvadratice kao seceno kamenje i stavlja ih na odredjeno mesto, moguce je pomeranje kamere*/
#include<iostream>
#include<cstdlib>
#include<GL/glut.h>
#include<GL/gl.h>
#include<math.h>
#include<vector>
#include"Image.h"
/*koristimo  slike u bmp formatu za odredjenu teskturu*/
#define FILENAME1 "po.bmp"// textura za grid
#define FILENAME0 "mozak.bmp"//textura za pozadinu pocetnog ekrana
using namespace std;
/*globalne promenljive*/
char boja; //boja odredjuje koja je boja izabrana i na osnovu nje biramo prelaze boja koji ce da se dese
int kx=0,ky=1,kz=0; //za koliko transliramo nasu kockicu u odnosu na ravan u kojoj se nalazi, takodje na osnovu njih se dodeljuju koordinate kvadrata
int ind=0;// da li je izabrana opcija 1 ili 2, u zavisnosti od opcije i nasa Kockica ce imati drugu poziciju
int cn;//koliko imamo cetvorouglova
static float matrix[16]; //matrica za teksture
/* Identifikatori tekstura. */
static GLuint names[2];

/* Konstanta pi. */
const static float pi = 3.141592653589793;

/* Dimenzije prozora */
static int window_width, window_height;


static float angle=0;//ugao rotacije
static float lx=0, lz=0, ly=0, r=30;//ugao za koji pomeramo nasu kameru pomeranjem strelica

/*promenljive za rotaciju misem*/
float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0;
float lastx, lasty;

/*struktura cetvorouglova*/
struct Quads{
    int x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;
    float boje[3];// niz koji cuva koju smo boju izabrali i koristi ih u glMaterialfv
    int state, total;
    char boja;// odredjujemo boju 
};
static void initialize();// fja koja namesta texturu preuzeta sa kursa
static void nadji_w();//na osnovu ugla rotacije preko strelica, odredjuje se kretanje kockica nagore
static void nadji_s();//na osnovu ugla rotacije preko strelica, odredjuje se kretanje kockica nadole
static void nadji_a();//na osnovu ugla rotacije preko strelica, odredjuje se kretanje kockica ulevo
static void nadji_d();//na osnovu ugla rotacije preko strelica, odredjuje se kretanje kockica udesno
static void povecaj_x();//fja povecava x koordinatu kockica
static void povecaj_y();//fja povecava y koordinatu kockica
static void povecaj_z();//fja povecava z koordinatu kockica
static void smanji_x();//fja smanjuje x koordinatu kockica
static void smanji_y();//fja smanjuje y koordinatu kockica
static void smanji_z();//fja smanjuje z koordinatu kockica
static void init_svetlo();//inicijalizacija svetla
void processSpecialKeys();//fja koja belezi sterlice
static void display2();
vector<Quads> Q;
/**funkcija koja pravi cetvorouglove*/
void DodajQuads(){
    /*dodajemo novi cetvorougao*/
    Q[0].state++;//brojimo koja je tacka quada na redu
    if(Q[0].state>4) Q[0].state=1;//ako je preslo 4 prekiddamo
    int st=Q[0].state;// pomocna promenljiva
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
    /* ukljucujemo svetlo jer je pre toga bilo iskljuceno*/
     glEnable(GL_LIGHTING);
    glNormal3f(0, 0, 1);//postaavljamo normalu na z-osu
   glEnable(GL_ALPHA);//dozvljavamo providnost
    glEnable(GL_BLEND);//ukljucujemo mogucnost blendovanja
    glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    
   
        for(i=1;i<Q[0].total+1;i++){
            /*svakom kvadraticu dodeljujemo odredjenu boju u zavisnosti sta smo izabrali*/
            GLfloat diffuse_coeffs[] = { Q[i].boje[0], Q[i].boje[1] , Q[i].boje[2], 1 };
                GLfloat ambient_coeffs[] =  { Q[i].boje[0], Q[i].boje[1] , Q[i].boje[2], 1 };
                glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
                glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                glBegin(GL_QUADS);
                glVertex3f(Q[i].x1, Q[i].y1, Q[i].z1);
                glVertex3f(Q[i].x2, Q[i].y2, Q[i].z2);
                glVertex3f(Q[i].x3, Q[i].y3, Q[i].z3);
                glVertex3f(Q[i].x4, Q[i].y4, Q[i].z4);
                glEnd();
            }
/* ovo drugo crtanje je dodato kako bi se prikazivala oba lica kvadrata, nije bilo moguce namestati preko glCullFace(GL_FRONT_AND_ACK), ovako napravljeno omoguceno je da ih vidimo kako god okrenuli kameru
oni kvadrati koji su nacrtani na nekom od vec napravljenih kvadrata videce im se samo back stana lica */
            //glEnable(GL_ALPHA_TEST);
            glDisable(GL_CULL_FACE);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
              
            for(i=1;i<Q[0].total+1;i++){
                //namestanje iste boje 
                GLfloat diffuse_coeffs[] = { Q[i].boje[0], Q[i].boje[1] , Q[i].boje[2], 1 };
                GLfloat ambient_coeffs[] =  { Q[i].boje[0], Q[i].boje[1] , Q[i].boje[2], 1 };
                glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
                glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                glBegin(GL_QUADS);
                glVertex3f(Q[i].x1, Q[i].y1, Q[i].z1);
                glVertex3f(Q[i].x2, Q[i].y2, Q[i].z2);
                glVertex3f(Q[i].x3, Q[i].y3, Q[i].z3);
                glVertex3f(Q[i].x4, Q[i].y4, Q[i].z4);
                glEnd();
            }
            glDisable(GL_CULL_FACE);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            
        glDisable(GL_BLEND);
        glDisable(GL_LIGHTING);  
    
}

/**funkcija koja brise cetvorouglove za prelaz sa jedne opcije na drugu*/
void deleteQuads(){
    Q.clear();
    Q.resize(100);
    
}
void Kockica(){
    /*crtamo kockicu*/
    glPushMatrix();
     glEnable(GL_LIGHTING);
     glNormal3f(0, 0, 1);
     /*postavaljmo njenu ambijatalnu , difuznu i spekularnu refleksiju materijala*/
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 0.7, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.2, 0.2, 1, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 1, 0, 0, 1 };

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 30;
   
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glShadeModel(GL_SMOOTH);//prelazi su glatki
    glTranslatef(kx,ky,kz);
    glutSolidCube(0.4);
    glRotatef(yrot,0.0,1.0,0.0);  
    glTranslated(-xpos,0.0f,-zpos);
     glDisable(GL_LIGHTING);
    glPopMatrix();
    
}
//pomeramo mis i racunamo za koliko treba da ga pomerimo, racunamo preko razlike prosle koordinate i trenutne koordinate
void mouseMovement(int x, int y) {
    int diffx=x-lastx; 
    int diffy=y-lasty; 
    lastx=x; 
    lasty=y; 
    xrot += (float) diffy;
    yrot += (float) diffx;
    //ako predje 360 stepeni vrati se
    if(xrot>=360)
        xrot-=360;
    if(yrot>=360)
        yrot-=360;
}
//fja koja belezi pritisak misa, ako je levo, pomerice se kamera nagore, odnosno figura ce da krene nadole
  void mouseButton(int button, int state, int x, int y) {
    
         if (button==GLUT_LEFT_BUTTON){
             if(state==GLUT_DOWN){
                 xrot += 1;
                if (xrot >360) xrot -= 360;//ako smo presli 360 vratice se
             }
         }else if(button==GLUT_RIGHT_BUTTON){
             if(state==GLUT_DOWN){
                xrot -= 1;
                if (xrot < -360) xrot += 360;//ako smo presli 360 vratice se
             }
         }
         glutPostRedisplay();
     
  }
void init(){
	//specijalna inicijalizacija scena1
     glClearColor(0.3, 0.2, 0.1, 0.5);
	
}
void init1(){
        //specijalna inicijalizacija scene2
        glClearColor(0.3, 0.3, 0.5, 0.5);	
}
/*crta se postolje za mozaik*/
void drawGrid(){
	int i;
    //crta se linija po linija, svaki kvadratic ili praznina je povrsine 1 samim tim kockica ce da se krece za 1
	for(i=0;i<60;i++){
		glPushMatrix();
		if(i<30) glTranslatef(0, 0, i); //se crtaju horizontalne linije
		if(i>=30){//crtaju se vertikalne linije
			glTranslatef(i-30, 0, 0);
			glRotatef(-90, 0, 1, 0);
		}
        //boja linije je bela, a debljina linije je 1
		glColor3f(1,1,1);
		glBegin(GL_LINES);
		glLineWidth(1);

		glVertex3f(0,-0.1,0);
		glVertex3f(29, -0.1, 0);
		glEnd();
        glRotatef(90, 0, 1, 0);
		glPopMatrix();
	}
    //crtamo texturu na osnovu velicine grid-a
            for(int j=0; j<10;j++){
                glPushMatrix();
                glTranslatef(0,0,j*3);//odredjujemo koja je kolona
                for(i=0;i<5;i++){
                    glPushMatrix();
                    glRotatef(90, 1,0 ,0);
                    glTranslatef(i*6,0,0);//pojedinacno ih pomeramo rotiramo kako bi bili na podlozi
            //preuzeto sa kursa
                    glBindTexture(GL_TEXTURE_2D, names[1]);
                    glBegin(GL_QUADS);
                    glNormal3f(0, 0, 1);

                    glTexCoord2f(0, 0);
                    glVertex3f(0, 0, 0);

                    glTexCoord2f(0.5, 0);
                    glVertex3f(6, 0, 0);

                    glTexCoord2f(0.5, 0.25);
                    glVertex3f(6, 3, 0);

                    glTexCoord2f(0, 0.25);
                    glVertex3f(0, 3, 0);
                    glEnd();
                    glPopMatrix();
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
                glPopMatrix();
            }

}
//postavljanje kamere na osnovu pokretanja misa, u sustini je globalna transformacija
void camera (void) {
    glRotatef(xrot,1.0,0.0,0.0);  
    glRotatef(yrot,0.0,1.0,0.0);  
    glTranslated(-xpos,-ypos,-zpos);
    glutPostRedisplay();
}
/*funkcija za prikaz opcije 2*/
void display1(){
      
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	init1();
    //iskljucujemo svetlo kako bi grid ostao beo u pozivima pojedinacnih fja ukljucuje se svetlo
    glDisable(GL_LIGHTING);
    glLoadIdentity();
    //pozicija kamere  u zavisnosti kako okrecemo sliku putem strelica
        gluLookAt(r*lx, 15,r*lz,
            0,-10,0,
            0,1,0
        );
        //cout<< angle<<endl;
        glMultMatrixf(matrix);
        camera();
        glPushMatrix();
        glRotatef(-90,1,0,0);
   
        glTranslatef(-29/2, -17, -29/2);
        drawGrid();
        
         glRotatef(-90,1,0,0);
         //donje postolje ima drugaciji cullface zato se iskljucuje u ovom koraku i ponovo ce se ukljuciti kad se ono iscrta
        glDisable(GL_CULL_FACE);
        drawGrid();
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
         glRotatef(-90,0,0,1);
         drawGrid();
         
         glRotatef(90,0,0,-1);
         glRotatef(180,0,-1,0);
        
         glTranslatef(0,0,-29);
      
        drawQuads();
         Kockica();
        glPopMatrix();
       
        
    glEnable(GL_LIGHTING);
        
	glutSwapBuffers();
        
        
}

void processSpecialKeys(int key, int xx, int yy) {
        
            switch (key) {
                    case GLUT_KEY_LEFT :
                        if(ind==0 || ind==1){
                            //pomeramo poziciju kamere
                            angle -= 0.05f;
                         if(angle<=-6)
                            angle=0;
                            lx = sin(angle);
                            lz =-cos(angle);
                        }else{
                            //transliramo celu scenu
                            float yrotrad;
                            yrotrad = (yrot / 180 * 3.141592654f);
                            xpos += float(cos(yrotrad)) * 0.2;
                            zpos += float(sin(yrotrad)) * 0.2;
                            
                        }
                           
                            break;
                    case GLUT_KEY_RIGHT :
                        if(ind==0 || ind==1){
                            //pomeramo poziciju kamere
                            angle += 0.05f;
                            if(angle>=6)
                                angle=0;
                                
                                lx = sin(angle);
                                lz =-cos(angle);
                        }
                        else{
                            
                            //transliramo celu scenu
                           float yrotrad;
                            yrotrad = (yrot / 180 * 3.141592654f);
                            xpos -= float(cos(yrotrad)) * 0.2;
                            zpos -= float(sin(yrotrad)) * 0.2;
                        }
                        break;
                    case GLUT_KEY_UP :
                        if(ind==0 || ind==1){
                            //zumiramo
                            if(r-0.5<=0)
                                r=0;
                            r -= 0.5;
                        }else{
                            //transliramo celu scenu sto treba da imitira zumiranje kamere   
                            float xrotrad, yrotrad;
                            yrotrad = (yrot / 180 * 3.141592654f);
                            xrotrad = (xrot / 180 * 3.141592654f);
                            xpos -= float(sin(yrotrad));
                            zpos += float(cos(yrotrad)) ;
                            ypos += float(sin(xrotrad));                           
                            
                        }
                          
                            break;
                    case GLUT_KEY_DOWN :
                        if(ind==0 || ind==1){
                            //zumiramo
                            if(r+0.5>60)
                                r=60;
                            r += 0.5;
                        }else{
                            //transliramo celu scenu sto treba da imitira zumiranje kamere   

                              float xrotrad, yrotrad;
                                yrotrad = (yrot / 180 * 3.141592654f);
                                xrotrad = (xrot / 180 * 3.141592654f);
                                xpos += float(sin(yrotrad)) ;
                                zpos -= float(cos(yrotrad)) ;
                                ypos -= float(sin(xrotrad)) ;
                          
                        }
                            
                        break;
            }
        
            
	glutPostRedisplay();
}

/*funkcija za prikaz opcije 1*/
void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	init();
	glLoadIdentity();
     glDisable(GL_LIGHTING);
        gluLookAt(r*lx,10,r*lz,
                  0,0,0,
                  0,1,0
            );
	glPushMatrix();
            glTranslatef(-29/2+2, 0, -29/2+2);
            drawGrid();
            drawQuads();
            Kockica();
        glPopMatrix();
       
         glEnable(GL_LIGHTING);
	glutSwapBuffers();
}
// fja za tastaturu i registruje koji su se tasteri pritisnuli
void pomeraj(unsigned char tast, int x,int y){
    /*Funkcija koja nadgleda sta je korisnik pritisnuo na tastaturi*/
    /*Kockica se krece gore*/
    if(tast=='w' || tast=='W'){
        
        if(ind==0){
           nadji_w();
        }else{
            nadji_w();
        }
    }
    /*Kockica se krece dole*/
    if(tast=='s' || tast=='S'){
        
        if(ind==0){
            nadji_s();
        }else{
           nadji_s();
        }
    }
    /*Kockica se krece ulevo*/
    if(tast=='a' || tast=='A'){
        
        if(ind==0){
            nadji_a();
        }else{
            nadji_a();
        }
        
    }
    /*Kockica se krece udesno*/
    if(tast=='d' || tast=='D'){
        
        if(ind==0){
            nadji_d();
        }else{
           nadji_d();
        }
        
    }
    /*samo opcija 2 ima ove tastere i Kockica se krece vertikalno nagore*/
    if((tast=='q' || tast=='Q') && ind==1){
        povecaj_y();
    }
    /*samo opcija 2 ima ove tastere i Kockica se krece vertikalno nadole*/
    if((tast=='e' || tast=='E') && ind==1){
        smanji_y();
    }
    
     /*scena 1*/
    if (tast=='1') {
        kx=0, ky=1, kz=0, ind=0;
        
        angle=0;
        lx=0, lz=0, ly=0, r=30;
        deleteQuads();
        glutDisplayFunc(display);
        
    }
     /*scena 2*/
    if (tast=='2') {
        kx=0, ky=0, kz=0, ind=1;
        angle=0;
        lx=0, lz=0, ly=0, r=30;
         deleteQuads();
        glutDisplayFunc(display1);
    }
    //scena 3, poslednja scena
    if(tast=='3'){
        kx=0, ky=0, kz=0, ind=2;
        angle=0;
        lx=0, lz=0, ly=0, r=30;
        glutDisplayFunc(display2);

    }
    if(tast==27)
        exit(0);
    if(tast==32) {
        DodajQuads();
    }
    //boje
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
//pocetni ekran koji ima pozadinu teksturu mozak.bmp
static void pocetni(void){
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Inicijalizuje se matrica transformacije. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 7, 0, 0, 0, 0, 1, 0);
    glClearColor(1,1,1,1);
    glDisable(GL_CULL_FACE);
    /* Primenjuje se matrica rotacije. */
    glPushMatrix();
    if(window_width==800)
        glTranslatef(-4,-3,0);
    else
        glTranslatef(-5.3,-3,0);
        
        //glRotatef(180,1,0,0);
    /* Crtaju se vrata kuce. */
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0);

        glTexCoord2f(1.5, 0);
        glVertex3f(window_width/100+5,0, 0.01);

        glTexCoord2f(1.5, 2.2);
        glVertex3f(window_width/100+5, window_height/100+10, 0.01);

        glTexCoord2f(0, 2.2);
        glVertex3f(0, window_height/100+10, 0.01);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
  
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glutSwapBuffers();
}
//fja za promenu velicine ekrana, pronadjeno na internetu
void changeSize(int w, int h) {

    window_width = w;
    window_height = h;
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glViewport(0, 0, w, h);
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}
int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Mozaik");
        
    glutKeyboardFunc(pomeraj);
    glutReshapeFunc(changeSize);
	glutSpecialFunc(processSpecialKeys);
    glutDisplayFunc(pocetni);
    init_svetlo();
    initialize();
    glutMouseFunc(mouseButton);
    glutPassiveMotionFunc(mouseMovement);
    glEnable(GL_NORMALIZE);
       
	glutMainLoop();
	return 0;
}
/*fje koje na osnovu promenljive angle, odredjuje kako ce se kockica setati po gridu, kockica ce pratiti kameru i na osnovu
ugla ce se kretati u pravcu x-ose ili u pravcu z-ose*/
void povecaj_z(){
        if(kz+1>29)/*ne sme da izadje iz kvadrata*/
            kz=29;
        else kz++;
}
void smanji_z(){
    if(kz-1<0)/*ne sme da izadje iz kvadrata*/
        kz=0;
    else kz--;
}
void povecaj_x(){
        if(kx+1>29)/*ne sme da izadje iz kvadrata*/
            kx=29;
        else kx++;
}
void smanji_x(){
    if(kx-1<0)/*ne sme da izadje iz kvadrata*/
        kx=0;
    else kx--;
}
void povecaj_y(){
        if(ky+1>29)/*ne sme da izadje iz kvadrata*/
            ky=29;
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
    else if((angle>=2.6 && angle<3.95) || (angle<-2.4 && angle>=-3.95)){
        smanji_z();
    }
    else if((angle>=3.95 && angle<4.7) ||(angle<-0.95 && angle>=-2.4)){
        povecaj_x();
    }
    else if((angle>=4.7 && angle<6.2) || (angle<-4.7 && angle>=-6.2)){
        povecaj_z();
    }
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
    else if((angle>=4.7 && angle<6.2) || (angle<-4.7 && angle>=-6.2))
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
    else if((angle>=4.7 && angle<6.2) || (angle<-4.7 && angle>=-6.2))
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
    else if((angle>=4.7 && angle<6.2) || (angle<-4.7 && angle>=-6.2))
        smanji_x();
}
//preuzet  sa casa kursa
static void initialize(void)
{
    /* Objekat koji predstavlja teskturu ucitanu iz fajla. */
    Image * image;

    /* Postavlja se boja pozadine. */
    glClearColor(1, 1, 1, 1);

    /* Ukljucuje se testiranje z-koordinate piksela. */
    glEnable(GL_DEPTH_TEST);

    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    image = image_init(0, 0);

    /* Kreira se prva tekstura. */
    image_read(image, FILENAME0);

    /* Generisu se identifikatori tekstura. */
    glGenTextures(2, names);

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Kreira se druga tekstura. */
    image_read(image, FILENAME1);

    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);

    /* Inicijalizujemo matricu rotacije. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}
//preuzeto sa casa kursa
void init_svetlo()
{

    /* Pozicija svetla (u pitanju je direkcionalno svetlo). */
    GLfloat light_position[] = { 0, 10, 0, 0 };

    /* Ambijentalna boja svetla. */
    GLfloat light_ambient[] = { 0.9, 0.9, 0.9, 1};

    /* Difuzna boja svetla. */
    GLfloat light_diffuse[] = { 0.9, 0.9, 0.9, 1};

    /* Spekularna boja svetla. */
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1};

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /* Ukljucuje se osvjetljenje i podesavaju parametri svetla */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  
    
}
static void init2(){
    glClearColor(0.4,0.5,0.6,1);
}
//*krajnja scena koja ima samo kretanje kamere i sluzi za prikaz onog sto je prikazano*/
void display2(){
      
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	init2();
    //iskljucujemo svetlo kako bi grid ostao beo u pozivima pojedinacnih fja ukljucuje se svetlo
    glDisable(GL_LIGHTING);
    glLoadIdentity();
        gluLookAt(-15, 15,-45,
            0,-10,0,
            0,1,0
        );
        glMultMatrixf(matrix);
        camera();
        glPushMatrix();
        glRotatef(-180,1,0,0);
	    glTranslatef(-29/2+2, -20, -29/2+2);
        
         //donje postolje ima drugaciji cullface zato se iskljucuje u ovom koraku i ponovo ce se ukljuciti kad se ono iscrta
        glDisable(GL_CULL_FACE);
        drawGrid();
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
         
      
        drawQuads();
        
        glPopMatrix();
        
    glEnable(GL_LIGHTING);
	glutSwapBuffers();     
}