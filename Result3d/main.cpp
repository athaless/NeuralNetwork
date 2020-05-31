/*----------------------------------------------------------------------
  Trabalho RNA - Reconhecimento de padrões
  Disciplina Inteligência Artificial 2
  Prof....: Fabiana Lorenzi
  Alunos..: Alessander Pires Oliveira
/*---------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

#define XS	 800
#define YS	 600
#define TAM	 20    // Número de Neurônios...
#define TDIM 14    
#define TMAX 15000

int tempo=0;
int altura,largura;		// dimensões em pixels da janela

// Vetor(Matriz) de entrada 
/* 
double v[TDIM][TDIM]= { {0,0,0,0,0,0,0,0,1,1,1,0,0,0},
	                    {0,0,0,0,0,0,0,1,1,1,1,0,0,0},
	                    {0,0,0,0,0,0,1,1,1,1,1,0,0,0},
	                    {0,0,0,0,0,1,1,0,1,1,1,0,0,0},
	                    {0,0,0,0,1,1,0,0,1,1,1,0,0,0},
	                    {0,0,0,1,1,0,0,0,1,1,1,0,0,0},
	                    {0,0,1,1,0,0,0,0,1,1,1,0,0,0},
	                    {0,0,1,1,1,1,1,1,1,1,1,1,1,0},
	                    {0,0,1,1,1,1,1,1,1,1,1,1,1,0},
	                    {0,0,0,0,0,0,0,0,1,1,1,0,0,0},
	                    {0,0,0,0,0,0,0,0,1,1,1,0,0,0},
	                    {0,0,0,0,0,0,0,0,1,1,1,0,0,0},
	                    {0,0,0,0,0,0,0,0,1,1,1,0,0,0},
	                    {0,0,0,0,0,0,0,0,1,1,1,0,0,0} 
                      };
                      
*/
                      
double v[TDIM][TDIM]= { {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	                    {0,0,0,1,1,1,1,1,1,1,1,0,0,0},
	                    {0,0,1,1,1,1,1,1,1,1,1,1,0,0},
	                    {0,0,1,1,0,0,0,0,0,0,1,1,0,0},
	                    {0,0,1,1,0,0,0,0,0,0,1,1,0,0},
	                    {0,0,1,1,1,1,1,1,1,1,1,1,0,0},
	                    {0,0,1,1,1,1,1,1,1,1,1,1,0,0},
	                    {0,0,0,1,1,1,1,1,1,1,1,1,0,0},
	                    {0,0,0,0,0,0,0,0,0,0,1,1,0,0},
	                    {0,0,0,0,0,0,0,0,0,0,1,1,0,0},
	                    {0,0,0,0,0,0,0,0,0,0,1,1,0,0},
	                    {0,0,1,1,1,1,1,1,1,1,1,1,0,0},
	                    {0,0,0,1,1,1,1,1,1,1,1,0,0,0},
	                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0} 
                      };


/* Rede de neuronios */
double rede[TAM][TAM][TDIM][TDIM];

double cubo[8][3]= { {0,0,0},
	                 {4,0,0},
	                 {4,4,0},
	                 {0,4,0},
	                 {0,0,4},
	                 {4,0,4},
	                 {4,4,4},
	                 {0,4,4},
                   };

void InicPesos();
void Redesenha();
void ExibeRede(int i, int j);
void AtualizaPesos();
void cor(double);

/* Inicializa aleatoriamente os pesos na rede */
void InicPesos()
{
 int i,j,k,l;
 srand((long)time(0));
 for(i=0;i<TAM;++i)
	for(j=0;j<TAM;++j)
		for(k=0;k<TDIM;++k)
		    for(l=0;l<TDIM;++l)
			    rede[i][j][k][l]=((float)rand()/RAND_MAX)*4.0;
}

void cor(double z)
{
  int c;
  c = (int) (255.0 * (1.0-(z/4.0)));
  glColor3ub(128,c,0);
}

// Exibe rede no canto inferior direito
void ExibeRede(int i, int j)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();					// salva projeção atual
	glLoadIdentity();
	gluOrtho2D(0,1,0,1);			// cria mapeamento ortográfico 0..1
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Define viewport no canto inferior direito
	int tamx = largura/5;			// 1/5 do tamanho original
	int tamy = altura/5;
	glViewport(largura-tamx,0,tamx,tamy);

	float delta = 1.0/TDIM;			// incremento para x e y
	float xf,yf;					// coordenadas na tela
	// Desenhando quads
	glBegin(GL_QUADS);
	yf = 0.0;
	for(int y=0;y<TDIM;++y)
	{
		xf = 0.0;
		for(int x=0;x<TDIM;++x)
		{
			cor(rede[i][j][TDIM-y][x]);
			glVertex2f(xf,yf);
			glVertex2f(xf+delta,yf);
			glVertex2f(xf+delta,yf+delta);
			glVertex2f(xf,yf+delta);
			xf+=delta;
		}
		yf+=delta;
	}
	glEnd();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();					// restaura projeção atual
	glMatrixMode(GL_MODELVIEW);	
	// Finalmente, restaura viewport original
	glViewport(0,0,largura,altura);
}

void Redesenha()
{
	int i,j,k;
	float ang;
	float x,z;
	char str[256];

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,0,0);
	glLoadIdentity();
	glTranslatef(0,0,-6);
	glRotatef(tempo,0,1,0);
	glTranslatef(-2,-2,-2);
//	glTranslatef(4,4,4);
//	gluLookAt(x,2,z,2,2,2,0,1,0);

	glBegin(GL_LINE_LOOP);
	for(i=0;i<4;++i) glVertex3dv(cubo[i]);
	glEnd();

	glBegin(GL_LINE_LOOP);
	for(i=4;i<8;++i) glVertex3dv(cubo[i]);
	glEnd();

	for(i=0;i<4;++i)
	{
		glBegin(GL_LINES);
		glVertex3dv(cubo[i]);
		glVertex3dv(cubo[i+4]);
		glEnd();
	}

	glColor3f(0,1,0);
	//Desenha linhas da grade num sentido
    for(i=0;i<TAM;++i)
	{
		for(j=0;j<TAM-1;++j)
		{
			glBegin(GL_LINES);
			cor(rede[i][j][2][0]);
			glVertex3dv(rede[i][j][2]);
			cor(rede[i][j+1][2][0]);
			glVertex3dv(rede[i][j+1][2]);
			glEnd();
		}
	}
	//Desenha linhas da grade no sentido perpendicular ao anterior
	for(j=0;j<TAM;++j)
	{
		for(i=0;i<TAM-1;++i)
		{
			glBegin(GL_LINES);
			cor(rede[i][j][2][0]);
			glVertex3dv(rede[i][j][2]);
			cor(rede[i+1][j][2][0]);
			glVertex3dv(rede[i+1][j][2]);
			glEnd();
		}
	}
	//move2D(0,0);
	//sprintf(str,"Tempo: %d",tempo);
	//charstr(str);

	ExibeRede(0,0);
	glutSwapBuffers();
}

void AtualizaPesos()
{
	int i,j,k,l;
	int cx,cy;		      /* Coordenadas do c.a. na rede */
	double dist,mindist;  /* Distancia euclidiana dos vetores */
	double razao, epsilon, sigma, hrr;

	/* Primeiro, encontrar centro de ativacao */
	cx = cy = 0;
	mindist = 1E10;
	for(i=0;i<TAM;++i)
	 for(j=0;j<TAM;++j)
       for(k=0;k<TDIM;++k) 
		{
			dist = 0;
            for(l=0;l<TDIM;++l) dist = dist + pow(rede[i][j][k][l]-v[k][l],2.0);
			dist = sqrt(dist);
			if(dist < mindist)
			{
				mindist = dist;
				cx = j;
				cy = i;
			}
		}

	/* C.A. esta em rede[cy][cx]  Agora, percorre-se a rede, atualizando os pesos */
	razao = (double)((double)tempo/(double)TMAX);
	epsilon = 0.4 * pow((0.05/0.9),razao);
	sigma   = 4 * pow(0.2,razao);
	for(i=0;i<TAM;++i)
		for(j=0;j<TAM;++j)
		{
			dist = sqrt((cy-i)*(cy-i) + (cx-j)*(cx-j));
			hrr = exp(-(dist*dist)/(2.0*(sigma*sigma)));
			for(k=0;k<TDIM;++k) 
			  for(l=0;l<TDIM;++l) 
                 rede[i][j][k][l]+=(epsilon*hrr*(v[k][l]-rede[i][j][k][l]));
		}
}

// callback de teclado
void key(unsigned char k, int x, int y)
{
	switch(k)
	{
	 case 27:	exit(0);
	 break;
	}
	glutPostRedisplay();
}

// Redesenha continuamente a tela
void idle(int dummy)
{
	int face;
	face = rand()%6;
	//printf("face: %d - ",face); 
	for (int i=0;i<TDIM;i++) 
	  for (int j=0;i<TDIM;i++) 
        v[i][j]=((float)rand()/RAND_MAX)*4.0;

    switch(face)
	{
		case 0:	// frente 
			for (int i=0;i<TDIM;i++) v[2][i] = 0.0;
			break;
		case 1: // atras 
			for (int i=0;i<TDIM;i++) v[2][i] = 4.0;
			break;
		case 2: // direita 
			for (int i=0;i<TDIM;i++) v[0][i] = 4.0;
			break;
		case 3: // esquerda 
			for (int i=0;i<TDIM;i++) v[0][i] = 0.0;
			break;
		case 4: // cima 
			for (int i=0;i<TDIM;i++) v[1][i] = 4.0;
			break;
		case 5: // embaixo 
			for (int i=0;i<TDIM;i++) v[1][i] = 4.0;
			break;
	}

	//printf("%3.2f %3.2f %3.2f\n",v[0],v[1],v[2]); 
	tempo+=1;
	AtualizaPesos();
	glutTimerFunc(100,idle,0);
	glutPostRedisplay();
}


// callback de redimensionamento
void resize(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;

	largura = w;
	altura = h;

	// Especifica as dimensões da viewport
	glViewport(0, 0, w, h);
 
	// Calcula a correção de aspecto
	float aspecto = (GLfloat)w/(GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,aspecto,0,1000);
	glMatrixMode(GL_MODELVIEW);
}

main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(XS,YS); 
	glutCreateWindow("Simulador KFM - Mapeamento 3D");

	glutDisplayFunc(Redesenha);
	glutKeyboardFunc(key);
	glutReshapeFunc(resize);
	glutTimerFunc(100,idle,0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,1.0,0,1000);
	glMatrixMode(GL_MODELVIEW);

	InicPesos();

	glutMainLoop();
}
