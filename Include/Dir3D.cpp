/* Mathematiques de l'informatique graphique    */
/* Direction en 3D                              */
/*                                              */
/* Auteur: Nicolas JANEY                        */
/* nicolas.janey@univ-fcomte.fr                 */
/* Mars 2020                                    */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "CH3D.h"
#include "Dir3D.h"
#include "Pos3D.h"

/* Constructeurs                                */

/* Construit le vecteur nul                     */

Dir3D::Dir3D(void):CH3D() {
}

/* Construit la direction (x,y,z)               */

Dir3D::Dir3D(float x,float y,float z):CH3D(x,y,z,0.0F) {
}

/* Construit un clone de la Dir3D d             */

Dir3D::Dir3D(Dir3D *d):CH3D(d) {
}

/* Construit la direction pipf                  */

Dir3D::Dir3D(Pos3D *pi,Pos3D *pf):CH3D(pf->x-pi->x,pf->y-pi->y,pf->z-pi->z,0.0F) {
}

/* Construit la direction produit vectoriel     */
/* de d1 par d2                                 */

Dir3D::Dir3D(Dir3D *d1,Dir3D *d2):CH3D() {
  produitVectoriel(d1,d2,this);
}

/* Destructeur                                  */

Dir3D::~Dir3D(void) {
}

/* Methode de calcul de la norme de this        */

float Dir3D::norme(void) {
  return((float) sqrt(x*x+y*y+z*z));
}

/* Methode de normalisation de this             */
/* Retourne la norme de this pre-normalisation  */

float Dir3D::normalisation(void) {
  float d = norme();
  if ( d != 0.0 ) {
    x /= d;
    y /= d;
    z /= d; }
  return d;
}

/* Methode de calcul du produit scalaire        */
/* de this et de la direction d                 */

float Dir3D::produitScalaire(Dir3D *d) {
  return(produitScalaire(this,d));
}

/* Methode statique de calcul                   */
/* du produit scalaire des deux directions      */
/* d1 et d2                                     */

float Dir3D::produitScalaire(Dir3D *d1,Dir3D *d2) {
  return(d1->x*d2->x+d1->y*d2->y+d1->z*d2->z);
}

/* Methode de calcul du produit vectoriel       */
/* de this par la direction d                   */
/* avec stockage du resultat dans this          */

void Dir3D::produitVectoriel(Dir3D *d) {
  produitVectoriel(this,d,this);
}

/* Methode de calcul du produit vectoriel       */
/* de la direction d1 par la direction d2       */
/* avec stockage du resultat dans this          */

void Dir3D::produitVectoriel(Dir3D *d1,Dir3D *d2) {
  produitVectoriel(d1,d2,this);
}

/* Methode statique de calcul                   */
/* du produit vectoriel de deux directions      */
/* d1 et d2 avec stockage du resultat           */
/* dans res                                     */

void Dir3D::produitVectoriel(Dir3D *d1,Dir3D *d2,Dir3D *res) {
  float x = d1->y*d2->z - d1->z*d2->y;
  float y = d1->z*d2->x - d1->x*d2->z;
  float z = d1->x*d2->y - d1->y*d2->x;
  res->x = x;
  res->y = y;
  res->z = z;
}
