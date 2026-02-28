#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/table_lexico.h"
#include "../src/table_declarations.h"
#include "../src/table_de_repr.h"
#include "../src/table_region.h"
#include "../src/affichage/affichage_tables.h"
#include "../src/arbre.h"
#include "../src/sauvegarde.h"
#include "machine_virtuelle.h"

// Déclarations externes
extern int pcv_d, ipcv_r, pcv_r;
extern TableDeclarations t_declarations;
extern TableLexico t_lexico;
extern TableRep t_representation;
extern TableRegion t_region;
extern cellule pile[TAILLE_PILE];
extern int region_courante_mv;

// Chebbah/Cinque/Arnaud/Mehenni
int main(int argc, char *argv[])
{
    init_table_hcode();
    init_table_lexico();
    init_table_declarations();
    init_table_rep();
    init_table_region();
    init_num_region();
    initialiser_machine_virtuelle();

    if (!charger_programme("programme.txt")) {
        printf("ERREUR: Échec du chargement\n");
        return 1;
    }

    printf("\nChargement réussi !\n\n");

    affiche_tables(stdout);
    printf("\n");
    affiche_arbre(t_region[0].arbre);
    printf("\n");
    executer_programme(region_courante_mv);

    afficher_pile_execution();

    exit(EXIT_SUCCESS);
}