/****************************************************************************************
    TABLEAU_1D

*****************************************************************************************/
#include "tableau1d.h"

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

//Reçoit un tableau a une dimension et un nb d'élément.
//Affiche le nb d'élément demandé du tableau.
void afficher_tableau1d(double* tableau, int nb_element) {

    //Affiche l'ensemble du tableau
    for (int i = 0; i < nb_element; i++) {
        printf("%0.3lf  ", tableau[i]);
    }
    printf("\n");
}

//Reçoit un tableau a une dimension et un nb d'élément.
//Affiche la somme des nb contenu dans le nb d'élément demandé.
//Retourne la somme.
double somme_tableau1d(double* tableau, int nb_element) {

    double somme = 0;   //somme du nb précédent et le suivant

    //Calcul du nb précédant + le suivant dans le tableau
    for (int i = 0; i < nb_element; i++) {
        somme = somme + tableau[i];
    }

    //Affiche le résultat
    printf("%0.3lf  ", somme);
    printf("\n");

    return somme;
}

//Reçoit un tableau a une dimension, un nb d'élément et une valeur.
//Affiche le nb d'élément demandé du tableau multiplié par la  valeur.
void produit_tableau1d(double* tableau, int nb_element, double valeur) {

    double produit; //produit de la valeur et du contenu du tableau

    //Calcul et affiche le produit de chaque nb
    for (int i = 0; i < nb_element; i++) {
        produit = valeur * tableau[i];
        printf("%0.3lf  ", produit);
    }
    printf("\n");
}

//Reçoit 2 tableaux a une dimension et un nb d'élément.
//Affiche le produit scalaire des 2 tableaux selon le nb d'élément demandé.
//Retourne le produit scalaire.
double produit_scalaire1d(double* tableau1, double* tableau2, int nb_element) {

    double produit;     //produit de 2 nb
    double scalaire = 0;//somme des produits

    //Calcule le produit scalaire
    for (int i = 0; i < nb_element; i++) {
        produit = tableau1[i] * tableau2[i];
        scalaire = scalaire + produit;
    }

    //Affiche le résultat
    printf("%0.3lf  ", scalaire);
    printf("\n");

    return scalaire;
}

//Reçoit un nb d'élément.
//Crée et affiche un tableau a une dimension de la longueur du nb d'élément.
//Retourne le pointeur du tableau.
double* creer_tableau1d(int nb_element){

    //Crée un tableau
    double* tableau = (double*) malloc(nb_element * sizeof(double));

    //Affiche l'ensemble du tableau
    for (int i = 0; i < nb_element; i++){
        tableau[i] = 0;
        printf("%0.3lf  ", tableau[i]);
    }
    printf("\n");

    return tableau;
}

//Reçoit le pointeur de la fonction creer_tableau1d.
//Détruit le contenu du pointeur.
//Affiche le pointeur détruit (devrait être 0).
void detruire_tableau1d(double* pointeur){

    //Détruit pointeur
    pointeur = NULL;
    printf("%0.1p", pointeur);
    printf("\n");
}

/****************************************************************************************
*                           DEFINTION DES FONCTIONS PRIVEES                            *
****************************************************************************************/
