%{
    #include <stdio.h>
    #include "../src/arbre.h"
    #include "../obj/y.tab.h"
    #include "../src/table_lexico.h"

    extern int num_ligne;
    extern char * ligne_courante;
    extern int colonne_decalage;

    
%}
X   [^"]
Y   [^"*]
Z   [^"*/]
%%
^[^\n]*\n { //pour garder le pointeur sur le debut d'une nouvelle ligne 
    ligne_courante = yytext;
    colonne_decalage = 0; //nouvelle ligne, on remet le decalage colonne a 0
    REJECT; //reject pour recommencer l'analyse sur les regles plus bas
}
\/\*({Y}|\"{X}*\"|\*\**{Z}|\*\**\"{X}*\")*\*\**\/  {printf("Commentaire détecté !\n");}
"program"   {colonne_decalage+=yyleng; return PROG;}
"type"   {colonne_decalage+=yyleng; return TYPE;}
"var"    {colonne_decalage+=yyleng; return VARIABLE;}
"procedure" {colonne_decalage+=yyleng; return PROCEDURE;}
"fonction"  {colonne_decalage+=yyleng; return FONCTION;}
"retour" {colonne_decalage+=yyleng; return RETOURNE;}
"alors"  {colonne_decalage+=yyleng; return ALORS;}
"&&"    {colonne_decalage+=yyleng; return ET;}
"||"    {colonne_decalage+=yyleng; return OU;}
"si"    {colonne_decalage+=yyleng; return SI;}
"sinon"  {colonne_decalage+=yyleng; return SINON;}
"faire"  {colonne_decalage+=yyleng; return FAIRE;}   
"tant_que"  {colonne_decalage+=yyleng; return TANT_QUE;}
"struct"    {colonne_decalage+=yyleng; return STRUCT;}
"fstruct"   {colonne_decalage+=yyleng; return FSTRUCT;}
"<"     {colonne_decalage+=yyleng; return INF;}
">"     {colonne_decalage+=yyleng; return SUP;}
","     {colonne_decalage+=yyleng; return VIRGULE;}
"=="    {colonne_decalage+=yyleng; return EGAL;}
"!="    {colonne_decalage+=yyleng; return DIFF;}
"<="    {colonne_decalage+=yyleng; return INF_EGAL;}
">="    {colonne_decalage+=yyleng; return SUP_EGAL;}
"vrai"  {colonne_decalage+=yyleng; return VRAI;}
"faux"  {colonne_decalage+=yyleng; return FAUX;}
"tab"   {colonne_decalage+=yyleng; return TABLEAU;}
"de"    {colonne_decalage+=yyleng; return DE;}
"{"     {colonne_decalage+=yyleng; return DEBUT;}
"}"     {colonne_decalage+=yyleng; return FIN;}
"["     {colonne_decalage+=yyleng; return CROCHET_OUVRANT;}
"]"     {colonne_decalage+=yyleng; return CROCHET_FERMANT;}
";"      {colonne_decalage+=yyleng; return POINT_VIRGULE;}
":"      {colonne_decalage+=yyleng; return DEUX_POINTS;}
"<-"     {colonne_decalage+=yyleng; return OPAFF;}
"."      {colonne_decalage+=yyleng; return POINT;}
".."     {colonne_decalage+=yyleng; return POINT_POINT;}
"("      {colonne_decalage+=yyleng; return PO; }
")"      {colonne_decalage+=yyleng; return PF; }
"-"      {colonne_decalage+=yyleng; return MOINS; }
"*"      {colonne_decalage+=yyleng; return MULT; }
"+"      {colonne_decalage+=yyleng; return PLUS; }
"/"      {colonne_decalage+=yyleng; return DIV; }
"!"    {colonne_decalage+=yyleng; return NON; }
"&"     {colonne_decalage+=yyleng; return CONCAT;}
"vide"  {colonne_decalage+=yyleng; return T_VIDE;}
"ent"    {colonne_decalage+=yyleng; return T_ENTIER;}
"car"   {colonne_decalage+=yyleng; return T_CARACTERE;}
"reel"  {colonne_decalage+=yyleng; return T_REEL;}
"bool"   {colonne_decalage+=yyleng; return T_BOOLEEN;}
"chaine" {colonne_decalage+=yyleng; return T_CHAINE;}
"LIRE" {colonne_decalage+=yyleng; return LIRE;}
"ECRIRE" {colonne_decalage+=yyleng; return ECRIRE;}
\'(\\.|[^\\'])\'      {
    colonne_decalage+=yyleng; 
    yylval.type_entier = yytext[1];
    return CSTE_CARACTERE; 
    }
\"([^\"\n]|\\.)*\"    {
    colonne_decalage+=yyleng;
    yylval.type_entier = inserer_lexeme(yytext, yyleng);
    return CSTE_CHAINE; 
    }
-?([1-9][0-9]*|0)[\.][0-9]+ {
    colonne_decalage+=yyleng;
    yylval.type_reel = atof(yytext); 
    return CSTE_REEL;
    }
-?[1-9][0-9]*|0 {
    colonne_decalage+=yyleng;
    yylval.type_entier = atoi(yytext); 
    return CSTE_ENTIERE;
    }
[a-zA-Z][a-zA-Z0-9]*  {
    colonne_decalage+=yyleng;
    yylval.type_entier = inserer_lexeme(yytext, yyleng);
    return IDF;
    }
\n       {num_ligne++;}
[ \t]       {colonne_decalage+=yyleng;} // ignorer les tabulations et les espaces
.          {
    colonne_decalage+=yyleng;
    printf("Erreur Lexicographique à la ligne %d : caractère %s interdit !\n", num_ligne,yytext);
    }
%%


int yywrap(){
    return 1;
}