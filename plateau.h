#ifndef _plateau_H_
#define _plateau_H_
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
//definition du type plateau
struct plateau{
	int ec[4];   	// chaque case du tableau represente l'ecurie d'un joueur selon indice
			// valeurs possibles un entier dans [0,nbChevaux]
			// ex: ec[1]=2   <=> le joueur1 a deux cheveau dans l'ecurie
			
	int co[58];	// represente les cases apres l'ecurie ou les joueurs peuvent rentrer en 
			// collision, numeroté de 0 à 57 dans le sens horaire.
			// ex: la case co[0] represente la case de depart du joueur en haut à 
			// droite càd joueur0, la case co[14] represente la case de départ du 
			// joueur en bas à droite c'est à dire joueur1.
			// valeurs possibles [-1,N-1] N étant le nombre de joueurs
			// -1 = case vide ; 0 = joueur0 sur la case ...
			
	int es[4][7]; 	// c'est un tableau regroupant les 4 escaliers du plateau
			// ex: es[x][0] escalier du joueurX premiere; case es[X][7]==1 veut dire 
			// joueurX a gagné.
			// valeur possibles : [0,nbChevaux] represente le nombre de cheveaux sur la case
};
typedef struct plateau plateau;



// procedure qui initialise le plateau <*p> selon le nombre de joueurs <N>,
// et le nombre de chevaux pour chaque joueur <nbChevaux>.
void initPlateau(int N, int nbChevaux, plateau *p);

// affiche le plateau sous sa forme 1D
void afficherPlateau1D(plateau *p);

// fct qui simule un lancé de dé en sortie de cette fct on a un nombre entier aleatoire
// dans l'intervalle [a, b]
//pas de contraintes sur l'ordre de a et b
int lancerDe(int a, int b);

// met à jour le plateau *p avec les valeur du plateau paj
void mettreAJourPlateau(plateau *p, plateau paj);

//fct renvoie 1 si quelqu'un a gagné sinon renvoie 0
int qqAGagne(plateau p);

#endif /*_plateau_H_*/
