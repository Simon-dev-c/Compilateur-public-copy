#ifndef MACHINE_VIRTUELLE_H
#define MACHINE_VIRTUELLE_H

#include "../src/arbre.h"

/* ========== CONSTANTES ========== */

#define TAILLE_PILE 10000

/* ========== TYPES ========== */

// Cellule de la pile (union pour différents types)
typedef union {
    int entier;
    float reel;
    int booleen;
    char caractere;
} cellule;

/* ========== VARIABLES GLOBALES ========== */

extern cellule pile[TAILLE_PILE];
extern int sommet_pile;
extern int BC;  // Base Courante
extern int region_courante_mv;

// Chebbah/Cinque/Arnaud/Mehenni
void initialiser_machine_virtuelle();

/* ========== AFFICHAGE PILE ========== */

// Chebbah/Cinque/Arnaud/Mehenni
void afficher_pile_execution();

/* ========== GESTION DES ADRESSES ========== */

/*
 * Chaînage statique = LISTE
 * pile[BC + 1] = région englobante la plus proche
 */
// Chebbah/Cinque/Arnaud/Mehenni
int calculer_adresse_pile(int num_decl);

/* ========== ÉVALUATION DES EXPRESSIONS ========== */

// Chebbah/Cinque/Arnaud/Mehenni
int evaluer_entier(arbre a);
// Chebbah/Cinque/Arnaud/Mehenni
int evaluer_booleen(arbre a);
// Chebbah/Cinque/Arnaud/Mehenni
float evaluer_reel(arbre a);
// Chebbah/Cinque/Arnaud/Mehenni
char evaluer_caractere(arbre a);
// Chebbah/Cinque/Arnaud/Mehenni
char* evaluer_chaine(arbre a);

/* ========== EXÉCUTION DES INSTRUCTIONS ========== */

// Chebbah/Cinque/Arnaud/Mehenni
void executer_instruction(arbre a);
// Chebbah/Cinque/Arnaud/Mehenni
void executer_affectation(arbre a);
// Chebbah/Cinque/Arnaud/Mehenni
void executer_si(arbre a);
// Chebbah/Cinque/Arnaud/Mehenni
void executer_tant_que(arbre a);
// Chebbah/Cinque/Arnaud/Mehenni
void executer_lire(arbre a);
// Chebbah/Cinque/Arnaud/Mehenni
void executer_ecrire(arbre a);


/* ========== APPEL DE PROCÉDURE ========== */

// Chebbah/Cinque/Arnaud/Mehenni
void executer_appel_procedure(arbre a);
// Chebbah/Cinque/Arnaud/Mehenni
void executer_liste_instructions(arbre a);
// Chebbah/Cinque/Arnaud/Mehenni
void executer_instruction(arbre a);

/* ========== APPEL DE FONCTION ========== */

// Chebbah/Cinque/Arnaud/Mehenni
void executer_appel_fonction(arbre a);

/* ========== PROGRAMME PRINCIPAL ========== */

// Chebbah/Cinque/Arnaud/Mehenni
void executer_programme(int num_region);

#endif /* MACHINE_VIRTUELLE_H */