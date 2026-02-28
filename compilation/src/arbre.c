#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbre.h"
#include "table_region.h"

// Chebbah/Cinque/Arnaud/Mehenni
arbre arbre_vide() { return NULL; }

// Chebbah/Cinque/Arnaud/Mehenni
arbre creer_noeud(int type, double num_lex, int num_decla)
{
    arbre n = (arbre)malloc(sizeof(noeud));
    if (n == NULL)
    {
        printf("Echec Malloc sur la creation d'un noeud\n");
        exit(EXIT_FAILURE);
    }
    n->type = type;
    n->num_lex = num_lex;
    n->num_decla = num_decla;
    n->frere = arbre_vide();
    n->fils = arbre_vide();

    return n;
}

// Chebbah/Cinque/Arnaud/Mehenni

/*
Ajouter le frere à la suite de tous ses freres si il y en a
*/
arbre concat_pere_frere(arbre pere, arbre frere)
{
    arbre courant = pere;
    while (courant->frere != NULL)
    {
        courant = courant->frere;
    }
    courant->frere = frere;

    return pere;
}

// Chebbah/Cinque/Arnaud/Mehenni
arbre concat_pere_fils(arbre pere, arbre fils)
{
    pere->fils = fils;
    return pere;
}

// Chebbah/Cinque/Arnaud/Mehenni
void liberer_arbre(arbre a)
{
    if (a == NULL)
        return;

    if (a->fils != NULL)
        liberer_arbre(a->fils);

    if (a->frere != NULL)
        liberer_arbre(a->frere);

    free(a);
}

void liberer_regions_arbre()
{
    int i;
    for (i = 0; i < TAILLE_TABLE_REGION; i++)
    {
        if (t_region[i].NIS != -1)
        {
            liberer_arbre(t_region[i].arbre);
        }
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
void affiche_arbre_bis(arbre a, int niveau)
{
    if (a == NULL)
    {
        return;
    }

    // Affichage de l'indentation
    for (int i = 0; i < niveau; i++)
    {
        printf("|\t");
    }

    // Affichage du type de nœud
    switch (a->type)
    {
    case A_CORPS:
        printf("A_CORPS");
        break;
    case A_PROG:
        printf("A_PROG");
        break;
    case A_LISTE_INSTRUCTION:
        printf("A_LISTE_INSTRUCTION");
        break;
    case A_CONCAT:
        printf("A_CONCAT");
        break;
    case A_RETOURNE:
        printf("A_RETOURNE");
        break;
    case A_IF_THEN_ELSE:
        printf("A_IF_THEN_ELSE");
        break;
    case A_TANT_QUE:
        printf("A_TANT_QUE");
        break;
    case A_OPAFF:
        printf("A_OPAFF");
        break;
    case A_LISTE_INDICES:
        printf("A_LISTE_INDICES");
        break;
    case A_IDF:
        printf("A_IDF");
        break;
    case A_VARIABLE:
        printf("A_VARIABLE");
        break;
    case A_CSTE_ENTIERE:
        printf("A_CSTE_ENTIERE");
        break;
    case A_CSTE_REEL:
        printf("A_CSTE_REEL");
        break;
    case A_CSTE_BOOLEEN:
        printf("A_CSTE_BOOLEEN");
        break;
    case A_CSTE_CARACTERE:
        printf("A_CSTE_CARACTERE");
        break;
    case A_CSTE_CHAINE:
        printf("A_CSTE_CHAINE");
        break;
    case A_EXPRESSION_ARITHMETIQUE:
        printf("A_EXPRESSION_ARITHMETIQUE");
        break;
    case A_EXPRESSION_BOOLEENNE:
        printf("A_EXPRESSION_BOOLEENNE");
        break;
    case A_PLUS:
        printf("A_PLUS");
        break;
    case A_MOINS:
        printf("A_MOINS");
        break;
    case A_MULT:
        printf("A_MULT");
        break;
    case A_DIV:
        printf("A_DIV");
        break;
    case A_MOD:
        printf("A_MOD");
        break;
    case A_OU:
        printf("A_OU");
        break;
    case A_ET:
        printf("A_ET");
        break;
    case A_NON:
        printf("A_NON");
        break;
    case A_INF:
        printf("A_INF");
        break;
    case A_INF_EGAL:
        printf("A_INF_EGAL");
        break;
    case A_SUP:
        printf("A_SUP");
        break;
    case A_SUP_EGAL:
        printf("A_SUP_EGAL");
        break;
    case A_EGAL:
        printf("A_EGAL");
        break;
    case A_DIFF:
        printf("A_DIFF");
        break;
    case A_APPEL_FCT:
        printf("A_APPEL_FCT");
        break;
    case A_APPEL_PROC:
        printf("A_APPEL_PROC");
        break;
    case A_LISTE_PARAM:
        printf("A_LISTE_PARAM");
        break;
    case A_LIRE:
        printf("A_LIRE");
        break;
    case A_ECRIRE:
        printf("A_ECRIRE");
        break;
    case A_LISTE_VARIABLES:
        printf("A_LISTE_VARIABLE");
        break;
    }

    // Affichage de la valeur selon le type
    if (a->type == A_IDF || a->type == A_CSTE_CHAINE)
    {
        printf(" : %s %d\n", lexeme(a->num_lex), a->num_decla);
    }
    else
    {
        printf(" : %f\n", a->num_lex);
    }

    // Affichage récursif des fils
    if (a->fils != NULL)
    {
        affiche_arbre_bis(a->fils, niveau + 1);
    }

    // Affichage récursif des frères
    if (a->frere != NULL)
    {
        affiche_arbre_bis(a->frere, niveau);
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
void affiche_arbre(arbre a)
{
    if (a == NULL)
    {
        printf("Arbre vide\n");
        return;
    }
    affiche_arbre_bis(a, 0);
}