#ifndef _TABLE_REGION_H_
#define _TABLE_REGION_H_

typedef struct noeud *arbre;

#define TAILLE_TABLE_REGION 100
#define MAX_PILE_NUM_REGION 30

typedef struct
{
    int taille;
    int NIS;
    arbre arbre;
} region;

typedef region TableRegion[TAILLE_TABLE_REGION];
typedef int Pile[MAX_PILE_NUM_REGION];

// Chebbah/Cinque/Arnaud/Mehenni
void init_table_region();
// Chebbah/Cinque/Arnaud/Mehenni
void affiche_table_region();
// Chebbah/Cinque/Arnaud/Mehenni
void affiche_pile_region();
// Chebbah/Cinque/Arnaud/Mehenni
void inserer_regions();
// Chebbah/Cinque/Arnaud/Mehenni
void inserer_arbre_region(arbre a);
// Chebbah/Cinque/Arnaud/Mehenni
void init_num_region();
// Chebbah/Cinque/Arnaud/Mehenni
void incremente_num_region();
// Chebbah/Cinque/Arnaud/Mehenni
void empiler_num_region();
// Chebbah/Cinque/Arnaud/Mehenni
void depiler_region();
// Chebbah/Cinque/Arnaud/Mehenni
int recup_num_region();
// Chebbah/Cinque/Arnaud/Mehenni
int recup_top_pile_num_region();

// Renvoie:
//  Si r englobe region courante :
//   le nombre de region englobante entre r et region courante
//   donc un entier >= 0
//  Sinon
//    -1
// Chebbah/Cinque/Arnaud/Mehenni
int englobe_region_courante(int r);

// Chebbah/Cinque/Arnaud/Mehenni
int recup_taille_pile();

// Chebbah/Cinque/Arnaud/Mehenni
int recup_nis_region(int region);

// Retourne le déplacement courant dans la région courante
// Chebbah/Cinque/Arnaud/Mehenni
int recup_depl_courant();

// Incrémenter le déplacement de la région courante
// Chebbah/Cinque/Arnaud/Mehenni
void increment_depl_region(int taille_var);

// Chebbah/Cinque/Arnaud/Mehenni
arbre recup_arbre_region(int num_region);

/* Variables globales de la table des régions */
extern TableRegion t_region;
extern int pcv_r;



#endif