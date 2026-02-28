PROG_LEX = ./src/analyseur_lex.l
PROG_YACC = ./src/analyseur_synt.y
CC = gcc
LEX = flex
YACC = bison -d
CFLAGS = #-Wall -g

TARGET = prog mv

all: $(TARGET)

prog : y.tab.o lex.yy.o table_lexico.o table_declarations.o table_de_repr.o table_region.o erreurs.o affichage_tables.o verif_semantique.o arbre.o sauvegarde.o main.o
	$(CC) $(CFLAGS) -o prog ./obj/y.tab.o ./obj/lex.yy.o ./obj/arbre.o ./obj/table_lexico.o ./obj/table_declarations.o ./obj/table_de_repr.o ./obj/table_region.o ./obj/erreurs.o ./obj/affichage_tables.o ./obj/verif_semantique.o ./obj/sauvegarde.o ./obj/main.o

mv : table_lexico.o table_declarations.o table_de_repr.o table_region.o arbre.o affichage_tables.o sauvegarde.o machine_virtuelle.o main_mv.o
	$(CC) $(CFLAGS) -o mv ./obj/arbre.o ./obj/table_lexico.o ./obj/table_declarations.o ./obj/table_de_repr.o ./obj/table_region.o ./obj/affichage_tables.o ./obj/sauvegarde.o ./obj_mv/machine_virtuelle.o ./obj_mv/main.o

# Générer l'analyseur syntaxique avec yacc ou bison
y.tab.c y.tab.h: $(PROG_YACC)
	$(YACC) -o ./obj/y.tab.c $(PROG_YACC)

# Générer l'analyseur lexical avec lex ou flex
lex.yy.c: $(PROG_LEX)
	$(LEX) -o ./obj/lex.yy.c $(PROG_LEX)

# Lex & Yacc
y.tab.o: y.tab.c
	$(CC) $(CFLAGS) -c ./obj/y.tab.c -o ./obj/y.tab.o

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c ./obj/lex.yy.c  -o ./obj/lex.yy.o

# Table lexico
table_lexico.o: ./src/table_lexico.c
	$(CC) $(CFLAGS) -c ./src/table_lexico.c -o ./obj/table_lexico.o

# Arbre d'instruction
arbre.o: ./src/arbre.c ./src/arbre.h
	$(CC) $(CFLAGS) -c ./src/arbre.c -o ./obj/arbre.o

# Table declarations
table_declarations.o: ./src/table_declarations.c
	$(CC) $(CFLAGS) -c ./src/table_declarations.c -o ./obj/table_declarations.o

# Table de representation
table_de_repr.o: ./src/table_de_repr.c
	$(CC) $(CFLAGS) -c ./src/table_de_repr.c -o ./obj/table_de_repr.o

# Table de region
table_region.o: ./src/table_region.c
	$(CC) $(CFLAGS) -c ./src/table_region.c -o ./obj/table_region.o

# Erreurs
erreurs.o: ./src/erreurs.c
	$(CC) $(CFLAGS) -c ./src/erreurs.c -o ./obj/erreurs.o

# Vérifications sémantiques
verif_semantique.o: ./src/verif_semantique.c
	$(CC) $(CFLAGS) -c ./src/verif_semantique.c -o ./obj/verif_semantique.o

# Sauvegarde
sauvegarde.o: ./src/sauvegarde.c
	$(CC) $(CFLAGS) -c ./src/sauvegarde.c -o ./obj/sauvegarde.o

# Affichage tables
affichage_tables.o: ./src/affichage/affichage_tables.c
	$(CC) $(CFLAGS) -c ./src/affichage/affichage_tables.c -o ./obj/affichage_tables.o

# Main
main.o: ./src/main.c
	$(CC) $(CFLAGS) -c ./src/main.c -o ./obj/main.o

# Deuxieme programme : machine virtuelle

# Machine virtuelle
machine_virtuelle.o: ./src_mv/machine_virtuelle.c
	$(CC) $(CFLAGS) -c ./src_mv/machine_virtuelle.c -o ./obj_mv/machine_virtuelle.o

# Main
main_mv.o: ./src_mv/main.c
	$(CC) $(CFLAGS) -c ./src_mv/main.c -o ./obj_mv/main.o

clean:
	rm -rf ./obj/* ./obj_mv/* $(TARGET)