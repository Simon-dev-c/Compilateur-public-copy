#include <stdio.h>
#include <stdlib.h>
#include "table_declarations.h"
#include "table_lexico.h"
#include "affichage/affichage_tables.h"
#include "erreurs.h"
#include "table_region.h"

int num_ligne = 1;
char *ligne_courante;
int colonne_decalage;
extern int yyleng;

void afficher_ligne_erreur()
{
    int i;
    printf("%4d:%-4d|%s\n", num_ligne, colonne_decalage - yyleng, ligne_courante);
    printf("%5s%-4s|", "", "");
    printf("%*s", colonne_decalage - yyleng, "");

    print_color("^", RED);

    for (i = 0; i < yyleng - 1; i++)
    {
        print_color("~", RED);
    }
    printf("\n\n");
}

void afficher_ligne_erreur2(int n)
{
    int i;
    printf("%4d:%-4d|%s\n", num_ligne, n, ligne_courante);
    printf("%5s%-4s|", "", "");
    printf("%*s", n, "");

    print_color("^", RED);

    for (i = 0; i < colonne_decalage; i++)
    {
        print_color("~", RED);
    }
    printf("\n\n");
}

// Chebbah/Cinque/Arnaud/Mehenni
void erreur_redecla(enum Nature nature, int num_lex)
{
    print_error("re-déclaration, un objet de même nature que <%s> et de même nom que <%s> est dejà déclaré dans la même region <%d>\n", natureToStr(nature), lexeme(num_lex), recup_num_region());
    afficher_ligne_erreur2(0);
}

void erreur_syntaxique(const char *message)
{
    print_error("%s\n", message);
    afficher_ligne_erreur();
}

void erreur_decla_var_type_inconnu(int num_lex)
{
    print_error("la déclaration de la variable <%s> fait référence à un type inconnu\n", lexeme(num_lex));
    afficher_ligne_erreur();
}

void erreur_decla_tab_type_elt_inconnu(int num_lex)
{
    print_error("la déclaration du tableau <%s> fait référence à un type d'éléments inconnu\n", lexeme(num_lex));
    afficher_ligne_erreur();
}

void erreur_decla_champ_type_inconnu(int num_lex, int num_lex_struct)
{
    print_error("le champ <%s> de la structure <%s> fait référence à un type inconnu\n", lexeme(num_lex), lexeme(num_lex_struct));
    afficher_ligne_erreur();
}

void erreur_appel_fct_proc_inconnu(int num_lex)
{
    print_error("appel à une fonction ou procedure <%s> inconnu\n", lexeme(num_lex));
    afficher_ligne_erreur();
}

void erreur_var_inconnu(int num_lex)
{
    print_error("la variable <%s> non défini ou hors de portée\n", lexeme(num_lex));
    afficher_ligne_erreur2(0);
}

void erreur_var_champ_inconnu(int num_lex_champ, int num_lex_var)
{
    print_error("la variable <%s> ne possède pas de champ <%s>\n", lexeme(num_lex_var), lexeme(num_lex_champ));
    afficher_ligne_erreur2(0);
}

void erreur_var_struct_tab(int num_lex)
{
    print_error("la variable <%s> n'est pas un tableau mais une structure\n", lexeme(num_lex));
    afficher_ligne_erreur2(0);
}

void erreur_var_tab_struct(int num_lex)
{
    print_error("la variable <%s> n'est pas une structure mais un tableau\n", lexeme(num_lex));
    afficher_ligne_erreur2(0);
}

void erreur_dimensions_tab_invalide(int num_lex)
{
    print_error("les dimensions du tableau <%s> sont inavlides\n", lexeme(num_lex));
    afficher_ligne_erreur2(0);
}

void erreur_var_type_base(int num_lex)
{
    print_error("la variable <%s> n'est ni un tableau ni une structure, mais une variable de type base\n", lexeme(num_lex));
    afficher_ligne_erreur2(0);
}

void erreur_affectation_type()
{
    print_error("l'affectation est impossible, type incompatible\n");
    afficher_ligne_erreur2(0);
}

void erreur_fct_retourne(int num_lex)
{
    print_error("la fonction <%s> doit terminé par l'instruction RETOURNE\n", lexeme(num_lex));
    afficher_ligne_erreur();
}

void erreur_dans_prog_principale()
{
    print_error("retour dans le prog principale interdit\n");
    afficher_ligne_erreur();
}

void erreur_type_retour()
{
    print_error("le type du retour est différent du type de la procedure ou fonction\n");
    afficher_ligne_erreur();
}

void erreur_args(int num_lex, int nb_args, int nb_params)
{
    print_error("appel à la fonction ou procedure <%s> avec un nombre d'arguments différent, %d au lieu de %d attendu\n", lexeme(num_lex), nb_args, nb_params);
    afficher_ligne_erreur();
}

void erreur_arg_type(int num_lex, int num_arg)
{
    print_error("le type de l'argument n°%d de la fonction ou procedure <%s> est un incompatible avec le type du parametre\n", num_arg, lexeme(num_lex));
    afficher_ligne_erreur();
}

void erreur_expr_arith()
{
    print_error("les operandes de l'expression arithmetique doivent être de même type\n");
    afficher_ligne_erreur2(0);
}