#include <stdlib.h>
#include <stdio.h>
#include "table_declarations.h"
#include "table_lexico.h"
#include "table_region.h"

// Numéro de ligne courant (défini dans analyseur_lex.l)
extern int num_ligne;

TableDeclarations t_declarations;
int pcv_d = TAILLE_TABLE_LEXICO; // premiere case vide de la zone de débordement

// Chebbah/Cinque/Arnaud/Mehenni
void init_table_declarations()
{
    int i;

    for (i = 0; i < 4; i++)
    {
        t_declarations[i].nature = TYPE_B;
        t_declarations[i].suivant = -1;
        t_declarations[i].region = 0;
        t_declarations[i].description = -1;
        t_declarations[i].execution = 1; // Taille = 1 pour tous les types de base (selon le cours)
    }

    for (i = 4; i < TAILLE_TABLE_DECLARATIONS; i++)
    {
        t_declarations[i].nature = -1;
        t_declarations[i].suivant = -1;
        t_declarations[i].region = -1;
        t_declarations[i].description = -1;
        t_declarations[i].execution = -1;
    }
}

// Renvoie le numero de decla
// Sinon -1
// Chebbah/Cinque/Arnaud/Mehenni
int inserer_declaration(int num_lex, enum Nature nature, int region, int description, int exec)
{
    int i = num_lex;
    int i_precedent;

    int i_decla;

    int region_courante = recup_num_region();

    int redecla = 0;

    if (t_declarations[num_lex].nature == -1)
    {
        i_decla = num_lex;
    }
    else
    {
        while (i != -1 && !redecla)
        {
            // On verifie qu'on ai pas dejà une decla de meme nom et de même nature dans la meme region
            if (t_declarations[i].region == region_courante)
            {
                if ((t_declarations[i].nature == TYPE_S || t_declarations[i].nature == TYPE_T) && (nature == TYPE_S || nature == TYPE_T))
                {
                    redecla = 1;
                }
                else if ((t_declarations[i].nature == VAR || t_declarations[i].nature == PARAM) && (nature == VAR || nature == PARAM))
                {
                    redecla = 1;
                }
                else if (t_declarations[i].nature == nature)
                {
                    redecla = 1;
                }
            }

            i_precedent = i;
            i = t_declarations[i].suivant;
        }

        if (redecla)
        {
            return -1;
        }

        t_declarations[i_precedent].suivant = pcv_d;
        i_decla = pcv_d++;
    }

    t_declarations[i_decla].nature = nature;
    t_declarations[i_decla].region = region;
    t_declarations[i_decla].description = description;
    t_declarations[i_decla].execution = exec;

    return i_decla;
}

// return le num decla du lexeme
// Chebbah/Cinque/Arnaud/Mehenni
int assoc_nom(int num_lex, enum Nature nature)
{
    int i = num_lex;
    int nb_min_region_englob = MAX_PILE_NUM_REGION;
    int nb_region_englob;
    int res = -1;

    while (i != -1)
    {

        if (t_declarations[i].nature == nature || ((t_declarations[i].nature == TYPE_S || t_declarations[i].nature == TYPE_T) && (nature == TYPE_S || nature == TYPE_T)) || ((t_declarations[i].nature == PROC || t_declarations[i].nature == FCT) && (nature == PROC || nature == FCT)) || ((t_declarations[i].nature == VAR || t_declarations[i].nature == PARAM) && (nature == VAR || nature == PARAM)))
        {
            /*
                On a trouvé une decla de même nature, MAIS
                on doit vérifier la portée de cette decla, càd si elle est accessible depuis la region courante.
                Il faut qu'elle soit declarer dans la region courante ou dans l'une des region englobante de la region courante.

                De plus comme on doit prendre en comptes le "masquage", il faut etre sur de retourner la decla la plus interne possible,
            */

            // est-ce que "t_declarations[i].region" englobe "region_courante" ?
            nb_region_englob = englobe_region_courante(t_declarations[i].region);
            if (nb_region_englob > -1) // donc la region r englobe region courante
            {
                if (nb_region_englob == 0)
                {
                    // déclaration locale, il ne peut plus y avoir de decla plus interne que ça
                    res = i;
                    i = -1; // on sort de la boucle
                }
                else if (nb_region_englob < nb_min_region_englob)
                {
                    // on a trouve une declaration encore plus proche à la region courante
                    res = i;
                    nb_min_region_englob = nb_region_englob;
                    i = t_declarations[i].suivant;
                }
            }
            else
                i = t_declarations[i].suivant;
        }
        else
        {
            i = t_declarations[i].suivant;
        }
    }

    return res;
}

// Chebbah/Cinque/Arnaud/Mehenni
int taille(int i_decla)
{
    // Pour tous les types (base et complexes), la taille est dans execution
    if (i_decla >= 0 && i_decla < TAILLE_TABLE_DECLARATIONS)
    {
        return t_declarations[i_decla].execution;
    }

    return -1;
}
