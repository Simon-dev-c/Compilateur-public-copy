#include <stdio.h>
#include <stdlib.h>
#include "verif_semantique.h"
#include "table_declarations.h"
#include "table_de_repr.h"
#include "table_lexico.h"
#include "table_region.h"
#include "arbre.h"
#include "erreurs.h"

extern TableDeclarations t_declarations;
extern TableRep t_representation;

// Retourne -1 si les dimensions sont inavalide 1 sinon
// Chebbah/Cinque/Arnaud/Mehenni
int verif_dimensions_tableau(arbre arbre_liste_i, int nb_dimensions)
{
    arbre abr;
    int i = 0;

    if (arbre_liste_i->type != A_LISTE_INDICES)
    {
        printf("L'arbre n'est pas un arbre de liste inddices\n");
        return -1;
    }
    abr = arbre_liste_i->fils;

    while (abr != arbre_vide())
    {
        if (verif_sem_valeur(abr) != 0) // 0 pour ent
            return -1;

        i++;
        abr = abr->frere;
    }

    if (i != nb_dimensions)
        return -1;

    return 1;
}

// Recherche dans la table de representation des types
// si on trouve le num lex du champ qu'on recherche parmis ceux de connu par la structure
// i_lex_champ : num lex du champ qu'on recherche
// i_rep : indice dans la table rep de la structure
// nb_champs : nombre de champs que contient notre structure
// Retourne : le numero de decla du type de champ si on la trouvé
// -1 si on la pas trouvé
// Chebbah/Cinque/Arnaud/Mehenni
int recherche_champ_dans_struct(int i_lex_champ, int i_rep, int nb_champs)
{
    int i = 0;
    int trouve = 0;
    int num_decla_type = -1;

    while (i < nb_champs && !trouve)
    {
        if (t_representation[i_rep + 1 + i * 3] == i_lex_champ)
        {
            trouve = 1;
            num_decla_type = t_representation[i_rep + 2 + i * 3];
        }
        i++;
    }

    return num_decla_type;
}

// verifie la semantique d'un variable
// si semantique valide :
//    Retourne : le num de decla du type de la varaible
//    Sinon -1
// Chebbah/Cinque/Arnaud/Mehenni
int verif_sem_variable(arbre abr_var)
{
    arbre idf;
    int i_rep; // indice dans la table de representation des types
    int i_decla_type;

    int nb_dimensions;
    int nb_champs;
    // int num_decla_type_finale; // numero de decla du type de la variable finale

    if (abr_var->type != A_VARIABLE)
    {
        printf("L'arbre en entrée n'est pas un arbre d'une variable\n");
        printf("%d\n", abr_var->type);
        return -1;
    }

    idf = abr_var->fils; // le premier idf
    // on fait l'assoc nom que sur le premier idf, car le reste c'est soit une liste d'indices
    // soit des champs
    idf->num_decla = assoc_nom(idf->num_lex, VAR);

    // Si la variable est non déclarée ou hors de portée
    if (idf->num_decla == -1)
    {
        erreur_var_inconnu(idf->num_lex);
        return -1;
    }

    // recuperer le num decla du type de la variable
    i_decla_type = t_declarations[idf->num_decla].description;

    while (idf->frere != arbre_vide())
    {
        i_rep = t_declarations[i_decla_type].description;

        if (t_declarations[i_decla_type].nature == TYPE_S)
        {
            // c'est une structure
            // donc on s'attend à ce que le noeud frere soit un A_IDF
            if (idf->frere->type == A_IDF)
            {
                nb_champs = t_representation[i_rep];
                // on doit verifier que le champ soit un champ connu de la structure
                if ((i_decla_type = recherche_champ_dans_struct(idf->frere->num_lex, i_rep, nb_champs)) == -1)
                {
                    erreur_var_champ_inconnu(idf->frere->num_lex, idf->num_lex);
                    return -1;
                }
            }
            else
            {
                erreur_var_struct_tab(idf->num_lex);
                return -1;
            }
        }
        else if (t_declarations[i_decla_type].nature == TYPE_T)
        {
            // c'est un tableau
            // donc on s'attend à ce que le noeud frere soit un A_LISTE_INDICES
            if (idf->frere->type == A_LISTE_INDICES)
            {
                i_decla_type = t_representation[i_rep]; // num decla du type des elements du tableau
                nb_dimensions = t_representation[i_rep + 1];

                if (verif_dimensions_tableau(idf->frere, nb_dimensions) == -1)
                {
                    erreur_dimensions_tab_invalide(idf->num_lex);
                    return -1;
                }
            }
            else
            {
                erreur_var_tab_struct(idf->num_lex);
                return -1;
            }
        }
        else
        {
            erreur_var_type_base(idf->num_lex);
            return -1;
        }
        idf = idf->frere;
    }

    return i_decla_type;
}

// En entrée on a l'arbre de la valeur
// ça peut etre une variable, expr, une constante, appel fct
// verifie la semantique d'un valeur
// si semantique valide :
//    Renvoie : le num de decla du type de la valeur
//    Sinon -1
// Chebbah/Cinque/Arnaud/Mehenni
int verif_sem_valeur(arbre abr_val)
{
    switch (abr_val->type)
    {
    // cas simple
    case A_CSTE_ENTIERE:
        return 0;
    case A_CSTE_REEL:
        return 1;
    case A_CSTE_BOOLEEN:
        return 2;
    case A_CSTE_CARACTERE:
        return 3;

    case A_VARIABLE:
        return verif_sem_variable(abr_val); // ok
    case A_APPEL_FCT:
        return verif_sem_appel(abr_val); // ok
    case A_APPEL_PROC:
        return verif_sem_appel(abr_val); // ok

    case A_EXPRESSION_ARITHMETIQUE:
        return verif_sem_expr_arith(abr_val->fils); // ok
    case A_EXPRESSION_BOOLEENNE:
        return verif_sem_expr_bool(abr_val->fils); // ok
    default:
        return -1;
    }
}

// En entrée on a l'arbre de l'affectation
// verifie la semantique d'une affectation
// càd la partie gauche et droite
// et fini par comparer les type
// si ce n'est pas ok : renvoie -1
// Chebbah/Cinque/Arnaud/Mehenni
int verif_sem_affectation(arbre abr_affect)
{
    // On doit recup les type de chacun des membres (gauche et droite)
    //    pour savoir si l'affectation est valide
    int t_var;
    int t_val;

    if (abr_affect->type != A_OPAFF)
    {
        printf("L'arbre en entrée n'est pas un arbre d'affectation\n");
        return -1;
    }

    // verifions la semantique de la variable
    t_var = verif_sem_variable(abr_affect->fils);
    if (t_var == -1)
    {
        return -1;
    }

    // verifions la semantique de la valeur
    t_val = verif_sem_valeur(abr_affect->fils->frere);
    if (t_val == -1)
    {
        return -1;
    }

    // si les types des deux membres sont different l'affectation est impossible
    if (t_var == t_val)
    {
        return 1;
    }
    else
        erreur_affectation_type();
    return -1;
}

// On doit verif que la derniere instruction soit un RETOURNE <var>
// Renvoie -1 si le corps d'une fct ne finis pas par l'instruction retourne
// 1 sinon
// Chebbah/Cinque/Arnaud/Mehenni
int verif_sem_corps_fct(arbre abr_crps, int num_lex_fct)
{
    // on va regarder la derniere instruction de l'arbre des instruction
    // pour verif si c'est un retour
    arbre abr = abr_crps;

    if (abr == arbre_vide() || abr->fils == arbre_vide())
    {
        erreur_fct_retourne(num_lex_fct);
        return -1;
    }

    // parcours des instructions
    while (abr->fils->frere != arbre_vide())
        abr = abr->fils->frere;

    // à ce stade abr->fils est la derniere instruction donc on verifie bien que c'est un RETOUR
    // sans verifier la semantique de l'instruction car verif_sem_retour le fait dejà dans le yacc
    if (abr->fils == arbre_vide() || abr->fils->type != A_RETOURNE)
    {
        erreur_fct_retourne(num_lex_fct);
        return -1;
    }

    return 1;
}

// Verifie qu'une instruction retourne, retourne le bon type
// Renvoie -1 si l'instruction retourne est invalide
// 1 sinon
// Chebbah/Cinque/Arnaud/Mehenni
int verif_sem_retour(arbre abr_retour)
{
    int num_type_retour; // le numero de decla du type de retour de la fct
    int type_retournee;  // le type retournée par l'instruction

    if (abr_retour->type != A_RETOURNE)
    {
        printf("L'arbre en entrée n'est pas un arbre d'instruction retourner\n");
        return -1;
    }

    // verifier si l'instruction retourner est pas presente dans le prog principale
    if (recup_num_region() == 0)
    {
        erreur_dans_prog_principale();
        return -1;
    }

    num_type_retour = abr_retour->num_lex; // numero du type de retour

    // si on a pas de valeur de retour
    if (abr_retour->fils == arbre_vide())
        type_retournee = -2; // c'est probablement une proc
    // donc on mets type_retournee = -1 pour que type_retournee = num_type_retour
    else
    {
        if ((type_retournee = verif_sem_valeur(abr_retour->fils)) == -1)
        {
            return -1;
        }
    }

    // verif finale est-ce que le type de retour = le type retournée
    if (num_type_retour == type_retournee)
        return 1;
    else
        erreur_type_retour();
    return -1;
}

int verif_sem_appel(arbre abr_appel)
{

    int i_decla_proc_fct;
    int i_rep;
    int nb_params;
    int compteur = 0; // compteur des params
    arbre abr_param;
    int i_type_param;
    int type_param;

    int num_type_retour;

    i_decla_proc_fct = abr_appel->num_decla;
    i_rep = t_declarations[i_decla_proc_fct].description;

    if (abr_appel->type == A_APPEL_FCT)
    {
        nb_params = t_representation[i_rep + 1];
        i_type_param = i_rep + 3;
        num_type_retour = t_representation[i_rep];
    }
    else if (abr_appel->type == A_APPEL_PROC)
    {
        nb_params = t_representation[i_rep];
        i_type_param = i_rep + 2;
        num_type_retour = -2;
    }
    else
    {
        printf("L'arbre n'est pas un arbre d'appel\n");
        return -1;
    }

    abr_param = abr_appel->fils->fils;

    while (abr_param != arbre_vide())
    {
        compteur++;
        if (compteur > nb_params)
        {
            erreur_args(abr_appel->num_lex, compteur, nb_params);
            return -1;
        }

        type_param = verif_sem_valeur(abr_param);
        if (type_param == -1)
        {
            return -1;
        }
        else if (type_param != t_representation[i_type_param])
        {
            erreur_arg_type(abr_appel->num_lex, compteur);
            return -1;
        }
        i_type_param += 2;
        abr_param = abr_param->frere;
    }

    if (compteur < nb_params)
    {
        erreur_args(abr_appel->num_lex, compteur, nb_params);
        return -1;
    }

    return num_type_retour;
}

// verifie la semantique d'une expression arith cad le type de chaque opperandes pour que l'expr ai du sens
// recursivement de maniere à verifier toutes les sous-expressions
// Renvoie le num du type dans ce ca 0 (ent) ou 1 (reel) comme il s'agit d'une expr arith
// -1 si l'expr n'est pas correcte
// Chebbah/Cinque/Arnaud/Mehenni
int verif_sem_expr_arith(arbre abr_expr_arith)
{
    // on doit verifer le type de l'operande gauche et de l'operande droite
    // on va utiliser la recurssion pour verifier le type de toutes les sous expression
    // le type des operandes doit etre soit un entier soit un reel, rien d'autre.

    int type_op_gauche;
    int type_op_droite;

    if (abr_expr_arith->type == A_PLUS || abr_expr_arith->type == A_MOINS || abr_expr_arith->type == A_MULT || abr_expr_arith->type == A_DIV)
    {
        // verife de l'op gauche
        type_op_gauche = verif_sem_expr_arith(abr_expr_arith->fils);
        if (type_op_gauche == -1)
            return -1;

        // 0 = ent, 1 = reel
        if (type_op_gauche != 0 && type_op_gauche != 1)
        {
            printf("ERREUR : une expression arithmetique ne peut être composé que d'entier ou de reel\n");
            return -1;
        }

        // verife de l'op droite
        type_op_droite = verif_sem_expr_arith(abr_expr_arith->fils->frere);
        if (type_op_droite == -1)
            return -1;

        // 0 = ent, 1 = reel
        if (type_op_droite != 0 && type_op_droite != 1)
        {
            printf("ERREUR : une expression arithmetique ne peut être composé que d'entier ou de reel\n");
            return -1;
        }
    }
    else // cas d'arret
        return verif_sem_valeur(abr_expr_arith);

    if (type_op_gauche == type_op_droite)
        return type_op_gauche;

    erreur_expr_arith();
    return -1;
}

// verifie la semantique d'une expression booleenne cad le type de chaque operande
// recursivement de maniere à vérifier toutes les sous-expressions
// Renvoie le num du type dans ce cas 2 (bool) comme il s'agit d'une expr bool
// -1 si l'expr n'est pas correcte
// Chebbah/Cinque/Arnaud/Mehenni
int verif_sem_expr_bool(arbre abr_expr_bool)
{
    // on doit vérifier le type des opérandes selon le type d'opération
    // pour les opérateurs logiques (ET, OU, NON) : les opérandes doivent être booléens
    // pour les comparaisons : les opérandes doivent être de même type (entier ou réel)
    // on va utiliser la récursion pour vérifier le type de toutes les sous expressions

    int type_op_gauche;
    int type_op_droite;

    // Cas des opérateurs logiques binaires (ET, OU)
    if (abr_expr_bool->type == A_ET || abr_expr_bool->type == A_OU)
    {
        // vérif de l'op gauche
        type_op_gauche = verif_sem_expr_bool(abr_expr_bool->fils);
        if (type_op_gauche == -1)
            return -1;

        // 2 = bool
        if (type_op_gauche != 2)
        {
            printf("ERREUR : les opérandes d'un opérateur logique (ET/OU) doivent être de type booléen\n");
            return -1;
        }

        // vérif de l'op droite
        type_op_droite = verif_sem_expr_bool(abr_expr_bool->fils->frere);
        if (type_op_droite == -1)
            return -1;

        // 2 = bool
        if (type_op_droite != 2)
        {
            printf("ERREUR : les opérandes d'un opérateur logique (ET/OU) doivent être de type booléen\n");
            return -1;
        }

        // Les deux opérandes sont booléens, on retourne booléen
        return 2;
    }
    // Cas de l'opérateur logique unaire (NON)
    else if (abr_expr_bool->type == A_NON)
    {
        // vérif de l'opérande
        type_op_gauche = verif_sem_expr_bool(abr_expr_bool->fils);
        if (type_op_gauche == -1)
            return -1;

        // 2 = bool
        if (type_op_gauche != 2)
        {
            printf("ERREUR : l'opérande d'un opérateur NON doit être de type booléen\n");
            return -1;
        }

        return 2;
    }
    // Cas des opérateurs de comparaison (<, >, <=, >=, ==, !=)
    else if (abr_expr_bool->type == A_INF || abr_expr_bool->type == A_SUP ||
             abr_expr_bool->type == A_INF_EGAL || abr_expr_bool->type == A_SUP_EGAL ||
             abr_expr_bool->type == A_EGAL || abr_expr_bool->type == A_DIFF)
    {
        // vérif de l'op gauche (doit être une expression arithmétique)
        type_op_gauche = verif_sem_expr_arith(abr_expr_bool->fils);
        if (type_op_gauche == -1)
            return -1;

        // 0 = ent, 1 = reel
        if (type_op_gauche != 0 && type_op_gauche != 1)
        {
            printf("ERREUR : les opérandes d'une comparaison doivent être de type entier ou réel\n");
            return -1;
        }

        // vérif de l'op droite (doit être une expression arithmétique)
        type_op_droite = verif_sem_expr_arith(abr_expr_bool->fils->frere);
        if (type_op_droite == -1)
            return -1;

        // 0 = ent, 1 = reel
        if (type_op_droite != 0 && type_op_droite != 1)
        {
            printf("ERREUR : les opérandes d'une comparaison doivent être de type entier ou réel\n");
            return -1;
        }

        // Les deux opérandes doivent être de même type
        if (type_op_gauche != type_op_droite)
        {
            printf("ERREUR : les opérandes d'une comparaison doivent être de même type\n");
            return -1;
        }

        // Une comparaison retourne toujours un booléen
        return 2;
    }
    // Cas d'arrêt : constante booléenne (VRAI/FAUX)
    else if (abr_expr_bool->type == A_CSTE_BOOLEEN)
    {
        return 2;
    }
    // Cas où on a une variable ou un appel de fonction qui devrait retourner un booléen
    else
    {
        int type_valeur = verif_sem_valeur(abr_expr_bool);

        if (type_valeur == -1)
            return -1;

        // On vérifie que la valeur est bien de type booléen
        if (type_valeur != 2)
        {
            printf("ERREUR : l'expression booléenne contient une valeur qui n'est pas de type booléen\n");
            return -1;
        }

        return 2;
    }
}