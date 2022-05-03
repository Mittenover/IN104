#ifndef GAMEENV_H
#define GAMEENV_H

#include <stdio.h>
#include <stdlib.h>


char** grid;
int** visited;
int dim;
int start_row;
int start_col;
int state_row;
int state_col;
int goal_row;
int goal_col;

void alloc_grid();

void grid_make(int);

void grid_render(); 


#endif