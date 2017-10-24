#include <stdlib.h>
#include <cmath>
#include <GL/glut.h>
#include <iostream>

using namespace std;


//transformações geométricas
typedef struct transformacao_geometrica{
	float rx, ry, rz;
	float tx, ty, tz;
	float ex, ey, ez;
	float ang_rot;
}tr;

bool flagBoom = false;

float r_a = 0.5, g_a = 0.2, b_a = 0.13;


float trans = 1.0;

bool eixos = true,
     wire = true;

float rotX = 0.0, 
      rotY = 0.0, 
      rotX_ini, rotY_ini;

float obsX = 0.0, 
      obsY = 0.0, 
      obsZ = 100.0,
      obsX_ini, obsY_ini, obsZ_ini;
      
int x_ini, y_ini, bot;

GLfloat fAspect;

//Variaveis de desenho
float altura_cepo = 2.5, raio_cepo = 1.0;
float tam_marreta = 2.0;
float tam_cabo_marreta = tam_marreta * 4.5;
float diametro_cabo_marreta = tam_marreta / 2.5;
float dist_rot = 5.5;

//prego
float comp_cabeca = 0.3, raio_cabeca = 1.5, comp_corpo = 6.5, raio_corpo = 0.4;


//cilindro -> cepo
GLUquadricObj *cepo_madeira, *cabo_marreta;

//TADs de Transformação dos objetos
tr t_cepo;
tr t_cabo;
tr t_cabeca_prego;
tr t_corpo_prego;
tr t_marreta;
tr t_arvore;


bool flagMarretada = true;

float raio = 1.0, p1 = 360, p2 = 360;
int cont = 0;
bool flagExplosao = false;


//responsável por carregar parametros de desenhos
void inicializa_estruturas(void){

	cepo_madeira = gluNewQuadric();
	cabo_marreta = gluNewQuadric();

	t_marreta.ang_rot = -45.0; 
	t_marreta.rx = t_marreta.rz = 0.0;
	t_marreta.ry = 1.0;
	t_marreta.tz = 0.0;
	t_marreta.ex = t_marreta.ey = t_marreta.ez = 3.0;

	t_cepo.ang_rot = 0.0; 
	t_cepo.rx = t_cepo.ry = t_cepo.rz = 0.0;
	t_cepo.tz = 0.0;
	t_cepo.ex = t_cepo.ey = t_cepo.ez = 12.0;

	t_cabo.ang_rot = 0.0; 
	t_cabo.rx = t_cabo.ry = t_cabo.rz = 0.0;
	t_cabo.tz = 0.0;
	t_cabo.ex = t_cabo.ey = t_cabo.ez = 1.0;

	t_cabeca_prego.ang_rot = 0.0;
	t_cabeca_prego.rx = t_cabeca_prego.ry = t_cabeca_prego.rz = 0.0;
	t_cabeca_prego.tz = 0.0;
	t_cabeca_prego.ex = t_cabeca_prego.ey = t_cabeca_prego.ez = 1.0;

	t_corpo_prego.ang_rot = 0.0;
	t_corpo_prego.rx = t_corpo_prego.ry = t_corpo_prego.rz = 0.0;
	t_corpo_prego.tz = 0.0;
	t_corpo_prego.ex = t_corpo_prego.ey = t_corpo_prego.ez = 1.0;

	t_arvore.ang_rot = 15.0; 
	t_arvore.rx = t_arvore.ry = t_arvore.rz = 0.0;
	t_arvore.tz = 0.0;
	t_arvore.ex = t_arvore.ey = t_arvore.ez = 1.0;
}


//-----------------------------------------------------------------------
void desenhaEixos()
{
    glLineWidth( 2.0f );
    
    glBegin( GL_LINES );
    	glColor3f ( 1.0f, 0.0f, 0.0f );
    	glVertex3f( 0.0f, 0.0f, 0.0f );
    	glVertex3f( 2.0f, 0.0f, 0.0f );
    	
    	glColor3f ( 0.0f, 1.0f, 0.0f );
    	glVertex3f( 0.0f, 0.0f, 0.0f );
    	glVertex3f( 0.0f, 2.0f, 0.0f );
    	
    	glColor3f ( 0.0f, 0.0f, 1.0f );
    	glVertex3f( 0.0f, 0.0f, 0.0f );
    	glVertex3f( 0.0f, 0.0f, 2.0f );
   	glEnd();
}
//------------------------------------------------------------------------------

void desenha_circulo(float r){ //desenha centro na origem
	int NUM_LINHAS = 360;
	glBegin(GL_POLYGON);
		for(int i = 0; i < NUM_LINHAS; i++){
			float ang = i * M_PI / 180.0;
			glVertex3f(r * cos(ang), r * sin(ang), 0.0);
		}
	glEnd();
}

void desenha_cilindro(float raio, float altura, tr t){
	//desenha o cilindro com o raio especifico e o centro geometrico localizado em:
	//altura/2.0  na direção positiva do eixo Z
	glPushMatrix();
		glRotatef(t.ang_rot, t.rx, t.ry, t.rz);
    	glScalef(t.ex, t.ey, t.ez);
	    gluCylinder(cepo_madeira, raio, raio, altura, 32, 32);
	    //"tampa" inferior
	    desenha_circulo(raio);
	    //"Tampa" superior
	    glPushMatrix();
	    	glTranslatef(0.0, 0.0, altura + t.tz);
	    	desenha_circulo(raio);
	    glPopMatrix();
	glPopMatrix();
}


void desenha_marreta(tr t){

	glPushMatrix();
		//não acrescento a dist rot aqui pois para ter uma mecanica legal de batida no prego
		//o ponto de rotaçãod eve estar alinhado com a parte superior do prego
		glTranslatef(t.tx, t.ty, t.tz);
		glRotatef(t.ang_rot, t.rx, t.ry, t.rz);
		glTranslatef(0.0, 0.0, -dist_rot);
		glScalef(t.ex, t.ey, t.ez);
		//desenha o cabo de madeira da marreta
		glPushMatrix();
			glColor3f(0.5, 0.2, 0.13 );
	    	desenha_cilindro(diametro_cabo_marreta, tam_cabo_marreta, t_cabo);
		glPopMatrix();
    	//desenha parte de metal da marreta
	   	glPushMatrix();
	    	glColor3f(0.3, 0.3, 0.3);
	    	glTranslatef(0.0, 0.0, tam_cabo_marreta);
	    	glScalef(2.5, 1.0, 1.0);
	    	glutSolidCube(tam_marreta);
	    glPopMatrix();
	glPopMatrix();
}

void desenha_arvore(tr t){

	glPushMatrix();
		//não acrescento a dist rot aqui pois para ter uma mecanica legal de batida no prego
		//o ponto de rotaçãod eve estar alinhado com a parte superior do prego
		glTranslatef(t.tx, t.ty, t.tz);
		glRotatef(t.ang_rot, t.rx, t.ry, t.rz);
		glTranslatef(0.0, 0.0, -dist_rot);
		glScalef(t.ex, t.ey, t.ez);
		//tronco da arvore
		glPushMatrix();
			glColor3f(r_a, g_a, b_a);
	    	desenha_cilindro(10.0, 75.0, t_arvore);
		glPopMatrix();
    	//folhas
    	if(!flagBoom){
    		glPushMatrix();
		    	glColor3f(0.0, 1.0, 0.0);
		    	glTranslatef(0.0, 0.0, 75.0);
		    	glutSolidCube(50);
		    glPopMatrix();
    	}
	   	
	glPopMatrix();
}


void desenhaCenarioSolid(){
	glPushMatrix();
		glColor3f(0.0, 0.45, 0.0);
		glBegin(GL_POLYGON);
			glVertex3f(-1000.0, -1000.0, 0.0);
			glVertex3f(-1000.0, 1000.0, 0.0);
			glVertex3f(1000.0, 1000.0, 0.0);
			glVertex3f(1000.0, -1000.0, 0.0);
		glEnd();
	glPopMatrix();

	t_arvore.tx = -50.0;
	t_arvore.ty = 80.0;
	t_arvore.ang_rot = 0.0;
	t_arvore.ry = 1.0;
	
	if(flagBoom){
		r_a = g_a = b_a = 0.1;
	}
	desenha_arvore(t_arvore);
	

	t_marreta.tx = tam_cabo_marreta*t_marreta.ex - dist_rot;
	//a melhor mecanica de batida no prego possivel..
	t_marreta.tz = altura_cepo*t_cepo.ez + comp_corpo*t_corpo_prego.ez + tam_marreta*t_marreta.ex;

    desenha_marreta(t_marreta);


    //desenhar o cepo
	glPushMatrix();
		glColor3f(0.5, 0.2, 0.13 );
		desenha_cilindro(raio_cepo, altura_cepo, t_cepo);
	glPopMatrix();

	glPushMatrix(); 
		glColor3f(0.7, 0.7, 0.7);
		glTranslatef(0.0, 0.0, altura_cepo*t_cepo.ez);
		glPushMatrix();
			//corpo prego
			desenha_cilindro(raio_corpo, comp_corpo, t_cabeca_prego);
			glPushMatrix();
			//cabeca prego
			glTranslatef(0.0, 0.0, comp_corpo);
			desenha_cilindro(raio_cabeca, comp_cabeca, t_cabeca_prego);
		glPopMatrix();
		glPopMatrix();
	glPopMatrix();

}    
//------------------------------------------------------------------------------
void display(void){
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

    if(eixos){
    	desenhaEixos();
    }

    desenhaCenarioSolid();

    if(flagExplosao){
    	glColor4f( 1.0, 1.0, 0.0, trans);
	    glPushMatrix();
	        glTranslatef( 0.0, 0.0, altura_cepo*t_cepo.ez);
	        glutSolidSphere( raio, p1, p2);
	    glPopMatrix();
    }

	glutSwapBuffers();
}

//------------------------------------------------------------------------------
void projecao(void)
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	gluPerspective( 60, fAspect, 0.1, 1000 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	gluLookAt( obsX,obsY,obsZ, 0,0,0, 0,1,0 );
}

//------------------------------------------------------------------------------
void reshape( GLsizei w, GLsizei h )
{
	if ( h == 0 ) h = 1;
	glViewport(0, 0, w, h);
	fAspect = (GLfloat)w/(GLfloat)h;

	projecao();
}

bool flagLuz0 = false, flagLuz1 = false;

//------------------------------------------------------------------------------
void keyboard( unsigned char key, int x, int y )
{
    switch( key )
    {
        case 27: exit(0); break;

        case 'w':
        case 'W': 
        	//faazer a camera se deslocar para seu norte
        	break;
                    
        case 'a':
        case 'A': eixos = !eixos;
        	      break;
       	case '0':
       		if(flagLuz0){
       			glDisable(GL_LIGHT0);
       		}else{
       			glEnable(GL_LIGHT0);
       		}
       		flagLuz0 = !flagLuz0;
       		break;

       	case '1':
       		if(flagLuz1){
       			glDisable(GL_LIGHT1);
       		}else{
       			glEnable(GL_LIGHT1);
       		}
       		flagLuz1 = !flagLuz1;
       		break;

       	case 'r':
       		flagBoom = false;
       		r_a = 0.5;
       		g_a = 0.2;
       		b_a = 0.13;
       		break; 	      
       		//glColor3f(0.5, 0.2, 0.13 );
    }    
    glutPostRedisplay();
}

//------------------------------------------------------------------------------
void Inicializa (void){   
	glEnable(GL_DEPTH_TEST);
	GLfloat ambient[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat difusa[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat posicao[] = {0.0, 0.0, altura_cepo*t_cepo.ez, 0.0}; 
	GLfloat posicao2[] = {0.0, -25.0, altura_cepo*t_cepo.ez / 2.0, 0.0}; 
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
	glLightfv(GL_LIGHT0, GL_POSITION, posicao);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, difusa);
	glLightfv(GL_LIGHT1, GL_POSITION, posicao2);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
}
//-----------------------------------------------------------------------
#define SENS_ROT 5.0
#define SENS_OBS 20.0
#define SENS_TRANSL 30.0

float ex = 1.0, ey = 1.0, ez = 1.0;

void motion( int x, int y){
    if( bot == GLUT_RIGHT_BUTTON )
    {
        int deltax = x_ini - x;
        int deltay = y_ini - y;
       	rotY = rotY_ini - deltax/SENS_ROT;
        rotX = rotX_ini - deltay/SENS_ROT;
    } 
    else if( bot == GLUT_RIGHT_BUTTON){
        int deltaz = y_ini - y;
        obsZ = obsZ_ini + deltaz/SENS_OBS;
    }
    else if( bot == GLUT_MIDDLE_BUTTON){
        int deltay = y_ini - y;
        //divide por 1500 que e o fator de velocidade do zoom
        float escala = ((float) deltay)/1500.0;
        if(ex + escala > 0.5 && ex + escala < 1.5){
        	ex += escala;
        	ey += escala;
        	ez += escala;
        }
    }
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    glTranslatef( -obsX, -obsY, -obsZ );
    glRotatef( rotX, 1, 0, 0 );
    glRotatef( rotY, 0, 1, 0 );
    glScalef(ex, ey, ez);
    
    glutPostRedisplay();
}

//-----------------------------------------------------------------------

void desenha_explosao(int arg){
	if(arg == 1){
		raio += 8;
		if(raio > 150){
			if(trans - 0.1 > 0){
				trans -= 0.1;
				flagBoom = true;
			}

		}
		
		cont++;
		int arg_aux = 1;
		if(cont >= 30){
			flagExplosao = false;
			cont = 0;
			raio = 1.0;
			arg_aux = 0;
			trans = 1.0;
		}
		glutPostRedisplay();
		glutTimerFunc(50, desenha_explosao, arg_aux);
	}

}

void timer_batida(int arg){
	if(arg == 1){
		if(t_marreta.ang_rot > -82){
			t_marreta.ang_rot -= 10;
			glutPostRedisplay();
			glutTimerFunc(25, timer_batida, 1);
		}else{
			flagExplosao = true;
			glutTimerFunc(100, desenha_explosao, 1);
		}
	}else if(arg == 2){
		if(t_marreta.ang_rot < -45){
			t_marreta.ang_rot += 1;
			glutPostRedisplay();
			glutTimerFunc(25, timer_batida, 2);
		}
		
	}

}


//-----------------------------------------------------------------------
void mouse( int button, int state, int x, int y ){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		if(flagMarretada){
			glutTimerFunc(5, timer_batida, 1);	
		}else{
			glutTimerFunc(5, timer_batida, 2);	
		}
		flagMarretada = !flagMarretada;		
	}

    if( state == GLUT_DOWN ){
        x_ini = x;
        y_ini = y;
        
        obsX_ini = obsX;
        obsY_ini = obsY;
        obsZ_ini = obsZ;
        
        rotX_ini = rotX;
        rotY_ini = rotY;
        
        bot = button;
    } else {
        bot = -1;
    }
}


//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{

	inicializa_estruturas();
	cout << endl << "0 - Acende a luz0" << endl;
	cout << "1 - Acende a luz1" << endl;
	cout << "PROCURE UMA ARVORE, EM SEGUIDA UTILIZE O PODER DE GUGU GAITERO" << endl;
	cout << "DEPOIS DO ARMAGEDON, PRESS R PARA RECONSTRUIR A ARVORE" << endl;
	cout << endl << "VALEUS, FALOUS E NOIS!" << endl;
	
    glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB ); 
	glutInitWindowSize( 450, 450 ); 
	glutCreateWindow("DC Studios - Cepo de Madeira");

	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    //glutTimerFunc(250, timer_batida, 0);

	Inicializa();
 
	glutMainLoop();
	return 0;
}
//------------------------------------------------------------------------------


void load_audio(void){

}
