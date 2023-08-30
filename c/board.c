#include "board.h"
#include "piece.h"

// 駒を配置する
void deployPiece(Board *board, Piece piece, int row, int column)
{
    (*board).squares[9 * (row - 1) + (column - 1)].isUsed = USED;
    (*board).squares[9 * (row - 1) + (column - 1)].piece = piece;
}

// 初期配置を返す
Board initBoard()
{
    Board board;
    for (int i = 0; i < NUMBER_OF_SQUARES; i++)
    {
        int row = i / 9 + 1;
        int column = i % 9 + 1;
        Address address = {row, column};
        Piece piece = generatePiece(NON, 0, FIRST);
        Square square = {address, NOT_USED, piece};
        board.squares[i] = square;
    }

    deployPiece(&board, generatePiece(KING, 1, FIRST), 9, 5);
    deployPiece(&board, generatePiece(ROOK, 2, FIRST), 8, 2);
    deployPiece(&board, generatePiece(BICHOP, 3, FIRST), 8, 8);
    deployPiece(&board, generatePiece(GOLDGENERAL, 4, FIRST), 9, 4);
    deployPiece(&board, generatePiece(GOLDGENERAL, 5, FIRST), 9, 6);
    deployPiece(&board, generatePiece(SILVERGENERAL, 6, FIRST), 9, 3);
    deployPiece(&board, generatePiece(SILVERGENERAL, 7, FIRST), 9, 7);
    deployPiece(&board, generatePiece(KNIGHT, 8, FIRST), 9, 2);
    deployPiece(&board, generatePiece(KNIGHT, 9, FIRST), 9, 8);
    deployPiece(&board, generatePiece(LANCE, 10, FIRST), 9, 1);
    deployPiece(&board, generatePiece(LANCE, 11, FIRST), 9, 9);
    deployPiece(&board, generatePiece(PAWN, 12, FIRST), 7, 1);
    deployPiece(&board, generatePiece(PAWN, 13, FIRST), 7, 2);
    deployPiece(&board, generatePiece(PAWN, 14, FIRST), 7, 3);
    deployPiece(&board, generatePiece(PAWN, 15, FIRST), 7, 4);
    deployPiece(&board, generatePiece(PAWN, 16, FIRST), 7, 5);
    deployPiece(&board, generatePiece(PAWN, 17, FIRST), 7, 6);
    deployPiece(&board, generatePiece(PAWN, 18, FIRST), 7, 7);
    deployPiece(&board, generatePiece(PAWN, 19, FIRST), 7, 8);
    deployPiece(&board, generatePiece(PAWN, 20, FIRST), 7, 9);

    deployPiece(&board, generatePiece(KING, 21, SECOND), 1, 5);
    deployPiece(&board, generatePiece(ROOK, 22, SECOND), 2, 8);
    deployPiece(&board, generatePiece(BICHOP, 23, SECOND), 2, 2);
    deployPiece(&board, generatePiece(GOLDGENERAL, 24, SECOND), 1, 4);
    deployPiece(&board, generatePiece(GOLDGENERAL, 25, SECOND), 1, 6);
    deployPiece(&board, generatePiece(SILVERGENERAL, 26, SECOND), 1, 3);
    deployPiece(&board, generatePiece(SILVERGENERAL, 27, SECOND), 1, 7);
    deployPiece(&board, generatePiece(KNIGHT, 28, SECOND), 1, 2);
    deployPiece(&board, generatePiece(KNIGHT, 29, SECOND), 1, 8);
    deployPiece(&board, generatePiece(LANCE, 30, SECOND), 1, 1);
    deployPiece(&board, generatePiece(LANCE, 31, SECOND), 1, 9);
    deployPiece(&board, generatePiece(PAWN, 32, SECOND), 3, 1);
    deployPiece(&board, generatePiece(PAWN, 33, SECOND), 3, 2);
    deployPiece(&board, generatePiece(PAWN, 34, SECOND), 3, 3);
    deployPiece(&board, generatePiece(PAWN, 35, SECOND), 3, 4);
    deployPiece(&board, generatePiece(PAWN, 36, SECOND), 3, 5);
    deployPiece(&board, generatePiece(PAWN, 37, SECOND), 3, 6);
    deployPiece(&board, generatePiece(PAWN, 38, SECOND), 3, 7);
    deployPiece(&board, generatePiece(PAWN, 39, SECOND), 3, 8);
    deployPiece(&board, generatePiece(PAWN, 40, SECOND), 3, 9);
    return board;
}

// 盤面・持ち駒の表示
void displayCondition(Condition condition)
{
    printf("|9|8|7|6|5|4|3|2|1|\n");
    for (int i1 = 0; i1 < NUMBER_OF_EDGE; i1++)
    {
        printf("|");
        for (int i2 = NUMBER_OF_EDGE - 1; i2 > -1; i2--)
        {
            Piece piece = condition.board.squares[i1 * NUMBER_OF_EDGE + i2].piece;
            if (piece.piece.name == NON)
            {
                printf("%s|", pieceToString(piece.piece));
            }
            else if (piece.turn == FIRST)
            {
                printf("\x1b[36m%s\x1b[0m|", pieceToString(piece.piece));
            }
            else
            {
                printf("\x1b[91m%s\x1b[0m|", pieceToString(piece.piece));
            }
        }
        printf("%d\n", i1 + 1);
    }

    printf("NotOnBoard:\n");
    for (int i = 0; i < NUMBER_OF_PIECES; i++)
    {
        Piece piece = condition.notOnBoard[i];
        if (piece.piece.name == NON)
        {
            continue;
        }
        else if (piece.turn == FIRST)
        {
            printf("\x1b[36m%s,\x1b[0m", pieceToString(piece.piece));
        }
        else
        {
            printf("\x1b[91m%s,\x1b[0m", pieceToString(piece.piece));
        }
    }
    printf("\n");
}

// 持ち駒の初期化
void initNotOnBoard(Piece notOnBoard[NUMBER_OF_PIECES])
{
    for (int i = 0; i < NUMBER_OF_PIECES; i++)
    {
        notOnBoard[i] = generatePiece(NON, 0, FIRST);
    }
}

// 状況の初期化
Condition initCondition()
{
    Board board = initBoard();
    Condition condition;
    condition.board = board;
    condition.turnNumber = 1;
    condition.turn = FIRST;
    initNotOnBoard(condition.notOnBoard);
    return condition;
}

// 任意の駒が盤面のどこに位置するか調べる　存在しない場合は、0,0を返す
Address serchPieceFromBoard(Board board, Piece piece)
{
    for (int i = 0; i < NUMBER_OF_SQUARES; i++)
    {
        Piece apiece = board.squares[i].piece;
        if (apiece.index == piece.index && apiece.piece.name != NON)
        {
            return board.squares[i].address;
        }
    }
    Address non = {0, 0};
    return non;
}

// 任意の駒が持ち駒の中のどこに位置するか調べる 存在しない場合は、-1を返す
int serchPieceFromNotOnBoard(Piece notOnBoard[NUMBER_OF_PIECES], Piece piece)
{
    for (int i = 0; i < NUMBER_OF_PIECES; i++)
    {
        Piece apiece = notOnBoard[i];
        if (apiece.index == piece.index)
        {
            return i;
        }
    }
    return -1;
}

// 盤面の任意の場所に駒を設置する
// deployPieceでカバー可能
void deployPieceOnBoard(Board *board, Address address, Piece piece)
{
    for (int i = 0; i < NUMBER_OF_SQUARES; i++)
    {
        if ((*board).squares[i].address.row == address.row && (*board).squares[i].address.column == address.column)
        {
            (*board).squares[i].piece = piece;
        }
    }
}

// 持ち駒の任意の場所に駒を設置する
void deployPieceNotOnBoard(Piece notOnBoard[NUMBER_OF_PIECES], int index, Piece piece)
{
    notOnBoard[index] = piece;
}

// 任意の位置にある駒が動くことが出来るか
bool ableMove(Turn turn, PieceName name, Address address)
{
    if (turn == FIRST)
    {
        if (name == KNIGHT && address.row < 3)
        {
            return false;
        }
        else if ((name == LANCE || name == PAWN) && address.row < 2)
        {
            return false;
        }
    }
    else if (turn == SECOND)
    {
        if (name == KNIGHT && address.row > 7)
        {
            return false;
        }
        else if ((name == LANCE || name == PAWN) && address.row > 8)
        {
            return false;
        }
    }
    return true;
}

// 指定の駒が成ることが出来るか
bool ableBe(Address oldAddress, Address newAddress, Piece piece)
{
    if (piece.piece.promotion)
    {
        if (piece.turn == FIRST)
        {
            return oldAddress.row < 4 || newAddress.row < 4;
        }
        else
        {
            return oldAddress.row > 6 || newAddress.row > 6;
        }
    }
    return false;
}

// 指定の方向に移動する
Square moveBoard(Board board, Piece piece, int row, int column, int length, Direction direction)
{
    Turn turn = piece.turn;

    Direction d0 = piece.piece.wayOfMove[direction].adirectionMove[0].direction;
    int ml0 = piece.piece.wayOfMove[direction].adirectionMove[0].maxLength;
    Direction d1 = piece.piece.wayOfMove[direction].adirectionMove[1].direction;
    int ml1 = piece.piece.wayOfMove[direction].adirectionMove[1].maxLength;

    // 後手の場合、向きを逆にする
    if (turn == SECOND)
    {
        d0 = switchDirection(d0);
        d1 = switchDirection(d1);
    }

    // lengthがmaxlengthを超えていた場合は、maxlengthに合わせる
    if (length > ml0)
    {
        length = ml0;
    }

    int vectorRow0;
    int vectorColumn0;
    int vectorRow1;
    int vectorColumn1;

    getAddressOfDirection(d0, &vectorRow0, &vectorColumn0);
    getAddressOfDirection(d1, &vectorRow1, &vectorColumn1);

    row += vectorRow0 * length + vectorRow1 * ml1;
    column += vectorColumn0 * length + vectorColumn1 * ml1;

    for (int i = 0; i < NUMBER_OF_SQUARES; i++)
    {
        Square square = board.squares[i];
        if (square.address.row == row && square.address.column == column)
        {
            return square;
        }
    }

    // 枠外の場合
    Address address = {0, 0};
    Piece p = generatePiece(NON, 0, FIRST);
    Square square = {address, NOT_USED, p};
    return square;
}

// 特定の方向にnマス移動した場所を返す
Square getSquareWhenMoved(Board board, Piece piece, int row, int column, int length, Direction direction)
{
    return moveBoard(board, piece, row, column, length, direction);
}

// 桂馬の動く先
Square getSquareWhenMovedKnight(Board board, Piece piece, int row, int column, Direction direction)
{
    Turn turn = piece.turn;
    if (turn == SECOND)
    {
        direction = switchDirection(direction);
    }
}

// 二歩を検知
bool isTwoSteps(Board board, Piece piece, Address address)
{
    if (piece.piece.name == PAWN)
    {
        for (int i = 0; i < NUMBER_OF_SQUARES; i++)
        {
            if (board.squares[i].address.column == address.column)
            {
                // 同じ行
                if (board.squares[i].piece.piece.name == PAWN && board.squares[i].piece.turn == piece.turn)
                {
                    // 自分の歩
                    return true;
                }
            }
        }
        return false;
    }
    else
    {
        return false;
    }
}

// 手を追加
void addMoves(Move **moves, int *count, Address address, Piece piece)
{
    Move *old = *moves;
    *moves = (Move *)calloc(*count + 1, sizeof(Move));
    for (int i = 0; i < *count; i++)
    {
        (*moves)[i] = old[i];
    }
    Move move = {address, piece};
    (*moves)[*count] = move;
    (*count)++;
    if(*count > 1)free(old);
}

// その場所に指すことができる手を追加する
void serchAndAddMoves(Move **pointableHands, int *count, Square fromSquare, Square targetSquare, Turn turn)
{
    if (ableBe(fromSquare.address, targetSquare.address, fromSquare.piece))
    {
        // 成ることが出来る場合
        if (ableMove(turn, fromSquare.piece.piece.name, targetSquare.address))
        {
            // 成らなくてもまだ、動くことが可能な場合
            addMoves(pointableHands, count, targetSquare.address, fromSquare.piece);
            Piece promotedPiece = fromSquare.piece;
            promotedPiece.piece.name = getPromotedPieceName(fromSquare.piece.piece.name);
            promotedPiece.piece.promotion = NOTPROMOTABLE;
            addMoves(pointableHands, count, targetSquare.address, promotedPiece);
        }
        else
        {
            // 成らないと動くことが出来なくなる場合
            Piece promotedPiece = fromSquare.piece;
            promotedPiece.piece.name = getPromotedPieceName(fromSquare.piece.piece.name);
            promotedPiece.piece.promotion = NOTPROMOTABLE;
            addMoves(pointableHands, count, targetSquare.address, promotedPiece);
        }
    }
    else
    {
        addMoves(pointableHands, count, targetSquare.address, fromSquare.piece);
    }
}

// 特定のアドレスから任意の場所に移動し、指すことが出来る手を追加する
void executeAddMoves(Move **pointableHands, int *count, Square fromSquare, Board board, Direction direction, Turn turn)
{
    WayOfMove wayOfMove = fromSquare.piece.piece.wayOfMove[direction];
    if (wayOfMove.adirectionMove[0].maxLength == NUMBER_OF_EDGE - 1)
    {
        for (int j = 1; j < NUMBER_OF_EDGE; j++)
        {
            Square targetSquare = getSquareWhenMoved(board, fromSquare.piece, fromSquare.address.row, fromSquare.address.column, j, direction);

            // 駒が置かれていたら停止
            if (targetSquare.piece.piece.name != NON)
            {
                // 置かれている駒が自分の駒の場合、その１つ前まで、置くことが出来る
                // 置かれている駒が相手の駒の場合、そこまで置くことが出来る
                if (targetSquare.piece.turn != turn)
                {
                    serchAndAddMoves(pointableHands, count, fromSquare, targetSquare, turn);
                }
                return;
            }
            // 盤面から外れたら停止
            else if (targetSquare.address.row == 0 && targetSquare.address.column == 0)
            {
                return;
            }
            else
            {
                serchAndAddMoves(pointableHands, count, fromSquare, targetSquare, turn);
            }
        }
    }
    else if (wayOfMove.adirectionMove[1].maxLength == 1)
    {
        Square targetSquare = getSquareWhenMoved(board, fromSquare.piece, fromSquare.address.row, fromSquare.address.column, 1, direction);

        // 駒が置かれていたら停止
        if (targetSquare.piece.piece.name != NON)
        {
            // 置かれている駒が自分の駒の場合、その１つ前まで、置くことが出来る
            // 置かれている駒が相手の駒の場合、そこまで置くことが出来る
            if (targetSquare.piece.turn != turn)
            {
                serchAndAddMoves(pointableHands, count, fromSquare, targetSquare, turn);
            }
            return;
        }
        // 盤面から外れたら停止
        else if (targetSquare.address.row == 0 && targetSquare.address.column == 0)
        {
            return;
        }
        else
        {
            serchAndAddMoves(pointableHands, count, fromSquare, targetSquare, turn);
        }
    }
    else if (wayOfMove.adirectionMove[0].maxLength == 1)
    {
        Square targetSquare = getSquareWhenMoved(board, fromSquare.piece, fromSquare.address.row, fromSquare.address.column, 1, direction);

        // 駒が置かれていたら停止
        if (targetSquare.piece.piece.name != NON)
        {
            // 置かれている駒が自分の駒の場合、その１つ前まで、置くことが出来る
            // 置かれている駒が相手の駒の場合、そこまで置くことが出来る
            if (targetSquare.piece.turn != turn)
            {
                serchAndAddMoves(pointableHands, count, fromSquare, targetSquare, turn);
            }
            return;
        }
        // 盤面から外れたら停止
        else if (targetSquare.address.row == 0 && targetSquare.address.column == 0)
        {
            return;
        }
        else
        {
            serchAndAddMoves(pointableHands, count, fromSquare, targetSquare, turn);
        }
    }
}

// 動かせる手を探索
int serchPointableHands(Condition condition, Move **pointableHands)
{
    // 動かせる手
    int count = 0;

    // 盤面
    for (int i = 0; i < NUMBER_OF_SQUARES; i++)
    {
        Piece piece = condition.board.squares[i].piece;
        if (piece.piece.name != NON && piece.turn == condition.turn)
        {
            Address address = condition.board.squares[i].address;
            // ８方向で行ける場所を探索
            for (int d = 0; d < NUMBER_OF_DIRECTIONS; d++)
            {
                executeAddMoves(pointableHands, &count, condition.board.squares[i], condition.board, d, condition.turn);
            }
        }
    }

    // 持ち駒
    for (int i = 0; i < NUMBER_OF_PIECES; i++)
    {
        Piece piece = condition.notOnBoard[i];
        // 有効な持ち駒
        if (piece.piece.name != NON && piece.turn == condition.turn)
        {
            for (int j = 0; j < NUMBER_OF_SQUARES; j++)
            {
                Address address = {j / 9 + 1, j % 9 + 1};
                // 駒が置かれていない場所
                if (condition.board.squares[j].piece.piece.name == NON)
                {
                    // 動けない場合
                    PieceName name = piece.piece.name;
                    if (!ableMove(condition.turn, name, address) || isTwoSteps(condition.board, piece, address))
                    {
                        continue;
                    }
                    addMoves(pointableHands, &count, address, piece);
                }
            }
        }
    }

    return count;
}

// 手を指し、状況を更新
void executeMove(Condition *condition, Move move)
{
    // 動かす対象の駒を探索し、現在の位置に存在する駒を除去する
    // 盤面
    Address address = serchPieceFromBoard((*condition).board, move.piece);
    if (address.row != 0 && address.column != 0)
    {
        deployPieceOnBoard(&(*condition).board, address, generatePiece(NON, 0, FIRST));
    }

    // 持ち駒
    int notOnBoardIndex = serchPieceFromNotOnBoard((*condition).notOnBoard, move.piece);
    if (notOnBoardIndex != -1)
    {
        deployPieceNotOnBoard((*condition).notOnBoard, notOnBoardIndex, generatePiece(NON, 0, FIRST));
    }

    // 駒を設置し、すでに設置されている駒があった場合は、持ち駒の空いている場所に移す
    for (int i = 0; i < NUMBER_OF_SQUARES; i++)
    {
        Square *square = &(*condition).board.squares[i];
        if ((*square).address.row == move.address.row && (*square).address.column == move.address.column)
        {
            if ((*square).piece.piece.name != NON)
            {
                // すでに設置されている駒があった場合は、持ち駒の空いている場所に移す
                int ind = serchPieceFromNotOnBoard((*condition).notOnBoard, generatePiece(NON, 0, FIRST));
                (*square).piece.turn = !(*square).piece.turn;
                (*square).piece.piece.name = getPieceNameBeforePromote((*square).piece.piece.name);
                (*square).piece.piece.promotion = getPromotion((*square).piece.piece.name);
                deployPieceNotOnBoard((*condition).notOnBoard, ind, (*square).piece);
            }
            
            // 駒を設置する
            deployPieceOnBoard(&(*condition).board, move.address, move.piece);
        }
    }
}

// 終了判定
bool isEnd(Condition condition, Turn *winner)
{
    for (int i = 0; i < NUMBER_OF_PIECES; i++)
    {
        if (condition.notOnBoard[i].piece.name == KING)
        {
            // 王が持ち駒に見つかったら終了
            *winner = condition.notOnBoard[i].turn;
            return true;
        }
    }
    return false;
}

void getBinaryFromBoard(char *out, Board board)
{
    for (int i = 0; i < NUMBER_OF_SQUARES; i++)
    {
        int num = getBinaryFromPiece(board.squares[i].piece);
        int index = (board.squares[i].address.row - 1) * NUMBER_OF_EDGE + (board.squares[i].address.column - 1);
        printb(&out[index * 11], 11, num);
    }
}

void getBinaryFromNotOnBoard(char *out, Piece notOnBoard[NUMBER_OF_PIECES])
{
    for (int i = 0; i < NUMBER_OF_PIECES; i++)
    {
        int num = getBinaryFromPiece(notOnBoard[i]);
        printb(&out[i * 11], 11, num);
    }
}

void getBinaryFromCondition(char *out, Condition condition)
{
    getBinaryFromBoard(&out[0], condition.board);
    getBinaryFromNotOnBoard(&out[891], condition.notOnBoard);
    printb(&out[1331], 9, condition.turnNumber);
    out[1340] = condition.turn ? '1' : '0';
}