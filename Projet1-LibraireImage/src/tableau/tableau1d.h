/****************************************************************************************
    TABLEAU 1D

 *****************************************************************************************/
#ifndef TABLEAU_1D
#define TABLEAU_1D
/****************************************************************************************
*                               DEFINTION DES CONSTANTES                                *
****************************************************************************************/



/****************************************************************************************
*                       DECLARATION DES FONCTIONS PUBLIQUES                             *
****************************************************************************************/
void afficher_tableau1d(double* tableau, int nb_element);

double somme_tableau1d(double* tableau, int nb_element);

void produit_tableau1d(double* tableau, int nb_element, double valeur);

double produit_scalaire1d(double* tableau1, double* tableau2, int nb_element);

double* creer_tableau1d(int nb_element);

void detruire_tableau1d(double* pointeur);

#endif