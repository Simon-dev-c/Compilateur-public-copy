#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "table_lexico.h"
#include "table_declarations.h"
#include "table_de_repr.h"
#include "table_region.h"
#include "affichage/affichage_tables.h"
#include "arbre.h"
#include "sauvegarde.h"
#include "verif_semantique.h"

extern TableRegion t_region;
extern int nb_erreurs_sem;

int yyparse(void);

// Chebbah/Cinque/Arnaud/Mehenni
int main(int argc, char *argv[])
{
    int opt;
    int num_region;

    init_table_hcode();
    init_table_lexico();
    init_table_declarations();
    init_table_rep();
    init_table_region();
    init_num_region();

    // ne pas toucher
    inserer_lexeme("ent", 3);
    inserer_lexeme("reel", 4);
    inserer_lexeme("bool", 4);
    inserer_lexeme("car", 3);

    if (yyparse() != 0 || nb_erreurs_sem > 0)
    {
        printf("========== Compilation echouée avec %d erreurs ==========\n", nb_erreurs_sem);
        liberer_regions_arbre();
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("========== Compilation réussi ==========\n");
        liberer_regions_arbre();
        sauvegarder_programme("programme.txt");
    }

    while ((opt = getopt(argc, argv, "htr:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            printf("Usage : %s < <fichier.txt> [options]", argv[0]);
            printf("Options :\n");
            printf("  -h        Affiche cette aide\n");
            printf("  -t        Affiche toutes les tables\n");
            printf("  -r <num_region>   Affiche l'arbre de la region num_region\n");
            break;
        case 't': // tables
            affiche_tables(stdout);
            printf("\n");
            break;
        case 'r': // affiche l'arbre de la region passé en param
            num_region = atoi(optarg);
            affiche_arbre(t_region[num_region].arbre);
            printf("\n");
            break;
        default:
            break;
        }
    }

    exit(EXIT_SUCCESS);
}