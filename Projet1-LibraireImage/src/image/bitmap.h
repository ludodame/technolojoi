/****************************************************************************************
    BITMAP.H
    
    Auteur  : Francis Bourdeau
    Date    : 3 juin 2014
    
    Ce module contient des sous-programmes qui permet de lire une image contenu
    dans une fichier bitmap et d'écrire une image dans un nouveau fichier
    bitmap.
    
    Une image est représenté sous la forme d'un tableau 3D. Puisque le seul
    type d'image supporté est RGB, toutes les images sont de tailles
    NL x NC x 3 ou NL et NC sont le nombre de lignes et de colonnes de l'image,
    respectivement. La troisième dimension contient l'information des couleurs:
    rouge, vert et bleu.
    
    Les types d'images supportées sont:
      - RGB 24 bits sans compression.
    
    Liste des sous-programmes publiques:
      - lire     : Permet de lire une image contenu dans un fichier .bmp;
      - ecrire   : Permet d'écrire une image dans un fichier .bmp.
      - detruire : Permet de libérer la mémoire allouée lors du chargement d'une image. 
    
*****************************************************************************************/

#ifndef ETS_INF_BITMAP
#define ETS_INF_BITMAP


/****************************************************************************************
*                               DÉFINTION DES CONSTANTES                                *
****************************************************************************************/

//
// La position des couleurs dans l'image.
//
#define ROUGE               0
#define VERT                1
#define BLEU                2
#define NB_COULEURS_RGB     3


/****************************************************************************************
*                       DÉCLARATION DES FONCTIONS PUBLIQUES                             *
****************************************************************************************/


/*
    LIRE

    Cette fonction permet de lire le contenu d'un fichier bitmap et d'obtenir
    l'image qu'il contient sous la forme d'un tableau 2D. L'image sera 
    convertie en niveau de gris.
    
    Si le fichier n'existe pas ou n'est pas du bon format, LIRE retourne 0. 
    Si l'image est chargé correctement, LIRE retourne 1.
    
    Paramètre:
        - [char* ] nom_fichier: Le chemin du fichier à ouvrir.
        - [void**] image     : L'addresse d'un pointeur non initialisé qui recevra l'image.

        - [int*  ] nb_lignes   : Le nombre de lignes dans l'image lue.
        - [int*  ] nb_colonnes : Le nombre de colonnes dans l'image lue.

    Retour: 
        1 si l'image est lu correctement, 0 sinon.
    
    Exemple d'utilisation:
    
        void* image;
        int   a_ete_charger;
        int   nb_lignes;
        int   nb_colonnes;

        a_ete_charger = lire("C:\Users\Nom\Documents\go_habs_go.bmp", &image, 
                                                                      &nb_lignes,
                                                                      &nb_colonnes);
        if(a_ete_charger)
        {
            [...]
        }
*/    
int lire(char* nom_fichier, void** image, int* nb_lignes, int* nb_colonnes);



/*
    ECRIRE

    Cette procédure permet de créer un fichier .bmp qui contient une image
    reçu en paramètre.
    
    Paramètres:
        - [char* ] nom_fichier : Le nom du fichier à créer.
        - [void* ] image       : Un tableau 2D qui contient l'image à sauvegarder.
        - [int   ] nb_lignes   : Le nombre de lignes dans l'image à sauvegarder.
        - [int   ] nb_colonnes : Le nombre de colonnes dans l'image à sauvegarder.
    
    Retour: 
        Aucun.

    Exemple d'utilisation :
        void* image;
        int nb_lignes;
        int nb_colonnes;

        [ ... Charger et traiter une image ... ]

        ecrire("test.bmp", image, nb_lignes, nb_colonnes);
*/    
void ecrire(char* nom_fichier, void* image, int nb_lignes, int nb_colonnes);



/*
    DETRUIRE

    Cette procédure permet de libérer la mémoire utiliser pour représenter l'image
    d'un fichier .bmp.
    
    Paramètres:
        - [void* ] image       : Un tableau 3D qui contient l'image à libérer.
    
    Retour: 
        Aucun.

        
    Exemple d'utilisation :
        void* image;
        int nb_lignes;
        int nb_colonnes;

        [ ... Charger et traiter une image ... ]

        detruire(image, nb_lignes, nb_colonnes);
*/    
void detruire(void* image, int nb_lignes, int nb_colonnes);


#endif