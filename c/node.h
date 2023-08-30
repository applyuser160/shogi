#ifndef NODE_H
#define NODE_H

#include "board.h"
#include "util.h"

// ノードのタイプ
typedef enum NODE_TYPE
{
    NODE = 0,
    GROUPED_NODE = 1,
} NODE_TYPE;

// モンテカルロ木のノード
typedef struct Node
{
    // ID
    char *id;

    // parentID
    char *parentId;

    // 次のノード数
    int childCount;

    // ターン数
    int turnNumber;

    // 指し手
    Move move;

    // 通過数
    int throughCount;

    // 引分数
    int drawCount;

    // 勝利数 FIRST
    int fiWinCount;

    // 勝利数 SECOND
    int seWinCount;

    // condition
    char *condition;
} Node;

// ノードの重複を修正するための構造体
typedef struct GroupedNode
{
    // parentID
    char *parentId;

    // ターン数
    int turnNumber;

    // 指し手
    Move move;

    // 重複数
    int count;

    // 通過数
    int throughCount;

    // 引分数
    int drawCount;

    // 勝利数 FIRST
    int fiWinCount;

    // 勝利数 SECOND
    int seWinCount;
} GroupedNode;

void initNode(Node *node);

void copyNode(Node *copy, Node *copied);

void copyGroupedNode(GroupedNode *copy, Node *copied);

void freeNode(Node *node);

#endif