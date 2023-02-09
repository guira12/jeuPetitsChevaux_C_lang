#include "pipesAndProcessusCreation.h"


int main(int argc, char **argv){
	int nbFilsCree=0; // compteur nombre fils crée
	pid_t PID;
	int indiceF; // pour recuperer l'indice du fils lors de sa creation
	int N; // nombre de joueurs
	
	// filtre commande
	if(argc!=2){
		fprintf(stderr,"error syntax : cmd N\nN is an integer in the interval [2,4].\n");
		exit(-3);
	}
	N=atoi(argv[1]);
	if(N<2  || N > 4){
		fprintf(stderr, "error: N must be in the interval [2,4].\n");
		exit(-4);
	}
	
	///////////////////////////// boucle de création de N fils, et traitement cas erreur lors de creation de N fils ///////////////////////////
	pid_t tab_pid_fils[N];	//tableau pid fils
	PID=creationFils(N, tab_pid_fils, &indiceF); //chaque fils portera son indice dans indiceF
	
	
	
	
	/////////////////////////////// comportement fils ////////////////////////////////////////
	if(PID==0){
		printf("Je suis le joueur n %d mon PID est %d celui de mon père est %d.\n", nbFilsCree, getpid(), getppid());	
		return 0;
	}
	////////////////////////////////////////// comportement père //////////////////////////////
	else{
		
		printf("Je suis le pere mon PID est %d et j'ai crée %d fils.\n", getpid(), nbFilsCree);
		
		attendreMortFils(N); // père attend la mort de ces N fils, puis meurt
		return 0;
	}

	
}

