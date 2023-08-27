#include "mcts.h"

void learn()
{
    // モンテカルロ木
    Node *tree = (Node*)calloc(1, sizeof(Node));

    // topnode sql:insert into [dbname].[tablename] values('sZG6neL2-sZQ7-oeLC-tjQ7-yfVZG7xeVL2t-524566', null, 0, 0, 0, -1, 0, 0, 0, 0, 0);
    char* topNodeId = "sZG6neL2-sZQ7-oeLC-tjQ7-yfVZG7xeVL2t-524566";
    selectWhereID(tree, topNodeId);

    for (int i = 0; i < NUMBER_OF_SEARCH; i++)
    {
        // 盤面
        Condition condition = initCondition();

        Node *currentNode = (Node*)calloc(condition.turnNumber, sizeof(Node));
        selectWhereID(currentNode, topNodeId);

        // 勝った方
        Turn winner;

        while (1)
        {
            Node *nextNodelist;
            int nextNodeCount;

            // UCBにより、手を評価し、選択する
            createNodeFromPossiblePlace(&nextNodelist, &nextNodeCount, currentNode[condition.turnNumber - 1], condition);
            // int selected = ucb(nextNodelist, nextNodeCount, condition.turnNumber, condition.turn);
            int selected = randBetween(nextNodeCount - 1,0);

            Node *oldNode = currentNode;
            currentNode = (Node*)calloc(condition.turnNumber + 1, sizeof(Node));
            for (int j = 0; j < condition.turnNumber; j++)
            {
                initNode(&(currentNode[j]));
                copyNode(&(oldNode[j]), &(currentNode[j]));
                freeNode(&oldNode[j]);
            }
            free(oldNode);
            initNode(&(currentNode[condition.turnNumber]));
            copyNode(&(nextNodelist[selected]), &(currentNode[condition.turnNumber]));
            for (int j = 0; j < nextNodeCount; j++)
            {
                freeNode(&nextNodelist[j]);
            }
            free(nextNodelist);

            // 手を指す
            executeMove(&condition, currentNode[condition.turnNumber].move);

            // 現在の状況をバイナリ化し、Nodeに組み込む
            getBinaryFromCondition(currentNode[condition.turnNumber].condition, condition);

            // 終了判定
            if (!isEnd(condition, &winner) && condition.turnNumber < 500)
            {
                // ターン数の加算
                condition.turnNumber++;
                // 次のターンへ
                condition.turn = !condition.turn;
            }
            else
            {
                break;
            }
        }

        // 結果を逆伝播する
        for (int j = condition.turnNumber; j > -1; j--)
        {
            // 通過数と結果を加算
            currentNode[j].throughCount++;
            if (condition.turnNumber == 500)
            {
                currentNode[j].drawCount++;
            }
            else
            {
                switch (winner)
                {
                case FIRST:
                    currentNode[j].fiWinCount++;
                    break;
                case SECOND:
                    currentNode[j].seWinCount++;
                    break;
                }
            }
        }
        // データベースに反映
        Node *nodeForInsert = (Node *)calloc(condition.turnNumber + 1, sizeof(Node));
        int nodeForInsertCount = 0;
        for (int j = 0; j <= condition.turnNumber; j++)
        {
            if (currentNode[j].throughCount > 1 || currentNode[j].turnNumber == 0)
            {
                updateFromNode(currentNode[j]);
            }
            else
            {
                initNode(&(nodeForInsert[nodeForInsertCount]));
                copyNode(&(currentNode[j]), &(nodeForInsert[nodeForInsertCount]));
                nodeForInsertCount++;
            }
        }
        for (int j = 0; j <= condition.turnNumber; j++)
        {
            freeNode(&currentNode[j]);
        }
        free(currentNode);
        bulkinsert(nodeForInsert, nodeForInsertCount);
        for (int j = 0; j < nodeForInsertCount - 1; j++)
        {
            freeNode(&nodeForInsert[j]);
        }
        free(nodeForInsert);

        time_t now = time(NULL);
        printf("serch turn:%d, %s", i, ctime(&now));
        usleep(100);

        if (i % LEARNS_CLEARN_TURN == LEARNS_CLEARN_TURN - 1)
        {
            now = time(NULL);
            printf("merge node start %s", ctime(&now));
            mergeNode();
            printf("merge node finshed %s", ctime(&now));
        }
    }

    printf("end\n");
}