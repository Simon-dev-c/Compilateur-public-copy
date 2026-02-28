#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sauvegarde.h"
#include "table_lexico.h"
#include "table_declarations.h"
#include "table_de_repr.h"
#include "table_region.h"
#include "arbre.h"


extern int t_hcode[TAILLE_TABLE_HCODE];
extern TableLexico t_lexico;
extern TableDeclarations t_declarations;
extern TableRep t_representation;
extern TableRegion t_region;
extern int pcv_d, ipcv_r, pcv_r;

/* ========== SAUVEGARDE ARBRE ========== */
// Chebbah/Cinque/Arnaud/Mehenni
static void sauvegarder_arbre(FILE *f, arbre a)
{
    if (!a)
    {
        fprintf(f, "N ");
        return;
    }

    fprintf(f, "%d ", a->type);

    fprintf(f, "%.0lf %d ", a->num_lex, a->num_decla);

    sauvegarder_arbre(f, a->fils);
    sauvegarder_arbre(f, a->frere);
}

/* ========== SAUVEGARDE PROGRAMME ========== */
// Chebbah/Cinque/Arnaud/Mehenni
void sauvegarder_programme(const char *nom_fichier)
{
    FILE *f = fopen(nom_fichier, "w");
    if (!f)
    {
        fprintf(stderr, "ERREUR: Impossible d'ouvrir '%s'\n", nom_fichier);
        exit(1);
    }

    // Compter les vraies déclarations (celles qui ne sont pas VIDE)
    int nb_declarations = 0;
    for (int i = 0; i < TAILLE_TABLE_DECLARATIONS; i++)
    {
        if (t_declarations[i].nature != -1)
        {
            nb_declarations++;
        }
    }

    // Ligne 1: Compteurs REELS
    fprintf(f, "%d %d %d\n", nb_declarations, ipcv_r, pcv_r);

    // Ligne 2: Table hachage (500 entiers sur une ligne)
    for (int i = 0; i < TAILLE_TABLE_HCODE; i++)
    {
        fprintf(f, "%d ", t_hcode[i]);
    }
    fprintf(f, "\n");

    // Table lexico: compter d'abord combien on en a
    int nb_lexemes = 0;
    for (int i = 0; i < TAILLE_TABLE_LEXICO; i++)
    {
        if (t_lexico[i].longueur > 0)
        {
            nb_lexemes++;
        }
    }

    // Sauvegarder le nombre puis chaque lexème
    fprintf(f, "%d\n", nb_lexemes);
    for (int i = 0; i < TAILLE_TABLE_LEXICO; i++)
    {
        if (t_lexico[i].longueur > 0)
        {
            fprintf(f, "%d %d %s %d\n",
                    i,
                    t_lexico[i].longueur,
                    t_lexico[i].lexeme,
                    t_lexico[i].suivant);
        }
    }

    // Table declarations: SEULEMENT celles qui ne sont pas VIDE
    // Format: index nature suivant region description execution
    for (int i = 0; i < TAILLE_TABLE_DECLARATIONS; i++)
    {
        if (t_declarations[i].nature != -1)
        {
            fprintf(f, "%d %d %d %d %d %d\n",
                    i, // L'INDEX est crucial !
                    t_declarations[i].nature,
                    t_declarations[i].suivant,
                    t_declarations[i].region,
                    t_declarations[i].description,
                    t_declarations[i].execution);
        }
    }

    // Table representation: SEULEMENT les ipcv_r premiers éléments
    for (int i = 0; i < ipcv_r; i++)
    {
        fprintf(f, "%d ", t_representation[i]);
    }
    fprintf(f, "\n");

    // Table regions: SEULEMENT les pcv_r premières régions
    for (int i = 0; i < pcv_r; i++)
    {
        fprintf(f, "%d %d \n",
                t_region[i].NIS,
                t_region[i].taille);
        sauvegarder_arbre(f, t_region[i].arbre);
        fprintf(f, "\n");
    }

    fclose(f);
    printf("Programme sauvegardé dans %s\n", nom_fichier);
}

/* ========== CHARGEMENT ARBRE ========== */
// Chebbah/Cinque/Arnaud/Mehenni
static arbre charger_arbre(FILE *f)
{
    char buffer[100];
    if (fscanf(f, "%s", buffer) != 1)
    {
        return NULL;
    }

    if (buffer[0] == 'N')
    {
        return NULL;
    }

    int type = atoi(buffer);

    if (fscanf(f, "%s", buffer) != 1)
    {
        return NULL;
    }

    double num_lexico = atof(buffer);

    if (fscanf(f, "%s", buffer) != 1)
    {
        return NULL;
    }

    double num_decla = atoi(buffer);

    arbre a;

    a = creer_noeud(type, num_lexico, num_decla);

    a->fils = charger_arbre(f);
    a->frere = charger_arbre(f);

    return a;
}

/* ========== CHARGEMENT PROGRAMME ========== */
// Chebbah/Cinque/Arnaud/Mehenni
int charger_programme(const char *nom_fichier)
{
    FILE *f = fopen(nom_fichier, "r");
    if (!f)
    {
        fprintf(stderr, "ERREUR: Impossible d'ouvrir '%s'\n", nom_fichier);
        return 0;
    }

    // Compteurs
    int nb_declarations, nb_repr, nb_regions;
    if (fscanf(f, "%d %d %d", &nb_declarations, &nb_repr, &nb_regions) != 3)
    {
        fprintf(stderr, "ERREUR: Fichier corrompu (compteurs)\n");
        fclose(f);
        return 0;
    }

    ipcv_r = nb_repr;
    pcv_r = nb_regions;

    // Réinitialiser toutes les déclarations à VIDE
    for (int i = 0; i < TAILLE_TABLE_DECLARATIONS; i++)
    {
        t_declarations[i].nature = -1;
        t_declarations[i].suivant = -1;
        t_declarations[i].region = -1;
        t_declarations[i].description = -1;
        t_declarations[i].execution = -1;
    }

    // Table hachage
    for (int i = 0; i < TAILLE_TABLE_HCODE; i++)
    {
        if (fscanf(f, "%d", &t_hcode[i]) != 1)
        {
            fprintf(stderr, "ERREUR: Fichier corrompu (hachage)\n");
            fclose(f);
            return 0;
        }
    }

    // Table lexico
    int nb_lexemes;
    if (fscanf(f, "%d", &nb_lexemes) != 1)
    {
        fprintf(stderr, "ERREUR: Fichier corrompu (nb lexemes)\n");
        fclose(f);
        return 0;
    }

    for (int i = 0; i < nb_lexemes; i++)
    {
        int idx, longueur, suivant;
        char lexeme[256];

        if (fscanf(f, "%d %d %s %d", &idx, &longueur, lexeme, &suivant) != 4)
        {
            fprintf(stderr, "ERREUR: Fichier corrompu (lexeme %d)\n", i);
            fclose(f);
            return 0;
        }

        t_lexico[idx].longueur = longueur;
        strcpy(t_lexico[idx].lexeme, lexeme);
        t_lexico[idx].suivant = suivant;
    }

    // Table declarations - on lit nb_declarations lignes
    // Format: index nature suivant region description execution
    for (int i = 0; i < nb_declarations; i++)
    {
        int idx, n, s, r, d, e;
        if (fscanf(f, "%d %d %d %d %d %d", &idx, &n, &s, &r, &d, &e) != 6)
        {
            fprintf(stderr, "ERREUR: Fichier corrompu (declaration %d)\n", i);
            fclose(f);
            return 0;
        }

        // Placer la déclaration à son INDEX d'origine
        if (idx >= 0 && idx < TAILLE_TABLE_DECLARATIONS)
        {
            t_declarations[idx].nature = n;
            t_declarations[idx].suivant = s;
            t_declarations[idx].region = r;
            t_declarations[idx].description = d;
            t_declarations[idx].execution = e;
        }
    }

    // Mettre à jour pcv_d (dernier index utilisé + 1)
    pcv_d = 0;
    for (int i = TAILLE_TABLE_DECLARATIONS - 1; i >= 0; i--)
    {
        if (t_declarations[i].nature != -1)
        {
            pcv_d = i + 1;
            break;
        }
    }

    // Table representation
    for (int i = 0; i < nb_repr; i++)
    {
        if (fscanf(f, "%d", &t_representation[i]) != 1)
        {
            fprintf(stderr, "ERREUR: Fichier corrompu (representation %d)\n", i);
            fclose(f);
            return 0;
        }
    }

    // Table regions
    for (int i = 0; i < nb_regions; i++)
    {
        char ligne[1000];
        int nis, taille;

        // Lire NIS et taille
        if (fscanf(f, "%d %d ", &nis, &taille) != 2)
        {
            fprintf(stderr, "ERREUR: Fichier corrompu (region %d - NIS/taille)\n", i);
            fclose(f);
            return 0;
        }

        t_region[i].NIS = nis;
        t_region[i].taille = taille;

        // Charger l'arbre
        t_region[i].arbre = charger_arbre(f);
    }

    printf("Programme chargé depuis %s\n", nom_fichier);

    // 2. Afficher les compteurs
    printf("=== COMPTEURS ===\n");
    printf("pcv_d (nb déclarations) : %d\n", nb_declarations);
    printf("ipcv_r (nb repr)        : %d\n", nb_repr);
    printf("pcv_r (nb régions)      : %d\n", nb_regions);

    fclose(f);
    return 1;
}
