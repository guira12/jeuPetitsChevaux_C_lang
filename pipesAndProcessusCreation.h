#ifndef _pipesAndProcessusCreation_H_
#define _pipesAndProcessusCreation_H_

#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


// procedure prend en argument un tableau de pipes et le nombre de pipes dans ce tableau
//et initialise avec la fct pipe() tout le tableau
void initialisationTabDePipes(int (*tabPipes)[2], int nb_pipes);

// fp pf fd : pipe ou tableau de pipes à configurer pour le pere
// N : est le nombre de joueurs choisi par l'utilisateur == nombre processus fils
void configPipesPere(int N, int (*pf)[2], int *fp, int (*fd)[2]);

// procedure fermeture des descripteur avant la mort du pere
// N : est le nombre de joueurs choisi par l'utilisateur == nombre processus fils
void fermetureDescripteursPere(int N, int (*pf)[2], int *fp);

// configuration pipes coté Fils 
// N : est le nombre de joueurs choisi par l'utilisateur == nombre processus fils
// indiceF : indice du processus qui execute le code
// indiceFPrecedent : indice du fils qui est positionné avant dans l'anneau
// fp pf fd : pipe ou tableau de pipes à configurer selon chaque processus fils
void configPipesFils(int N, int indiceF,int indiceFPrecedent, int *fp, int (*pf)[2], int (*fd)[2]);

// Procedure fermeture des descripteurs avant la mort de chaque fils
void fermetureDescripteursFils(int indiceF,int indiceFPrecedent, int *fp, int (*pf)[2], int (*fd)[2]);

// dans un anneaux de processus relié par des pipes (où chaque fils a un indice) 
// nbFilsDansAnneau : nombre de fils constiuant l'anneau
// indiceCourant : indice du processus qui execute le code
// valeur de retour : fct qui pour un fils executant le code retourne l'indice du fils qui est positionné avant dans l'anneau
int indicePrecedentDansAnneau(int indiceCourant, int nbFilsDansAnneau);

// pour un indice il calcul l'indice suivant dans l'anneau
int indiceSuivantDansAnneau(int indice, int nbFilsDansAnneau);

// attendre la mort de nb fils et liberation ressource de ces fils morts
void attendreMortFils(int nb);

// contrainte d'utilisation : un pere relié à ses fils via un pipe chacun 
//arguments: 	-pf: tableau de tout les pipes reliant le pere et les fils (pere en ecriture fils en lecture)
//		-nb: nombre de pipes dans tableau pf
//		-message: adresse de l'entier à envoyer
// si echec affichage message d'erreur et sortie du programme avec status -1
void broadcastInt(int (*pf)[2], int nb, int *message);

//arguments: 	-nb: nombre fils à creer
//		-tab_fils: tableau de pid qui sera initialisé avec les pid des fils crée
//                         contrainte tableau de taille nb
//		-indiceF: pointeur vers entier pour recuperer l'indice du processus courant cree le fork.
// cette fct crée nb fils à partir du processus père et les fait attendre si pas d'erreur les libere ,
// initialise le tableau avec les pid des fils crées, et traite le cas d'erreur ou nb de fils crée pas atteint,
// return value : PID du processus courant
int creationFils(int nb, pid_t *tab_fils, int *indiceF);






#endif /*_pipesAndProcessusCreation_H_*/
