#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "machine_virtuelle.h"
#include "../src/table_lexico.h"
#include "../src/table_declarations.h"
#include "../src/table_region.h"
#include "../src/arbre.h"

/* ========== VARIABLES GLOBALES ========== */

cellule pile[TAILLE_PILE];
int sommet_pile;
int BC;  // Base courante
int region_courante_mv;
int NIS_precedent; // le NIS de la zone de mémoire précédente dans la pile d'execution

extern TableDeclarations t_declarations;
extern TableRegion t_region;
extern TableLexico t_lexico;

/* ========== INITIALISATION ========== */

// Chebbah/Cinque/Arnaud/Mehenni
void initialiser_machine_virtuelle() {
    sommet_pile = 0;
    BC = 0;
    region_courante_mv = 0;
    NIS_precedent = 0;

    for (int i = 0; i < TAILLE_PILE; i++) {
        pile[i].entier = -1;
    }

    fprintf(stderr,"Machine virtuelle initialisée\n");
}

/* ========== AFFICHAGE PILE ========== */

// Chebbah/Cinque/Arnaud/Mehenni
void afficher_pile_execution() {
    fprintf(stderr," ==== Affichage Pile d'execution ==== \n");
    for (int i = 0; i < sommet_pile; i++) {
        fprintf(stderr,"%d : entier=%d reel=%f bool=%d car=%c\n",i, pile[i].entier, pile[i].reel, pile[i].booleen, pile[i].caractere);
    }
}

/* ========== GESTION DES ADRESSES ========== */

/*
 * Renvoie le déplacement pour acceder à la zone d'une variable
 * ou 
 * Renvoie la BC d'une proc/fct
 */
// Chebbah/Cinque/Arnaud/Mehenni
int calculer_adresse_pile(int num_decl){ 
    
    int deplacement_total;
    int deplacement_variable;
    int chainage_dyn;
    int chainage_stat;

    int region_decl;

    int base_region_decl;
    int diff_NIS;

    if (t_declarations[num_decl].nature == VAR || t_declarations[num_decl].nature == PARAM){
        deplacement_variable = t_declarations[num_decl].execution;

        region_decl = t_declarations[num_decl].region;

        if (region_decl == 0){
            chainage_stat = 1; // On met un chainage dynamique et statique à la région 0.
        }else{
            chainage_stat = t_region[region_decl].NIS;
            deplacement_variable = deplacement_variable - (t_region[region_courante_mv].NIS + 1);
        }

        chainage_dyn = 1;

        deplacement_total = chainage_dyn + chainage_stat + deplacement_variable;

        diff_NIS = t_region[region_courante_mv].NIS - t_region[region_decl].NIS;
        if (region_courante_mv == 0){
            fprintf(stderr, "Region courante : %d, deplacement_total : %d\n",region_courante_mv,deplacement_total);
            return deplacement_total; // commence à 0
        }else{
            if (diff_NIS == 0){
                fprintf(stderr, "Region courante : %d, deplacement_total : %d BC : %d \n",region_courante_mv,deplacement_total, BC);
                return deplacement_total + BC;
            }else{
                fprintf(stderr, "BC: %d diff_NIS : %d deplacement_total : %d\n",BC, diff_NIS, deplacement_total);
                return pile[BC + diff_NIS].entier + deplacement_total; // commence à NIS + 1
            }
        }  
    }
    fprintf(stderr,"calculer adresse pile avec des données ni var ni param ni fct ni proc \n");
    return BC + t_declarations[num_decl].execution;
}

/* ========== ÉVALUATION DES EXPRESSIONS ========== */

// Chebbah/Cinque/Arnaud/Mehenni
int evaluer_entier(arbre a) {
    if (!a) return 0;

    switch (a->type) {
        case A_CSTE_ENTIERE: return a->num_lex;

        case A_IDF:
            return pile[calculer_adresse_pile(a->num_decla)].entier;
        case A_VARIABLE:
            return evaluer_entier(a->fils);

        case A_EXPRESSION_ARITHMETIQUE:
            return evaluer_entier(a->fils);

        case A_PLUS:
            return evaluer_entier(a->fils) + evaluer_entier(a->fils->frere);

        case A_MOINS:
            return a->fils->frere
                   ? evaluer_entier(a->fils) - evaluer_entier(a->fils->frere)
                   : -evaluer_entier(a->fils);

        case A_MULT:
            return evaluer_entier(a->fils) * evaluer_entier(a->fils->frere);

        case A_DIV: {
            int d = evaluer_entier(a->fils->frere);
            if (d == 0) { fprintf(stderr,"Division par zéro\n"); exit(EXIT_FAILURE); }
            return evaluer_entier(a->fils) / d;
        }

        case A_MOD: {
            int d = evaluer_entier(a->fils->frere);
            if (d == 0) { fprintf(stderr,"Modulo par zéro\n"); exit(EXIT_FAILURE); }
            return evaluer_entier(a->fils) % d;
        }

        default:
            fprintf(stderr,"Type entier non géré %d\n", a->type);
            return 0;
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
int evaluer_booleen(arbre a) {
    if (!a) return 0;

    switch (a->type) {
        case A_CSTE_BOOLEEN: return a->num_lex;

        case A_IDF:
            return pile[calculer_adresse_pile(a->num_decla)].booleen;
        case A_VARIABLE:
            return evaluer_booleen(a->fils);

        case A_EXPRESSION_BOOLEENNE:
            return evaluer_booleen(a->fils);

        case A_ET:
            return evaluer_booleen(a->fils) && evaluer_booleen(a->fils->frere);

        case A_OU:
            return evaluer_booleen(a->fils) || evaluer_booleen(a->fils->frere);

        case A_NON:
            return !evaluer_booleen(a->fils);

        case A_INF: return evaluer_entier(a->fils) < evaluer_entier(a->fils->frere);
        case A_INF_EGAL: return evaluer_entier(a->fils) <= evaluer_entier(a->fils->frere);
        case A_SUP: return evaluer_entier(a->fils) > evaluer_entier(a->fils->frere);
        case A_SUP_EGAL: return evaluer_entier(a->fils) >= evaluer_entier(a->fils->frere);
        case A_EGAL: return evaluer_entier(a->fils) == evaluer_entier(a->fils->frere);
        case A_DIFF: return evaluer_entier(a->fils) != evaluer_entier(a->fils->frere);

        default:
            fprintf(stderr,"Type booléen non géré %d\n", a->type);
            return 0;
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
float evaluer_reel(arbre a) {
    if (!a) return 0.0;

    switch (a->type) {
        case A_CSTE_REEL: return a->num_lex;
        case A_CSTE_ENTIERE: return (float)a->num_lex;

        case A_IDF:
            return pile[calculer_adresse_pile(a->num_decla)].reel;
        case A_VARIABLE:
            return evaluer_reel(a->fils);

        case A_PLUS:
            return evaluer_reel(a->fils) + evaluer_reel(a->fils->frere);

        case A_MOINS:
            return a->fils->frere
                   ? evaluer_reel(a->fils) - evaluer_reel(a->fils->frere)
                   : -evaluer_reel(a->fils);

        case A_MULT:
            return evaluer_reel(a->fils) * evaluer_reel(a->fils->frere);

        case A_DIV: {
            float d = evaluer_reel(a->fils->frere);
            if (d == 0.0) { fprintf(stderr,"Division par zéro\n"); exit(EXIT_FAILURE); }
            return evaluer_reel(a->fils) / d;
        }

        default:
            fprintf(stderr,"Type réel non géré %d\n", a->type);
            return 0.0;
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
char evaluer_caractere(arbre a) {
    if (!a) return '\0';

    switch (a->type) {
        case A_CSTE_CARACTERE:
            return (char)a->num_lex;

        case A_IDF:
            return pile[calculer_adresse_pile(a->num_decla)].caractere;
        case A_VARIABLE:
            return evaluer_caractere(a->fils);

        default:
            fprintf(stderr,"Type caractère non géré %d\n", a->type);
            return '\0';
    }
}


/* ========== EXÉCUTION DES INSTRUCTIONS ========== */

// Chebbah/Cinque/Arnaud/Mehenni
void executer_instruction(arbre a);

// Chebbah/Cinque/Arnaud/Mehenni
void executer_affectation(arbre a) {
    arbre var = a->fils;
    arbre expr = var->frere;
    int num_decla = var->fils->num_decla;
    fprintf(stderr, "Affectation : arbre : %d  fils : %d  fils->frere : %d \n",a->type, var->type, expr->type);
    fprintf(stderr, "Affectation : fils->fils : %d  fils->fils->num_decl : %d \n",var->fils->type,var->fils->num_decla);
    int adr = calculer_adresse_pile(num_decla);
    fprintf(stderr, "Affectation : calculer adresse pile(%d) = %d \n",num_decla,adr);
    int type = t_declarations[num_decla].description;

    if (type == 0){
        pile[adr].entier = evaluer_entier(expr);
        fprintf(stderr,"case %d : %d \n",adr, pile[adr].entier);
    }
    else if (type == 1){
        pile[adr].reel = evaluer_reel(expr);
        fprintf(stderr,"case %d : %f \n",adr, pile[adr].reel);
    }
    else if (type == 2){
        pile[adr].booleen = evaluer_booleen(expr);
        fprintf(stderr,"case %d : %d \n",adr, pile[adr].booleen);
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
void executer_si(arbre a) {
    arbre cond = a->fils;
    arbre alors = cond->frere;
    arbre sinon = alors ? alors->frere : NULL;

    if (evaluer_booleen(cond)) {
        if (alors) executer_instruction(alors);
    } else {
        if (sinon) executer_instruction(sinon);
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
void executer_tant_que(arbre a) {
    arbre cond = a->fils;
    arbre bloc = cond->frere;

    while (evaluer_booleen(cond)) {
        executer_instruction(bloc);
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
void executer_lire(arbre a) {
    arbre var = a->fils;
    int adr = calculer_adresse_pile(var->num_decla);
    fprintf(stderr, "calculer adresse pile(%d) = %d \n",var->num_decla,adr);
    int type = t_declarations[var->num_decla].description;

    if (type == 0) scanf("%d", &pile[adr].entier);
    else if (type == 1) scanf("%f", &pile[adr].reel);
}

// Chebbah/Cinque/Arnaud/Mehenni
void executer_ecrire(arbre a) {
    arbre e = a->fils;

    switch (e->type) {
        case A_CSTE_ENTIERE:
        case A_PLUS:
        case A_MOINS:
        case A_MULT:
        case A_DIV:
            fprintf(stderr,"%d\n", evaluer_entier(e));
            break;

        case A_CSTE_REEL:
            fprintf(stderr,"%f\n", evaluer_reel(e));
            break;

        case A_CSTE_CARACTERE:
            fprintf(stderr,"%c\n", evaluer_caractere(e));
            break;

        /*case A_CSTE_CHAINE:
        case A_CONCAT:
            fprintf(stderr,"%s\n", evaluer_chaine(e));
            break;
        */

        case A_IDF: {
            int type = t_declarations[e->num_decla].description;
            int adr = calculer_adresse_pile(e->num_decla);
            fprintf(stderr, "calculer adresse pile(%d) = %d \n",e->num_decla,adr);

            if (type == 0) fprintf(stderr,"%d\n", pile[adr].entier);
            else if (type == 1) fprintf(stderr,"%f\n", pile[adr].reel);
            else if (type == 2) fprintf(stderr,"%s\n", pile[adr].booleen ? "vrai" : "faux");
            else if (type == 3) fprintf(stderr,"%c\n", pile[adr].caractere);
            //else if (type == 4) fprintf(stderr,"%s\n", pile[adr].chaine);
            break;
        }
        case A_VARIABLE: 
            executer_ecrire(a->fils);

        default:
            fprintf(stderr,"ECRIRE: type non géré %d\n", e->type);
    }
}


/* ========== APPEL DE PROCÉDURE ========== */

// Chebbah/Cinque/Arnaud/Mehenni
void executer_appel_procedure(arbre a) {
    int num_decl = a->num_decla;
    int num_region_proc = t_declarations[num_decl].execution; // recup region procedure

    int BC_parent;
    int region_parent;

    int NIS_parent;
    int NIS_region_proc;
    int diff_NIS;

    int i;


    // BC_parent = base de la pile de la région appelante
    BC_parent = BC;

    BC = sommet_pile;

    region_courante_mv = num_region_proc;

    // recup NIS de la region parent
    region_parent = t_declarations[num_decl].region;
    NIS_parent = t_region[region_parent].NIS;

    // Recup nis region proc
    NIS_region_proc = t_region[num_region_proc].NIS;

    // Il faut la différence entre le nis de la région précédente et le nis de la region_proc
    diff_NIS = NIS_precedent - NIS_region_proc; // permet de compter de combien de niveau on est descendu
    fprintf(stderr,"diff_NIS = %d | NIS_precedent = %d | NIS_region_proc = %d \n",diff_NIS, NIS_precedent, NIS_region_proc);

    if (diff_NIS < 0){ // si l'on augmente le NIS de 1 on monte un niveau au dessus
        // chainage dynamique
        pile[sommet_pile++].entier = BC_parent; 
        fprintf(stderr,"case %d : %d (chainage dynamique) \n",sommet_pile - 1, pile[sommet_pile-1].entier);

        // chainage statique
        pile[sommet_pile++].entier = BC_parent;
        fprintf(stderr,"case %d : %d (chainage statique) \n",sommet_pile - 1, pile[sommet_pile-1].entier);
        for (i = 0; i < (NIS_region_proc - 1); i++){ // On recopie le chainage statique qui nous interesse.
            pile[sommet_pile++].entier = pile[BC_parent + 1 + i].entier;
            fprintf(stderr,"case %d : %d (chainage statique) \n",sommet_pile - 1, pile[sommet_pile-1].entier);
        }
    }else{
        // chainage dynamique
        pile[sommet_pile++].entier = pile[BC_parent + diff_NIS + 1].entier; 
        fprintf(stderr,"case %d : %d (chainage dynamique) \n",sommet_pile - 1, pile[sommet_pile-1].entier);

        // chainage statique
        for (i = 0; i < NIS_region_proc; i++){ // On recopie le chainage statique qui nous interesse.
            pile[sommet_pile++].entier = pile[BC_parent + diff_NIS + 1 + i].entier;
            fprintf(stderr,"case %d : %d (chainage statique) \n",sommet_pile - 1, pile[sommet_pile-1].entier);
        }
    }

    sommet_pile += t_region[num_region_proc].taille - t_region[num_region_proc].NIS - 1; // on enleve le NIS car on le compte dans les sommet_pile ++;

    NIS_precedent = NIS_region_proc;

    executer_instruction(t_region[num_region_proc].arbre);

    region_courante_mv = region_parent;
}

/* ========== APPEL DE FONCTION ========== */

// Chebbah/Cinque/Arnaud/Mehenni
void executer_appel_fonction(arbre a) { 
    int num_decl = a->num_decla;
    int num_region_proc = t_declarations[num_decl].execution; // recup region procedure

    int BC_parent;
    int region_parent;

    int NIS_parent;
    int NIS_region_proc;
    int diff_NIS;

    int i;


    // BC_parent = base de la pile de la région appelante
    BC_parent = BC;

    BC = sommet_pile;

    region_courante_mv = num_region_proc;

    // recup NIS de la region parent
    region_parent = t_declarations[num_decl].region;
    NIS_parent = t_region[region_parent].NIS;

    // Recup nis region proc
    NIS_region_proc = t_region[num_region_proc].NIS;

    // Il faut la différence entre le nis de la région précédente et le nis de la region_proc
    diff_NIS = NIS_precedent - NIS_region_proc; // permet de compter de combien de niveau on est descendu
    fprintf(stderr,"diff_NIS = %d | NIS_precedent = %d | NIS_region_proc = %d \n",diff_NIS, NIS_precedent, NIS_region_proc);

    if (diff_NIS < 0){ // si l'on augmente le NIS de 1 on monte un niveau au dessus
        // chainage dynamique
        pile[sommet_pile++].entier = BC_parent; 
        fprintf(stderr,"case %d : %d (chainage dynamique) \n",sommet_pile - 1, pile[sommet_pile-1].entier);

        // chainage statique
        pile[sommet_pile++].entier = BC_parent;
        fprintf(stderr,"case %d : %d (chainage statique) \n",sommet_pile - 1, pile[sommet_pile-1].entier);
        for (i = 0; i < (NIS_region_proc - 1); i++){ // On recopie le chainage statique qui nous interesse.
            pile[sommet_pile++].entier = pile[BC_parent + 1 + i].entier;
            fprintf(stderr,"case %d : %d (chainage statique) \n",sommet_pile - 1, pile[sommet_pile-1].entier);
        }
    }else{
        // chainage dynamique
        pile[sommet_pile++].entier = pile[BC_parent + diff_NIS + 1].entier; 
        fprintf(stderr,"case %d : %d (chainage dynamique) \n",sommet_pile - 1, pile[sommet_pile-1].entier);

        // chainage statique
        for (i = 0; i < NIS_region_proc; i++){ // On recopie le chainage statique qui nous interesse.
            pile[sommet_pile++].entier = pile[BC_parent + diff_NIS + 1 + i].entier;
            fprintf(stderr,"case %d : %d (chainage statique) \n",sommet_pile - 1, pile[sommet_pile-1].entier);
        }
    }

    sommet_pile += t_region[num_region_proc].taille - t_region[num_region_proc].NIS - 1; // on enleve le NIS car on le compte dans les sommet_pile ++;

    NIS_precedent = NIS_region_proc;

    executer_instruction(t_region[num_region_proc].arbre);

    region_courante_mv = region_parent;
}

// Chebbah/Cinque/Arnaud/Mehenni
void executer_liste_instructions(arbre a) {
    arbre i = a->fils;
    while (i) {
        executer_instruction(i);
        i = i->frere;
    }
}

// Chebbah/Cinque/Arnaud/Mehenni
void executer_instruction(arbre a) {
    if (!a) return;

    switch (a->type) {
        case A_OPAFF: executer_affectation(a); break;
        case A_IF_THEN_ELSE: executer_si(a); break;
        case A_TANT_QUE: executer_tant_que(a); break;
        case A_LIRE: executer_lire(a); break;
        case A_ECRIRE: executer_ecrire(a); break;
        case A_APPEL_PROC: executer_appel_procedure(a); break;
        case A_APPEL_FCT: executer_appel_fonction(a); break;
        case A_LISTE_INSTRUCTION: executer_liste_instructions(a); break;
        case A_CORPS: executer_instruction(a->fils); break;
        case A_EXPRESSION_ARITHMETIQUE: evaluer_entier(a->fils); break;
        case A_EXPRESSION_BOOLEENNE: evaluer_booleen(a->fils); break;
        //case A_EXPRESSION_CHAINE: evaluer_chaine(a->fils); break;
        case A_LISTE_INDICES:
        case A_LISTE_PARAM:
        case A_LISTE_VARIABLES:
            executer_liste_instructions(a);
            break;

        default: break;
    }
}

/* ========== PROGRAMME PRINCIPAL ========== */

// Chebbah/Cinque/Arnaud/Mehenni
void executer_programme(int num_region) {
    fprintf(stderr,"\n===== DÉBUT EXÉCUTION =====\n");

    region_courante_mv = num_region;
    BC = 0;

    // Pour chainage statique
    pile[BC].entier = num_region; // chainage dynamique region 0
    pile[BC+1].entier = num_region; // chainage statique region 0

    sommet_pile = 2 + t_region[num_region].taille;
    fprintf(stderr,"Sommet pile : %d \n", sommet_pile);

    executer_instruction(t_region[num_region].arbre);

    fprintf(stderr,"\n===== FIN EXÉCUTION =====\n");
}
