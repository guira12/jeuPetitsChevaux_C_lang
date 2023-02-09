#include "pipesAndProcessusCreation.h"


// procedure prend en argument un tableau de pipes et le nombre de pipes dans ce tableau
//et initialise avec la fct pipe() tout le tableau
void initialisationTabDePipes(int (*tabPipes)[2], int nb_pipes){
	for(int i=0; i<nb_pipes; i++){
		pipe(tabPipes[i]);
	}
}

// fp pf fd : pipe ou tableau de pipes à configurer pour le pere
// N : est le nombre de joueurs choisi par l'utilisateur == nombre processus fils
void configPipesPere(int N, int (*pf)[2], int *fp, int (*fd)[2]){
	close(fp[1]);
	for(int i=0; i<N; i++){
		close(pf[i][0]);
		close(fd[i][0]);
		close(fd[i][1]);
	}
}
// procedure fermeture des descripteur avant la mort du pere
// N : est le nombre de joueurs choisi par l'utilisateur == nombre processus fils
void fermetureDescripteursPere(int N, int (*pf)[2], int *fp){
	close(fp[0]);
	for(int i=0; i<N; i++){
		close(pf[i][1]);
	}
}

// configuration pipes coté Fils 
// N : est le nombre de joueurs choisi par l'utilisateur == nombre processus fils
// indiceF : indice du processus qui execute le code
// indiceFPrecedent : indice du fils qui est positionné avant dans l'anneau
// fp pf fd : pipe ou tableau de pipes à configurer selon chaque processus fils
void configPipesFils(int N, int indiceF,int indiceFPrecedent, int *fp, int (*pf)[2], int (*fd)[2]){
	close(fp[0]);
	for(int i=0; i<N; i++){
		if(i==indiceF){
			close(pf[i][1]);
		}
		else{
			close(pf[i][0]);
			close(pf[i][1]);
		}
		if(i!=indiceF && i!=indiceFPrecedent){
			close(fd[i][0]);
			close(fd[i][1]);
		}
	}
	close(fd[indiceF][0]);
	close(fd[indiceFPrecedent][1]);
}

// Procedure fermeture des descripteurs avant la mort de chaque fils
void fermetureDescripteursFils(int indiceF,int indiceFPrecedent, int *fp, int (*pf)[2], int (*fd)[2]){
	close(fp[1]);
	close(pf[indiceF][0]);
	close(fd[indiceF][1]);
	close(fd[indiceFPrecedent][0]);
}

// dans un anneaux de processus relié par des pipes (où chaque fils a un indice) 
// nbFilsDansAnneau : nombre de fils constiuant l'anneau
// indiceCourant : indice du processus qui execute le code
// valeur de retour : fct qui pour un fils executant le code retourne l'indice du fils qui est positionné avant dans l'anneau
int indicePrecedentDansAnneau(int indiceCourant, int nbFilsDansAnneau){
	if(indiceCourant==0){
		return nbFilsDansAnneau - 1 ;
	}
	else{
		return indiceCourant -1 ;
	}
}

// pour un indice il calcul l'indice suivant dans l'anneau
int indiceSuivantDansAnneau(int indice, int nbFilsDansAnneau){
	if(indice==nbFilsDansAnneau-1){
		return 0;
	}
	else{
		return indice + 1;
	}
}
// attendre la mort de nb fils et liberation ressource de ces fils morts
void attendreMortFils(int nb){
	for(int i=0; i<nb; i++){
			wait(NULL);
	}
}
// contrainte d'utilisation : un pere relié à ses fils via un pipe chacun 
//arguments: 	-pf: tableau de tout les pipes reliant le pere et les fils (pere en ecriture fils en lecture)
//		-nb: nombre de pipes dans tableau pf
//		-message: adresse de l'entier à envoyer
// si echec affichage message d'erreur et sortie du programme avec status -1
void broadcastInt(int (*pf)[2], int nb, int *message){
	for(int i=0; i<nb; i++){
		if( write(pf[i][1], message, sizeof(int)) == -1){
			perror("error in write() in fct broadcastInt() :");
			exit(-1);
		}
	}
}

//arguments: 	-nb: nombre fils à creer
//		-tab_fils: tableau de pid qui sera initialisé avec les pid des fils crée
//                         contrainte tableau de taille nb
//		-indiceF: pointeur vers entier pour recuperer l'indice du processus courant cree le fork.
// cette fct crée nb fils à partir du processus père et les fait attendre si pas d'erreur les libere ,
// initialise le tableau avec les pid des fils crées, et traite le cas d'erreur ou nb de fils crée pas atteint,
// return value : PID du processus courant
int creationFils(int nb, pid_t *tab_fils, int *indiceF){
	int nbFilsCree=0; // compteur nombre fils crée
	pid_t PID;
	//////////////////////// boucle de création de nb fils 
	while(nbFilsCree!=nb && (PID=fork())!=-1 && PID!=0){	//ordre des booleans très important
		kill(PID, SIGSTOP); //chaque fils cree n'execute rien
		nbFilsCree++;  // creation fils avec succes donc incrementation compteur fils crée
		tab_fils[nbFilsCree-1]=PID; //enregistrer les pid des fils cree
	}
	
	//////////////////////// cas erreur lors de creation de N fils
	if(PID==-1){ // cas erreur père fait :
		perror("fork");
		for(int i=0; i<nbFilsCree; i++){ // signaux provoquant la mort des fils crées
			kill(tab_fils[i], SIGKILL);
			attendreMortFils(1); //wait une fois liberation ressources fils cree
		}
		exit(-1);
	}
	else if(PID!=0){ /// si succes creation N processus fils, père fait :
		for(int i=0; i<nbFilsCree; i++){ // on laisse les fils continuer leurs executions
			kill(tab_fils[i], SIGCONT);
		}
	}
	*indiceF=nbFilsCree; // recuperer l'indice du fils
	return PID; // recuperer le pid du processus courant
}


