#include "pipesAndProcessusCreation.h"




int main(int argc, char **argv){
	pid_t PID;
	int N; // nombre de joueurs
	int indiceF; // pour recuperer l'indice du fils lors de sa creation
	
	
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
	
	
	
	// declaration et initialisation de tous les pipes
	int fp[2];  // un pipe liant tout les fils au père(fils ecrivent le pere lit)
	pipe(fp);
	int pf[N][2]; // tableau de pipes liants le pere en ecriture et les fils en lecture
	int fd[N][2]; // tableau de pipes constituants l'anneau entre les fils
	initialisationTabDePipes(pf, N);
	initialisationTabDePipes(fd, N);
	
	
	
	
	///////////////////////////// boucle de création de N fils, et traitement cas erreur lors de creation de N fils ///////////////////////////
	pid_t tab_pid_fils[N];	//tableau pid fils
	PID=creationFils(N, tab_pid_fils, &indiceF); //chaque fils portera son indice dans indiceF
	
	/////////////////////////////// comportement fils ////////////////////////////////////////
	if(PID==0){
		
		//determiner indice du fils precedant dans l'anneau
		int indiceFPrecedent=indicePrecedentDansAnneau(indiceF, N);
		//printf("Je suis le joueur n %d mon PID est %d celui de mon père est %d.\n", indiceF, getpid(), getppid());
		//configuration des pipes coté fils
		configPipesFils(N, indiceF,indiceFPrecedent, fp, pf, fd);
		
		
		
		
		
		
		//test des pipes pf
		int message0;
		if(read(pf[indiceF][0], &message0, sizeof(int)) == -1){
			perror("read");
			exit(-2);
		}
		printf("Je suis le joueur n %d j'ai recu du père l'entier: %d\n", indiceF, message0);
		sleep(1);
		//test de l'anneau entre les joueurs pipes fd
		char message1;
		if(indiceF==0){
			printf("TEST de l'anneau de pipes entre les fils\n");
			printf("----------------------------------------\n");
			message1='A';
			write(fd[indiceF][1], &message1, 1);
			printf("je suis le joueur0 j'ai envoye au joueur1 le caractere %c.\n", message1);
			read(fd[indiceFPrecedent][0], &message1, 1);
			printf("je suis le joueur0 j'ai recu du joueur%d le caractere %c.\n", indiceFPrecedent, message1);
		}
		else{
			read(fd[indiceFPrecedent][0], &message1, 1);
			printf("je suis le joueur%d j'ai recu du joueur%d le caractere %c.\n", indiceF, indiceFPrecedent, message1);
			message1++;
			write(fd[indiceF][1], &message1, 1);
		}
		
		sleep(1);
		//test du pipe fp
		if(indiceF==0){
			printf("TEST du pipe fp ou les fils ecrivent et le pere lit\n");
			printf("---------------------------------------------------\n");
		}else{
			sleep(1);
		}
		int message2 = indiceF;
		if(write(fp[1], &message2, sizeof(int)) == -1){
			perror("error in fct write in pipe testing");
			exit(-1);
		}
		printf("je suis le joueur%d j'ai envoye mon indice %d au pere\n", indiceF, message2);
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		fermetureDescripteursFils(indiceF, indiceFPrecedent, fp, pf, fd);
		return 0;
	}
	////////////////////////////////////////// comportement père //////////////////////////////
	else{
		
		//printf("Je suis le pere mon PID est %d et j'ai crée %d fils.\n", getpid(), N);
		// configuration des pipes coté père
		configPipesPere(N, pf, fp, fd);
		
		
		
		
		
		
		
		//test des pipes père vers fils
		int message=13579;
		printf("TEST l'ensemble de pipes reliants le pere a chaque fils\n");
		printf("-------------------------------------------------------\n");
		broadcastInt(pf, N, &message); //envoie message à tout les fils instantanement
		printf("Je suis le pere j'ai envoye l'entier %d à tout les joueurs\n", message);
		//test du pipe fp
		printf("je suis le pere j'ai recu les indice des fils suivant:");
		for(int i=0; i<N; i++){
			read(fp[0], &message, sizeof(int));
			printf(" %d", message);
		}
		printf("\n");
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		fermetureDescripteursPere(N, pf, fp); // fermeture descripteurs ouverts
		attendreMortFils(N); // père attend la mort de ces N fils, puis meurt
		return 0;
	}

	
}

