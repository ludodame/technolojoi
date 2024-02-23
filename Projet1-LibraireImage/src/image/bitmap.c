/****************************************************************************************
    BITMAP.C
    
    Auteur  : Francis Bourdeau
    Date    : 3 juin 2014
    
    Ce module contient des sous-programmes qui permet de lire une image contenu
    dans une fichier bitmap et d'�crire une image dans un nouveau fichier
    bitmap.
****************************************************************************************/
#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>



/****************************************************************************************
*                               D�FINTION DES CONSTANTES                                *
****************************************************************************************/

// Le nombre de bits dans un octet. Devrait toujours �tre 8.
#define BITS_PAR_OCTET              8

// Le nombre d'octet d'une colonne doit �tre un multiple de cette valeur.
#define MULTIPLE_TAILLE_COLONNE     4

// Les deux premiers caract�res contenu dans un fichier bitmap.
#define ID1 'B'
#define ID2 'M'

// Indique qu'il n'y a pas de compression.
#define SANS_COMPRESSION    0

// Le nombre de pixel par m�tre lorsqu'on �crit une image.
#define PIXEL_PAR_METRE     4000

// Le nombre de bits dans une image RGB et le nombre de couleurs.
#define NB_BITS_3_COULEURS  24

// Les valeurs binaires.
#define VRAI    1
#define FAUX    0

#pragma pack(1)

/*
    T_ENTETE_BMP
  
    Le type t_entete_bmp contient l'information de l'ent�te du fichier bitmap.
    Cette ent�te permet de v�rifier le format du fichier et de voir s'il est
    corrompu.
*/
typedef struct
{    
    unsigned char id[2];            // Le symbole BM, pour v�rifier le format du fichier.
    unsigned int taille;            // La taille du fichier en octets.
    unsigned short reserve_1;       // Deux valeurs r�serv�es qui d�pendent de l'application qui 
    unsigned short reserve_2;       // a cr��e le fichier.
    unsigned int debut_image;       // Le position du d�but de l'image, en octets.
    
}t_entete_bmp;


/*
    T_ENTETE_DIB

    Le type t_entete_dib contient l'information � propos de l'image contenu
    dans le fichier (BITMAPINFOHEADER)
*/
typedef struct 
{
    int  taille_entete;             // La taille en octets de l'ent�te DIB. 
                                    // Devrait toujours �tre 40.
    int   largeur;                  // La largeur de l'image (nombre de colonnes).
    int   hauteur;                  // La hauteur de l'image (nombre de lignes).
    short nb_plans_couleur;         // Le nombre de "plan" de couleur. Devrait toujours 
                                    // �tre 1.
    short nb_bits_pixel;            // Le nombre de bits par pixel.
    int type_compression;           // Le type de compression utilis�.
    int taille;                     // La taille de l'image en octets.
    int resolution_horizontale;     // La r�solution horizontale de l'image en 
                                    // pixel par m�tre.
    int resolution_verticale;       // La r�solution verticale de l'image en pixel 
                                    // par m�tre.
    int nb_couleurs_palette;        // Le nombre de couleur dans la palette.
    int nb_couleurs_importantes;    // Le nombre de couleurs importantes. Devrait �tre 0.

}t_entete_dib;

#pragma pack(8)


// Repr�sentation d'un byte en C.
typedef unsigned char byte;



/****************************************************************************************
*                           D�CLARATION DES FONCTIONS PRIV�ES                           *
****************************************************************************************/


/*
    OCTETS_A_SAUTER
    
    Cette fonction permet d'obtenir le nombre d'octets � sauter lorsqu'une
    image .bmp est lue ou �crite. Il faut sauter des octets parce que
    le nombre d'octets pour une colonne doit �tre un multiple de 4.
    
    Param�tres:
      [int] nb_bits_par_pixel   : Le nombre de bits par pixel, souvent 24.
      [int] nb_colonnes         : Le nombre de lignes de l'image.
    
    Retour: Le nombre d'octets � sauter par ligne.
*/    
static int octets_a_sauter(int nb_bits_par_pixel, int nb_colonnes);



/*
    EST_BITMAP
    
    Cette fonction permet de v�rifier si le ficher ouvert respecte le format
    bitmap. Dans un bitmap, les 2 premiers octets devrait repr�senter les
    caract�res "BM".
    
    Param�tre:
      - [t_entete_bmp] entete: L'entete lue au d�but du fichier qui contient le 
                               type de l'image charg�e.
    
    Retour: 1 si le fichier est un bitmap, 0 sinon.
*/    
static int est_bitmap(t_entete_bmp entete);



/*
    CONVERSION_2D_A_1D

    Cette fonction permet de convertir une image (tableau 3D) en un
    tableau 1D sous le format du standard bitmap.
    
    Param�tre:
      - [void *] image: L'image � convertir.
      - [int   ] nb_lignes      : Le nombre de lignes de l'image finale.
      - [int   ] nb_colonnes    : Le nombre de colonnes de l'image finale.
    
    Retour: L'image sous la forme d'un tableau 1D.
*/
static void* conversion_2D_a_1D(void* image, int nb_lignes, int nb_colonnes);



/*
    CONVERSION_1D_A_2D
    Cette fonction converti une image qui a �t� vectoris�e (1D) en tableau
    2D.
    
    Param�tres:
      - [void* ] image_1D       : L'image 1D � modifier.
      - [int   ] nb_lignes      : Le nombre de lignes de l'image finale.
      - [int   ] nb_colonnes    : Le nombre de colonnes de l'image finale.
      - [int   ] nb_bits_pixel  : Le nombre de bits par pixel de l'image.
    
    Retour: L'image re�u en param�tre, mais organis�e en tableau 2D.
*/    
static void* conversion_1D_a_2D(void* image_1D, int nb_lignes,
                                                int nb_colonnes,
                                                int nb_bits_pixel);



/*
    CREER_IMAGE_2D

    Cette fonction g�n�re les tableaux n�cessaires pour repr�senter une
    image RGB de 'nb_lignes' et 'nb_colonnes'
    
    Param�tres:
      - [void* ] image_1D       : L'image 1D � modifier.
      - [int   ] nb_lignes      : Le nombre de lignes de l'image finale.
      - [int   ] nb_colonnes    : Le nombre de colonnes de l'image finale.
      - [int   ] nb_bits_pixel  : Le nombre de bits par pixel de l'image.
    
    Retour: L'image re�u en param�tre, mais organis�e en tableau 2D.
*/    
static double** creer_image_2D(int nb_lignes, int nb_colonnes);



/*
    CLAMP
    
    Garder une valeur dans un intervalle.
    
    Param�tres:
      [double] valeur : La valeur a tester.
      [double] min    : La valeur minimum acceptable.
      [double] max    : La valeur maximum acceptable.
    
    Retour: La valeur dans l'intervale.
*/    
static double clamp(double valeur, double min, double max);



/****************************************************************************************
*                           D�FINTION DES FONCTIONS PUBLIQUES                            *
****************************************************************************************/
int lire(char* nom_fichier, void** image, int* nb_lignes, int* nb_colonnes)
{
    int             a_ete_charger;  // La r�ussite ou l'�chec de la lecture du fichier.
    FILE*           no_fichier;     // Le num�ro du fichier.
    t_entete_bmp    entete_bmp;     // L'ent�te du fichier bitmap.
    t_entete_dib    entete_dib;     // L'ent�te qui contient l'information sur l'image.
    byte*           image_1D;       // L'image qui sera retourn�e par la fonction dans 
                                    // un tableau 1D.
    
    // On emet comme hypothese que le fichier ne sera pas charg�.
    a_ete_charger = FAUX;
    
    // Ouvrir l'image re�u en lecture binaire.
    no_fichier = fopen(nom_fichier, "rb");
    if(no_fichier != NULL)
    {
        // Lire les ent�tes du fichier.
        fread(&entete_bmp, sizeof(entete_bmp), 1, no_fichier);
        fread(&entete_dib, sizeof(entete_dib), 1, no_fichier);

        // V�rifier si le fichier respecte le format bitmap.
        if(est_bitmap(entete_bmp))
        {

            // Si il y a de la compression ou si le nombre de bits par pixel n'est pas 24, on arr�te.
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
            
                // L'image est charg� avec success.
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
    byte* image_1D;             // L'image � �crire sur le disque, en version 1D.
    t_entete_bmp entete_bmp;    // Les deux ent�tes du fichier bitmap.
    t_entete_dib entete_dib;
    long    taille_image;       // Le nombre de byte de donn�es dans l'image.
            
    // Tranformer l'image 3D en un tableau 1D.
    image_1D     = (byte*) conversion_2D_a_1D(image, nb_lignes, nb_colonnes);
    taille_image =  nb_lignes*(nb_colonnes * NB_COULEURS_RGB +
                               octets_a_sauter(NB_BITS_3_COULEURS, nb_colonnes));

    // Initialiser les ent�tes.
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
    
    // Ouvrir le fichier en �criture binaire.
    no_fichier = fopen(nom_fichier, "wb");
    if(no_fichier != NULL)
    {
    
        // �crire les deux ent�tes.
        fwrite(&entete_bmp, sizeof(entete_bmp), 1, no_fichier);
        fwrite(&entete_dib, sizeof(entete_dib), 1, no_fichier);
    
        // �crire l'image.
        fwrite(image_1D, entete_dib.taille, 1, no_fichier);
    
        // Fermer le fichier.
        fclose(no_fichier);
    }
}


void detruire(void* image, int nb_lignes, int nb_colonnes)
{
    int i;              // It�rateur pour cr�er chaques colonnes de l'images.
    double** image2D;   // L'image � cr�er.

    // On convertie l'image.
    image2D = (double**) image;
    
    // On doit lib�rer le tableau repr�sentant chaque ligne
    for(i=0; i<nb_lignes; i++)
    {
        free(image2D[i]);
    }

    //On peut maintenant lib�rer le tableau de ligne.
    free(image2D);
}

/****************************************************************************************
*                           D�FINTION DES FONCTIONS PRIV�ES                             *
****************************************************************************************/


static int octets_a_sauter(int nb_bits_par_pixel, int nb_colonnes)
{
    int nb_octets;          // Le nombre d'octet � sauter sur chaque colonne.
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
    byte* image1D;          // L'image � retourner.
    double** image2D;       // L'image � convertir.
    int ligne;              // It�rateurs pour les lignes, colonnes et 
    int colonne;            // couleurs de l'image 2D.
    int couleur;
    long n;                 // It�rateurs pour parcourir l'image 1D.
    int octets_tampon;      // Le nombre d'octet � ignorer sur chaque colonne.
    long taille1D;          // La taille de l'image vectoris�.
    
    // On convertie l'image � traiter dans le bon type.
    image2D = (double**) image;
    
    // Calculer le nombre d'octets � ignorer sur chaque colonne.
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
        
        // Apr�s chaque colonne, on saut des octets.
        n += octets_tampon;
    }
    
    // Retourner l'image 1D.
    return (void*) image1D;
}

static void* conversion_1D_a_2D(void* image_1D, int nb_lignes,
                                                int nb_colonnes,
                                                int nb_bits_pixel)
{
    double** image;        // L'image � retourner.
    byte* image1D;          // L'image � convertir.
    int ligne;              // It�rateurs pour les lignes, colonnes et les 
    int colonne;            // couleurs de l'image.
    int couleur;
    int pixel_gris;         // La conversion d'un pixel RGB en gris.
    long n;                 // It�rateurs pour parcourir l'image 1D.
    int octets_tampon;      // Le nombre d'octet � ignorer sur chaque colonne.
    
    // On typecast l'image re�u
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

        // Apr�s chaque colonne, on saut des octets.
        n += octets_tampon;
    }
    
    // Retourner l'image 2D.
    return (void*) image;
}


static double** creer_image_2D(int nb_lignes, int nb_colonnes)
{
    int i;              // It�rateur pour cr�er chaques colonnes de l'images.
    double** image;     // L'image � cr�er.

    // On cr�e un tableau qui contiendra chaque ligne de l'image.
    image = (double**) malloc(nb_lignes * sizeof(double*));
    
    // Pour chaque ligne, on lui ajoute le nombre n�cessaire de colonnes.
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

    // On ramene valeur a l'int�rieur de l'intervalle lorsqu'elle est trop petite.
    if (valeur < min)
        resultat = min;

    // On ramene valeur a l'int�rieur de l'intervalle lorsqu'elle est trop grande.
    else if (valeur > max)
        resultat = max;
    
    // Valeur est deja dans l'intervalle.
    else
        resultat = valeur;
    
    return resultat;
}