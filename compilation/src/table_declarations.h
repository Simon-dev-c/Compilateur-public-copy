#ifndef _TABLE_DECLA_H_
#define _TABLE_DECLA_H_

#define TAILLE_TABLE_DECLARATIONS 5000

enum Nature
{

    TYPE_B, // ent, reel, bool, car

    TYPE_S, // type structure
    TYPE_T, // type tableau
    VAR,    // déclaration de variable
    PARAM,  // déclaration de paramètre

    PROC, // déclaration procedure
    FCT,  //  déclaration fonction

};

typedef struct
{
    enum Nature nature;
    int suivant;
    int region;
    int description;
    int execution;
} ChampTDCL;

typedef ChampTDCL TableDeclarations[TAILLE_TABLE_DECLARATIONS];

extern TableDeclarations t_declarations;

// Chebbah/Cinque/Arnaud/Mehenni
void init_table_declarations();
// Renvoie le numero de decla
// Sinon -1
// Chebbah/Cinque/Arnaud/Mehenni
int inserer_declaration(int num_lex, enum Nature nature, int region, int description, int exec);
// Chebbah/Cinque/Arnaud/Mehenni
int assoc_nom(int num_lex, enum Nature nature);

// Chebbah/Cinque/Arnaud/Mehenni
int taille(int i_decla);

#endif