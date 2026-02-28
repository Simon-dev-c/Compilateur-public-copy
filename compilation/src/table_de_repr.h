#ifndef _TABLE_REPR_H_
#define _TABLE_REPR_H_

#define TAILLE_TABLE_REP 50

typedef int TableRep[TAILLE_TABLE_REP];

extern TableRep t_representation;

// Chebbah/Cinque/Arnaud/Mehenni
void init_table_rep();
// Chebbah/Cinque/Arnaud/Mehenni
void inserer_tab_rep(int val);

/* -------- Fonction et Procedure -------- */

// Chebbah/Cinque/Arnaud/Mehenni
int init_procedure();
// Chebbah/Cinque/Arnaud/Mehenni
int init_fonction();
// Chebbah/Cinque/Arnaud/Mehenni
void inserer_fonction_type_retour(int num_type);

// Chebbah/Cinque/Arnaud/Mehenni
void increment_nb_params();
// Chebbah/Cinque/Arnaud/Mehenni
void inserer_nb_params();

/* -------- Type Structure -------- */
// Chebbah/Cinque/Arnaud/Mehenni
int init_struct();
// Chebbah/Cinque/Arnaud/Mehenni
void increment_nb_champs();
// Chebbah/Cinque/Arnaud/Mehenni
void inserer_nb_champs();

// Chebbah/Cinque/Arnaud/Mehenni
int init_tableau();
// Chebbah/Cinque/Arnaud/Mehenni
void inserer_type_elt(int num_type);

// Chebbah/Cinque/Arnaud/Mehenni
void increment_nb_dimension();
// Chebbah/Cinque/Arnaud/Mehenni
void inserer_nb_dimensions();

// Chebbah/Cinque/Arnaud/Mehenni
int increment_depl(int d);
// Chebbah/Cinque/Arnaud/Mehenni
void inserer_depl();

// Chebbah/Cinque/Arnaud/Mehenni
int calcul_taille_tab();

extern int ipcv_r;


#endif