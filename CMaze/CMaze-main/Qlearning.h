#ifndef QLEARNING_H
#define QLEARNING_H

#include <stdlib.h>
#include <stdio.h>

#include "mazeEnv.h"

double** table_reward;
double alpha;
double epsilon;

void alloc_table_reward();

void fill_tableau();

void print_table_reward();

double max_future_state(int s);

int action_to_int(action a);

void learn(char *maze);

void one_learning();

void one_learning_sarsa();

int dfs2();

void add_crumbs();

action next_move();


#endif