#ifndef MCTS_H
#define MCTS_H

#include "node.h"
#include "tree.h"
#include "board.h"
#include "db_midiate.h"

#define LEARN true
#define LEARNS_CLEARN_LIMIT 100
#define LEARNS_CLEARN_TURN 20

void learn();

#endif