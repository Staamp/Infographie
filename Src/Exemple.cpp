/* Fonctionnement de GLUt                       */
/*                                              */
/* Auteur: Nicolas JANEY                        */
/* nicolas.janey@univ-fcomte.fr                 */
/* Janvier 2019                                 */

#include <stdlib.h>
#include <stdio.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159
#endif


static float rx = 0.0F;
static float ry = 0.0F;
static float rz = 0.0F;
static int nb = 100;
static int nP = 0;

static float bleu[4] = { 0.0F,0.0F,1.0F,1.0F };
static float blanc[4] = { 1.0F,1.0F,1.0F,1.0F };
static float brun[4] = { 0.59F,0.34F,0.09F,1.0F };


/* Variables globales                           */

static int wTx = 480;      // Resolution horizontale de la fenetre
static int wTy = 480;      // Resolution verticale de la fenetre
static int wPx = 50;       // Position horizontale de la fenetre
static int wPy = 50;       // Position verticale de la fenetre

static int mx;
static float r1 = 0.0F;
static int mouseActive = 0;
static int isLine = 0;		// Affichage fil de fer

						   /* Fonction d'initialisation des parametres     */
						   /* OpenGL ne changeant pas au cours de la vie   */
						   /* du programme                                 */


void mySolidCube(double c) {
	c /= 2.0;
	glBegin(GL_QUADS);
	glNormal3f(0.0F, 0.0F, 1.0F);
	glVertex3d(c, c, c); // 1 
	glVertex3d(-c, c, c); // 2 
	glVertex3d(-c, -c, c); // 3 
	glVertex3d(c, -c, c); // 4 
	glNormal3f(0.0F, 1.0F, 0.0F);
	glVertex3d(c, c, c); // 1 
	glVertex3d(c, c, -c); // 5 
	glVertex3d(-c, c, -c); // 6 
	glVertex3d(-c, c, c); // 2 
	glNormal3f(1.0F, 0.0F, 0.0F);
	glVertex3d(c, c, c); // 1 
	glVertex3d(c, -c, c); // 4 
	glVertex3d(c, -c, -c); // 7 
	glVertex3d(c, c, -c); // 5 
	glNormal3f(0.0F, -1.0F, 0.0F);
	glVertex3d(c, -c, c); // 4 
	glVertex3d(-c, -c, c); // 3 
	glVertex3d(-c, -c, -c); // 8 
	glVertex3d(c, -c, -c); // 7 
	glNormal3f(-1.0F, 0.0F, 0.0F);
	glVertex3d(-c, -c, c); // 3 
	glVertex3d(-c, c, c); // 2 
	glVertex3d(-c, c, -c); // 6 
	glVertex3d(-c, -c, -c); // 8 
	glNormal3f(0.0F, 0.0F, -1.0F);
	glVertex3d(-c, c, -c); // 6 
	glVertex3d(c, c, -c); // 5 
	glVertex3d(c, -c, -c); // 7 
	glVertex3d(-c, -c, -c); // 8 
	glEnd();
}

void brasRobot(float r1, float r2) { 
	glPushMatrix(); 
	glRotatef(r1, 0.0F, 1.0F, 0.0F); 
	glTranslatef(1.5F, 0.0F, 0.0F); 
	glScalef(3.0F, 1.0F, 1.0F); 
	glPushMatrix(); 
	glScalef(3.0F, 1.0F, 1.0F); 
	mySolidCube(1.0); 
	glPopMatrix(); 
	glTranslatef(1.5F, 0.0F, 0.0F); 
	glRotatef(r2, 0.0F, 1.0F, 0.0F); 
	glTranslatef(1.5F, 0.0F, 0.0F); 
	glPushMatrix(); 
	glScalef(3.0F, 0.8F, 0.8F); 
	mySolidCube(1.0); 
	glPopMatrix(); 
	glPopMatrix(); 
}

static void mySolidCylindre(double hauteur, double rayon, int ns) {
	/* Protection contre la modification de la normale */
	/* et du flag normalisation                        */
	GLboolean nm = glIsEnabled(GL_NORMALIZE);
	if (!nm)
		glEnable(GL_NORMALIZE);
	float normale[4];
	glGetFloatv(GL_CURRENT_NORMAL, normale);
	/* Modelisation geometrique */
	glPushMatrix();
	hauteur /= 2.0F;
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= ns; i++) {
		float a = (2 * 3.14*i) / ns;
		float cs = cos(a);
		float sn = -sin(a);
		glNormal3f(cs, 0.0F, sn);
		float x = rayon*cs;
		float z = rayon*sn;
		glVertex3f(x, hauteur, z);
		glVertex3f(x, -hauteur, z);
	}
	glEnd();
	glPopMatrix();
	/* Restoration de la normale et du flag normalisation */
	glNormal3f(normale[0], normale[1], normale[2]);
	if (!nm)
		glDisable(GL_NORMALIZE);
}

/* Fonction qui modélise un rectangle 
 * h : hauteur
 * l : largeur
 * lo : longueur
 */
static void myRectangle(double h, double l, double lo) {
	glBegin(GL_QUADS);
		glNormal3f(0.0F, 0.0F, 1.0F);
		glVertex3d(l, h, lo); // 1 
		glVertex3d(-l, h, lo); // 2 
		glVertex3d(-l, -h, lo); // 3 
		glVertex3d(l, -h, lo); //4 

		glNormal3f(0.0F, h, 0.0F);
		glVertex3d(l, h, lo); // 1 
		glVertex3d(l, h, -lo); // 5 
		glVertex3d(-l, h, -lo); // 6 
		glVertex3d(-l, h, lo); // 2 

		glNormal3f(1.0F, 0.0F, 0.0F);
		glVertex3d(l, h, lo); // 1 
		glVertex3d(l, -h, lo); // 4 
		glVertex3d(l, -h, -lo); // 7 
		glVertex3d(l, h, -lo); // 5 

		glNormal3f(0.0F, -1.0, 0.0F);
		glVertex3d(l, -h, lo);  // 4 
		glVertex3d(l, -h, lo); // 3 
		glVertex3d(l, -h, -lo); // 8 
		glVertex3d(l, -h, -lo); // 7 

		glNormal3f(-1.0F, 0.0F, 0.0F);
		glVertex3d(-l, -h, lo); // 3 
		glVertex3d(-l, h, lo); // 2 
		glVertex3d(-l, h, -lo); // 6 
		glVertex3d(-l, -h, -lo); // 8 

		glNormal3f(0.0F, 0.0F, -1.0F);
		glVertex3d(-l, h, -lo); // 6 
		glVertex3d(l, h, -lo); // 5 
		glVertex3d(l, -h, -lo); // 7 
		glVertex3d(-l, -h, -lo); // 8 
	glEnd();

}

static void myCylindre(float r, float h, int n) {
	float* cs = (float*)calloc(n, sizeof(float));
	float* sn = (float*)calloc(n, sizeof(float));
	int i;
	for (i = 0; i < n; i++) {
		float a = (2 * M_PI * i) / n;
		sn[i] = sin(a);
		cs[i] = cos(a);
	}
	glPushMatrix();
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= n; i++) {
		float x = r * cs[i % n];
		float z = -r * sn[i % n];
		glNormal3f(cs[i % n], 0.0F, -sn[i % n]);
		glVertex3f(x, h / 2, z);
		glVertex3f(x, -h / 2, z);
	}
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0F, 1.0F, 0.0F);
	for (i = 0; i < n; i++) {
		float si = -sn[i];
		float ci = cs[i];
		glVertex3f(r * ci, h / 2, r * si);
	}
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0F, -1.0F, 0.0F);
	for (i = 0; i < n; i++) {
		float si = sn[i];
		float ci = cs[i];
		glVertex3f(r * ci, -h / 2, r * si);
	}
	glEnd();
	free(cs);
	free(sn);
	glPopMatrix();
}

static void linkLuge() {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, bleu);
	myRectangle(1.0F, 0.5f, 0.5F);
	glTranslatef(3.0F, 0.5F, 0.0F);
	myRectangle(0.5F, 3.0f, 0.5F);
	glTranslatef(3.0F, -0.5F, 0.0F);
	myRectangle(1.0F, 0.5f, 0.5F);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blanc);
}

static void myLuge() {


	glMaterialfv(GL_FRONT, GL_DIFFUSE, brun);

	//Planche de la luge
	glPushMatrix();
	myRectangle(0.5F, 3.0F, 7.0F);
	glTranslatef(0.0F,0.0F,7.0F);
	myCylindre(3.0F, 1.0F, 1000);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.0F, -2.0F, 0.0F);
	myRectangle(0.5F, 0.5F, 8.0F);
	glTranslatef(0.0F, 0.3F, 8.0F);
	glRotatef(90.0, 0.0F, 0.0F, 1.0F);
	myCylindre(0.8F, 1.0F, 1000);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2.0F, -2.0F, 0.0F);
	myRectangle(0.5F, 0.5F, 8.0F);
	glTranslatef(0.0F, 0.3F, 8.0F);
	glRotatef(90.0, 0.0F, 0.0F, 1.0F);
	myCylindre(0.8F, 1.0F, 1000);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, blanc);

	linkLuge();




}






static void init(void) {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

/* Scene dessinee                               */
static void scene(void) {
	

	glPushMatrix();
	myLuge();
	
	


	glPopMatrix();
}

/* Fonction executee lors d'un rafraichissement */
/* de la fenetre de dessin                      */

static void display(void) {
	//printf("D\n");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(rx, 1.0F, 0.0F, 0.0F);
	glRotatef(ry, 0.0F, 1.0F, 0.0F);
	glRotatef(rz, 0.0F, 0.0F, 1.0F);
	glScalef(0.05F,0.05F,0.05F);
	scene();
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
	int error = glGetError();
	if (error != GL_NO_ERROR)
		printf("Attention erreur %d\n", error);
}

/* Fonction executee lors d'un changement       */
/* de la taille de la fenetre OpenGL            */

static void reshape(int wx, int wy) {
	printf("R\n");
	wTx = wx;
	wTy = wy;
	glViewport(0, 0, wx, wy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* Fonction executee lorsqu'aucun evenement     */
/* n'est en file d'attente                      */

static void idle(void) {
	//printf("I\n");
	r1 += 1.0F;
	glutPostRedisplay();
}

/* Fonction executee lors de l'appui            */
/* d'une touche alphanumerique du clavier       */

static void keyboard(unsigned char key, int x, int y) {
	printf("K  %4c %4d %4d\n", key, x, y);
	switch (key) {
	case 0x1B: // Echape
		exit(0);
		break;
	case 0x0D: { // Entree
			static int anim = 1;
			anim = !anim;
			glutIdleFunc((anim) ? idle : NULL);
		}
		break;
	case 0x20: // Espace
		if (!isLine) {
			printf("Passe en mode fil de fer\n");
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			isLine = 1;
			glutPostRedisplay();
		}
		else {
			printf("Passe en mode plein\n");
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			isLine = 0;
			glutPostRedisplay();
		}
		break;
	}
	
}

/* Fonction executee lors de l'appui            */
/* d'une touche speciale du clavier :           */
/*   - touches de curseur                       */
/*   - touches de fonction                      */

static void special(int key, int x, int y) {
	printf("M  %4d %4d %4d\n", key, x, y);
	switch (key) {
	case GLUT_KEY_UP:
		rx++;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		rx--;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		ry++;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		ry--;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_UP:
		switch (glutGetModifiers()) {
		case GLUT_ACTIVE_ALT:
			nb++;
			glutPostRedisplay();
			break;
		case GLUT_ACTIVE_CTRL:
			nP++;
			if (nP == nb)
				nP = nb - 1;
			glutPostRedisplay();
			break;
		default:
			rz++;
			glutPostRedisplay();
		}
		break;
	case GLUT_KEY_PAGE_DOWN:
		switch (glutGetModifiers()) {
		case GLUT_ACTIVE_ALT:
			nb--;
			if (nb == 1)
				nb = 2;
			if (nP == nb)
				nP = nb - 1;
			glutPostRedisplay();
			break;
		case GLUT_ACTIVE_CTRL:
			nP--;
			if (nP < 0)
				nP = 0;
			glutPostRedisplay();
			break;
		default:
			rz--;
			glutPostRedisplay();
		}
		break;
	}
}

/* Fonction executee lors de l'utilisation      */
/* de la souris sur la fenetre                  */

static void mouse(int button, int state, int x, int y) {
	printf("M  %4d %4d %4d %4d\n", button, state, x, y);
	/*if (state == GLUT_DOWN) {
		mouseActive = 1;
		mx = x;
	}
	if (state == GLUT_UP) {
		mouseActive = 0;
	}*/
}

/* Fonction executee lors du passage            */
/* de la souris sur la fenetre                  */
/* avec un bouton presse                        */

static void mouseMotion(int x, int y) {
	printf("MM %4d %4d\n", x, y);
	/*r1 += (x - mx);
	mx = x;
	glutPostOverlayRedisplay();*/
}

/* Fonction executee lors du passage            */
/* de la souris sur la fenetre                  */
/* sans bouton presse                           */

static void passiveMouseMotion(int x, int y) {
	printf("PM %4d %4d\n", x, y);
}

/* Fonction exécutée automatiquement            */
/* lors de l'exécution de la fonction exit()    */

static void clean(void) {
	printf("Bye\n");
}

/* Fonction principale                          */
int main(int argc, char **argv) {

	atexit(clean);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(wTx, wTy);
	glutInitWindowPosition(wPx, wPy);
	glutCreateWindow("Gestion événementielle de GLUt");
	init();
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	//glutPassiveMotionFunc(passiveMouseMotion);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	//glutIdleFunc(NULL);
	glutDisplayFunc(display);
	glutMainLoop();
	return(0);
}
