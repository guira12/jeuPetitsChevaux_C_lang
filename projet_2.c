#include "pipesAndProcessusCreation.h"
#include "plateau.h"
#include "unTourJeu.h"

int main(int argc, char **argv){
	srand(time(NULL)); // graine == time(NULL) pour avoir à chaque execution une sequance 
			    //differente de lancé de dé
	pid_t PID;
	int N; // nombre de joueurs passé en argument par l'utilisateur
	int nbChevaux; // nombre de chevaux pour chaque joueur passé en argument par l'utilisateur
	int indiceF; // pour recuperer l'indice du fils lors de sa creation
	
	
	// filtre commande
	if(argc!=3){
		fprintf(stderr, "error syntax : cmd N nbChevaux\n");
		exit(-3);
	}
	N=atoi(argv[1]);   // nombre joueurs
	nbChevaux=atoi(argv[2]);   // nombre chevaux pour chaque joueur
	if(N<2  || N > 4){
		fprintf(stderr, "error: N must be in the interval [2,4].\n");
		exit(-4);
	}
	if( nbChevaux < 2  || nbChevaux > 4 ){
		fprintf(stderr, "error: nbChevaux must be in the interval [2,4].\n");
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
		
		
		
		
		
		////////***********F
		plateau p;
		initPlateau(N, nbChevaux, &p); // initialisation du plateau selon le nombre de
						// joueurs et de chevaux pour chaque
		int tour; // buffer indice joueur qui commence la partie
		
		int indBlock; // indice du fils qui bloque le tour de l'info dans l'anneau
		int rejouer; // si vrai le meme joueur doit rejouer ce boolean tourne dans l'anneau,
				// et il est envoyer au père qui fait rejouer le meme joueur
		
		///////// boucle de communication
		do{
			// reception resultat lancé de dé à N faces
			if( read(pf[indiceF][0], &tour, sizeof(int)) == -1 ){
				perror("read");
				exit(-5);
			}
			
			if( tour == indiceF){	// si c'est mon tour de jouer	
				//lancer dé à six face
				int de=lancerDe(1, 6);
				if(de==6){ // rejouer est vrai si de=6
					rejouer=1;
				}
				else{
					rejouer=0;
				}
				// affichage resultat lancé à l'écran 
				printf("LANCE DU DE A 6 FACES PAR LE JOUEUR%d--> ", indiceF);
				fflush(stdout);
				sleep(1);
				printf("%d\n", de);
				
				jouerTour(de, &p);
				indBlock = indiceFPrecedent; // indice du dernier fils qui recevera l'info
				write(fd[indiceF][1], &indBlock, sizeof(int)); // envoyer l'indice du dernier fils qui reçoi l'info au joueur suivant
				write(fd[indiceF][1], &rejouer, sizeof(int)); // envoyer bool indiquant que je doit rejouer
				write(fd[indiceF][1], &p, sizeof(plateau)); // envoyer plateau à jour au joueur suivant
				
			
			}
			else{  // si ce n'est pas mon tour de jouer
				plateau paj; // plateau à jour, qui circule dans l'anneau
				read(fd[indiceFPrecedent][0], &indBlock, sizeof(int)); // reçoi ou le tour d'info s'arrete
				read(fd[indiceFPrecedent][0], &rejouer, sizeof(int)); // recevoir boolean indiquant si le fils qui a joué le tour doit rejouer 
				read(fd[indiceFPrecedent][0], &paj, sizeof(plateau)); // reçoi plateau à jour
				mettreAJourPlateau(&p, paj); // mise à jour plateau fils courant
				if(indBlock==indiceF){ // si je suis le dernier fils qui doit recevoir l'info
					write(fp[1], &rejouer, sizeof(int)); // envoyer bool indiquant que je doit rejouer
					write(fp[1], &paj, sizeof(plateau)); // j'envoie le plateau au père
				}
				else{ // si je ne suis pas le dernier fils qui doit recevoir l'info de l'anneau
					write(fd[indiceF][1], &indBlock, sizeof(int)); // envoi dernier joueur qui doit recevoir l'info
					write(fd[indiceF][1], &rejouer, sizeof(int)); // envoyer bool indiquant que je doit rejouer
					write(fd[indiceF][1], &paj, sizeof(plateau)); // envoi plateau à jour au joueur suivant
				}
				
				
			}
		}
		while(!qqAGagne(p));

		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		fermetureDescripteursFils(indiceF, indiceFPrecedent, fp, pf, fd);
		return 0;
	}
	////////////////////////////////////////// comportement père //////////////////////////////
	else{
		//printf("Je suis le pere mon PID est %d et j'ai crée %d fils.\n", getpid(), N);
		// configuration des pipes coté père
		configPipesPere(N, pf, fp, fd);
		
		
		
		
		
		
		
				
		printf("        ----------------------------\n");
		printf("________DEBUT PARTIE PETITS CHEVEAUX_________\n");
		printf("        ----------------------------\n");
		
		int rejouer=0;
		plateau p;
		initPlateau(N, nbChevaux, &p); // initialisation du plateau selon le nombre de
						// joueurs et de chevaux pour chaque  
						
		// affichage plateau au debut du jeu     
		afficherPlateau1D(&p);
		//lancer le dé et determiner qui commence la partie
		int quiJoue=lancerDe(0, N-1);
		printf("qui commence la partie?\n");
		printf("LANCE DU DE A %d FACES --> ", N);
		fflush(stdout);
		sleep(1);
		printf("%d\n", quiJoue);
		
		
		// Boucle de communication  le pere gere le jeu
		do{
			if(!rejouer){
				printf("le tour est au JOUEUR%d :\n", quiJoue);
			}
			// broadcast l'indice du joueur qui doit jouer (destinataiares=tout les joueurs)
			broadcastInt(pf, N, &quiJoue);
			// le pere écoute dans le pipe si le joueur a recu un 6 et doit rejouer
			read(fp[0], &rejouer, sizeof(int)); // reception bool indiquant si rejouer ou pas
			// écoute dans le tube fils vers pere pour recevoir le plateau dans son nouveau état
			read(fp[0], &p, sizeof(plateau));
			// affichage du plateau
			afficherPlateau1D(&p);
			// calcul du joueur suivant
			if(!rejouer){
				quiJoue=indiceSuivantDansAnneau(quiJoue, N);
			}
			else if(!qqAGagne(p) && rejouer){
				printf("JOUEUR%d vous devez rejouer encore une fois vous avez un de=6\n", quiJoue);
			}
		}
		while(!qqAGagne(p)); // faire tant que personne n'a gagné
		
		//affichage gagnant
		
		

		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		fermetureDescripteursPere(N, pf, fp); // fermeture descripteurs ouverts
		attendreMortFils(N); // père attend la mort de ces N fils, puis meurt
		return 0;
	}

	
}

