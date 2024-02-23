/****************************************************************************************
    BITMAP.C
    
    Auteur  : Francis Bourdeau
    Date    : 3 juin 2014
    
    Ce module contient des sous-programmes qui permet de lire une image contenu
    dans une fichier bitmap et d'écrire une image dans un nouveau fichier
    bitmap.
****************************************************************************************/
#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>



/****************************************************************************************
*                               DÉFINTION DES CONSTANTES                                *
****************************************************************************************/

// Le nombre de bits dans un octet. Devrait toujours être 8.
#define BITS_PAR_OCTET              8

// Le nombre d'octet d'une colonne doit être un multiple de cette valeur.
#define MULTIPLE_TAILLE_COLONNE     4

// Les deux premiers caractères contenu dans un fichier bitmap.
#define ID1 'B'
#define ID2 'M'

// Indique qu'il n'y a pas de compression.
#define SANS_COMPRESSION    0

// Le nombre de pixel par mètre lorsqu'on écrit une image.
#define PIXEL_PAR_METRE     4000

// Le nombre de bits dans une image RGB et le nombre de couleurs.
#define NB_BITS_3_COULEURS  24

// Les valeurs binaires.
#define VRAI    1
#define FAUX    0

#pragma pack(1)

/*
    T_ENTETE_BMP
  
    Le type t_entete_bmp contient l'information de l'entête du fichier bitmap.
    Cette entête permet de vérifier le format du fichier et de voir s'il est
    corrompu.
*/
typedef struct
{    
    unsigned char id[2];            // Le symbole BM, pour vérifier le format du fichier.
    unsigned int taille;            // La taille du fichier en octets.
    unsigned short reserve_1;       // Deux valeurs réservées qui dépendent de l'application qui 
    unsigned short reserve_2;       // a créée le fichier.
    unsigned int debut_image;       // Le position du début de l'image, en octets.
    
}t_entete_bmp;


/*
    T_ENTETE_DIB

    Le type t_entete_dib contient l'information à propos de l'image contenu
    dans le fichier (BITMAPINFOHEADER)
*/
typedef struct 
{
    int  taille_entete;             // La taille en octets de l'entête DIB. 
                                    // Devrait toujours être 40.
    int   largeur;                  // La largeur de l'image (nombre de colonnes).
    int   hauteur;                  // La hauteur de l'image (nombre de lignes).
    short nb_plans_couleur;         // Le nombre de "plan" de couleur. Devrait toujours 
                                    // être 1.
    short nb_bits_pixel;            // Le nombre de bits par pixel.
    int type_compression;           // Le type de compression utilisé.
    int taille;                     // La taille de l'image en octets.
    int resolution_horizontale;     // La résolution horizontale de l'image en 
                                    // pixel par mètre.
    int resolution_verticale;       // La résolution verticale de l'image en pixel 
                                    // par mètre.
    int nb_couleurs_palette;        // Le nombre de couleur dans la palette.
    int nb_couleurs_importantes;    // Le nombre de couleurs importantes. Devrait être 0.

}t_entete_dib;

#pragma pack(8)


// Représentation d'un byte en C.
typedef unsigned char byte;



/****************************************************************************************
*                           DÉCLARATION DES FONCTIONS PRIVÉES                           *
****************************************************************************************/


/*
    OCTETS_A_SAUTER
    
    Cette fonction permet d'obtenir le nombre d'octets à sauter lorsqu'une
    image .bmp est lue ou écrite. Il faut sauter des octets parce que
    le nombre d'octets pour une colonne doit être un multiple de 4.
    
    Paramètres:
      [int] nb_bits_par_pixel   : Le nombre de bits par pixel, souvent 24.
      [int] nb_colonnes         : Le nombre de lignes de l'image.
    
    Retour: Le nombre d'octets à sauter par ligne.
*/    
static int octets_a_sauter(int nb_bits_par_pixel, int nb_colonnes);



/*
    EST_BITMAP
    
    Cette fonction permet de vérifier si le ficher ouvert respecte le format
    bitmap. Dans un bitmap, les 2 premiers octets devrait représenter les
    caractères "BM".
    
    Paramètre:
      - [t_entete_bmp] entete: L'entete lue au début du fichier qui contient le 
                               type de l'image chargée.
    
    Retour: 1 si le fichier est un bitmap, 0 sinon.
*/    
static int est_bitmap(t_entete_bmp entete);



/*
    CONVERSION_2D_A_1D

    Cette fonction permet de convertir une image (tableau 3D) en un
    tableau 1D sous le format du standard bitmap.
    
    Paramètre:
      - [void *] image: L'image à convertir.
      - [int   ] nb_lignes      : Le nombre de lignes de l'image finale.
      - [int   ] nb_colonnes    : Le nombre de colonnes de l'image finale.
    
    Retour: L'image sous la forme d'un tableau 1D.
*/
static void* conversion_2D_a_1D(void* image, int nb_lignes, int nb_colonnes);



/*
    CONVERSION_1D_A_2D
    Cette fonction converti une image qui a été vectorisée (1D) en tableau
    2D.
    
    Paramètres:
      - [void* ] image_1D       : L'image 1D à modifier.
      - [int   ] nb_lignes      : Le nombre de lignes de l'image finale.
      - [int   ] nb_colonnes    : Le nombre de colonnes de l'image finale.
      - [int   ] nb_bits_pixel  : Le nombre de bits par pixel de l'image.
    
    Retour: L'image reçu en paramètre, mais organisée en tableau 2D.
*/    
static void* conversion_1D_a_2D(void* image_1D, int nb_lignes,
                                                int nb_colonnes,
                                                int nb_bits_pixel);



/*
    CREER_IMAGE_2D

    Cette fonction génère les tableaux nécessaires pour représenter une
    image RGB de 'nb_lignes' et 'nb_colonnes'
    
    Paramètres:
      - [void* ] image_1D       : L'image 1D à modifier.
      - [int   ] nb_lignes      : Le nombre de lignes de l'image finale.
      - [int   ] nb_colonnes    : Le nombre de colonnes de l'image finale.
      - [int   ] nb_bits_pixel  : Le nombre de bits par pixel de l'image.
    
    Retour: L'image reçu en paramètre, mais organisée en tableau 2D.
*/    
static double** creer_image_2D(int nb_lignes, int nb_colonnes);



/*
    CLAMP
    
    Garder une valeur dans un intervalle.
    
    Paramètres:
      [double] valeur : La valeur a tester.
      [double] min    : La valeur minimum acceptable.
      [double] max    : La valeur maximum acceptable.
    
    Retour: La valeur dans l'intervale.
*/    
static double clamp(double valeur, double min, double max);



/****************************************************************************************
*                           DÉFINTION DES FONCTIONS PUBLIQUES                            *
****************************************************************************************/
int lire(char* nom_fichier, void** image, int* nb_lignes, int* nb_colonnes)
{
    int             a_ete_charger;  // La réussite ou l'échec de la lecture du fichier.
    FILE*           no_fichier;     // Le numéro du fichier.
    t_entete_bmp    entete_bmp;     // L'entête du fichier bitmap.
    t_entete_dib    entete_dib;     // L'entête qui contient l'information sur l'image.
    byte*           image_1D;       // L'image qui sera retournée par la fonction dans 
                                    // un tableau 1D.
    
    // On emet comme hypothese que le fichier ne sera pas chargé.
    a_ete_charger = FAUX;
    
    // Ouvrir l'image reçu en lecture binaire.
    no_fichier = fopen(nom_fichier, "rb");
    if(no_fichier != NULL)
    {
        // Lire les entêtes du fichier.
        fread(&entete_bmp, sizeof(entete_bmp), 1, no_fichier);
        fread(&entete_dib, sizeof(entete_dib), 1, no_fichier);

        // Vérifier si le fichier respecte le format bitmap.
        if(est_bitmap(entete_bmp))
        {

            // Si il y a de la compression ou si le nombre de bits par pixel n'est pas 24, on arrête.
            if(entete_dib.type_compression == SANS_COMPRESSION && 
               entete_dib.nb_bits_pixel    == NB_BITS_3_COULEURS)
            {
        
                // Lire l'image (tableau de pixels) au complet.
                image_1D = (byte*) malloc(entete_dib.taille * sizeof(byte));
                fread(image_1D, sizeof(byte), entete_dib.taille, no_fichier);
            
                // Transformer l'image en tableau 3D.
                (*image) = conversion_1D_a_2D(image_1D, entete_dib.hauteur,
                                                        entete_dib.largeur, 
                                                        entete_dib.nb_bits_pixel);
            
                // L'image est chargé avec success.
                *nb_lignes    = entete_dib.hauteur;
                *nb_colonnes  = entete_dib.largeur;
                a_ete_charger = VRAI;
        
            }
        }
        
        // Fermer le fichier.
        fclose(no_fichier);
    }

    return a_ete_charger;
}



void ecrire(char* nom_fichier, void* image, int nb_lignes, int nb_colonnes)
{
    FILE* no_fichier;           // L'identificateur du fichier.
    byte* image_1D;             // L'image à écrire sur le disque, en version 1D.
    t_entete_bmp entete_bmp;    // Les deux entêtes du fichier bitmap.
    t_entete_dib entete_dib;
    long    taille_image;       // Le nombre de byte de données dans l'image.
            
    // Tranformer l'image 3D en un tableau 1D.
    image_1D     = (byte*) conversion_2D_a_1D(image, nb_lignes, nb_colonnes);
    taille_image =  nb_lignes*(nb_colonnes * NB_COULEURS_RGB +
                               octets_a_sauter(NB_BITS_3_COULEURS, nb_colonnes));

    // Initialiser les entêtes.
    entete_bmp.id[0] = ID1;
    entete_bmp.id[1] = ID2;
    entete_bmp.debut_image = sizeof(entete_bmp) + sizeof(entete_dib);
    entete_bmp.taille      = sizeof(entete_bmp) + sizeof(entete_dib) + taille_image;
                             
                                                
    
    entete_dib.taille_entete = sizeof(entete_dib);
    entete_dib.largeur = nb_colonnes;
    entete_dib.hauteur = nb_lignes;
    entete_dib.nb_plans_couleur = 1;
    entete_dib.nb_bits_pixel = NB_BITS_3_COULEURS;
    entete_dib.type_compression = SANS_COMPRESSION;
    entete_dib.taille =taille_image;
    entete_dib.resolution_horizontale = PIXEL_PAR_METRE;
    entete_dib.resolution_verticale = PIXEL_PAR_METRE;
    entete_dib.nb_couleurs_palette = 0;
    entete_dib.nb_couleurs_importantes = 0;
    
    // Ouvrir le fichier en écriture binaire.
    no_fichier = fopen(nom_fichier, "wb");
    if(no_fichier != NULL)
    {
    
        // Écrire les deux entêtes.
        fwrite(&entete_bmp, sizeof(entete_bmp), 1, no_fichier);
        fwrite(&entete_dib, sizeof(entete_dib), 1, no_fichier);
    
        // Écrire l'image.
        fwrite(image_1D, entete_dib.taille, 1, no_fichier);
    
        // Fermer le fichier.
        fclose(no_fichier);
    }
}


void detruire(void* image, int nb_lignes, int nb_colonnes)
{
    int i;              // Itérateur pour créer chaques colonnes de l'images.
    double** image2D;   // L'image à créer.

    // On convertie l'image.
    image2D = (double**) image;
    
    // On doit libérer le tableau représentant chaque ligne
    for(i=0; i<nb_lignes; i++)
    {
        free(image2D[i]);
    }

    //On peut maintenant libérer le tableau de ligne.
    free(image2D);
}

/****************************************************************************************
*                           DÉFINTION DES FONCTIONS PRIVÉES                             *
****************************************************************************************/


static int octets_a_sauter(int nb_bits_par_pixel, int nb_colonnes)
{
    int nb_octets;          // Le nombre d'octet à sauter sur chaque colonne.
    long taille_colonne;    // La taille en octets d'une colonne. 
    
    // On calcule le nombre d'octet sur une colonne de l'image.
    taille_colonne = nb_colonnes * nb_bits_par_pixel / BITS_PAR_OCTET;
    
    // Tant que le nombre d'octets d'une colonne n'est pas un multiple
    // de 4, on saute un octet de plus.
    nb_octets = 0;
    while( (taille_colonne + nb_octets) % MULTIPLE_TAILLE_COLONNE != 0)
        nb_octets++;
    
    return nb_octets;
}


static int est_bitmap(t_entete_bmp entete)
{
    return entete.id[0] == ID1 && 
           entete.id[1] == ID2 ;
}


static void* conversion_2D_a_1D(void* image, int nb_lignes, int nb_colonnes)
{
    byte* image1D;          // L'image à retourner.
    double** image2D;       // L'image à convertir.
    int ligne;              // Itérateurs pour les lignes, colonnes et 
    int colonne;            // couleurs de l'image 2D.
    int couleur;
    long n;                 // Itérateurs pour parcourir l'image 1D.
    int octets_tampon;      // Le nombre d'octet à ignorer sur chaque colonne.
    long taille1D;          // La taille de l'image vectorisé.
    
    // On convertie l'image à traiter dans le bon type.
    image2D = (double**) image;
    
    // Calculer le nombre d'octets à ignorer sur chaque colonne.
    octets_tampon = octets_a_sauter(NB_BITS_3_COULEURS, nb_colonnes);
    
    // Ajuster l'image de retour.
    taille1D = (nb_lignes*(nb_colonnes * NB_COULEURS_RGB + octets_tampon)*sizeof(byte));
    image1D  = (byte*) malloc(taille1D);

    // Copier l'information de l'image 3D vers l'image 1D.
    n = 0;
    for (ligne = nb_lignes-1; ligne >= 0; ligne--)
    {
        for (colonne = 0; colonne < nb_colonnes; colonne++)
        {
            for (couleur = NB_COULEURS_RGB-1; couleur>=0; couleur--)
            {
                image1D[n] = (byte) clamp(image2D[ligne][colonne] * 255, 0, 255);
                n++;
            }
        }
        
        // Après chaque colonne, on saut des octets.
        n += octets_tampon;
    }
    
    // Retourner l'image 1D.
    return (void*) image1D;
}

static void* conversion_1D_a_2D(void* image_1D, int nb_lignes,
                                                int nb_colonnes,
                                                int nb_bits_pixel)
{
    double** image;        // L'image à retourner.
    byte* image1D;          // L'image à convertir.
    int ligne;              // Itérateurs pour les lignes, colonnes et les 
    int colonne;            // couleurs de l'image.
    int couleur;
    int pixel_gris;         // La conversion d'un pixel RGB en gris.
    long n;                 // Itérateurs pour parcourir l'image 1D.
    int octets_tampon;      // Le nombre d'octet à ignorer sur chaque colonne.
    
    // On typecast l'image reçu
    image1D = (byte*) image_1D;

    // On calculue le nombre d'octet
    octets_tampon = octets_a_sauter(nb_bits_pixel, nb_colonnes);
    
    // Ajuster l'image de retour.
    image = creer_image_2D(nb_lignes, nb_colonnes);

    // Copier l'information de l'image 1D vers l'image 2D.
    n = 0;
    for(ligne = nb_lignes-1; ligne >= 0; ligne--)
    {
        for(colonne = 0; colonne < nb_colonnes; colonne++)
        {
            // On converti un pixel RGB en niveau de gris.
            pixel_gris = 0;
            for(couleur = NB_COULEURS_RGB -1; couleur >= 0; couleur--, n++)
            {
                pixel_gris += image1D[n];
            }

            image[ligne][colonne] =  ((double) pixel_gris) / (NB_COULEURS_RGB * 255);
        }

        // Après chaque colonne, on saut des octets.
        n += octets_tampon;
    }
    
    // Retourner l'image 2D.
    return (void*) image;
}


static double** creer_image_2D(int nb_lignes, int nb_colonnes)
{
    int i;              // Itérateur pour créer chaques colonnes de l'images.
    double** image;     // L'image à créer.

    // On crée un tableau qui contiendra chaque ligne de l'image.
    image = (double**) malloc(nb_lignes * sizeof(double*));
    
    // Pour chaque ligne, on lui ajoute le nombre nécessaire de colonnes.
    for(i=0; i<nb_lignes; i++)
    {
        image[i] = (double*) malloc(nb_colonnes * sizeof(double));
    }

    // On retourne l'addresse de l'image.
    return image;
}

static double clamp(double valeur, double min, double max)
{
    double resultat;

    // On ramene valeur a l'intérieur de l'intervalle lorsqu'elle est trop petite.
    if (valeur < min)
        resultat = min;

    // On ramene valeur a l'intérieur de l'intervalle lorsqu'elle est trop grande.
    else if (valeur > max)
        resultat = max;
    
    // Valeur est deja dans l'intervalle.
    else
        resultat = valeur;
    
    return resultat;
}