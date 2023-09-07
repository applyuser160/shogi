import numpy as np
import cshogi
import random
import re
from typing import List, Self

pieceNames = [
    ["+r", "0", [0,0,0,0,1]], ["+R", "1", [1,0,0,0,1]],
    ["+b", "2", [0,0,0,1,0]], ["+B", "3", [1,0,0,1,0]],
    ["+s", "4", [0,0,0,1,1]], ["+S", "5", [1,0,0,1,1]],
    ["+k", "6", [0,0,1,0,0]], ["+K", "7", [1,0,1,0,0]],
    ["+l", "8", [0,0,1,0,1]], ["+L", "9", [1,0,1,0,1]],
    ["+p", "a", [0,0,1,1,0]], ["+P", "b", [1,0,1,1,0]],
    ["k", "c", [0,0,1,1,1]], ["K", "d", [1,0,1,1,1]],
    ["r", "e", [0,1,0,0,0]], ["R", "f", [1,1,0,0,0]],
    ["b", "g", [0,1,0,0,1]], ["B", "h", [1,1,0,0,1]],
    ["g", "i", [0,1,0,1,0]], ["G", "j", [1,1,0,1,0]],
    ["s", "k", [0,1,0,1,1]], ["S", "l", [1,1,0,1,1]],
    ["n", "m", [0,1,1,0,0]], ["N", "n", [1,1,1,0,0]],
    ["l", "o", [0,1,1,0,1]], ["L", "p", [1,1,1,0,1]],
    ["p", "q", [0,1,1,1,0]], ["P", "r", [1,1,1,1,0]],
    ["e", "s", [0,0,0,0,0]]
]

def convertForBoard(board: str) -> np.ndarray:
    board, turn, nob, turnNumber = board.split(' ')

    iterlist = re.finditer('[0-9]', board)
    valuelist = re.findall('[0-9]', board)
    newBoard = ""
    rep = next(iterlist).start()
    index = 0
    for i, v in enumerate(board):
        if int(rep) == i:
            empty = "e" * int(valuelist[index])
            newBoard += empty
            try:
                rep = next(iterlist).start()
            except StopIteration:
                pass
            index += 1
        else:
            newBoard += v
    newBoard = newBoard.replace('/', '')

    iterlist = re.finditer('[0-9]', nob)
    valuelist = re.findall('[0-9]', nob)
    newNob = ""
    try:
        rep = next(iterlist).start()
    except StopIteration:
        pass
    index = 0
    for i, v in enumerate(nob):
        if int(rep) == i:
            same = nob[i + 1] * (int(valuelist[index]) - 1)
            newNob += same
            try:
                rep = next(iterlist).start()
            except StopIteration:
                pass
            index += 1
        elif v == "-":
            continue
        else:
            newNob += v
    fill = 40 - len(newNob)
    newNob += "e" * fill

    for i in pieceNames:
        newBoard = newBoard.replace(i[0], i[1])
        newNob = newNob.replace(i[0], i[1])

    result: List[float] = []

    for i in newBoard:
        binary = []
        for j in pieceNames:
            if j[1] == i:
                binary = j[2]
        result.extend(binary)
    for i in newNob:
        binary = []
        for j in pieceNames:
            if j[1] == i:
                binary = j[2]
        result.extend(binary)
    
    if turn == "b":
        result.append(1)
    else:
        result.append(0)

    binary = f"{int(turnNumber):0=9b}"
    for i in binary:
        result.append(float(i))

    return np.array(result)

def convertForMove(move: int) -> np.ndarray:
    move_str = f"{move:032b}"
    result: List[float] = []
    for i in move_str:
        result.append(float(i))
    return np.array(result)