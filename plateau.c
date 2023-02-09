#include "plateau.h"
#include "pipesAndProcessusCreation.h"

// procedure qui initialise le plateau <*p> selon le nombre de joueurs <N>,
// et le nombre de chevaux pour chaque joueur <nbChevaux>.
void initPlateau(int N, int nbChevaux, plateau *p){
	// initialisation ecurie
	for(int i=0; i<N; i++){
		p->ec[i] = nbChevaux; // au depart tout les chevaux dans l'ecurie
	}
	for(int i=N; i<4; i++){
		p->ec[i] = 0;
	}
	// initialisation escalier
	for(int i=0; i<4; i++){
		for(int j=0; j<7; j++){
			p->es[i][j] = 0;
		}
	}
	// initialisation co
	for(int i=0; i<58; i++){
		p->co[i] = -1;
	}

}


// affiche le plateau sous sa forme 1D
void afficherPlateau1D(plateau *p){
	// affichage ecurie
	printf("--------------------------------------------------------\n");
	printf("|            PLATEAU  1D                               |\n");
	printf("--------------------------------------------------------\n");
	printf("Ecurie: ");
	for(int i=0; i<4; i++){
		printf("%d ", p->ec[i]);
	}
	printf("\n-------\n");
	
	// affichage co
	printf("Espase collision: ");
	for(int i=0; i<58; i++){
		printf("%d ", p->co[i]);
	}
	printf("\n----------------\n");
	
	// affichage escaliers
	
	for(int i=0; i<4; i++){
		printf("Escalier joueur%d: ", i);
		for(int j=0; j<7; j++){
			printf("%d ", p->es[i][j]);
		}
		printf("\n---------------\n");
	}
	printf("--------------------------------------------------------\n");

}

// fct qui simule un lancé de dé en sortie de cette fct on a un nombre entier aleatoire
// dans l'intervalle [a, b]
//pas de contraintes sur l'ordre de a et b
int lancerDe(int a, int b){
	if(a>b){
		int tmp=a;
		a=b;
		b=tmp;
	}
	return a + (rand()%(b-a+1));
}

// met à jour le plateau *p avec les valeur du plateau paj
void mettreAJourPlateau(plateau *p, plateau paj){
	for(int i=0; i<4; i++){
		p->ec[i] = paj.ec[i]; // mise à jour écurie
		for(int j=0; j<7; j++){
			p->es[i][j] = paj.es[i][j]; // mise à jour escaliers
		}
	}
	for(int i=0; i<58; i++){
		p->co[i] = paj.co[i]; // mise à jour espace collision
	}
}

//fct renvoie 1 si quelqu'un a gagné sinon renvoie 0
int qqAGagne(plateau p){
	int i=0;
	while(i<4 && p.es[i][6]==0){
		i++;
	}
	if(i<4){
		return 1;
	}
	else{
		return 0;
	}
}

