#include <stdio.h>
#include <stdarg.h>
#include "../table_declarations.h"
#include "../table_de_repr.h"
#include "../table_lexico.h"
#include "../table_region.h"
#include "affichage_tables.h"

extern TableLexico t_lexico;
extern TableRep t_representation;
extern TableDeclarations t_declarations;
extern TableRegion t_region;

char *natureToStr(enum Nature nature)
{
    switch (nature)
    {
    case TYPE_B:
        return "TYPE_B";
    case TYPE_S:
        return "TYPE_S";
    case TYPE_T:
        return "TYPE_T";
    case VAR:
        return "VAR";
    case PARAM:
        return "PARAM";
    case PROC:
        return "PROC";
    case FCT:
        return "FCT";
    default:
        return "";
    }
}

void affiche_table_rep()
{
    int largeur_terminal = 150;

    int largeur_colonne = 10;

    int cols_par_ligne = (largeur_terminal - 4) / largeur_colonne; // -4 pour les marges
    if (cols_par_ligne < 1)
        cols_par_ligne = 1;
    if (cols_par_ligne > TAILLE_TABLE_REP)
        cols_par_ligne = TAILLE_TABLE_REP;

    int pos = 0;
    while (pos < TAILLE_TABLE_REP)
    {
        int fin = pos + cols_par_ligne;
        if (fin > TAILLE_TABLE_REP)
            fin = TAILLE_TABLE_REP;
        int nb_cols = fin - pos;

        printf("       ");
        for (int i = pos; i < fin; i++)
        {
            printf("%*d ", 7, i);
        }
        printf("\n");

        printf("      +");
        for (int i = 0; i < nb_cols; i++)
        {
            for (int j = 0; j < 7 + 1; j++)
                printf("-");
            printf("+");
        }
        printf("\n");

        printf("      |");
        for (int i = pos; i < fin; i++)
        {
            printf("%*d |", 7, t_representation[i]);
        }
        printf("\n");

        printf("      +");
        for (int i = 0; i < nb_cols; i++)
        {
            for (int j = 0; j < 7 + 1; j++)
                printf("-");
            printf("+");
        }
        printf("\n\n");

        pos += cols_par_ligne;
    }
}

void affiche_tables(FILE *dest)
{
    int i = 0;

    fprintf(dest, "\tTable Lexicographoque : \t\t\t\t\t Table Déclarations :\n");
    fprintf(dest, "-----------------------------------------------\t\t -----------------------------------------------------------------\n");
    fprintf(dest, "| %-3s | %-8s | %-15s | %-9s | \t | %-5s | %-8s | %-7s | %-9s | %-12s | %-6s |\n", "i", "Longueur", "Lexeme", "Suivant", "i", "Nature", "Suivant", "Région", "Description", "Exec");
    fprintf(dest, "-----------------------------------------------\t\t -----------------------------------------------------------------\n");

    while (i < 520 || i < TAILLE_TABLE_LEXICO)
    {
        char *format1 = "| %3d | %8d | %15s | %9d | \t | %5d | %8s | %7d | %-9d | %12d | %-6d |\n";
        char *format2 = "  %3s   %8s   %15s   %9s   \t | %5d | %8s | %7d | %-9d | %12d | %-6d |\n";

        if (i < TAILLE_TABLE_LEXICO)
        {
            fprintf(dest, format1, i, t_lexico[i].longueur, t_lexico[i].lexeme, t_lexico[i].suivant, i, natureToStr(t_declarations[i].nature), t_declarations[i].suivant, t_declarations[i].region, t_declarations[i].description, t_declarations[i].execution);
        }
        else
        {
            fprintf(dest, format2, "", "", "", "", i, natureToStr(t_declarations[i].nature), t_declarations[i].suivant, t_declarations[i].region, t_declarations[i].description, t_declarations[i].execution);
        }
        i++;
    }

    fprintf(dest, "\tTable e représentation des types...\n");

    affiche_table_rep();
    affiche_table_region();
}

void print_color(const char *txt, const char *color_code)
{
    printf("%s%s\033[0m", color_code, txt);
}

void print_error(const char *txt, ...)
{
    va_list args;

    print_color("ERREUR: ", RED);

    va_start(args, txt);
    vprintf(txt, args);
    va_end(args);
}