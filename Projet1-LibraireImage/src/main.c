#include <stdlib.h>
#include <stdio.h>
#include "tableau/tableau1d.h"
#include "tableau/tableau2d.h"

void tester_tableau1d();
void tester_tableau2d();

int main() {

    tester_tableau1d();
    tester_tableau2d();

    return 0;
}
void tester_tableau1d(){

    double tableau1[5] = {1,  2,  3,   4,   5};
    double tableau2[5] = {1.1,2.5,3.0, 4.25,5.123};
    double tableau3[5] = {1.0,1.0,1.0, 1.0, 1.0};
    double tableau4[5] = {1,  2,  3,   4,   5};
    double tableau5[5] = {1.5,2.0,1.25,0.5, 0};
    double tableau6[5] = {0,  0,  0,   0,   0};

    printf("**********************************************************\n");
    printf("*                      TEST TABLEAU1D                    *\n");
    printf("**********************************************************\n");

    printf("\n");
    printf("FONCTION AFFICHER_TABLEAU1D\n");

    double* tableau = tableau1;
    int nb_element = 5;
    printf("T1 : ");
    afficher_tableau1d(tableau, nb_element);

    tableau = tableau2;
    nb_element = 5;
    printf("T2 : ");
    afficher_tableau1d(tableau, nb_element);

    tableau = tableau5;
    nb_element = 5;
    printf("T3 : ");
    afficher_tableau1d(tableau, nb_element);

    printf("\n");
    printf("FONCTION SOMME\n");

    tableau = tableau1;
    nb_element = 5;
    printf("T1 : ");
    somme_tableau1d(tableau, nb_element);

    tableau = tableau2;
    nb_element = 5;
    printf("T2 : ");
    somme_tableau1d(tableau, nb_element);

    tableau = tableau2;
    nb_element = 0;
    printf("T3 : ");
    somme_tableau1d(tableau, nb_element);

    printf("\n");
    printf("FONCTION PRODUIT\n");

    tableau = tableau1;
    nb_element = 5;
    double valeur = 5;
    printf("T1 : ");
    produit_tableau1d(tableau, nb_element, valeur);

    tableau = tableau2;
    nb_element = 5;
    valeur = .1;
    printf("T2 : ");
    produit_tableau1d(tableau, nb_element, valeur);

    tableau = tableau3;
    nb_element = 3;
    valeur = 0;
    printf("T3 : ");
    produit_tableau1d(tableau, nb_element, valeur);

    printf("\n");
    printf("FONCTION PRODUIT SCALERE\n");

    double* tab_01 = tableau4;
    double* tab_02 = tableau5;
    nb_element = 5;
    printf("T1 : ");
    produit_scalaire1d(tab_01, tab_02, nb_element);

    tab_01 = tableau4;
    tab_02 = tableau6;
    nb_element = 5;
    printf("T2 : ");
    produit_scalaire1d(tab_01, tab_02, nb_element);

    printf("\n");
    printf("FONCTION CREER TABLEAU\n");

    nb_element = 5;
    printf("T1 : ");
    creer_tableau1d(nb_element);
    double* pointeur_01 = tableau;


    nb_element = 3;
    printf("T2 : ");
    creer_tableau1d(nb_element);
    double* pointeur_02 = tableau;

    printf("\n");
    printf("FONCTION DETRUIRE TABLEAU\n");

    printf("T1 : ");
    detruire_tableau1d(pointeur_01);

    printf("T1 : ");
    detruire_tableau1d(pointeur_02);

    printf("\n");
}

void tester_tableau2d(){

    int lignes = 3;
    int colonnes = 2;

    double** tableau1 = creer_tableau2d(lignes, colonnes);

    tableau1[0][0] = 1; tableau1[0][1] = 2;
    tableau1[1][0] = 4; tableau1[1][1] = 5;
    tableau1[2][0] = 7; tableau1[2][1] = 8;

    lignes = 3;
    colonnes = 4;

    double** tableau2 = creer_tableau2d(lignes, colonnes);

    tableau2[0][0] = 1.1; tableau2[0][1] = 2.1; tableau2[0][2] = 3.1; tableau2[0][3] = 0.25;
    tableau2[1][0] = 4.1; tableau2[1][1] = 5.1; tableau2[1][2] = 6.1; tableau2[1][3] = 0.33;
    tableau2[2][0] = 7.1; tableau2[2][1] = 8.1; tableau2[2][2] = 9.1; tableau2[2][3] = 0.80;

    lignes = 3;
    colonnes = 2;

    double** tableau3 = creer_tableau2d(lignes, colonnes);

    tableau3[0][0] = 0.25; tableau3[0][1] = 0.50;
    tableau3[1][0] = 0.75; tableau3[1][1] = 1.00;
    tableau3[2][0] = 0.33; tableau3[2][1] = 0.66;

    double** tableau4 = NULL;

    double* tableau5 = NULL;

    printf("**********************************************************\n");
    printf("*                      TEST TABLEAU2D                    *\n");
    printf("**********************************************************\n");

    printf("\n");
    printf("FONCTION AFFICHER_TABLEAU2D\n");

    double** tableau = tableau1;
    lignes = 3;
    colonnes = 2;

    printf("T1 :\n");
    afficher_tableau2d(tableau, lignes, colonnes);

    tableau = tableau2;
    lignes = 3;
    colonnes = 4;

    printf("T2 :\n");
    afficher_tableau2d(tableau, lignes, colonnes);

    tableau = tableau3;
    lignes = 3;
    colonnes = 2;

    printf("T3 :\n");
    afficher_tableau2d(tableau, lignes, colonnes);

    printf("FONCTION DETRUIRE TABLEAU\n");

    tableau = tableau1;
    lignes = 3;

    printf("T1 : ");
    detruire_tableau2d(tableau, lignes);

    tableau = tableau2;
    lignes = 3;

    printf("T2 : ");
    detruire_tableau2d(tableau, lignes);

    tableau = tableau3;
    lignes = 3;

    printf("T3 : ");
    detruire_tableau2d(tableau, lignes);

    printf("FONCTION INITIALISER_TABLEAU2D\n");

    tableau = tableau1;
    lignes = 3;

    printf("T1 : ");
    detruire_tableau2d(tableau, lignes);

}