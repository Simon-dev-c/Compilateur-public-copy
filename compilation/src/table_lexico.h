#ifndef _TABLE_LEXICO_H_
#define _TABLE_LEXICO_H_

#define TAILLE_TABLE_HCODE 32
#define TAILLE_TABLE_LEXICO 500

typedef struct
{
    int longueur;
    char lexeme[256];
    int suivant;
} ChampTL;

typedef ChampTL TableLexico[TAILLE_TABLE_LEXICO];
typedef int TableHashCode[TAILLE_TABLE_HCODE];

// Chebbah/Cinque/Arnaud/Mehenni
void init_table_lexico();
// Chebbah/Cinque/Arnaud/Mehenni
void init_table_hcode();

// Chebbah/Cinque/Arnaud/Mehenni
int hcode(char *lexeme);
// Chebbah/Cinque/Arnaud/Mehenni
int inserer_lexeme(char *lexeme, int len);
// Chebbah/Cinque/Arnaud/Mehenni
char *lexeme(int num_lexico);

/* Variables globales de la table lexicographique */
extern TableLexico t_lexico;
extern TableHashCode t_hcode;
extern int pcv;


#endif