#ifndef _AFFICHAGE_H_
#define _AFFICHAGE_H_

#include <stdio.h>

#define RED "\033[31m"
// const char *GREEN = "\033[32m";
// const char *YELLOW = "\033[33m";
// const char *BLUE = "\033[34m";
// const char *MAGENTA = "\033[35m";
// const char *CYAN = "\033[36m";

char *natureToStr(enum Nature nature);
void affiche_tables(FILE *dest);
void affiche_table_rep();
void print_color(const char *txt, const char *color_code);
void print_error(const char *txt, ...);

#endif