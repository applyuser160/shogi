#include "node.h"
#include "util.h"

void initNode(Node *node)
{
    (*node).id = (char*)calloc(43, sizeof(char));
    generateUUID((*node).id);
    (*node).parentId = (char*)calloc(43, sizeof(char));
    (*node).childCount = 0;
    (*node).turnNumber = 0;
    (*node).move.address.row = 0;
    (*node).move.address.column = 0;
    (*node).move.piece = generatePiece(NON, 0, FIRST);
    (*node).throughCount = 0;
    (*node).drawCount = 0;
    (*node).fiWinCount = 0;
    (*node).seWinCount = 0;
    (*node).condition = (char*)calloc(1341, sizeof(char));
    (*node).condition[0] = '0';
}

void copyNode(Node *copy, Node *copied)
{
    for (int i = 0; i < 43; i++)
    {
        (*copied).id[i] = (*copy).id[i];
        (*copied).parentId[i] = (*copy).parentId[i];
    }
    (*copied).childCount = (*copy).childCount;
    (*copied).turnNumber = (*copy).turnNumber;
    (*copied).move.address.row = (*copy).move.address.row;
    (*copied).move.address.column = (*copy).move.address.column;
    (*copied).move.piece = (*copy).move.piece;
    (*copied).throughCount = (*copy).throughCount;
    (*copied).drawCount = (*copy).drawCount;
    (*copied).fiWinCount = (*copy).fiWinCount;
    (*copied).seWinCount = (*copy).seWinCount;
    for (int i = 0; i < 1341; i++)
    {
        (*copied).condition[i] = (*copy).condition[i];
    }
}

void copyGroupedNode(GroupedNode *copy, Node *copied)
{
    for (int i = 0; i < 43; i++)
    {
        (*copied).parentId[i] = (*copy).parentId[i];
    }
    (*copied).turnNumber = (*copy).turnNumber;
    (*copied).move.address.row = (*copy).move.address.row;
    (*copied).move.address.column = (*copy).move.address.column;
    (*copied).move.piece = (*copy).move.piece;
    (*copied).throughCount = (*copy).throughCount;
    (*copied).drawCount = (*copy).drawCount;
    (*copied).fiWinCount = (*copy).fiWinCount;
    (*copied).seWinCount = (*copy).seWinCount;
}

void freeNode(Node *node)
{
    free((*node).id);
    free((*node).parentId);
    free((*node).condition);
}