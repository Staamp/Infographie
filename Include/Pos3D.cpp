/* Mathematiques de l'informatique graphique    */
/* Position en 3D                               */
/*                                              */
/* Auteur: Nicolas JANEY                        */
/* nicolas.janey@univ-fcomte.fr                 */
/* Mars 2020                                    */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "CH3D.h"
#include "Pos3D.h"

/* Construit l'origine                          */

Pos3D::Pos3D(void):CH3D(0.0F,0.0F,0.0F,1.0F) {
}

/* Construit le point (x,y,z)                   */

Pos3D::Pos3D(float x,float y,float z):CH3D(x,y,z,1.0F) {
}

/* Construit un clone de la Pos3D p             */

Pos3D::Pos3D(Pos3D *p):CH3D(p) {
}

/* Destructeur                                  */

Pos3D::~Pos3D(void) {
}
