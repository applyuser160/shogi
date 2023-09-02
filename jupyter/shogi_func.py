from typing import List, Self
from shogi_enum import Direction, PieceName, Promotion, Turn, TemplateMove, SquareStatus, ConsoleColor

def switchDirection(direction: Direction) -> Direction:
    return (direction + 4) % 8

def printb(size: int, v: int) -> str:
    return f"{v:0={size}b}"
    
def getPromotion(name: PieceName) -> Promotion:
    if name == PieceName.ROOK or name == PieceName.BICHOP \
        or name == PieceName.SILVERGENERAL or name == PieceName.KNIGHT \
            or name == PieceName.LANCE or name == PieceName.PAWN:
        return Promotion.PROMOTABLE
    else:
        return Promotion.NOTPROMOTABLE
    
def getPromotedPieceName(name: PieceName) -> PieceName:
    if name == PieceName.NON:
        return PieceName.NON
    elif name == PieceName.KING:
        return PieceName.NON
    elif name == PieceName.ROOK:
        return PieceName.PROMOTED_ROOK
    elif name == PieceName.BICHOP:
        return PieceName.PROMOTED_BICHOP
    elif name == PieceName.GOLDGENERAL:
        return PieceName.NON
    elif name == PieceName.SILVERGENERAL:
        return PieceName.PROMOTED_SILVERGENERAL
    elif name == PieceName.KNIGHT:
        return PieceName.PROMOTED_KNIGHT
    elif name == PieceName.LANCE:
        return PieceName.PROMOTED_LANCE
    elif name == PieceName.PAWN:
        return PieceName.PROMOTED_PAWN
    elif name == PieceName.PROMOTED_ROOK:
        return PieceName.NON
    elif name == PieceName.PROMOTED_BICHOP:
        return PieceName.NON
    elif name == PieceName.PROMOTED_SILVERGENERAL:
        return PieceName.NON
    elif name == PieceName.PROMOTED_KNIGHT:
        return PieceName.NON
    elif name == PieceName.PROMOTED_LANCE:
        return PieceName.NON
    elif name == PieceName.PROMOTED_PAWN:
        return PieceName.NON
    
def getPieceNameBeforePromote(name: PieceName) -> PieceName:
    if name == PieceName.NON:
        return PieceName.NON
    elif name == PieceName.KING:
        return PieceName.KING
    elif name == PieceName.ROOK:
        return PieceName.ROOK
    elif name == PieceName.BICHOP:
        return PieceName.BICHOP
    elif name == PieceName.GOLDGENERAL:
        return PieceName.GOLDGENERAL
    elif name == PieceName.SILVERGENERAL:
        return PieceName.SILVERGENERAL
    elif name == PieceName.KNIGHT:
        return PieceName.KNIGHT
    elif name == PieceName.LANCE:
        return PieceName.LANCE
    elif name == PieceName.PAWN:
        return PieceName.PAWN
    elif name == PieceName.PROMOTED_ROOK:
        return PieceName.ROOK
    elif name == PieceName.PROMOTED_BICHOP:
        return PieceName.BICHOP
    elif name == PieceName.PROMOTED_SILVERGENERAL:
        return PieceName.SILVERGENERAL
    elif name == PieceName.PROMOTED_KNIGHT:
        return PieceName.KNIGHT
    elif name == PieceName.PROMOTED_LANCE:
        return PieceName.LANCE
    elif name == PieceName.PROMOTED_PAWN:
        return PieceName.PAWN
    

