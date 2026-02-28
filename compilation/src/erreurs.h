#ifndef _ERREURS_H_
#define _ERREURS_H_

// Chebbah/Cinque/Arnaud/Mehenni
void afficher_ligne_erreur();
void afficher_ligne_erreur2(int n);
void erreur_redecla(enum Nature nature, int num_lex);
void erreur_syntaxique(const char *message);
void erreur_decla_var_type_inconnu(int num_lex);
void erreur_decla_tab_type_elt_inconnu(int num_lex);
void erreur_decla_champ_type_inconnu(int num_lex, int num_lex_struct);
void erreur_appel_fct_proc_inconnu(int num_lex);
void erreur_var_inconnu(int num_lex);
void erreur_var_champ_inconnu(int num_lex_champ, int num_lex_var);
void erreur_var_struct_tab(int num_lex);
void erreur_var_tab_struct(int num_lex);
void erreur_dimensions_tab_invalide(int num_lex);
void erreur_var_type_base(int num_lex);
void erreur_affectation_type();
void erreur_fct_retourne(int num_lex);
void erreur_dans_prog_principale();
void erreur_type_retour();
void erreur_args(int num_lex, int nb_args, int nb_params);
void erreur_arg_type(int num_lex, int num_arg);
void erreur_expr_arith();
#endif