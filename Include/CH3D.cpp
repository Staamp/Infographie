/* Mathematiques de l'informatique graphique    */
/* Coordonnees homogenes en 3D                  */
/*                                              */
/* Auteur: Nicolas JANEY                        */
/* nicolas.janey@univ-fcomte.fr                 */
/* Mars 2020                                    */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "CH3D.h"

/* Construit le vecteur nul                     */

CH3D::CH3D(void) {
  x = y = z = w = 0.0F;
}

/* Construit la CH3D (x,y,z,w)                  */

CH3D::CH3D(float x,float y,float z,float w) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
}

/* Construit un clone de la CH3D ch             */

CH3D::CH3D(CH3D *ch) {
  x = ch->x;
  y = ch->y;
  z = ch->z;
  w = ch->w;
}

/* Destructeur                                  */

CH3D::~CH3D(void) {
}

/* Methode d'affichage texte                    */

void CH3D::print(void) {
  printf("%10.4f %10.4f %10.4f %10.4f",x,y,z,w);
}
