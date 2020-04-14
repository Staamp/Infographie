/* Mathematiques de l'informatique graphique    */
/* Direction en 3D                              */
/*                                              */
/* Auteur: Nicolas JANEY                        */
/* nicolas.janey@univ-fcomte.fr                 */
/* Mars 2020                                    */

#ifndef ____DIR3D____
#define ____DIR3D____

class Pos3D;

#include "CH3D.h"

class Dir3D : public CH3D {

  public :

    /* Construit le vecteur nul                 */

    Dir3D(void);

    /* Construit la direction (x,y,z)           */

    Dir3D(float x,float y,float z);

    /* Construit un clone de la Dir3D d         */

    Dir3D(Dir3D *d);

    /* Construit la direction pipf              */

    Dir3D(Pos3D *pi,Pos3D *pf);

    /* Construit la direction produit vectoriel */
    /* de d1 par d2                             */

    Dir3D(Dir3D *d1,Dir3D *d2);

    /* Destructeur                              */

    ~Dir3D(void);

    /* Methode de calcul de la norme de this    */

    float norme(void);

    /* Methode de normalisation de this         */
    /* Retourne la norme de this                */
    /* avant normalisation                      */

    float normalisation(void);

    /* Methode de calcul du produit scalaire    */
    /* de this et de la direction d             */

    float produitScalaire(Dir3D *d);

    /* Methode statique de calcul               */
    /* du produit scalaire des deux directions  */
    /* d1 et d2                                 */

    static float produitScalaire(Dir3D *d1,Dir3D *d2);

    /* Methode de calcul du produit vectoriel   */
    /* de this par la direction d               */
    /* avec stockage du resultat dans this      */

    void produitVectoriel(Dir3D *d);

    /* Methode de calcul du produit vectoriel   */
    /* de la direction d1 par la direction d2   */
    /* avec stockage du resultat dans this      */

    void produitVectoriel(Dir3D *d1,Dir3D *d2);

    /* Methode statique de calcul               */
    /* du produit vectoriel de deux directions  */
    /* d1 et d2 avec stockage du resultat       */
    /* dans res                                 */

    static void produitVectoriel(Dir3D *d1,Dir3D *d2,Dir3D *res);
};

#endif
