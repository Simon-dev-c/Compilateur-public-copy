#ifndef _VERIF_SEMANTIQUE_H_
#define _VERIF_SEMANTIQUE_H_
#include "arbre.h"

// Retourne -1 si les dimensions sont inavalide 1 sinon
// Chebbah/Cinque/Arnaud/Mehenni
int verif_dimensions_tableau(arbre arbre_liste_i, int nb_dimensions);
// Recherche dans la table de representation des types
// si on trouve le num lex du champ qu'on recherche parmis ceux de connu par la structure
// i_lex_champ : num lex du champ qu'on recherche
// i_rep : indice dans la table rep de la structure
// nb_champs : nombre de champs que contient notre structure
// Retourne : le numero de decla du type de champ si on la trouvé
// -1 si on la pas trouvé
// Chebbah/Cinque/Arnaud/Mehenni
int recherche_champ_dans_struct(int i_lex_champ, int i_rep, int nb_champs);
// verifie la semantique d'un variable
// si semantique valide :
//    Retourne : le num de decla du type de la varaible
//    Sinon -1
// Chebbah/Cinque/Arnaud/Mehenni
int verif_sem_variable(arbre abr_var); // ok
// En entrée on a l'arbre de la valeur
// ça peut etre une variable, expr, une constante, appel fct
// verifie la semantique d'un valeur
// si semantique valide :
//    Renvoie : le num de decla du type de la valeur
//    Sinon -1
// Chebbah/Cinque/Arnaud/Mehenni
int verif_sem_valeur(arbre abr_val); // pas tout à fait ok
// En entrée on a l'arbre de l'affectation
// verifie la semantique d'une affectation
// càd la partie gauche et droite
// et fini par comparer les type
// si ce n'est pas ok : renvoie -1
// Chebbah/Cinque/Arnaud/Mehenni
int verif_sem_affectation(arbre abr_affect); // ok
// On doit verif que la derniere instruction soit un RETOURNE <var>
// Renvoie -1 si le corps d'une fct ne finis pas par l'instruction retourne
// 1 sinon
// Chebbah/Cinque/Arnaud/Mehenni
int verif_sem_corps_fct(arbre abr_crps, int num_lex_fct); // ok
// Verifie qu'une instruction retourne, retourne le bon type
// Renvoie -1 si l'instruction retourne est invalide
// 1 sinon
// Chebbah/Cinque/Arnaud/Mehenni
int verif_sem_retour(arbre abr_retour); // ok
// Chebbah/Cinque/Arnaud/Mehenni
int verif_sem_appel(arbre abr_appel); // pas ok
// verifie la semantique d'une expression arith cad le type de chaque opperandes pour que l'expr ai du sens
// recursivement de maniere à verifier toutes les sous-expressions
// Renvoie le num du type dans ce ca 0 (ent) ou 1 (reel) comme il s'agit d'une expr arith
// -1 si l'expr n'est pas correcte
// Chebbah/Cinque/Arnaud/Mehenni
int verif_sem_expr_arith(arbre abr_expr_arith); // ok
int verif_sem_expr_bool(arbre abr_expr_bool);
#endif
