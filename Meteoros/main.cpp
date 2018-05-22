/*
 *CHAVEZ HERNANDEZ SERGIO IVAN
 *SANCHEZ LOPEZ EDUARDOO GEOVANNI
 *AGOSTO-DICIEMBRE 2017
 */
#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include "ImageLoader.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <direct.h>
#define GetCurrentDirectory _getcwd
#include <string.h>

float girosphere=0;
int mousex=0,mousey=0;
int alto=480,ancho=640;
float rota_x=0,rota_y=0,rota_z=0;
float tras_x=0,tras_y=0,tras_z=0;
float cx=0,cy=0,cz=0;
float scala=1;
float avaZ=0;
int avances=0;
int avanMeteoros=0;
int contPasos=0;
int contPasosMet=0;
int centros [5] = {-4,-2,0,2,4};
int escalas [5] = {0,0,0,0,0};
int vidas = 5,retardo=250;
int cuantos=0;
int cuales[5];
int bajada =3;
int aumenVel=0;
//Colores
float colores[6][3]={{0.855,0.647,0.125},{0,0.502,0},{0.251,0.878,0.816},{0,0,0.502},{1,0,1},{1,0,0}};

GLdouble vfovy=0,vaspect=0,vnear=0,vfar=20;
GLdouble ojox=0, ojoy=1, ojoz=3;
GLdouble upx=0,upy=1,upz=0;
GLdouble centx=0,centy=0,centz=0;
float player[3] = {ojox,ojoy,ojoz};

pthread_t threadAvanzar, colisionCheck, threadGirar;

GLUquadricObj *sphere;
GLuint _text1;
GLuint _text2;
GLuint _text3;
GLuint _text4;
GLuint _text5;
GLuint _text6;

GLuint loadTexture(Image* image) {
	GLuint idtextura;
	glGenTextures(1, &idtextura);
	glBindTexture(GL_TEXTURE_2D, idtextura);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return idtextura;
}
int aleatorio(int a, int b) {
    static bool first = true;
    if(first){
        srand(time(NULL));
        first = false;
    }
    return a + rand() % (b - a + 1);
}
void cargarImagenes() {


    char cCurrentPath[FILENAME_MAX];
    if(!GetCurrentDirectory(cCurrentPath,sizeof(cCurrentPath))){

    }
    char heart[FILENAME_MAX];
    strcpy(heart,cCurrentPath);

	//Image* lado1 = loadBMP(strcat(heart,"/texturas/heart.bmp"));
	Image* lado1 = loadBMP("texturas/heart.bmp");

	_text1 = loadTexture(lado1);
	delete lado1;

	char hole[FILENAME_MAX];
    strcpy(hole,cCurrentPath);

    //Image* lado2 = loadBMP(strcat(hole,"/texturas/hole.bmp"));
    Image* lado2 = loadBMP("texturas/hole.bmp");

	_text2 = loadTexture(lado2);
	delete lado2;

	char two[FILENAME_MAX];
    strcpy(two,cCurrentPath);

	//Image* lado3 = loadBMP(strcat(two,"/texturas/2.bmp"));
	Image* lado3 = loadBMP("texturas/2.bmp");

	_text3 = loadTexture(lado3);
    delete lado3;

    char four[FILENAME_MAX];
    strcpy(four,cCurrentPath);

    //Image* lado4 = loadBMP(strcat(four,"/texturas/4.bmp"));
    Image* lado4 = loadBMP("texturas/asteroid.bmp");

	_text4 = loadTexture(lado4);
	delete lado4;

	char five[FILENAME_MAX];
    strcpy(five,cCurrentPath);

    //Image* lado5 = loadBMP(strcat(five,"/texturas/5.bmp"));
	Image* lado5 = loadBMP("texturas/star.bmp");

	_text5 = loadTexture(lado5);
	delete lado5;

	char six[FILENAME_MAX];
    strcpy(six,cCurrentPath);

    //Image* lado5 = loadBMP(strcat(five,"/texturas/5.bmp"));
	Image* lado6 = loadBMP("texturas/fondo2.bmp");

	_text6 = loadTexture(lado6);
	delete lado6;
}
void cargarTextura(GLuint _textura) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
}
void paredes()
{
float j=0;
for(int i=-5;i<26;i+=5,j+=0.1)
{
    cargarTextura(_text5);
    glBegin(GL_POLYGON);//Cara 1 Blanca
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-5.1,-1,avaZ-i);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(-5.1,0.5-j,avaZ-i);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(-5.1,0.5-j,avaZ-(i-5));
        glTexCoord2f(0.0f, 1.0f);glVertex3f(-5.1,-1,avaZ-(i-5));
    glEnd();
    cargarTextura(_text5);
    glBegin(GL_POLYGON);//Cara 1 Blanca
        glTexCoord2f(0.0f, 0.0f);glVertex3f(5.1,-1,avaZ-i);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(5.1,0.5-j,avaZ-i);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(5.1,0.5-j,avaZ-(i-5));
        glTexCoord2f(0.0f, 1.0f);glVertex3f(5.1,-1,avaZ-(i-5));
    glEnd();

}
glDisable(GL_TEXTURE_2D);
}
void dibujarPiso(){
    //Pared lateral izquierda
    //cargarTextura(_text3);
    glBegin(GL_POLYGON);//Cara 1 Blanca

        glVertex3f(-5,-1,avaZ+5);
        glVertex3f(5,-1,avaZ+5);
        glColor3f(colores[avances%6][0],colores[avances%6][1],colores[avances%6][2]);
        glVertex3f(5,-1,avaZ);
        glVertex3f(-5,-1,avaZ);
    glEnd();
    glBegin(GL_POLYGON);//Cara 1 Blanca

        glVertex3f(-5,-1,avaZ);
        glVertex3f(5,-1,avaZ);
        glColor3f(colores[(avances+1)%6][0],colores[(avances+1)%6][1],colores[(avances+1)%6][2]);
        glVertex3f(5,-1,avaZ-5);
        glVertex3f(-5,-1,avaZ-5);
    glEnd();
    glBegin(GL_POLYGON);//Cara 1 Blanca

        glVertex3f(-5,-1,avaZ-5);
        glVertex3f(5,-1,avaZ-5);
        glColor3f(colores[(avances+2)%6][0],colores[(avances+2)%6][1],colores[(avances+2)%6][2]);
        glVertex3f(5,-1,avaZ-10);
        glVertex3f(-5,-1,avaZ-10);
    glEnd();
    glBegin(GL_POLYGON);//Cara 1 Blanca

        glVertex3f(-5,-1,avaZ-10);
        glVertex3f(5,-1,avaZ-10);
        glColor3f(colores[(avances+3)%6][0],colores[(avances+3)%6][1],colores[(avances+3)%6][2]);
        glVertex3f(5,-1,avaZ-15);
        glVertex3f(-5,-1,avaZ-15);
    glEnd();
    glBegin(GL_POLYGON);//Cara 1 Blanca

        glVertex3f(-5,-1,avaZ-15);
        glVertex3f(5,-1,avaZ-15);
        glColor3f(colores[(avances+4)%6][0],colores[(avances+4)%6][1],colores[(avances+4)%6][2]);
        glVertex3f(5,-1,avaZ-20);
        glVertex3f(-5,-1,avaZ-20);
    glEnd();
    glBegin(GL_POLYGON);//Cara 1 Blanca

        glVertex3f(-5,-1,avaZ-20);
        glVertex3f(5,-1,avaZ-20);
        glColor3f(colores[(avances+5)%6][0],colores[(avances+5)%6][1],colores[(avances+5)%6][2]);
        glVertex3f(5,-1,avaZ-25);
        glVertex3f(-5,-1,avaZ-25);
    glEnd();
}

void dibujarVidas(){
      glPushMatrix();
      glTranslated(5,0.5,player[2]-5);
      glRotated(girosphere,0,1,0);

      for(int i=0;i<vidas;i++){

        cargarTextura(_text1);
        glBegin(GL_POLYGON);
             glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5,i,0);
             glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5,i,0);
             glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5,i+1,0);
             glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5,i+1,0);
        glEnd();
    }
    glPopMatrix();
}

void dibujaMeteoro(float x, float y, float z, int escalera){
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotated(girosphere,0,1,0);

    sphere = gluNewQuadric();
  //gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    cargarTextura(_text4);
    gluSphere(sphere, 0+(0.5*escalera), 20, 20);
    //gluDeleteQuadric(sphere);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
/* GLUT callback Handlers */

static void reshape(int w, int h) {
    const float ar = (float) w / (float) h;
    if(w!=ancho&&h!=alto){
        ancho=w;
        alto=h;
    }
	glViewport(0,0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//Inicializa la matriz de proyeccion
    //glOrtho(-10.0, 10.0, -10.0, 10.0, vnear, vfar); // WorkSpace
    glFrustum(-ar, ar, -1.0, 1.0,1,750.0);
	glMatrixMode(GL_MODELVIEW);
}

void fondo(){
cargarTextura(_text6);
    glBegin(GL_POLYGON);//Cara 1 Blanca
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-500,-500,-500+player[2]);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(500,-500,-500+player[2]);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(500,500,-500+player[2]);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(-500,500,-500+player[2]);
    glEnd();
cargarTextura(_text6);
    glBegin(GL_POLYGON);//Cara 1 Blanca
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-500,-500,500+player[2]);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(-500,-500,-500+player[2]);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(-500,500,-500+player[2]);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(-500,500,500+player[2]);
    glEnd();
    cargarTextura(_text6);
    glBegin(GL_POLYGON);//Cara 1 Blanca
        glTexCoord2f(0.0f, 0.0f);glVertex3f(500,-500,500+player[2]);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(500,-500,-500+player[2]);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(500,500,-500+player[2]);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(500,500,500+player[2]);
    glEnd();
    cargarTextura(_text6);
    glBegin(GL_POLYGON);//Cara 1 Blanca
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-500,500,500+player[2]);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(-500,500,-500+player[2]);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(500,500,-500+player[2]);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(500,500,500+player[2]);
    glEnd();
}
void bloques(){
    for(int i=0;i<cuantos;i++)
        dibujaMeteoro(centros[cuales[i]],bajada,(-8*(avanMeteoros+1))+4, escalas[cuales[i]]);
}

/* GLUT callback Handlers */

static void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(tras_x,tras_y,tras_z);
	glScalef(scala,scala,scala);
	glRotatef(rota_x,1.0,0.0,0.0);
	glRotatef(rota_y,0,1.0,0);
	glRotatef(rota_z,0,0,1.0);

    gluLookAt(ojox,ojoy,ojoz,centx,centy,centz,upx,upy,upz);
    glColor3f(0,0,0);
    dibujarPiso();
    glColor3f(1,1,1);
    dibujarVidas();
    bloques();
    fondo();
    paredes();

    glFlush();
    glutSwapBuffers();
}
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case 32: //espacio
        case 97://a
        case 119://w
        case 115://s
        case 101://e
        case 100://d
        case 110://r
        case 114:

            break;
        case 102:

            break;
        case 116:

            break;
        case 103:

            break;
        case 121:

            break;
        case 120:
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

void ArrowKey(int key, int x, int y) {
	switch(key){
	            case GLUT_KEY_RIGHT:
	                if(ojox<4)
                        {
                            player[0]+=2;
                            ojox+=2;
                            centx+=2;
                        }
	                break;
                case GLUT_KEY_LEFT:
                    if(ojox>-4)
                    {ojox-=2;
                    player[0]-=2;
                    centx-=2;
                    }
                    break;
                case GLUT_KEY_UP:

                    //printf("player: %f %f %f\n",player[0],player[1],player[2]);

                    break;
                case GLUT_KEY_DOWN:
                    /*ojoz++;
                    centz++;
                    avaZ++;*/
                    break;
                case GLUT_KEY_HOME:
                case GLUT_KEY_END:
                    break;
                case GLUT_KEY_PAGE_UP:
                        ojoy++;
                    break;
                case GLUT_KEY_PAGE_DOWN:
                        ojoy--;
                    break;
                break;

                }
                //printf("%f,%f,%f avances: %i color1: %i\n",centx,centy,centz,avances,avances%6);
	glutPostRedisplay();
}
void mirarmouse(int x, int y)
{
    int x1=ancho/3,x2=x1*2;
    int y1=alto/3,y2=y1*2;
    if (x<x1)
    {
        if(y<y1)
        {rota_y-=0.55;rota_x-=0.55;}
        else if (y<y2)
            rota_y-=0.55;
        else
        {
            rota_y-=0.55;rota_x+=0.55;
        }
    }
    else if(x<x2)
    {
        if(y<y1)
        rota_x-=0.55;
        else if (y>y2)
        rota_x+=0.55;
    }
    else if (x<ancho)
    {
        if(y<y1)
        {rota_y+=0.55;rota_x-=0.55;}
        else if (y<y2)
            rota_y+=0.55;
        else
        {
            rota_y+=0.55;rota_x+=0.55;
        }
    }

}

/* Program entry point */
void *girarEsferas(void *args)
{
    while(vidas>0)
    {Sleep(100);
        girosphere+=10;
        if(girosphere>360)girosphere=10;
    }

}
void *avanzarMario(void *args)
{
    while(vidas>0){
        Sleep(retardo);
                    retardo=retardo<60?retardo:retardo-1;
                    ojoz--;
                    player[2]--;
                    centz--;
                    bajada=bajada>0?bajada-1:bajada;
                    if(contPasos<4)
                        contPasos++;
                    else{
                        contPasos=0;
                        avaZ-=5;
                        avances++;
                    }
                    if(contPasosMet<7){
                        contPasosMet++;
                    }else{
                        bajada=3;
                        contPasosMet=0;
                        avanMeteoros++;
                        escalas[0]=0;
                        escalas[1]=0;
                        escalas[2]=0;
                        escalas[3]=0;
                        escalas[4]=0;
                        cuantos=aleatorio(1,4);
                        for(int i=0;i<cuantos;i++){
                            cuales[i]=aleatorio(0,4);
                            escalas[cuales[i]]++;
                        }
                      //  printf("Voy en la %f y meteoro en la %i\n",player[2],temp);
                    }
                    if(retardo>250)retardo-=50;
                    //printf("%d\n",retardo);
    }

}
void *revisaColision(void *args)
{
    while(vidas>0)
    {
        if(cuantos>0)
                    for(int i=0;i<cuantos;i++)
                            {//printf("Meteoro no: %d\t",cuales[i]);
                                //printf("\tx:%d",centros[cuales[i]]);
                                //printf("\ty:%d",1);
                                //printf("\tz:%d\n", ((-8)*(avanMeteoros+1)+4));
                                if(player[2]==((-8)*(avanMeteoros+1)+4))
                                    if(player[0]==centros[cuales[i]])
                                {   vidas--;
                                    printf("Moriste: vidas%d\n",vidas);
                                    retardo=600;
                                }
                            }
        Sleep(retardo);

    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(ancho,alto);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Proyecto Graficación");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(ArrowKey);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mirarmouse);
    cargarImagenes();
    init();
    glEnable(GL_DEPTH_TEST);

    pthread_create(&threadAvanzar,NULL,avanzarMario,NULL);
    pthread_create(&colisionCheck,NULL,revisaColision,NULL);
	pthread_create(&threadGirar,NULL,girarEsferas,NULL);
	pthread_detach(threadGirar);
	pthread_detach(threadAvanzar);
	pthread_detach(colisionCheck);
    glutMainLoop();

    return EXIT_SUCCESS;
}
