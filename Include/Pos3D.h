/* Mathematiques de l'informatique graphique    */
/* Position en 3D                               */
/*                                              */
/* Auteur: Nicolas JANEY                        */
/* nicolas.janey@univ-fcomte.fr                 */
/* Mars 2020                                    */

#ifndef ____POS3D____
#define ____POS3D____

#include "CH3D.h"

class Pos3D : public CH3D {

  public :

    /* Construit l'origine                      */

    Pos3D(void);

    /* Construit le point (x,y,z)               */

    Pos3D(float x,float y,float z);

    /* Construit un clone de la Pos3D p         */

    Pos3D(Pos3D *p);

    /* Destructeur                              */

    ~Pos3D(void);
};

#endif
