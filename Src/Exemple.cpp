/* Projet Infographie Bobsleigh				    */
/*                                              */
/* Auteurs: Nicolas Courvoisier					*/
/*		   Tanguy Plaza							*/
/*		   Adrien Signoret						*/
/* Master Informatique						    */
/* Semestre 8 2019-2020                         */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

#include "PNG/ChargePngFile.h"
#include "Pos3D.h"

#ifndef M_PI
#define M_PI 3.14159
#endif


/* Variables globales                           */

/* Definition des couleurs						*/
static float bleu[4] = { 0.0F,0.0F,1.0F,1.0F };
static float blanc[4] = { 1.0F,1.0F,1.0F,1.0F };
static float brun[4] = { 0.59F,0.34F,0.09F,1.0F };
static float noir[4] = { 0.0F,0.0F,0.0F,1.0F };
static float gris[4] = { 0.65F,0.65F,0.65F,1.0F };
static const float blanc1[] = { 1.2F,1.2F,1.2F,1.0F };


static int wTx = 480;					// Resolution horizontale de la fenetre
static int wTy = 480;					// Resolution verticale de la fenetre
static int wPx = 50;					// Position horizontale de la fenetre
static int wPy = 50;					// Position verticale de la fenetre

static float rx = 0.0F;					// Rotation en x
static float ry = 0.0F;					// Rotation en y
static float rz = 0.0F;					// Rotation en z

static double px = 0.0;					// pour les cameras et glutlookat
static double py = 0.0;					// pour les cameras et glutlookat
static double pz = 10.0;				// pour les cameras et glutlookat
static int versionCamera = 0;			//changer le type de camera

static int isLine = 0;					// Affichage fil de fer

static int mx;							// pour la souris
static int mouseActive = 0;				// pour la souris

static unsigned int textureGlace = 0;	//Texture glace



static int nbPoints = 10;

static float NRUBS[4][4] = { { -1.0 / 6.0,  3.0 / 6.0, -3.0 / 6.0,  1.0 / 6.0 },
							  {  3.0 / 6.0, -6.0 / 6.0,  3.0 / 6.0,        0.0 },
							  { -3.0 / 6.0,		   0.0,  3.0 / 6.0,        0.0 },
							  {  1.0 / 6.0,  4.0 / 6.0,  1.0 / 6.0,        0.0 } };

static float CATMULL_ROM[4][4] = { { -1.0 / 2.0,  3.0 / 2.0, -3.0 / 2.0,  1.0 / 2.0 },
									{  2.0 / 2.0, -5.0 / 2.0,  4.0 / 2.0, -1.0 / 2.0 },
									{ -1.0 / 2.0,        0.0,  1.0 / 2.0,        0.0 },
									{        0.0,  2.0 / 2.0,        0.0,        0.0 } };



static int nbPointsPos = 37;
static Pos3D* tPos1[] = { new Pos3D(6.5F, 4.9F,-3.00F), new Pos3D(7.0F, 5.0F,-3.00F), new Pos3D(7.5F, 4.9F,-2.00F),
						 new Pos3D(8.0F, 4.6F, 0.00F), new Pos3D(7.5F, 4.4F, 2.00F), new Pos3D(7.0F, 4.2F, 3.50F),
						 new Pos3D(6.0F, 3.8F, 4.00F), new Pos3D(5.0F, 2.8F, 4.00F), new Pos3D(4.0F, 1.8F, 4.00F),
						 new Pos3D(3.0F, 0.8F, 4.00F), new Pos3D(0.0F, 0.0F, 4.00F), new Pos3D(-1.4F, 0.7F, 3.75F),
						 new Pos3D(-2.0F, 2.0F, 3.50F), new Pos3D(-1.4F, 3.3F, 3.25F), new Pos3D(0.0F, 4.0F, 3.00F),
						 new Pos3D(1.4F, 3.3F, 2.75F), new Pos3D(2.0F, 2.0F, 2.50F), new Pos3D(1.0F, 0.7F, 2.25F),
						 new Pos3D(-1.0F, 0.0F, 2.00F), new Pos3D(-2.4F, 0.7F, 1.75F), new Pos3D(-3.0F, 2.0F, 1.50F),
						 new Pos3D(-2.4F, 3.3F, 1.25F), new Pos3D(-1.0F, 4.0F, 1.00F), new Pos3D(0.4F, 3.3F, 0.75F),
						 new Pos3D(1.0F, 2.0F, 0.50F), new Pos3D(0.0F, 0.7F, 0.25F), new Pos3D(-2.0F, 0.2F, 0.00F),
						 new Pos3D(-7.0F, 0.0F, 1.50F), new Pos3D(-8.0F, 0.0F,-1.30F), new Pos3D(-7.0F, 0.3F,-1.90F),
						 new Pos3D(-6.0F, 1.8F,-2.40F), new Pos3D(-4.5F, 4.9F,-2.40F), new Pos3D(-3.0F, 3.2F,-2.40F),
						 new Pos3D(0.5F, 2.4F,-2.80F), new Pos3D(3.5F, 4.4F,-2.80F), new Pos3D(4.5F, 4.7F,-2.90F),
						 new Pos3D(5.5F, 4.8F,-3.00F) };



static Pos3D* tPos[] = { new Pos3D(0.0F, 1.0F,1.0F), new Pos3D(7.0F, 5.0F,-3.00F), new Pos3D(7.5F, 4.9F,-2.00F),
						 new Pos3D(0.0F, 2.0F, 2.0F), new Pos3D(7.5F, 4.4F, 2.00F), new Pos3D(7.0F, 4.2F, 3.50F),
						 new Pos3D(0.0F, 3.0F, 3.0F), new Pos3D(5.0F, 2.8F, 4.00F), new Pos3D(4.0F, 1.8F, 4.00F),
						 new Pos3D(3.0F, 0.8F, 4.00F), new Pos3D(0.0F, 0.0F, 4.00F), new Pos3D(-1.4F, 0.7F, 3.75F),
						 new Pos3D(-2.0F, 2.0F, 3.50F), new Pos3D(-1.4F, 3.3F, 3.25F), new Pos3D(0.0F, 4.0F, 3.00F),
						 new Pos3D(1.4F, 3.3F, 2.75F), new Pos3D(2.0F, 2.0F, 2.50F), new Pos3D(1.0F, 0.7F, 2.25F),
						 new Pos3D(-1.0F, 0.0F, 2.00F), new Pos3D(-2.4F, 0.7F, 1.75F), new Pos3D(-3.0F, 2.0F, 1.50F),
						 new Pos3D(-2.4F, 3.3F, 1.25F), new Pos3D(-1.0F, 4.0F, 1.00F), new Pos3D(0.4F, 3.3F, 0.75F),
						 new Pos3D(1.0F, 2.0F, 0.50F), new Pos3D(0.0F, 0.7F, 0.25F), new Pos3D(-2.0F, 0.2F, 0.00F),
						 new Pos3D(-7.0F, 0.0F, 1.50F), new Pos3D(-8.0F, 0.0F,-1.30F), new Pos3D(-7.0F, 0.3F,-1.90F),
						 new Pos3D(-6.0F, 1.8F,-2.40F), new Pos3D(-4.5F, 4.9F,-2.40F), new Pos3D(-3.0F, 3.2F,-2.40F),
						 new Pos3D(0.5F, 2.4F,-2.80F), new Pos3D(3.5F, 4.4F,-2.80F), new Pos3D(4.5F, 4.7F,-2.90F),
						 new Pos3D(5.5F, 4.8F,-3.00F) };



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



static void vertex(Pos3D* p, int couleur, double taille) {
	printf("VERTEX\n");
	glPushMatrix();
	glTranslated(p->x, p->y, p->z);
	glutSolidSphere(taille, 36, 18);
	glPopMatrix();
}












/* Calcul la position d'un point sur une courbe  */
/* B-Spline controlee par quatre points          */
/* tPos : le tableau des 4 points de controle    */
/* t : la valeur de calcul du point              */
/*     t a prendre dans l'intervalle [0.0,1.0]   */
/* mb : la matrice de base                       */
/* point : le point resultat                     */

static void positionSurBSpline(Pos3D** tPos, float t, float mb[4][4], Pos3D* point) {
	float vt[4] = { t * t * t,t * t,t,1.0F };
	float vtmb[4] = { 0.0F,0.0F,0.0F,0.0F };
	for (int j = 0; j < 4; j++) {
		for (int k = 0; k < 4; k++)
			vtmb[j] += vt[k] * mb[k][j];
	}
	point->x = point->y = point->z = 0.0;
	for (int j = 0; j < 4; j++) {
		point->x += vtmb[j] * tPos[j]->x;
		point->y += vtmb[j] * tPos[j]->y;
		point->z += vtmb[j] * tPos[j]->z;
	}
}

/* Modelise une courbe B-Spline par morceaux     */
/* definie par un ensemble de points de controle */
/* nbPoints : le nombre de points de contrôle    */
/* tPos : le tableau des points de controle      */
/* mb : la matrice de base                       */
/* n : le nombre de points a calculer            */
/* typePrimitive : le type de primitive OpenGL   */
/*                 a utiliser                    */

static void BSpline(int nbPoints, Pos3D** tPos, float mb[4][4], int n, GLenum typePrimitive) {
	glBegin(typePrimitive);
	for (int i = 0; i < n; i++) {
		float t = i / (n - 1.0) * (nbPoints - 3);
		int nb = (int)t;
		if (nb == nbPoints - 3)
			nb = nbPoints - 4;
		Pos3D point;
		positionSurBSpline(&tPos[nb], t - nb, mb, &point);
		glVertex3f(point.x, point.y, point.z);
	}
	glEnd();
}
















/* Fonction d'initialisation des parametres     */
/* OpenGL ne changeant pas au cours de la vie   */
/* du programme                                 */
/* Contient en particulier l'initialisation     */
/* de trois textures 2D                         */
static void chargeTexture(unsigned int textureID, char* filename) {
	printf("CHARGE TEXTURE\n");
	glClearColor(0.25F, 0.25F, 0.25F, 1.0F);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, blanc);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	{ int rx;
	int ry;
	unsigned char* img = chargeImagePng(filename, &rx, &ry);
	if (img) {
		glTexImage2D(GL_TEXTURE_2D, 0, 3, rx, ry, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		free(img);
		printf("Texture chargee %d\n", textureID);
	}
	else {
		glDeleteTextures(1, &textureID);
		textureID = 0;
		printf("Texture non chargee\n");
	} }
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

/* Calcul la position d'un point sur une courbe  */
/* B-Spline controlee par quatre sommets         */
/* g : le tableau des 4 sommets de controle      */
/* t : la valeur de calcul du point              */
/*     t a prendre dans l'intervalle [0.0,1.0]   */
/* mb : la matrice de base                       */
/* p : le point resultat                         */
static void determinationPositionSurBSpline(Pos3D** g, double t, double mb[4][4], Pos3D* p) {
	double vt[4] = { t * t * t,t * t,t,1.0 };
	double vtmb[4] = { 0.0,0.0,0.0,0.0 };
	for (int j = 0; j < 4; j++) {
		for (int k = 0; k < 4; k++)
			vtmb[j] += vt[k] * mb[k][j];
	}
	p->x = p->y = p->z = 0.0;
	for (int j = 0; j < 4; j++) {
		p->x += vtmb[j] * g[j]->x;
		p->y += vtmb[j] * g[j]->y;
		p->z += vtmb[j] * g[j]->z;
	}
}

/* Calcul les points definissant une courbe      */
/* B-Spline par morceaux definie par un ensemble */
/* de sommets de controle                        */
/* tPos : le tableau des sommets de controle     */
/* n : le nombre de sommets de contr�le          */
/* mb : la matrice de base                       */
/* nb : le nombre de points a calculer           */
/* tRes : le tableau de points resultat          */
static void calculBSpline(Pos3D** tPos, int n, double mb[4][4], int nb, Pos3D** tRes) {
	for (int i = 0; i < nb; i++) {
		double pos = i / (nb - 1.0) * (n - 3);
		int nb = (int)pos;
		if (nb == n - 3)
			nb = n - 4;
		double t = pos - nb;
		determinationPositionSurBSpline(&tPos[nb], t, mb, tRes[i]);
	}
}

/* Fonction qui realise cube cote c */
/* c : taille d'un cote				*/
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

/* Platforme de lancement du boblseigh */
void startPlateforme() {
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(0.0F, -3.0F, 0.0F);
	glScalef(30.0F, 0.5F, 100.0F);
	mySolidCube(1.0F);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

/* Platforme de lancement du boblseigh */
void stopPlateforme() {
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(500.0F, -300.0F, 0.0F);
	glScalef(30.0F, 0.5F, 100.0F);
	mySolidCube(1.0F);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

/* Fonction qui genere un cylindre	*/
/* hauteur : la hauteur du cylindre */
/* rayon : le rayon du cylindre     */
/* ns : le nombre de facettes	    */
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
		float a = (2 * 3.14 * i) / ns;
		float cs = cos(a);
		float sn = -sin(a);
		glNormal3f(cs, 0.0F, sn);
		float x = rayon * cs;
		float z = rayon * sn;
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

/* Fonction qui modelise un rectangle
 * h : hauteur du rectangle
 * l : largeur du rectangle
 * lo : longueur du rectangle
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

/* Fonction qui genere un cylindre plein	*/
/* h : la hauteur du cylindre		    */
/* r : le rayon du cylindre			    */
/* n : le nombre de facettes				*/
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

/* Fonction qui fait les essieux de la luge */
static void linkLuge() {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, noir);
	myRectangle(1.0F, 0.5f, 0.5F);
	glTranslatef(2.0F, 0.3F, 0.0F);
	myRectangle(0.5F, 2.0f, 0.5F);
	glTranslatef(2.0F, -0.5F, 0.0F);
	myRectangle(1.0F, 0.5f, 0.5F);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blanc);
}

/* Fonction qui genere la luge*/
static void myLuge() {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, brun);

	glPushMatrix();
	myRectangle(0.5F, 3.0F, 7.0F);
	glTranslatef(0.0F, 0.0F, 7.0F);
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

	glPushMatrix();
	glTranslatef(-2.0F, -0.6F, 6.0F);
	linkLuge();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-2.0F, -0.6F, -6.0F);
	linkLuge();
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, noir);
	glTranslatef(0.0F, -1.7F, 8.0F);
	glRotatef(90.0F, 0.0F, 0.0F, 1.0F);
	mySolidCylindre(3.0F, 0.20F, 100);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blanc);
	glPopMatrix();
}

static void bezier(int nbPoints, Pos3D** tPos, int n, GLenum typePrimitive) {
	float t, mt;
	float x, y, z, fac;
	long long* cn = (long long*)calloc(nbPoints, sizeof(long long));
	cn[0] = 1;
	cn[1] = nbPoints - 1;
	for (int i = 2; i < nbPoints; i++)
		cn[i] = (cn[i - 1] * (nbPoints - i)) / i;
	glBegin(typePrimitive);
	for (int i = 0; i < n; i++) {
		t = (float)i / (n - 1);
		mt = 1.0F - t;
		x = y = z = 0.0F;
		for (int j = 0; j < nbPoints; j++) {
			fac = cn[j] * pow(t, j) * pow(mt, nbPoints - 1 - j);
			x += fac * tPos[j]->x;
			y += fac * tPos[j]->y;
			z += fac * tPos[j]->z;
		}
		glVertex3f(x, y, z);
	}
	glEnd();
	free(cn);
}









////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void init(void) {
	/*const GLfloat mat_shininess[] = { 50.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, blanc1);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, blanc);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, blanc);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, blanc);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);*/
	
	// Chagrment des textures dans le dossier Bin/image
	chargeTexture(textureGlace, "Image/glacePNG.png");
}

/* Scene dessinee                               */
static void scene(void) {
	/*glPushMatrix();
	myLuge();
	startPlateforme();
	stopPlateforme();
	glPopMatrix();*/

	//Pos3D p = new Pos3D(4.0,5.0,6.0);
	//vertex(&p, 5, 1.0);


	BSpline(nbPointsPos, tPos, NRUBS, 100, GL_QUADS);

	//BSpline(nbPointsPos, tPos, CATMULL_ROM, 100, GL_LINE_STRIP);

	//bezier(nbPointsPos, tPos, 100, GL_LINE_STRIP);

}

/* Fonction executee lors d'un rafraichissement */
/* de la fenetre de dessin                      */
static void display(void) {
	//printf("D\n");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const GLfloat light0_position[] = { 0.0,0.0,10.0,1.0 };
	glPushMatrix();

	glRotatef(rx, 1.0F, 0.0F, 0.0F);
	glRotatef(ry, 0.0F, 1.0F, 0.0F);
	glRotatef(rz, 0.0F, 0.0F, 1.0F);

	switch (versionCamera) {
	case 0:
		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		gluLookAt(px, py, pz, px, py, pz-1, 0.0, 1.0, 0.0);
		break;
	case 1:
		gluLookAt(px, py, pz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		break;
	}
	scene();
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
	int error = glGetError();
	if (error != GL_NO_ERROR)
		printf("Erreur OpenGL: %d\n", error);
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
	double ratio = (double)wx / wy;
	if (ratio >= 1.0)
		gluPerspective(80.0, ratio, 1.0, 2000.0);
	else
		gluPerspective(80.0 / ratio, ratio, 1.0, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

/* Fonction executee lorsqu'aucun evenement     */
/* n'est en file d'attente                      */
static void idle(void) {
	//printf("I\n");
	glutPostRedisplay();
}

/* Fonction executee lors de l'appui            */
/* d'une touche alphanumerique du clavier       */
static void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 0x0D:	//enter
		glutPostRedisplay();
		break;
	case 0x20:	//space
		glutPostRedisplay();
		break;
	case 0x1B:	//echape	
		exit(0);
		break;
	case 0x66:	//f minuscule
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
	case 0x7A: //z
		py += 0.1;
		glutPostRedisplay();
		break;
	case 0x73: //s
		py -= 0.1;
		glutPostRedisplay();
		break;
	case 0x71: //q
		px -= 0.1;
		glutPostRedisplay();
		break;
	case 0x64://d
		px += 0.1;
		glutPostRedisplay();
		break;
	case 0x61://a
		pz -= 0.1;
		glutPostRedisplay();
		break;
	case 0x65://e
		pz += 0.1;
		glutPostRedisplay();
		break;
	case 0x62: //b
		px = 0.0;
		py = 0.0;
		pz = 20.0;
		rx = 0.0;
		ry = 0.0;
		rz = 0.0;
		glutPostRedisplay();
		break;
	}
}


/* Fonction executee lors de l'appui            */
/* d'une touche speciale du clavier :           */
/*   - touches de curseur                       */
/*   - touches de fonction                      */
static void special(int key, int x, int y) {
	//printf("M  %4d %4d %4d\n", key, x, y);
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
		rz++;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		rz--;
		glutPostRedisplay();
		break;
	}
}


/* Fonction executee lors de l'utilisation      */
/* de la souris sur la fenetre                  */
static void mouse(int button, int state, int x, int y) {
	//printf("M  %4d %4d %4d %4d\n", button, state, x, y);
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
	//printf("MM %4d %4d\n", x, y);
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

/* Fonction executee automatiquement            */
/* lors de l'execution de la fonction exit()    */
static void clean(void) {
	printf("Bye\n");
}

/* Fonction principale                          */
int main(int argc, char** argv) {

	atexit(clean);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(wTx, wTy);
	glutInitWindowPosition(wPx, wPy);
	glutCreateWindow("Bobsleigh");
	init();
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);

	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	//glutPassiveMotionFunc(passiveMouseMotion);
	glutReshapeFunc(reshape);
	//glutIdleFunc(NULL);
	glutDisplayFunc(display);
	glutMainLoop();
	return(0);
}
