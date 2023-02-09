#include "plateau.h"


// fct jouer un tour
void jouerTour(int de, plateau *p){
	int val=p->es[0][0];
	for(int i=0; i<4; i++){
		(p->es[i][0])++;
		(p->es[i][val])++;
	}
	
}

