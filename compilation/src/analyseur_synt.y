%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "../src/table_declarations.h"
    #include "../src/table_de_repr.h"
    #include "../src/table_region.h"
    #include "../src/table_lexico.h"
    #include "../src/erreurs.h"
    #include "../src/verif_semantique.h"
    #include "../src/arbre.h"

    int yylex();

    int num_lex_decla_type;
    int num_type_retour;

    int nb_erreurs_sem = 0; // Compteur d'erreurs sémantiques

    void yyerror(const char *message){
        erreur_syntaxique(message);
        nb_erreurs_sem++;
    }

%}

%define parse.error verbose

%union{
    arbre type_arbre;
    int type_entier;
    double type_reel;
}

/* token type entier   */

%token<type_entier> CSTE_ENTIERE CSTE_CARACTERE CSTE_CHAINE IDF
%token<type_reel> CSTE_REEL

/* Mots-clefs principaux */
%token<type_entier> PROG DEBUT FIN
%token SI ALORS SINON TANT_QUE FAIRE
%token<type_entier> PROCEDURE FONCTION RETOURNE


/* Types */
%token TYPE T_CHAINE T_ENTIER T_CARACTERE T_REEL T_BOOLEEN T_VIDE


/* Identificateurs et variables */
%token VARIABLE OPAFF "<-"


/* Symboles */
%token DEUX_POINTS ":" POINT_VIRGULE ";" POINT_POINT ".." VIRGULE "," POINT "."
%token CROCHET_OUVRANT "[" CROCHET_FERMANT "]" PO "(" PF ")"
%token<type_entier> TABLEAU STRUCT FSTRUCT

/* Operateurs */
%token<type_entier> PLUS MOINS MULT DIV CONCAT
%token SUP ">" INF "<" SUP_EGAL ">=" INF_EGAL "<=" EGAL "==" DIFF "!="
%token OU "||" ET "&&" NON "!"


/* Booleens */
%token VRAI FAUX /* 1 = VRAI, 0 = FAUX */


/* Divers */
%token DE

/* procedures predefinies  */
%token LIRE ECRIRE

/* symb nn termi avec type <type_entier>   */
%type<type_entier> type_simple nom_type suite_declaration_type

/* symb nn term type <arb> */

%type<type_arbre> suite_liste_inst instruction
%type<type_arbre> conditionelle sinon_block tant_que affectation resultat_retourne
%type<type_arbre> appel liste_arguments liste_args un_arg
%type<type_arbre> lecture ecriture liste_variables li 


/* Symboles avec type ET arbre (nouvelle structure expr) */
%type<type_arbre> expression expression_arith e1 e2 e3
%type<type_arbre> expression_booleenne eb eb1 eb2 eb3 comparaison
%type<type_arbre> variable

/* Completion */
%type<type_arbre> declaration_variable liste_indice_acces suite_ecriture

%type<type_entier> liste_dimensions une_dimension

// Pas dans l'arbre donc a remplacé par une fonction
%type<type_entier> liste_champs un_champ

%type<type_arbre> declaration_procedure declaration_fonction corps

/* non typés */
/* ld ldt ldv ldpf suite_lpdf liste_parametres liste_param un_param declaration_type */


%%
programme: PROG  {
        inserer_regions();
        incremente_num_region();
        empiler_num_region();
    }
    corps
    ;

corps:
    DEBUT {$1 = num_type_retour;} // sauvegarde du num_type_retour de la donction avant la ld
    ld {num_type_retour = $1;} // apres la ld on peut restaurer le bon num_type_retour
    li 
    FIN {
        /* On insere l'arbre des instruction pour ce corps dans la table des regions */
        inserer_arbre_region($5);
        $$ = $5;
    }
    ;

/* ------------------- Liste de déclaration -------------------- */

ld:
    ldt ldv ldpf
    ;
    
ldt:
    | declaration_type POINT_VIRGULE ldt
    ;

ldv:
    | declaration_variable POINT_VIRGULE ldv
    ;

ldpf:
    | suite_lpdf ldpf 
    ;
    
suite_lpdf: 
    declaration_fonction 
    | declaration_procedure
    ;

// Decla Struct, Tableau
declaration_type:
    TYPE 
    IDF { num_lex_decla_type = $2; } // on en a besoin car la regle suite_declaration_type l'utilise
    DEUX_POINTS
    suite_declaration_type
    ;

suite_declaration_type :
    STRUCT { $1 = init_struct(); /*renvoie l'indice dans la table de representation des types...*/ }
    liste_champs { inserer_nb_champs(); }
    FSTRUCT {
        if ((inserer_declaration(num_lex_decla_type, TYPE_S, recup_num_region(), $1, $3)) == -1) {
            erreur_redecla(TYPE_S, num_lex_decla_type);
            nb_erreurs_sem++;
        }
    }
    |
    TABLEAU { $1 = init_tableau(); }
    dimension { inserer_nb_dimensions(); }
    DE nom_type {

        if ($6 == -1) {
            erreur_decla_tab_type_elt_inconnu(num_lex_decla_type);
            nb_erreurs_sem++;
        }

        inserer_type_elt($6);
        
        if ((inserer_declaration(num_lex_decla_type, TYPE_T, recup_num_region(), $1, calcul_taille_tab()) == -1)) {
            erreur_redecla(TYPE_T, num_lex_decla_type);
            nb_erreurs_sem++;
        }
    }
    ;

liste_champs:
    un_champ {
        increment_nb_champs();
        $$ = $1;
    }
    |liste_champs POINT_VIRGULE un_champ {
        increment_nb_champs();
        $$ = $3;
    }
    ;

un_champ: IDF DEUX_POINTS nom_type {
            if ($3 == -1) {
                erreur_decla_champ_type_inconnu($1, num_lex_decla_type);
                nb_erreurs_sem++;
            }

            inserer_tab_rep($1);
            inserer_tab_rep($3); 
            inserer_depl();

            $$ = increment_depl(taille($3));

        }
    ;

dimension: CROCHET_OUVRANT liste_dimensions CROCHET_FERMANT 
    ;

liste_dimensions:
    liste_dimensions VIRGULE une_dimension
    |une_dimension
    ;

une_dimension: CSTE_ENTIERE POINT_POINT CSTE_ENTIERE {        
        inserer_tab_rep($1);
        inserer_tab_rep($3);

        increment_nb_dimension();
    }
    ;

// Decla variable
declaration_variable : VARIABLE IDF DEUX_POINTS nom_type {
        if ($4 == -1) {
            erreur_decla_var_type_inconnu($2);
            nb_erreurs_sem++;
        }

        if ((inserer_declaration($2, VAR, recup_num_region(), $4, recup_depl_courant())) == -1) {
            erreur_redecla(VAR, $2);
            nb_erreurs_sem++;
        } else {
            // Incrémenter le déplacement seulement si l'insertion a réussi
            increment_depl_region(taille($4));
        }
    }
    ;

// Decla procedure et fonction
declaration_procedure: 
    PROCEDURE {
        $1 = recup_num_region(); // region englobante
        inserer_regions();
        
        incremente_num_region();
        empiler_num_region();
       
    }
    IDF {
        if ((inserer_declaration($3, PROC, $1, init_procedure(), recup_num_region())) == -1) {
            erreur_redecla(PROC, $3);
            nb_erreurs_sem++;
        }
    }
    liste_parametres {
        inserer_nb_params();
        num_type_retour = -2; // une proc n'a pas de type de retour
    }
    corps {   
        depiler_region();
    }
    ;

declaration_fonction : 
    FONCTION {
        $1 = recup_num_region(); // region englobante
        inserer_regions();
        incremente_num_region();
        empiler_num_region();
    }
    IDF {
            if ((inserer_declaration($3, FCT, $1, init_fonction(), recup_num_region())) == -1) {
                erreur_redecla(FCT, $3);
                nb_erreurs_sem++;
            }
        }
    liste_parametres {inserer_nb_params();}
    RETOURNE 
    type_simple {
        inserer_fonction_type_retour($8);
        num_type_retour = $8;
    }
    corps { 
        depiler_region();
       
        // verifier le corps de la fonction si 
        if (verif_sem_corps_fct($10, $3) == -1) {
            nb_erreurs_sem++;
        }
    }
    ;

liste_parametres:
    PO PF 
    |PO liste_param PF 
    ;

liste_param:
    un_param
    |liste_param POINT_VIRGULE un_param
    ;

un_param: IDF DEUX_POINTS type_simple {
            increment_nb_params();

            if ((inserer_declaration($1, PARAM, recup_num_region(), $3, recup_depl_courant())) == -1) {
                erreur_redecla(PARAM, $1);
                nb_erreurs_sem++;
            }

            // Incrémenter la taille de la zone de données de la region
            increment_depl_region(taille($3));

            inserer_tab_rep($1); // insere le num lexico
            inserer_tab_rep($3); // puis son num type
    }
    ;

/* ------------------- Liste d'instructions -------------------- */


li: {$$ = creer_noeud(A_LISTE_INSTRUCTION, -1, -1);} // On a pas d'instruction mais quand meme crée un noeud on sait jamais...
    |suite_liste_inst {$$ = $1;}
    ;
    
suite_liste_inst:    
    instruction {$$ = concat_pere_fils(creer_noeud(A_LISTE_INSTRUCTION, -1, -1), $1);}
    |instruction suite_liste_inst {
        $$ = concat_pere_fils(creer_noeud(A_LISTE_INSTRUCTION, -1, -1), concat_pere_frere($1, $2));
    }
    ;

instruction : 
    conditionelle {$$ = $1;} // construction arbre ok
    |tant_que {$$ = $1;} // construction arbre ok
    |appel POINT_VIRGULE {$$ = $1;} 
    |RETOURNE resultat_retourne POINT_VIRGULE {
        $$ = concat_pere_fils(creer_noeud(A_RETOURNE, num_type_retour, -1), $2);
        if (verif_sem_retour($$) == -1) {
            nb_erreurs_sem++;
        }
    }
    |affectation POINT_VIRGULE {
        // verifier la semantique de l'affectation
        if (verif_sem_affectation($1) == -1) {
            nb_erreurs_sem++;
        }
        $$ = $1;
    } 
    |lecture POINT_VIRGULE  {$$ = $1;}
    |ecriture POINT_VIRGULE {$$ = $1;}
    ;


nom_type:
    type_simple {$$ = $1;}
    |IDF {
        $$ = assoc_nom($1, TYPE_S);
    }
    ;

type_simple:
    T_ENTIER {$$ = 0;}
    |T_REEL {$$ = 1;}
    |T_BOOLEEN {$$ = 2;}
    |T_CARACTERE {$$ = 3;}
    ;

resultat_retourne :
    {$$ = arbre_vide();}
    | expression {$$ = $1;}
    ;


appel: 
    IDF liste_arguments {
        int i_d = assoc_nom($1, PROC);

        if (i_d != -1) {
            if (t_declarations[i_d].nature == FCT) {
                $$ = concat_pere_fils(creer_noeud(A_APPEL_FCT, $1, i_d), $2);
            } else {
                $$ = concat_pere_fils(creer_noeud(A_APPEL_PROC, $1, i_d), $2);
            }

            if (verif_sem_appel($$) == -1) {
                nb_erreurs_sem++;
            }
        } else {
            // appelle à une fct ou proc non déclaré ou hors de porté
            erreur_appel_fct_proc_inconnu($1);
            // on crée un noeud instruction vide, juste pour pas que ça casse tout
            $$ = creer_noeud(A_LISTE_INSTRUCTION, -1, -1);
        }
    }
    ;

liste_arguments :
    PO PF { $$ = creer_noeud(A_LISTE_PARAM, -1, -1);}
    |PO liste_args PF  {$$ = concat_pere_fils(creer_noeud(A_LISTE_PARAM, -1, -1), $2);}
    ;

liste_args:
    un_arg {$$ = $1;}
    |un_arg VIRGULE liste_args {$$ = concat_pere_frere($1, $3);}
    ;

un_arg: 
    expression {$$ = $1;}
    ;

conditionelle : 
    SI PO expression PF ALORS DEBUT li FIN sinon_block {
        $$ = concat_pere_fils(creer_noeud(A_IF_THEN_ELSE, -1, -1), concat_pere_frere($3,concat_pere_frere($7, $9)));
    }
    ;

sinon_block: {$$ = arbre_vide();}
    |SINON DEBUT li FIN { $$ = $3; }
    |SINON conditionelle { $$ = $2; }
    ;

tant_que: TANT_QUE PO expression PF FAIRE DEBUT li FIN {
        $$ = concat_pere_fils(creer_noeud(A_TANT_QUE, -1, -1), concat_pere_frere($3, $7));
    };

affectation: 
    variable OPAFF expression {
        $$ = concat_pere_fils(creer_noeud(A_OPAFF, -1, -1), concat_pere_frere(concat_pere_fils(creer_noeud(A_VARIABLE, -1, -1), $1), $3));
    }
    ;

variable: 
    IDF {
        $$ = creer_noeud(A_IDF, $1, -1);
    }
    | IDF POINT variable {
        $$ = concat_pere_frere(creer_noeud(A_IDF, $1, -1), $3);
    }
    | IDF CROCHET_OUVRANT liste_indice_acces CROCHET_FERMANT {
        $$ = concat_pere_frere(creer_noeud(A_IDF, $1, -1), concat_pere_fils(creer_noeud(A_LISTE_INDICES, -1, -1), $3));
    }
    | IDF CROCHET_OUVRANT liste_indice_acces CROCHET_FERMANT POINT variable {
        $$ = concat_pere_frere(creer_noeud(A_IDF, $1, -1), concat_pere_frere( concat_pere_fils(creer_noeud(A_LISTE_INDICES, -1, -1), $3),$6));
    }
    ;

liste_indice_acces:
    expression_arith { $$ = $1; }
    |liste_indice_acces VIRGULE expression_arith { 
        $$ = concat_pere_frere($1, $3);
    }
    ;

expression :
    expression_arith {
        if ($1->type == A_CSTE_ENTIERE 
        || $1->type == A_CSTE_REEL 
        || $1->type == A_VARIABLE 
        || $1->type == A_APPEL_FCT 
        || $1->type == A_APPEL_PROC) {
            $$ = $1;
        } else {
            $$ = concat_pere_fils(creer_noeud(A_EXPRESSION_ARITHMETIQUE, -1, -1), $1);
        }
    } 
    |expression_booleenne {$$ = $1;} // ok
    | CSTE_CARACTERE {$$ = creer_noeud(A_CSTE_CARACTERE, $1, -1);}
    | CSTE_CHAINE {$$ = creer_noeud(A_CSTE_CHAINE, $1, -1);}
    ;


expression_arith : 
    e1 {$$ = $1;}  
    ;

e1 : e1 PLUS e2 {
        $$ = concat_pere_fils(creer_noeud(A_PLUS, -1, -1), concat_pere_frere($1, $3));
    }
    |e1 MOINS e2 {
        $$ = concat_pere_fils(creer_noeud(A_MOINS, -1, -1), concat_pere_frere($1, $3));
    }
    |e2 {$$ = $1;}
    ;

e2 : e2 MULT e3 {
       $$ = concat_pere_fils(creer_noeud(A_MULT, -1, -1), concat_pere_frere($1, $3));
    }
    |e2 DIV e3 {
        $$ = concat_pere_fils(creer_noeud(A_DIV, -1, -1), concat_pere_frere($1, $3));
    }
    |e3 {$$ = $1;}
    ;

e3: PO e1 PF {$$ = $2;}
    |CSTE_ENTIERE { 
        $$ = creer_noeud(A_CSTE_ENTIERE, $1, -1); 
    }
    |CSTE_REEL { 
        $$ = creer_noeud(A_CSTE_REEL, $1, -1); 
    }
    |MOINS e3 {
        $$ = concat_pere_fils(creer_noeud(A_MOINS, -1, -1), $2);
    }
    |variable { 
        $$ = concat_pere_fils(creer_noeud(A_VARIABLE, -1, -1), $1); 
    }
    |appel { $$ = $1; }
    ;


expression_booleenne : eb {
        $$ = concat_pere_fils(creer_noeud(A_EXPRESSION_BOOLEENNE, -1, -1), $1);
    }
    ;

eb: eb OU eb1{
        $$ = concat_pere_fils(creer_noeud(A_OU, -1, -1), concat_pere_frere($1, $3));
    }
    |eb1 {$$ = $1;}
    ;

eb1: eb1 ET eb2 {
        $$ = concat_pere_fils(creer_noeud(A_ET, -1, -1), concat_pere_frere($1, $3));
    } 
    |eb2 {$$ = $1;}
    ;

eb2: NON eb3 {
        $$ = concat_pere_fils(creer_noeud(A_NON, -1, -1), $2);
    }
    |eb3 {$$ = $1;}
    ;

eb3: VRAI {
        $$ = creer_noeud(A_CSTE_BOOLEEN, 1, -1);
    }  
    |FAUX {
        $$ = creer_noeud(A_CSTE_BOOLEEN, 0, -1);
    } 
    |comparaison {$$ = $1;}   
    |PO eb PF  {$$ = $2;}
    ;


comparaison: 
    expression_arith INF expression_arith {
        $$ = concat_pere_fils(creer_noeud(A_INF, -1, -1), concat_pere_frere($1, $3));
    }
    | expression_arith SUP expression_arith {
        $$ = concat_pere_fils(creer_noeud(A_SUP, -1, -1), concat_pere_frere($1, $3));
    }
    | expression_arith INF_EGAL expression_arith {
        $$ = concat_pere_fils(creer_noeud(A_INF_EGAL, -1, -1), concat_pere_frere($1, $3));
    }
    | expression_arith SUP_EGAL expression_arith {
        $$ = concat_pere_fils(creer_noeud(A_SUP_EGAL, -1, -1), concat_pere_frere($1, $3));
    }
    | expression_arith EGAL expression_arith {  
        $$ = concat_pere_fils(creer_noeud(A_EGAL, -1, -1), concat_pere_frere($1, $3));
    }
    | expression_arith DIFF expression_arith {
        $$ = concat_pere_fils(creer_noeud(A_DIFF, -1, -1), concat_pere_frere($1, $3));
    }
    ;

/* ----------------------------Procédures prédéfinies--------------------------- */

lecture: LIRE PO liste_variables PF  {    

    $$ = concat_pere_fils(creer_noeud(A_LIRE, -1, -1), concat_pere_fils(creer_noeud(A_LISTE_VARIABLES, -1, -1), $3));

    }
    ;

ecriture: ECRIRE PO CSTE_CHAINE suite_ecriture PF {
        $$ = concat_pere_fils(creer_noeud(A_ECRIRE, -1, -1), concat_pere_frere(creer_noeud(A_CSTE_CHAINE, $3, -1), $4));
    }
    ;

liste_variables: 
    variable {
        $$ = concat_pere_fils(creer_noeud(A_VARIABLE, -1, -1), $1);
    }
    | variable VIRGULE liste_variables {
        $$ = concat_pere_frere(concat_pere_fils(creer_noeud(A_VARIABLE, -1, -1), $1), $3);
    }
    ;

suite_ecriture: {$$ = arbre_vide();}
    |VIRGULE liste_variables {$$ = $2;}
    ;
%%
