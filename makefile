#############################################################################
# Fichier Makefile 
# UE SYS - IM2AG - UGA - 2022/2023
#############################################################################
# utilisation des variables internes $@ $< $^ $*
# $@ : correspond au nom de la cible
# $< : correspond au nom de la premiere dependance
# $^ : correspond à toutes les dépendances
# $* : correspond au nom du fichier sans extension 
#       (dans les regles generiques uniquement)
#############################################################################
# information sur la regle executee avec la commande @echo
# (une commande commancant par @ n'est pas affichee a l'ecran)
#############################################################################


#############################################################################
# definition des variables locales
#############################################################################

# compilateur C
CC = gcc

# options de compilation
COMPILOPTS = -Wall

# liste des executables
EXECUTABLES = projet_0 projet_1 projet_2


#############################################################################
# definition des regles
#############################################################################

########################################################
# la règle par défaut
all : $(EXECUTABLES)

########################################################
# regle generique : 
#  remplace les regles de compilation separee de la forme
#	module.o : module.c module.h
#		$(CC) -c $(COMPILOPTS) module.c
%.o : %.c %.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module "$*
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

########################################################
# regles explicites de compilation separee de modules
# n'ayant pas de fichier .h ET/OU dependant d'autres modules
pipesAndProcessusCreation.o : pipesAndProcessusCreation.c pipesAndProcessusCreation.h

plateau.o : plateau.c plateau.h pipesAndProcessusCreation.h

unTourJeu.o : unTourJeu.c unTourJeu.h plateau.h

projet_0.o : projet_0.c pipesAndProcessusCreation.h

projet_1.o : projet_1.c pipesAndProcessusCreation.h

projet_2.o : projet_2.c pipesAndProcessusCreation.h unTourJeu.h plateau.h

########################################################
# regles explicites de creation des executables

projet_0 :  projet_0.o pipesAndProcessusCreation.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@
	@echo "---------------------------------------------"
	$(CC) $^ -o $@

projet_1 : projet_1.o pipesAndProcessusCreation.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@
	@echo "---------------------------------------------"
	$(CC) $^ -o $@

projet_2 : projet_2.o pipesAndProcessusCreation.o plateau.o unTourJeu.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@
	@echo "---------------------------------------------"
	$(CC) $^ -o $@




# regle pour "nettoyer" le répertoire
clean:
	rm -fR $(EXECUTABLES) *.o 
