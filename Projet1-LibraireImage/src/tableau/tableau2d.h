/****************************************************************************************
    TABLEAU 2D

 *****************************************************************************************/
#ifndef TABLEAU_2D
#define TABLEAU_2D
/****************************************************************************************
*                               DEFINTION DES CONSTANTES                                *
****************************************************************************************/



/****************************************************************************************
*                       DECLARATION DES FONCTIONS PUBLIQUES                             *
****************************************************************************************/
double** creer_tableau2d(int lignes, int colonnes);

void afficher_tableau2d(double** tableau, int lignes, int colonnes);

void detruire_tableau2d(double** tableau, int lignes);

void initialiser_tableau2d(double** tableau, int lignes, int colonnes, double valeur);

#endif