#ifndef ARBRE_H
#define ARBRE_H
#include "table_lexico.h"

typedef struct noeud
{
    int type; // A_PROG, A_CORPS etc.....
    double num_lex;
    int num_decla;
    struct noeud *frere;
    struct noeud *fils;
} noeud;

typedef struct noeud *arbre;

typedef enum
{
    A_PROG,
    A_CORPS,
    A_LISTE_INSTRUCTION,
    A_EXPRESSION_ARITHMETIQUE,
    A_EXPRESSION_BOOLEENNE,
    A_EXPRESSION_CHAINE,
    A_LISTE_INDICES,
    A_RETOURNE,
    A_LISTE_PARAM,
    A_IF_THEN_ELSE,
    A_TANT_QUE,
    A_OPAFF,
    A_IDF,
    A_VARIABLE,
    A_CSTE_ENTIERE,
    A_CSTE_REEL,
    A_CSTE_BOOLEEN,
    A_CSTE_CARACTERE,
    A_CSTE_CHAINE,
    A_PLUS,
    A_MOINS,
    A_MULT,
    A_DIV,
    A_MOD,
    A_CONCAT,
    A_APPEL_FCT,
    A_APPEL_PROC,
    A_ENTREE,
    A_SORTIE,
    A_LISTE_VARIABLES,
    A_OU,
    A_ET,
    A_NON,
    A_INF,
    A_INF_EGAL,
    A_SUP,
    A_SUP_EGAL,
    A_EGAL,
    A_DIFF,
    A_LIRE,
    A_ECRIRE
} type_noeud;

// Chebbah/Cinque/Arnaud/Mehenni
arbre arbre_vide();
// Chebbah/Cinque/Arnaud/Mehenni
arbre creer_noeud(int type, double num_lex, int num_decla);
// Chebbah/Cinque/Arnaud/Mehenni
arbre concat_pere_frere(arbre pere, arbre frere);
// Chebbah/Cinque/Arnaud/Mehenni
arbre concat_pere_fils(arbre pere, arbre fils);
// Chebbah/Cinque/Arnaud/Mehenni
void liberer_arbre(arbre a);
// Chebbah/Cinque/Arnaud/Mehenni
void affiche_arbre_bis(arbre a, int decallage);
// Chebbah/Cinque/Arnaud/Mehenni
void affiche_arbre(arbre a);
// Chebbah/Cinque/Arnaud/Mehenni
void liberer_regions_arbre();
#endif