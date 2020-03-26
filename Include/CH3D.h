/* Mathematiques de l'informatique graphique    */
/* Coordonnees homogenes en 3D                  */
/*                                              */
/* Auteur: Nicolas JANEY                        */
/* nicolas.janey@univ-fcomte.fr                 */
/* Mars 2020                                    */

#ifndef ____CH3D____
#define ____CH3D____

class CH3D  {

  public :

    /* Coordonnees x, y et z                    */

    float x;
    float y;
    float z;

  private :

    /* Coordonnee w                             */

    float w;

  protected :

    /* Construit le vecteur nul                 */

    CH3D(void);

    /* Construit la CH3D (x,y,z,w)              */

    CH3D(float x,float y,float z,float w);

    /* Construit un clone de la CH3D ch         */

    CH3D(CH3D *ch);

    /* Destructeur                              */

    ~CH3D(void);

  public :

    /* Methode d'affichage texte                */

    void print(void);
};

#endif
