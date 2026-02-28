#include <stdio.h>
#include <string.h>
#include "table_lexico.h"

/* Var globale */
TableHashCode t_hcode; // - table hash-code
TableLexico t_lexico;  // - table lexico colonnes : longueur | lexeme | suivant (de meme h-code)
int pcv = 0;           // - premiere case vide (pcv), par défut 0

// Chebbah/Cinque/Arnaud/Mehenni
void init_table_lexico()
{
    int i;
    for (i = 0; i < TAILLE_TABLE_LEXICO; i++)
    {
        t_lexico[i].longueur = -1;
        t_lexico[i].suivant = -1;
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
void init_table_hcode()
{
    int i;
    for (i = 0; i < TAILLE_TABLE_HCODE; i++)
        t_hcode[i] = -1;
}

// Chebbah/Cinque/Arnaud/Mehenni
int hcode(char *lexeme)
{
    int hcode = 0;
    while (*lexeme)
        hcode += *lexeme++;

    return hcode % TAILLE_TABLE_HCODE;
}

// Chebbah/Cinque/Arnaud/Mehenni
int inserer_lexeme(char *lexeme, int len)
{
    int hc, indice, nouveau;
    hc = hcode(lexeme);   // hashcode du lexème, qu'on va utiliser pour recup l'indice du 1er lexème de meme hashcode dans la table lexico
    indice = t_hcode[hc]; // maintenant on a l'indice du 1er lexeme de meme hashcode
                          //  et en meme temps on a toute la chaine de tous les lexèmes de meme hashcode grace à "suivant"

    /* On doit d'abord vérifier que le lexeme n'existe pas dejà dans la chaine des lexeme de meme hashcode */
    /* Tant qu'il y a des lexèmes dans la chaine */
    while (indice != -1)
    {
        // Si il est dejà présent dans la chaine alors on ne fait rien de plus et on retourne directement sont indice (dans la table lexico)
        if (len == t_lexico[indice].longueur)
        {
            if (strcmp(t_lexico[indice].lexeme, lexeme) == 0)
                return indice;
        }

        // Sinon on passe au suivant de la chaine, <indice> devient l'indice du suivant
        indice = t_lexico[indice].suivant;
    }

    /* A ce stade on sait que le lexème n'existe pas dans la table lexico */
    /* Donc on l'insert à la premiere position non vide (pcv)  */
    nouveau = pcv;
    t_lexico[nouveau].longueur = len;
    strcpy(t_lexico[nouveau].lexeme, lexeme);
    /*
        Mise à jour du chainage (zone des lexèmes de meme hashcode)
        Dans le chainage des lexèmes de meme hashcode que notre <lexeme>

        on greffe la chaine a notre nouveau lexème donc (t_lexico[nouveau].suivant = t_hcode[hc])
        et la tete de chaine devient de notre nouveau lexème (t_hcode[hc] = nouveau;)

        En gros on ajoute le nouveau lexème en tete de liste.
    */
    t_lexico[nouveau].suivant = t_hcode[hc];
    t_hcode[hc] = nouveau;

    pcv++;

    return nouveau;
}

// Chebbah/Cinque/Arnaud/Mehenni
char *lexeme(int num_lexico)
{
    if (num_lexico < 0 || num_lexico >= pcv)
    {
        return NULL;
    }

    return t_lexico[num_lexico].lexeme;
}

// Chebbah/Cinque/Arnaud/Mehenni
void affiche_tl(FILE *dest)
{
    int i;

    fprintf(dest, "\tTable Lexicographique : \n");
    fprintf(dest, "-----------------------------------------------\n");

    fprintf(dest, "| %-3s | %-8s | %-15s | %-9s |\n", "i", "Longueur", "Lexeme", "Suivant");
    fprintf(dest, "-----------------------------------------------\n");
    for (i = 0; i < TAILLE_TABLE_LEXICO; i++)
    {
        fprintf(dest, "| %3d | %8d | %15s | %9d |\n", i, t_lexico[i].longueur, t_lexico[i].lexeme, t_lexico[i].suivant);
    }
    fprintf(dest, "-----------------------------------------------\n");
}

// Chebbah/Cinque/Arnaud/Mehenni
void affiche_tl_ligne(FILE *dest, int i)
{
    if (i < TAILLE_TABLE_LEXICO)
    {
        fprintf(dest, "| %3d | %8d | %15s | %9d |", i, t_lexico[i].longueur, t_lexico[i].lexeme, t_lexico[i].suivant);
    }
    else
    {
        fprintf(dest, "  %3s   %8s   %15s   %9s  ", "", "", "", "");
    }
}