#include "game.h"
#include "board.c"

void game()
{
    Condition condition = initCondition();
    Move *pointableHands;
    int fc;
    Turn winner;

    for (int i = 1; i <= 400; i++)
    {
        if (i != 1)
        {
            condition.turn = !condition.turn;
            condition.turnNumber = i;
        }
        fc = serchPointableHands(condition, &pointableHands);
        int x = randBetween(fc - 1, 0);
        executeMove(&condition, pointableHands[x]);
        if (isEnd(condition, &winner))
        {
            if (winner)
            {
                printf("first won\n");
            }
            else
            {
                printf("second won\n");
            }
            return;
        }
    }
    printf("dwaw\n");
}