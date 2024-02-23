/****************************************************************************************
    TABLEAU_2D

*****************************************************************************************/
#include "tableau2d.h"

#include <stdio.h>
#include <stdlib.h>

/****************************************************************************************
*                               DEFINTION DES CONSTANTES                                *
****************************************************************************************/



/****************************************************************************************
*                           DECLARATION DES FONCTIONS PRIVEES                           *
****************************************************************************************/



/****************************************************************************************
*                           DEFINTION DES FONCTIONS PUBLIQUES                            *
****************************************************************************************/
double** creer_tableau2d(int lignes, int colonnes){
    double** tableau = (double**)malloc(lignes * sizeof(double*));
    for (int i = 0; i < lignes; i++) {
        tableau[i] = (double*)malloc(colonnes * sizeof(double));
    }

    return tableau;
}

void afficher_tableau2d(double** tableau, int lignes, int colonnes){
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            printf(" %0.3lf ", tableau[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void detruire_tableau2d(double** tableau, int lignes){
    for (int i = 0; i < lignes; i++) {
        free(tableau[i]);
    }
    free(tableau);
    tableau = NULL;

    printf("%0.1p", tableau);
    printf("\n");
}

void initialiser_tableau2d(double** tableau, int lignes, int colonnes, double valeur){
    tableau = (double**)realloc(tableau, lignes * sizeof(double*));
    for (int i = 0; i < lignes; i++){
        tableau[i] = (double*)realloc(tableau, colonnes * sizeof(double));
        for (int j = 0; j < colonnes; i++){
            tableau[i][j] = valeur;
        }
    }
}


/****************************************************************************************
*                           DEFINTION DES FONCTIONS PRIVEES                            *
****************************************************************************************/
