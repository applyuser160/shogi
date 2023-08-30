#ifndef TREE_H
#define TREE_H

#include <math.h>
#include "board.h"
#include "node.h"
#include "util.h"
#include "db_midiate.h"
#include "mcts.h"

#define NUMBER_OF_SEARCH 10000
#define NUMBER_OF_CHARACTERS_FOR_A_NODE 235

bool equalAddress(Address left, Address right);
bool equalAPiece(Piece left, Piece right);
bool equalMove(Move left, Move right);

// ノードが既に作成済みの場合インデックスを返す
int isCreated(Node *child, Node *parent);

// UCBアルゴリズム
int ucb(Node *nextNodelist, int nextNodeCount, int t, Turn turn);

// 現在指すことができる手のノードを作成する
void createNodeFromPossiblePlace(Node **nextNodelist, int *nextNodeCount, Node node, Condition condition);

// データベースに保存されているノードの中から重複しているものを抜き出し、統合する
void mergeNode();

#endif