#include "mazeEnv.h" 
#include "functions.h"
#include "Qlearning.h"

void alloc_maze(){
     maze = malloc(rows * sizeof(char*));

     for(int i=0; i<rows; i++) {
         maze[i] = malloc(cols * sizeof(char*));
     }
}

void maze_make(char* file_name){
     char c;
     char rows_s[3] ={'\0'};
     char cols_s[3] ={'\0'};
     int rows_i = 0;
     int cols_i = 0;
     int swap = 0;

     FILE* file = fopen(file_name, "r");

     if (file) {
         /* lire la premiere ligne avant EOF */
         while( (c=getc(file)) != EOF) {
               if(c== '\n'){
                      break;
               } else if (c==',') {
                      swap = 1;
               } else if (!swap) {
                      rows_s[rows_i]=c;
                      rows_i++;
               } else {
                      cols_s[cols_i]= c;
                      cols_i++;
               }
         }
     }

     /* convertir le string en nombre */
     rows = atoi(rows_s);
     cols = atoi(cols_s);

     alloc_maze();

     for (int i=0; i<rows; i++){
         for (int j=0; j < cols; j++){
             c = getc(file);

             if (c=='\n'){
                 c = getc(file);
             } else if (c == 's'){
               start_row = i;
               start_col = j;
             } else if ( c == 'g'){
               goal_row = i;
               goal_col = j;
             }

             maze[i][j] = c;
         }
     }

     fclose(file);
}


void maze_render(){
     for (int i=0; i<rows; i++) {
         for (int j=0; j< cols; j++){
             printf("%c ", maze[i][j]);
         }
         printf("\n");
     }
     printf("\n");
}


void delete_crumb(){
    int i, j;
        for (i = 0; i < rows; ++i) {
                for (j = 0; j < cols; ++j) {
                        if (visited[i][j] == crumb) {
                                visited[i][j] = unknown;
                        }
                }
        }
}


void maze_reset(){
     state_row = start_row;
     state_col = start_col;
     delete_crumb();
     delete_point();
}

void delete_point(){
    int i, j;
        for (i = 0; i < rows; ++i) {
                for (j = 0; j < cols; ++j) {
                        if (maze[i][j] == '.') {
                                maze[i][j] = ' ';
                        }
                }
        }
}

envOutput maze_step(action a){
    // int reward = 0;
    // int done = 0;
    envOutput stepOut;

    // On introduit des variables locales, qui permettront de mieux réagir face aux murs
    int nouv_row = state_row;
    int nouv_col = state_col;

    // Pour chaque situation, on doit prendre en compte les bordes et les murs, et appliquer des récompenses négatives

    switch (a){
        case up:
            nouv_row = max(0,state_row -1);
        break;

        case down:
            nouv_row = min(rows,state_row +1);
        break;

        case right:
            nouv_col = min(cols,state_col +1);
        break;

        case left:
            nouv_col = max(0,state_col -1);
        break;

        default:
        break;
    }

    // On vérifie chaque terrain d'état futur, et on ajuste les valeur de stepOut

    switch (visited[nouv_row][nouv_col]){
        case unknown:
            // Si la case est inconnue, elle devient crumb (rencontrée), et la récompense est positive. Le sujet bouge
            stepOut.reward = 1;
            stepOut.new_row = nouv_row;
            stepOut.new_col = nouv_col;
            visited[nouv_row][nouv_col] = crumb;
        break;
        case wall:
            // Si c'est un mur, on avance pas et la recompense est négative
            stepOut.reward = -10;
            stepOut.new_row = state_row;
            stepOut.new_col = state_col;
        break;
        case crumb:
            // Si déjà rencontrée, on avance mais on applique une récompense négative
            stepOut.reward = -0.1;
            stepOut.new_row = nouv_row;
            stepOut.new_col = nouv_col;
        break;
        case goal:
            // Si c'est l'objectif on se place dessus et on done une bonne récompense
            stepOut.reward = 10;
            stepOut.new_row = nouv_row;
            stepOut.new_col = nouv_col;
        break;
        default:
        break;
    }

   return stepOut;
}

action env_action_sample(){
  return (enum action)(rand() % number_actions);
}

action env_action_sample2(){
    int s = state_row*cols + state_col;
    double max = table_reward[s][0];
    action result = up;

    if (table_reward[s][1] > max)
    {
        max = table_reward[s][1];
        result = down;
    }
    if (table_reward[s][2] > max)
    {
        max = table_reward[s][2];
        result = right;
    }
    if (table_reward[s][3] > max)
    {
        max = table_reward[s][3];
        result = left;
    }
    return  result;
}

action env_action_greedy(){
    // Pour l'instant je ne sais pas chosir un nombre aléatoire facilement, on va y aller bourrin
    double n = (double)(rand() % 100000)/100000;
    // printf("%f\n", n);

    epsilon = 1;
    if (n < epsilon)
    {
        return (enum action)(rand() % number_actions); // Aléatoirement
    }
    else {
        return env_action_sample2();
    }
}

action env_action_boltzmann(int s)
{
    // POUR L'INSTANT BOUCLE INFINIE

    // On fait la somme des exponentielles
    double sum = 0;
    for (int i = 0; i < 4; ++i)
    {
        // Problème avec l'exponentielle
        // sum = sum + exp(table_reward[s][i]);
    }

    double proba0 = table_reward[s][0]/sum;
    double proba1 = table_reward[s][1]/sum;
    double proba2 = table_reward[s][2]/sum;
    double proba3 = table_reward[s][3]/sum;
    // On détermine l'action future

    // Pour l'instant je ne sais pas chosir un nombre aléatoire facilement, on va y aller bourrin
    double n = (double)(rand() % 100000)/100000;
    // printf("%f\n", n);

    if (n < proba0)
    {
        enum action future_a = up;
        return future_a;
    }
    else if (n < proba0 + proba1)
    {
        enum action future_a = down;
        return future_a;
    }
    else if (n < proba0 + proba1 + proba2)
    {
        enum action future_a = left;
        return future_a;
    }
    else
    {
        enum action future_a = right;
        return future_a;
    }
}

void alloc_visited()
{
        visited = malloc(rows * sizeof(int*));
        int i;
        for (i = 0; i < rows; ++i){
                visited[i] = malloc(cols * sizeof(int*));
        }
}



void init_visited()
{
        alloc_visited();

        int i, j;
        for (i = 0; i < rows; ++i) {
                for (j = 0; j < cols; ++j) {
                        if (maze[i][j] == '+') {
                                visited[i][j] = wall;
                        } else if (maze[i][j] == 'g') {
                                visited[i][j] = goal;
                        } else {
                                visited[i][j] = unknown;
                        }
                }
        }
}

