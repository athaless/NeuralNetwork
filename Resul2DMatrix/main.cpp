/*----------------------------------------------------------------------
  Trabalho RNA - Reconhecimento de padrões
  Disciplina Inteligência Artificial 2
  Prof....: Fabiana Lorenzi
  Alunos..: Alessander Pires Oliveira
            Rodrigo de Azevedo e Souza Victor
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
#define TMAX 15000 //é o que determina o valor de razão (distância EUCLIDIANA)
                   //que é tempo atual / TMAX
#define TDELAY 800 
                   

int tempo=0;
int altura,largura;	// dimensões em pixels da janela
int cx,cy;		    // Coordenadas do c.a. na rede 

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

void InicPesos();
void Redesenha();
void AtualizaPesos();
void ExibeNeuronios(float x, float y, int i, int j);
void DesenhaTexto(char *string, int x, int y);
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
  float c = 1.0-(z/4.0);
  float r = 1-c;
  float g = c;
  float b = 0;
  glColor3f(r,g,b);
}

// Exibe nodo da rede em uma posição específica da tela (0..1)
void ExibeNeuronios(float x, float y, int i, int j)
{
	glPushMatrix();      // salva a transformação anterior (se houver), glPopMatrix restaura no final
	glTranslatef(x,y,0); // serve pra posicionar o canto inferior esquerdo no local especificado

	float delta = 1.0/TAM/TDIM;			// incremento para x e y
	float xf,yf;					// coordenadas na tela
	
    // Desenhando quads
    // desenho dos elementos de cada Neurônio...
	glBegin(GL_QUADS);
	yf = 0.0;
	for(int y=1;y<TDIM;++y)
	{
		xf = 0.0;
		for(int x=1;x<TDIM;++x)
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
	
	//Tenta marcar o Neurônio vencedor
	//com um retângulo branco em volta
	if(j==cx && i==cy)
	{
		glColor3f(1,1,1);
		glLineWidth(4);
		glBegin(GL_LINE_LOOP);
		glVertex2f(0,0);
		glVertex2f(delta*TDIM,0);
		glVertex2f(delta*TDIM,delta*TDIM);
		glVertex2f(0,delta*TDIM);
		glEnd();
	}
	glPopMatrix();
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

	glColor3f(0,1,0);
	float xf,yf;
	float delta = 1.0/TAM;
	yf = 0.0;
	for(int i=0;i<TAM;++i)
	{
		xf = 0.0;
		for(int j=0;j<TAM;++j)
		{
			ExibeNeuronios(xf,yf,i,j);
			xf+=delta;
		}
		yf+=delta;
	}
	glutSwapBuffers();
}

// Desenha um texto na janela GLUT
void DesenhaTexto(char *string, int x, int y)
{
    glPushMatrix();
    // Posição no universo onde o texto será colocado
    glRasterPos2f(x,y);
    // Exibe caracter a caracter
    while(*string)
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*string++);
    glPopMatrix();
}

void AtualizaPesos()
{
	int i,j,k,l;
	double dist,mindist;  /* Distancia EUCLIDIANA dos vetores */
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

	//printf("%3.2f %3.2f\n",v[i][j]); 

	tempo+=1;

	//printf("%3.2f \n",(float)tempo); 

	AtualizaPesos();
	glutTimerFunc(TDELAY,idle,0);
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
 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,1,0,1);			// cria mapeamento ortográfico 0..1
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
	glutTimerFunc(TDELAY,idle,0);

	InicPesos();

	glutMainLoop();
}
