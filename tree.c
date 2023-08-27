#include "tree.h"

bool equalAddress(Address left, Address right)
{
    return left.row == right.row && left.column == right.column;
}

bool equalAPiece(Piece left, Piece right)
{
    return left.index == right.index && left.piece.name == right.piece.name;
}

bool equalMove(Move left, Move right)
{
    return equalAddress(left.address, right.address) && equalAPiece(left.piece, right.piece);
}

// ノードが既に作成済みの場合インデックスを返す
int isCreated(Node *child, Node *parent)
{
    Node *childlist;
    int childlistCount = selectWhereParentID((*parent).id, childlist);
    for (int i = 0; i < childlistCount; i++)
    {
        if (equalMove(childlist[i].move, (*child).move))
        {
            return i;
        }
    }
    return -1;
}

// UCBアルゴリズム
int ucb(Node *nextNodelist, int nextNodeCount, int t, Turn turn)
{
    int selected_arm = 0;
    double max_ucb = 0.0;

    for (int i = 0; i < nextNodeCount; i++)
    {
        if (nextNodelist[i].throughCount == 0)
        {
            selected_arm = i;
            break;
        }

        int winCount;
        switch (turn)
        {
        case FIRST:
            winCount = nextNodelist[i].fiWinCount;
            break;
        case SECOND:
            winCount = nextNodelist[i].seWinCount;
            break;
        }

        double avg_reward = ((double)winCount + (double)nextNodelist[i].drawCount * 0.5) / (double)nextNodelist[i].throughCount;

        double ucb_value = avg_reward + sqrt(2 * log(t) / nextNodelist[i].throughCount);

        if (ucb_value > max_ucb)
        {
            max_ucb = ucb_value;
            selected_arm = i;
        }
    }
    return selected_arm;
}

// 現在指すことができる手のノードを作成する
void createNodeFromPossiblePlace(Node **nextNodelist, int *nextNodeCount, Node node, Condition condition)
{
    Move *pointableHands;
    *nextNodeCount = serchPointableHands(condition, &pointableHands);

    *nextNodelist = (Node*)calloc(*nextNodeCount, sizeof(Node));

    // データベースより子ノードの取得
    Node* child = (Node*)calloc(*nextNodeCount, sizeof(Node));
    int count = 0;
    if (!LEARN) count = selectWhereParentID(node.id, child);

    for (int i = 0; i < *nextNodeCount; i++)
    {
        bool hasCreated = false;
        for (int j = 0; j < count; j++)
        {
            if (equalMove((child[j]).move, pointableHands[i]))
            {
                hasCreated = true;
                (*nextNodelist)[i] = child[j];
                break;
            }
        }

        if (!hasCreated)
        {
            Node newNode;
            initNode(&newNode);
            newNode.turnNumber = node.turnNumber + 1;
            newNode.move.address.row = pointableHands[i].address.row;
            newNode.move.address.column = pointableHands[i].address.column;
            newNode.move.piece = pointableHands[i].piece;
            copyUuid(&node.id, &newNode.parentId);
            (*nextNodelist)[i] = newNode;
        }
    }
    free(child);
    free(pointableHands);
}


void mergeNode()
{
    // 重複しているレコードの統合
    GroupedNode *groupedNode = (GroupedNode*)calloc(LEARNS_CLEARN_LIMIT, sizeof(GroupedNode));
    Node *node = (Node*)calloc(LEARNS_CLEARN_LIMIT * LEARNS_CLEARN_TURN, sizeof(Node));
    int dupIdCount = 0;
    int duplicatedCount = selectDuplicated(groupedNode, node, &dupIdCount);

    Node *deleteNode = (Node*)calloc(LEARNS_CLEARN_TURN * 2 * duplicatedCount, sizeof(Node));
    int deleteCount = 0;

    Node *newNodes = (Node*)calloc(duplicatedCount, sizeof(Node));
    for (int i = 0; i < duplicatedCount; i++)
    {
        int updateCount = 0;
        Node *updateNode = (Node*)calloc(LEARNS_CLEARN_TURN * 2, sizeof(Node));

        initNode(&newNodes[i]);
        copyGroupedNode(&groupedNode[i], &newNodes[i]);

        free(groupedNode[i].parentId);

        for (int j = 0; j < dupIdCount; j++)
        {
            bool eqPid = strcmp(newNodes[i].parentId, node[j].parentId) == 0;
            bool eqtnum = newNodes[i].turnNumber == node[j].turnNumber;
            bool eqmove = equalMove(newNodes[i].move, node[j].move);
            bool eqall = eqPid && eqtnum && eqmove;
            if (eqall)
            {
                if (node[j].condition != "0" && node[j].condition != "")
                {
                    for (int k = 0; k < 1341; k++)
                    {
                        newNodes[i].condition[k] = node[j].condition[k];
                    }
                }

                initNode(&updateNode[updateCount]);
                copyNode(&node[j], &updateNode[updateCount]);
                updateCount++;

                initNode(&deleteNode[deleteCount]);
                copyNode(&node[j], &deleteNode[deleteCount]);
                deleteCount++;
                if (updateCount == LEARNS_CLEARN_TURN * 2) break;
            }
        }
        updateParentId(newNodes[i], updateNode, updateCount);

        for (int j = 0; j < updateCount; j++)
        {
            freeNode(&updateNode[j]);
        }
        free(updateNode);
    }
    free(groupedNode);

    deleteDuplicated(deleteNode, deleteCount);
    bulkinsert(newNodes, duplicatedCount);

    for (int i = 0; i < dupIdCount; i++)
    {
        freeNode(&node[i]);
    }
    free(node);

    for (int i = 0; i < deleteCount; i++)
    {
        freeNode(&deleteNode[i]);
    }
    free(deleteNode);

    for (int i = 0; i < duplicatedCount; i++)
    {
        freeNode(&newNodes[i]);
    }
    free(newNodes);
}