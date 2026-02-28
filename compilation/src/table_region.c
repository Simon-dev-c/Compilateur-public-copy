#include <stdio.h>
#include <string.h>
#include "table_region.h"
#include "arbre.h"

int pcv_r = 0;
int sommet = -1; // sommet de pile
int region_courante = -1;

TableRegion t_region;
Pile pile_num_region;

// Chebbah/Cinque/Arnaud/Mehenni
void init_table_region()
{
    for (int i = 0; i < TAILLE_TABLE_REGION; i++)
    {
        t_region[i].taille = -1;
        t_region[i].NIS = -1;
        t_region[i].arbre = NULL;
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
void affiche_table_region()
{
    int i;
    printf("Affiche du tableau des regions : \n\n");
    printf("    |   taille   |   NIS   |   arbre   |\n");
    for (i = 0; i < TAILLE_TABLE_REGION; i++)
    {
        if (t_region[i].NIS != -1)
        {
            printf("%d   | ", i);
            printf("   %d    |      %d     |     %p     |\n", t_region[i].taille, t_region[i].NIS, (void *)t_region[i].arbre);
        }
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
void affiche_pile_region()
{
    int i = 0;
    while (pile_num_region[i] != -1)
    {
        printf("| %d |", pile_num_region[i]);
        i++;
    }
    printf("\n");
}

// Chebbah/Cinque/Arnaud/Mehenni
void inserer_regions()
{
    int NIS = recup_taille_pile();

    if (pcv_r >= TAILLE_TABLE_REGION)
    {
        printf("Erreur: table des regions pleine!\n");
        return;
    }

    // Si on est dans la regio du prog princ, il n'y a pas de chainage dynamique
    // ni de chainage statique
    if (pcv_r == 0)
        t_region[pcv_r].taille = 0;
    else
        t_region[pcv_r].taille = NIS + 1;

    t_region[pcv_r].NIS = NIS;
    t_region[pcv_r].arbre = NULL;

    pcv_r++;
    // printf("Region insérer : %d \nNIS : %d \nTaille : %d\n",pcv_r-1,t_region[pcv_r-1].NIS,t_region[pcv_r-1].taille);
}

// Chebbah/Cinque/Arnaud/Mehenni
void inserer_arbre_region(arbre a)
{
    t_region[recup_num_region()].arbre = a;
}

// Chebbah/Cinque/Arnaud/Mehenni
void init_num_region()
{
    int i;
    for (i = 0; i < MAX_PILE_NUM_REGION; i++)
    {
        pile_num_region[i] = -1;
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
void incremente_num_region()
{
    region_courante++;
}

// Chebbah/Cinque/Arnaud/Mehenni
void empiler_num_region()
{
    if (sommet < MAX_PILE_NUM_REGION)
    {
        sommet++;
        pile_num_region[sommet] = region_courante;
    }
    // printf("Pile augmenter : %d à l'ind %d \n", region_courante, sommet);
}

// Chebbah/Cinque/Arnaud/Mehenni
void depiler_region()
{
    if (sommet >= 0)
    {
        pile_num_region[sommet] = -1;
        sommet--;
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
int recup_num_region()
{
    if (sommet == -1)
        return -1;

    return pile_num_region[sommet];
}

// Chebbah/Cinque/Arnaud/Mehenni
int recup_top_pile_num_region()
{
    return sommet;
}

// Renvoie:
//  Si r englobe region courante :
//   le nombre de region englobante entre r et region courante
//   donc un entier >= 0
//  Sinon
//    -1
// Chebbah/Cinque/Arnaud/Mehenni
int englobe_region_courante(int r)
{
    int i = sommet;
    int res = -1;

    while (i > -1)
    {
        if (pile_num_region[i] == r)
        {
            res = sommet - i;
            i = -1;
        }

        i--;
    }

    return res;
}

// Chebbah/Cinque/Arnaud/Mehenni
int recup_taille_pile()
{
    int i = 0;
    while (pile_num_region[i] != -1)
    {
        i++;
    }
    return i;
}

// Chebbah/Cinque/Arnaud/Mehenni
int recup_nis_region(int region)
{
    return t_region[region].NIS;
}

// Retourne le déplacement courant dans la région courante
// Chebbah/Cinque/Arnaud/Mehenni
int recup_depl_courant()
{
    if (region_courante == -1 || region_courante >= TAILLE_TABLE_REGION)
    {
        fprintf(stderr, "ERREUR get_depl_courant(): region_courante=%d invalide\n", region_courante);
        return 0;
    }
    return t_region[region_courante].taille;
}

// Incrémenter le déplacement de la région courante
// Chebbah/Cinque/Arnaud/Mehenni
void increment_depl_region(int taille_var)
{
    if (region_courante == -1 || region_courante >= TAILLE_TABLE_REGION)
    {
        fprintf(stderr, "ERREUR increment_depl_region(): region_courante=%d invalide\n", region_courante);
        return;
    }
    t_region[region_courante].taille += taille_var;
}

// Chebbah/Cinque/Arnaud/Mehenni
arbre recup_arbre_region(int num_region)
{
    if (num_region < 0 || num_region >= TAILLE_TABLE_REGION)
    {
        fprintf(stderr, "ERREUR recup_arbre_region(): num_region=%d invalide\n", num_region);
        return NULL;
    }
    return t_region[num_region].arbre;
}