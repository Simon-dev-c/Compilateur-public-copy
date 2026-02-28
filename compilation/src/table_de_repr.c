#include <stdio.h>
#include "table_de_repr.h"
#include <stdlib.h>
#include "table_declarations.h"

TableRep t_representation;

int ipcv_r = 0; // indice premiere case vide

/* Pour les fct et proc */
int ictr = 0; // indice case type de retour
int icnp = 0; // indice case nbre params
int nb_params = 0;

/* Pour les struct et tableau */
int icnc = 0; // indice case nbre champs
int icte = 0; // indice case type element
int nb_champs = 0;
int nb_dimensions = 0;

int depl = 0; // deplacement

// Chebbah/Cinque/Arnaud/Mehenni
void init_table_rep()
{
    int i;
    for (i = 0; i < TAILLE_TABLE_REP; i++)
    {
        t_representation[i] = -1;
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
void inserer_tab_rep(int val)
{
    t_representation[ipcv_r] = val;
    ipcv_r++;
}

/* -------- Fonction et Procedure -------- */

// Chebbah/Cinque/Arnaud/Mehenni
int init_procedure()
{
    icnp = ipcv_r;
    ipcv_r++;

    nb_params = 0;

    return icnp;
}

// Chebbah/Cinque/Arnaud/Mehenni
int init_fonction()
{
    ictr = ipcv_r;
    ipcv_r++;

    icnp = ipcv_r;
    ipcv_r++;
    nb_params = 0;

    return ictr;
}

// Chebbah/Cinque/Arnaud/Mehenni
void inserer_fonction_type_retour(int num_type)
{
    t_representation[ictr] = num_type;
}

// Chebbah/Cinque/Arnaud/Mehenni
void increment_nb_params()
{
    nb_params++;
}

// Chebbah/Cinque/Arnaud/Mehenni
void inserer_nb_params()
{
    t_representation[icnp] = nb_params;
}

/* -------- Type Structure -------- */

// Chebbah/Cinque/Arnaud/Mehenni
int init_struct()
{
    icnc = ipcv_r;
    ipcv_r++;

    nb_champs = 0;
    depl = 0;

    return icnc;
}

// Chebbah/Cinque/Arnaud/Mehenni
void increment_nb_champs()
{
    nb_champs++;
}

// Chebbah/Cinque/Arnaud/Mehenni
void inserer_nb_champs()
{
    t_representation[icnc] = nb_champs;
}

/* -------- Type Tableau -------- */

// Chebbah/Cinque/Arnaud/Mehenni
int init_tableau()
{
    icte = ipcv_r;
    ipcv_r++;
    ipcv_r++;

    nb_dimensions = 0;

    return icte;
}

// Chebbah/Cinque/Arnaud/Mehenni
void inserer_type_elt(int num_type)
{
    t_representation[icte] = num_type;
}

// Chebbah/Cinque/Arnaud/Mehenni
void increment_nb_dimension()
{
    nb_dimensions++;
}

// Chebbah/Cinque/Arnaud/Mehenni
void inserer_nb_dimensions()
{
    t_representation[icte + 1] = nb_dimensions;
}

// Chebbah/Cinque/Arnaud/Mehenni
int calcul_taille_tab()
{
    int i;
    int taille_tot;
    int nb_dimensions;

    int i_repr = icte;
    taille_tot = taille(t_representation[i_repr]);

    i_repr++;
    nb_dimensions = t_representation[i_repr];
    i_repr++;

    for (i = 0; i < (nb_dimensions * 2); i += 2)
    {
        taille_tot *= t_representation[i_repr + i + 1] - t_representation[i_repr + i] + 1; // TYPE | NB_MIN | MIN | MAX  dans la la tab des repr
    }

    return taille_tot;
}

// increment le deplacement de d et retourne le nouveau deplacement
// Chebbah/Cinque/Arnaud/Mehenni
int increment_depl(int d)
{
    depl += d;

    return depl;
}

// Chebbah/Cinque/Arnaud/Mehenni
void inserer_depl()
{
    inserer_tab_rep(depl);
}
