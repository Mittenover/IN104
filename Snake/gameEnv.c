#include "gameEnv.h" 
#include "Qlearning.h"


start_col=5;
start_row=8;




void alloc_grid(){  //On alloue dynamiquement un tableau carré de caractère de la taille de la dimension
     grid = malloc(dim * sizeof(char*));

     for(int i=0; i<dim; i++) {
         grid[i] = malloc(dim * sizeof(char*));
     }
}

void grid_make(int size){
	for(int i=0;i<size;++i){
		grid[i][0]='+';
		grid[0][i]='+';
		grid[size-1][i]='+';
		grid[0][size-1]='+';
	}
}

void grid_render(){
     for (int i=0; i<dim; i++) {
         for (int j=0; j< dim; j++){
             printf("%c ", grid[i][j]);
         }
         printf("\n");
     }
     printf("\n");

}


void new_fruit(){
	int i=rand()%dim;
	int j=rand()%dim;
	int k=0;
	while((grid[i][j]==wall)||(grid[i][j]==snake)||(k<100*dim*dim)){  //on tire des positions aléatoire dans la grille, si cette position correspond à un mur on retire jusqu'à avoir une position vide. Le compteur k permet de mettre une limite sur les tirages au cas où à la fun du jeu plus aucune position ne peut accueillir de fruit
		i=rand()%dim;
		j=rand()%dim;
		k+=1;
	}
	grid[i][j]='f';

}

void init_snake(){  //Création du serpent initial qui occupe 3 cases
	struct bout_queue *b0;
	b0->queue_col=start_col;
	b0->queue_row=start_row;
	queue->elem=b0;

	struct queue *next_1;
	struct bout_queue *b1;
	b1->queue_col=start_col-1;
	b1->queue_row=start_row-1;
	next_1->elem=b1;

	struct queue *next_2;
	struct bout_queue *b2;
	b2->queue_col=start_col-2;
	b2->queue_row=start_row-2;
	next_2->elem=b2;

	next_1->next = next_2;
	queue->next = next_1;
	for(int i=0;i<3;++i){
		grid[start_row][start_col-i]='.';
	}
}