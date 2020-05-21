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
static float blanc1[] = { 1.2F,1.2F,1.2F,1.0F };


static int wTx = 480;					// Resolution horizontale de la fenetre
static int wTy = 480;					// Resolution verticale de la fenetre
static int wPx = 50;					// Position horizontale de la fenetre
static int wPy = 50;					// Position verticale de la fenetre

static float rx = 4.0F;					// Rotation en x
static float ry = -182.0F;				// Rotation en y
static float rz = 0.0F;					// Rotation en z

static float px = 0.0;					// pour les cameras et glutlookat
static float py = 1.0;					// pour les cameras et glutlookat
static float pz = -1.0;					// pour les cameras et glutlookat
static int versionCamera = 0;			//changer le type de camera

static int isLine = 0;					// Affichage fil de fer
static int isNormalTan = 1;				// Affichage des tangentes et des normals en tout point de la piste

static int mx;							// pour la souris
static int mouseActive = 0;				// pour la souris

static unsigned int textureGlace = 0;	//Texture glace

static int nbp = 500;					// Nombre de point que l'on veut calculer et generer sur notre B-Spline
static int nbPoints = 18;				// Nombre de points de controle pour realiser notre B-Spline
/* Tableau de CH3D de nos positions de nos points de controle pour realiser la B-Spline et la piste de luge */
static CH3D* tPosT[] = { new Pos3D(0.0F , 15.0F ,-50.0F),
						 new Pos3D(0 , -3 ,10),
						 new Pos3D(0 , -9 , 30),
						 new Pos3D(-3.0F , -12 , 48),
						 new Pos3D(-30.0F ,-20 ,75),
						 new Pos3D(30.0F , -27 , 80),
						 new Pos3D(30 , -32 , 90),
						 new Pos3D(0.0F , -42 ,130),
						 new Pos3D(-30.0F , -49 ,150),
						 new Pos3D(-40.0F , -55 , 170),
						 new Pos3D(-35 , -62 , 185),
						 new Pos3D(-0 ,-67 ,200),
						 new Pos3D(-12 , -71 , 225),
						 new Pos3D(-15 , -73 , 240),
						 new Pos3D(-30 , -74 ,250),
						 new Pos3D(-30 , -75 ,265),
						 new Pos3D(-30 , -69 , 285.0F),
						 new Pos3D(-30 , -50 , 350),
};


static float NRUBS[4][4] = { { -1.0 / 6.0,  3.0 / 6.0, -3.0 / 6.0,  1.0 / 6.0 },
							  {  3.0 / 6.0, -6.0 / 6.0,  3.0 / 6.0,        0.0 },
							  { -3.0 / 6.0,		   0.0,  3.0 / 6.0,        0.0 },
							  {  1.0 / 6.0,  4.0 / 6.0,  1.0 / 6.0,        0.0 } };

static float CATMULL_ROM[4][4] = { { -1.0 / 2.0,  3.0 / 2.0, -3.0 / 2.0,  1.0 / 2.0 },
									{  2.0 / 2.0, -5.0 / 2.0,  4.0 / 2.0, -1.0 / 2.0 },
									{ -1.0 / 2.0,        0.0,  1.0 / 2.0,        0.0 },
									{        0.0,  2.0 / 2.0,        0.0,        0.0 } };







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
/* B-Spline controlee par quatre points          */
/* tPos : le tableau des points de controle      */
/* t : la valeur de calcul du point              */
/*     t a prendre dans l'intervalle [0.0,1.0]   */
/* mb : la matrice de base                       */
/* point : le point resultat                     */
static void positionSurBSpline(CH3D** tPos, float t, float mb[4][4], Pos3D* point) {
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

/* Calcul la tangente d'un point sur une courbe  */
/* B-Spline controlee par quatre points          */
/* tPos : le tableau des points de controle      */
/* t : la valeur de calcul du point              */
/*     t a prendre dans l'intervalle [0.0,1.0]   */
/* mb : la matrice de base                       */
/* point : le point resultat                     */
static void tangenteSurBSpline(CH3D** tPos, float t, float mb[4][4], Pos3D* point) {
	float vt[4] = { 3 * t * t,2 * t,1.0F,0 };
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

/* Permet de faire le produit vectorielle entre  */
/* deux vecteurs					             */
/* p1 : le vecteur V1		                     */
/* p2 : le vecteur V2		                     */
/* pRes : le vecteur resultat                    */
static void produitVectorielle(Pos3D p1, Pos3D p2, Pos3D pRes) {
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;
	float z = p1.z - p2.z;
	pRes.x = x;
	pRes.y = y;
	pRes.z = z;
}

/* Permet de faire le produit scalaire entre	 */
/* deux vecteurs					             */
/* p1 : le vecteur V1		                     */
/* p2 : le vecteur V2		                     */
static float produitScalaire(Pos3D p1, Pos3D p2) {
	return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

/* Permet de calculer un vecteur				 */
/* x : coordonne x du vecteur 1		                     */
/* y : coordonne y du vecteur 1		                     */
/* z : coordonne z du vecteur 1		                     */
/* tx : coordonne x du vecteur 2		                     */
/* ty : coordonne y du vecteur 2		                     */
/* tz : coordonne z du vecteur 2	                     */
/* r : vecteur resultat		                     */
static void calculVecteur(float x, float y, float z, float tx, float ty, float tz, Pos3D* r) {
	r->x = x - tx;
	r->y = y - ty;
	r->z = z - tz;
}

static void calculNormal(float x, float y, float z, float tx, float ty, float tz, float x1, float y1, float z1) {
	//printf("%f %f %f | %f %f %f",x,y,z,tx,ty,tz);
	Pos3D v1;
	calculVecteur(x, y, z, tx, ty, tz, &v1);
	//printf("  = %f %f %f", v1.x, v1.y, v1.z);
	Pos3D v2;
	calculVecteur(x, y, z, x1, y1, z1, &v2);
	//printf("  = %f %f %f\n\n", v2.x, v2.y, v2.z);
	//A partir de la nous avons deux equations qui forment un plan.
}

/* Affichage de la tangente d'un point sur une courbe */
/* x : coordonne x du point sur la courbe			  */
/* y : coordonne y du point sur la courbe			  */
/* z : coordonne z du point sur la courbe			  */
/* vx : coordonne vx pour la tangente				  */
/* vy : coordonne vy pour la tangente				  */
/* vz : coordonne vz pour la tangente				  */
void traceLigne(float x, float y, float z, float vx, float vy, float vz) {
	glBegin(GL_LINES);
	glVertex3f(x, y, z);
	glVertex3f(vx, vy, vz);
	glEnd();
}

/* Permet de modeliser la piste en suivant la Bspline										   */
/* A : tableaux des positions pour faire le demi tube en respectant un cerlce trigonometrique  */
/* B : tableaux des positions pour faire le demi tube en respectant un cerlce trigonometrique  */
/* C : tableaux des positions pour faire le demi tube en respectant un cerlce trigonometrique  */
/* D : tableaux des positions pour faire le demi tube en respectant un cerlce trigonometrique  */
/* E : tableaux des positions pour faire le demi tube en respectant un cerlce trigonometrique  */
/* F : tableaux des positions pour faire le demi tube en respectant un cerlce trigonometrique  */
/* G : tableaux des positions pour faire le demi tube en respectant un cerlce trigonometrique  */
/* H : tableaux des positions pour faire le demi tube en respectant un cerlce trigonometrique  */
/* I : tableaux des positions pour faire le demi tube en respectant un cerlce trigonometrique  */
static void modelisationPiste(Pos3D* A, Pos3D* B, Pos3D* C, Pos3D* D, Pos3D* E, Pos3D* F, Pos3D* G, Pos3D* H, Pos3D* I) {
	int n = 500;
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < n; i++) {
		glVertex3f(A[i].x, A[i].y, A[i].z);
		glVertex3f(B[i].x, B[i].y, B[i].z);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < n; i++) {
		glVertex3f(B[i].x, B[i].y, B[i].z);
		glVertex3f(C[i].x, C[i].y, C[i].z);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < n; i++) {
		glVertex3f(C[i].x, C[i].y, C[i].z);
		glVertex3f(D[i].x, D[i].y, D[i].z);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < n; i++) {
		glVertex3f(D[i].x, D[i].y, D[i].z);
		glVertex3f(E[i].x, E[i].y, E[i].z);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < n; i++) {
		glVertex3f(B[i].x, B[i].y, B[i].z);
		glVertex3f(C[i].x, C[i].y, C[i].z);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < n; i++) {
		glVertex3f(E[i].x, E[i].y, E[i].z);
		glVertex3f(F[i].x, F[i].y, F[i].z);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < n; i++) {
		glVertex3f(F[i].x, F[i].y, F[i].z);
		glVertex3f(G[i].x, G[i].y, G[i].z);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < n; i++) {
		glVertex3f(G[i].x, G[i].y, G[i].z);
		glVertex3f(H[i].x, H[i].y, H[i].z);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < n; i++) {
		glVertex3f(H[i].x, H[i].y, H[i].z);
		glVertex3f(I[i].x, I[i].y, I[i].z);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}


/* Modelise une courbe B-Spline par morceaux     */
/* definie par un ensemble de points de controle */
/* nbPoints : le nombre de points de contrôle    */
/* tPos : le tableau des points de controle      */
/* mb : la matrice de base                       */
/* n : le nombre de points a calculer            */
/* typePrimitive : le type de primitive OpenGL   */
/*                 a utiliser                    */
static void BSpline(int nbPoints, CH3D** tPos, CH3D** tPos2, float mb[4][4], int n, GLenum typePrimitive) {
	n = 500;

	Pos3D* pts = new Pos3D[n];
	Pos3D* tan = new Pos3D[n];
	Pos3D* normal = new Pos3D[n];

	glBegin(typePrimitive);
	for (int i = 0; i < n; i++) {

		float t = i / (n - 1.0F) * ((float)nbPoints - 3.0F);
		int nb = (int)t;
		if (nb == nbPoints - 3)
			nb = nbPoints - 4;
		Pos3D point;
		Pos3D tgt;
		positionSurBSpline(&tPos[nb], t - nb, mb, &point);
		if (!isNormalTan) {
			glVertex3f(point.x, point.y, point.z);
		}
		tangenteSurBSpline(&tPos2[nb], t - nb, mb, &tgt);

		pts[i] = point;
		tan[i] = tgt;
	}
	glEnd();

	Pos3D* c1 = new Pos3D[n];
	Pos3D* c2 = new Pos3D[n];
	Pos3D* c3 = new Pos3D[n];
	Pos3D* c4 = new Pos3D[n];
	Pos3D* c5 = new Pos3D[n];
	Pos3D* c6 = new Pos3D[n];
	Pos3D* c7 = new Pos3D[n];
	Pos3D* c8 = new Pos3D[n];
	Pos3D* c9 = new Pos3D[n];

	float pi54 = 5 * M_PI / 4;
	float pi74 = 7 * M_PI / 4;
	float pi43 = 4 * M_PI / 3;
	float pi53 = 5 * M_PI / 3;
	float pi116 = 11 * M_PI / 6;
	float pi76 = 7 * M_PI / 6;
	float pi32 = 3 * M_PI / 2;


	for (int i = 0; i < n; i++) {
		float d = sqrt(tan[i].x * tan[i].x + tan[i].y * tan[i].y + tan[i].z * tan[i].z);
		float vx = pts[i].x + tan[i].x / d;
		float vy = pts[i].y + tan[i].y / d;
		float vz = pts[i].z + tan[i].z / d;
		if (!isNormalTan) {
			traceLigne(pts[i].x, pts[i].y, pts[i].z, vx, vy, vz);
		}

		c1[i].x = vx + 1;
		c1[i].y = vy;
		c1[i].z = vz;

		c2[i].x = vx + (cos(pi116) * 1.0);
		c2[i].y = vy + (sin(pi116) * 1.0);
		c2[i].z = vz;

		c3[i].x = vx + (cos(pi74) * 1.0);
		c3[i].y = vy + (sin(pi74) * 1.0);
		c3[i].z = vz;

		c4[i].x = vx + (cos(pi53) * 1.0);
		c4[i].y = vy + (sin(pi53) * 1.0);
		c4[i].z = vz;

		c5[i].x = vx + (cos(pi32) * 1.0);
		c5[i].y = vy + (sin(pi32) * 1.0);
		c5[i].z = vz;

		c6[i].x = vx + (cos(pi43) * 1.0);
		c6[i].y = vy + (sin(pi43) * 1.0);
		c6[i].z = vz;

		c7[i].x = vx + (cos(pi54) * 1.0);
		c7[i].y = vy + (sin(pi54) * 1.0);
		c7[i].z = vz;

		c8[i].x = vx + (cos(pi76) * 1.0);
		c8[i].y = vy + (sin(pi76) * 1.0);
		c8[i].z = vz;

		c9[i].x = vx - 1;
		c9[i].y = vy;
		c9[i].z = vz;

		Pos3D norm;
		norm.x = pts[i].x + 0.05;
		norm.y = pts[i].y + 2;
		norm.z = pts[i].z + 0.05;
		if (!isNormalTan) {
			traceLigne(pts[i].x, pts[i].y, pts[i].z, norm.x, norm.y, norm.z);
		}
		normal[i] = norm;
	}
	glPushMatrix();
	modelisationPiste(c1, c2, c3, c4, c5, c6, c7, c8, c9);
	glPopMatrix();
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
		double pos = i / (nb - 1.0) * ((double)n - 3);
		int nb = (int)pos;
		if (nb == n - 3)
			nb = n - 4;
		double t = pos - nb;
		determinationPositionSurBSpline(&tPos[nb], t, mb, tRes[i]);
	}
}

/* Fonction qui realise cube cote c */
/* c : taille d'un cote				*/
void mySolidCube(float c) {
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

	glNormal3f(0.0F, 1.0F, 0.0F);
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



////////////////////////////////////////////////////////////////////////////////////////////////////////////



static void init(void) {
	// Chagrment des textures dans le dossier Bin/image
	chargeTexture(textureGlace, "../src/Image/glacePNG.png");
}

/* Platforme de lancement du boblseigh */
void startPlateforme() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	//printf(" startp %d\n", GL_TEXTURE_2D);
	glTranslatef(0.0F, -3.0F, 0.0F);
	myRectangle(0.5F, 10.0F, 25.0F);
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

/* Scene dessinee                               */
static void scene(void) {
	glPushMatrix();
	glScalef(0.1, 0.1, 0.1);
	myLuge();
	startPlateforme();
	glPopMatrix();

	glPushMatrix();
	BSpline(nbPoints, tPosT, tPosT, NRUBS, nbp, GL_POINTS);
	glPopMatrix();
}

/* Fonction executee lors d'un rafraichissement */
/* de la fenetre de dessin                      */
static void display(void) {
	//printf("D\n");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const GLfloat light0_position[] = { 20.0,10.0,0.0,1.0 };

	const GLfloat light1_position[] = { -30.0,-50.0,350.0,1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	const GLfloat light2_position[] = { 32.0,-18.0,110.0,0.0 };
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glRotatef(rx, 1.0F, 0.0F, 0.0F);
	glRotatef(ry, 0.0F, 1.0F, 0.0F);
	glRotatef(rz, 0.0F, 0.0F, 1.0F);

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	gluLookAt(px, py, pz, px, py, pz -1.0F, 0.0, 1.0, 0.0);
	
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
	if (ratio >= 1.0F)
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
	case 0x66:
	case 'F':	//f minuscule
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
	case 'g':
	case 'G':
		if (!isNormalTan) {
			printf("Passe en mode affichage des tangentes et normales\n");
			isNormalTan = 1;
			glutPostRedisplay();
		}
		else {
			printf("Passe en mode affichage de la piste\n");
			isNormalTan = 0;
			glutPostRedisplay();
		}
		break;
	case 0x7A:
	case 'Z' ://z
		py += 1.0;
		glutPostRedisplay();
		break;
	case 0x73:
	case 'S' ://s
		py -= 1.0;
		glutPostRedisplay();
		break;
	case 0x71:
	case 'Q' ://q
		px -= 1.0;
		glutPostRedisplay();
		break;
	case 0x64:
	case 'D' ://d
		px += 1.0;
		glutPostRedisplay();
		break;
	case 0x61:
	case 'A' ://a
		pz -= 1.0;
		glutPostRedisplay();
		break;
	case 0x65://e
	case 'E':
		pz += 1.0;
		glutPostRedisplay();
		break;
	case 'v': //b
		printf("px %f | py %f | pz %f | rx %f | ry %f | rz %f\n", px,py,pz,rx,ry,rz);
		glutPostRedisplay();
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
		rx+=2.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		rx-=2.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		ry+=2.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		ry-=2.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_UP:
		rz+=2.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		rz-=2.0;
		glutPostRedisplay();
		break;

	case GLUT_KEY_F1:
		px = 61.0;
		py = -1.0;
		pz = 263.0;
		rx = 20.0;
		ry = -44.0;
		rz = 0.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_F2:
		px = -69.0;
		py = 54.0;
		pz = 31.0;
		rx = 28.0;
		ry = -226.0;
		rz = 0.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_F3:
		px = 32.0;
		py = -18.0;
		pz = 110.0;
		rx = 12.0;
		ry = -64.0;
		rz = 0.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_F4:
		px = 0.0;
		py = 1.0;
		pz = -1.0;
		rx = 4.0;
		ry = -182.0;
		rz = 0.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_F5:
		px = 27.0;
		py = -19.0;
		pz = 74.0;
		rx = 12.0;
		ry = -146.0;
		rz = 0.0;
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

	free(tPosT);
	free(bleu);
	free(blanc);
	free(brun);
	free(noir);
	free(gris);
	free(blanc1);
	free(NRUBS);
	free(CATMULL_ROM);

	return(0);
}
