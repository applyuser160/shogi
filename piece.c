#include "piece.h"

Promotion getPromotion(PieceName name)
{
    if (name == ROOK || name == BICHOP || name == SILVERGENERAL || name == KNIGHT || name == LANCE || name == PAWN)
    {
        return PROMOTABLE;
    }
    else
    {
        return NOTPROMOTABLE;
    }
}

WayOfMove getTemplateMove(TemplateMove move, Direction direction)
{
    WayOfMove way;
    switch (move)
    {
    case NEXTTO:
        way.through = false;
        way.adirectionMove[0].maxLength = 1;
        way.adirectionMove[0].direction = direction;
        way.adirectionMove[1].maxLength = 0;
        break;
    case UNLIMITED:
        way.through = false;
        way.adirectionMove[0].maxLength = NUMBER_OF_EDGE - 1;
        way.adirectionMove[0].direction = direction;
        way.adirectionMove[1].maxLength = 0;
        break;
    case HOP:
        way.through = true;
        way.adirectionMove[0].maxLength = 1;
        way.adirectionMove[0].direction = direction;
        way.adirectionMove[1].maxLength = 1;
        way.adirectionMove[1].direction = UP;
        break;
    case NOT:
        way.through = false;
        way.adirectionMove[0].maxLength = 0;
        way.adirectionMove[0].direction = direction;
        way.adirectionMove[1].maxLength = 0;
        break;
    }
    return way;
}

AbstractPiece generateAbstractPiece(PieceName name)
{
    AbstractPiece piece;
    piece.name = name;
    piece.promotion = getPromotion(name);

    switch (name)
    {
    case NON:
        piece.wayOfMove[UP] = getTemplateMove(NOT, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(NOT, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(NOT, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(NOT, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(NOT, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(NOT, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(NOT, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(NOT, UP_RIGHT);
        break;
    case KING:
        piece.wayOfMove[UP] = getTemplateMove(NEXTTO, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(NEXTTO, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(NEXTTO, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(NEXTTO, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(NEXTTO, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(NEXTTO, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(NEXTTO, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(NEXTTO, UP_RIGHT);
        break;
    case ROOK:
        piece.wayOfMove[UP] = getTemplateMove(UNLIMITED, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(NOT, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(UNLIMITED, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(NOT, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(UNLIMITED, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(NOT, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(UNLIMITED, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(NOT, UP_RIGHT);
        break;
    case BICHOP:
        piece.wayOfMove[UP] = getTemplateMove(NOT, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(UNLIMITED, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(NOT, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(UNLIMITED, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(NOT, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(UNLIMITED, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(NOT, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(UNLIMITED, UP_RIGHT);
        break;
    case GOLDGENERAL:
        piece.wayOfMove[UP] = getTemplateMove(NEXTTO, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(NEXTTO, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(NEXTTO, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(NOT, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(NEXTTO, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(NOT, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(NEXTTO, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(NEXTTO, UP_RIGHT);
        break;
    case SILVERGENERAL:
        piece.wayOfMove[UP] = getTemplateMove(NEXTTO, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(NEXTTO, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(NOT, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(NEXTTO, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(NOT, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(NEXTTO, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(NOT, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(NEXTTO, UP_RIGHT);
        break;
    case KNIGHT:
        piece.wayOfMove[UP] = getTemplateMove(NOT, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(HOP, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(NOT, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(NOT, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(NOT, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(NOT, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(NOT, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(HOP, UP_RIGHT);
        break;
    case LANCE:
        piece.wayOfMove[UP] = getTemplateMove(UNLIMITED, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(NOT, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(NOT, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(NOT, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(NOT, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(NOT, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(NOT, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(NOT, UP_RIGHT);
        break;
    case PAWN:
        piece.wayOfMove[UP] = getTemplateMove(NEXTTO, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(NOT, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(NOT, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(NOT, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(NOT, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(NOT, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(NOT, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(NOT, UP_RIGHT);
        break;
    case PROMOTED_ROOK:
        piece.wayOfMove[UP] = getTemplateMove(UNLIMITED, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(NEXTTO, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(UNLIMITED, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(NEXTTO, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(UNLIMITED, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(NEXTTO, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(UNLIMITED, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(NEXTTO, UP_RIGHT);
        break;
    case PROMOTED_BICHOP:
        piece.wayOfMove[UP] = getTemplateMove(NEXTTO, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(UNLIMITED, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(NEXTTO, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(UNLIMITED, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(NEXTTO, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(UNLIMITED, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(NEXTTO, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(UNLIMITED, UP_RIGHT);
        break;
    case PROMOTED_SILVERGENERAL:
        piece.wayOfMove[UP] = getTemplateMove(NEXTTO, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(NEXTTO, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(NEXTTO, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(NOT, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(NEXTTO, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(NOT, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(NEXTTO, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(NEXTTO, UP_RIGHT);
        break;
    case PROMOTED_KNIGHT:
        piece.wayOfMove[UP] = getTemplateMove(NEXTTO, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(NEXTTO, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(NEXTTO, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(NOT, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(NEXTTO, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(NOT, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(NEXTTO, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(NEXTTO, UP_RIGHT);
        break;
    case PROMOTED_LANCE:
        piece.wayOfMove[UP] = getTemplateMove(NEXTTO, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(NEXTTO, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(NEXTTO, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(NOT, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(NEXTTO, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(NOT, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(NEXTTO, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(NEXTTO, UP_RIGHT);
        break;
    case PROMOTED_PAWN:
        piece.wayOfMove[UP] = getTemplateMove(NEXTTO, UP);
        piece.wayOfMove[UP_LEFT] = getTemplateMove(NEXTTO, UP_LEFT);
        piece.wayOfMove[LEFT] = getTemplateMove(NEXTTO, LEFT);
        piece.wayOfMove[DOWN_LEFT] = getTemplateMove(NOT, DOWN_LEFT);
        piece.wayOfMove[DOWN] = getTemplateMove(NEXTTO, DOWN);
        piece.wayOfMove[DOWN_RIGHT] = getTemplateMove(NOT, DOWN_RIGHT);
        piece.wayOfMove[RIGHT] = getTemplateMove(NEXTTO, RIGHT);
        piece.wayOfMove[UP_RIGHT] = getTemplateMove(NEXTTO, UP_RIGHT);
        break;
    }
    return piece;
}

Piece generatePiece(PieceName name, int index, Turn turn)
{
    AbstractPiece p = generateAbstractPiece(name);
    Piece ap = {p, index, turn};
    return ap;
}

char pieceToString(AbstractPiece piece)
{
    switch (piece.name)
    {
    case NON:
        return ' ';
    case KING:
        return 'K';
    case ROOK:
        return 'R';
    case BICHOP:
        return 'B';
    case GOLDGENERAL:
        return 'G';
    case SILVERGENERAL:
        return 's';
    case KNIGHT:
        return 'n';
    case LANCE:
        return 'l';
    case PAWN:
        return 'p';
    case PROMOTED_ROOK:
        return 'D';
    case PROMOTED_BICHOP:
        return 'H';
    case PROMOTED_SILVERGENERAL:
        return 'S';
    case PROMOTED_KNIGHT:
        return 'N';
    case PROMOTED_LANCE:
        return 'L';
    case PROMOTED_PAWN:
        return 'P';
    }
}

Direction switchDirection(Direction direction)
{
    return (direction + 4) % 8;
}

void getAddressOfDirection(Direction direction, int *vectorRow, int *vectorColumn)
{
    switch (direction)
    {
    case UP:
        *vectorRow = -1;
        *vectorColumn = 0;
        break;
    case UP_LEFT:
        *vectorRow = -1;
        *vectorColumn = 1;
        break;
    case LEFT:
        *vectorRow = 0;
        *vectorColumn = 1;
        break;
    case DOWN_LEFT:
        *vectorRow = 1;
        *vectorColumn = 1;
        break;
    case DOWN:
        *vectorRow = 1;
        *vectorColumn = 0;
        break;
    case DOWN_RIGHT:
        *vectorRow = 1;
        *vectorColumn = -1;
        break;
    case RIGHT:
        *vectorRow = 0;
        *vectorColumn = -1;
        break;
    case UP_RIGHT:
        *vectorRow = -1;
        *vectorColumn = -1;
        break;
    }
}

PieceName getPromotedPieceName(PieceName name)
{
    switch (name)
    {
    case NON:
        return NON;
    case KING:
        return NON;
    case ROOK:
        return PROMOTED_ROOK;
    case BICHOP:
        return PROMOTED_BICHOP;
    case GOLDGENERAL:
        return NON;
    case SILVERGENERAL:
        return PROMOTED_SILVERGENERAL;
    case KNIGHT:
        return PROMOTED_KNIGHT;
    case LANCE:
        return PROMOTED_LANCE;
    case PAWN:
        return PROMOTED_PAWN;
    case PROMOTED_ROOK:
        return NON;
    case PROMOTED_BICHOP:
        return NON;
    case PROMOTED_SILVERGENERAL:
        return NON;
    case PROMOTED_KNIGHT:
        return NON;
    case PROMOTED_LANCE:
        return NON;
    case PROMOTED_PAWN:
        return NON;
    }
}

PieceName getPieceNameBeforePromote(PieceName name)
{
    switch (name)
    {
    case NON:
        return NON;
    case KING:
        return KING;
    case ROOK:
        return ROOK;
    case BICHOP:
        return BICHOP;
    case GOLDGENERAL:
        return GOLDGENERAL;
    case SILVERGENERAL:
        return SILVERGENERAL;
    case KNIGHT:
        return KNIGHT;
    case LANCE:
        return LANCE;
    case PAWN:
        return PAWN;
    case PROMOTED_ROOK:
        return ROOK;
    case PROMOTED_BICHOP:
        return BICHOP;
    case PROMOTED_SILVERGENERAL:
        return SILVERGENERAL;
    case PROMOTED_KNIGHT:
        return KNIGHT;
    case PROMOTED_LANCE:
        return LANCE;
    case PROMOTED_PAWN:
        return PAWN;
    }
}

int getBinaryFromPiece(Piece piece)
{
    int result = 0;
    result += (piece.index << 5);
    result += ((piece.piece.name + 1) << 1);
    result += piece.turn ? 1 : 0;
    return result;
}