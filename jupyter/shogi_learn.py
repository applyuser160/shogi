import shogi_convert as sc
import shogi_node as sn
from sql_setting import *
from sqlalchemy import Row, Select, select
import pandas as pd
from typing import List
from enum import IntEnum
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
import cshogi
from cshogi import Board
import copy
import random
from datetime import datetime, timedelta

class LearnData(IntEnum):
    BOARD = 1
    MOVE = 2
    OUTPUT = 3

class ModelType(IntEnum):
    WIN = 1 # 勝率を導く
    CUT = 2 # モンテカルロ木探索の枝切り

def getLearnData(limit: int = 1000, type: ModelType = ModelType.WIN) -> List[Row]:
    query: Select[Any]
    if type == ModelType.WIN:
        parent = select(sn.Node).subquery("p")
        query = select(
            parent.c.board,
            sn.Node.move,
            (sn.Node.drawCount / sn.Node.throughCount).label("probabilityOfDraw"),
            (sn.Node.firstWinCount / sn.Node.throughCount).label("probabilityOfFirstWin"),
            (sn.Node.secondWinCount / sn.Node.throughCount).label("probabilityOfSecondWin")
        ).join(
            parent,
            sn.Node.parentId == parent.c.ID
        ).limit(limit)
    elif type == ModelType.CUT:
        query = select(
            sn.Node.board,
            (sn.Node.drawCount / sn.Node.throughCount).label("probabilityOfDraw"),
            (sn.Node.firstWinCount / sn.Node.throughCount).label("probabilityOfFirstWin"),
            (sn.Node.secondWinCount / sn.Node.throughCount).label("probabilityOfSecondWin")
        ).order_by(sn.Node.turnNumber).limit(limit)
    results = list(session.execute(query))
    return results

def convertToDataFrame(data: List[Row]) -> pd.DataFrame:
    return pd.DataFrame(data)

def selectionLearnData(data: pd.DataFrame, type: LearnData) -> pd.DataFrame:
    length = len(data)
    if type == LearnData.BOARD:
        nparray = np.zeros((length, 615), dtype= float)
        for index in range(length):
            nparray[index] = sc.convertForBoard(data['board'][index])
        return pd.DataFrame(nparray)
    elif type == LearnData.MOVE:
        nparray = np.zeros((length, 32), dtype= float)
        for index in range(length):
            nparray[index] = sc.convertForMove(data['move'][index])
        return pd.DataFrame(nparray)
    else:
        result = data[[
            'probabilityOfDraw',
            'probabilityOfFirstWin',
            'probabilityOfSecondWin'
        ]]
        result['probabilityOfDraw'] = result['probabilityOfDraw'].astype(float)
        result['probabilityOfFirstWin'] = result['probabilityOfFirstWin'].astype(float)
        result['probabilityOfSecondWin'] = result['probabilityOfSecondWin'].astype(float)
        return result
    
def createModel(type: ModelType = ModelType.WIN) -> tf.keras.Model:
    if type == ModelType.WIN:
        m_input_board = tf.keras.Input(shape=615)
        m_input_move = tf.keras.Input(shape=32)
        m_inputs = tf.keras.layers.concatenate([m_input_board, m_input_move])
        m_hide1 = tf.keras.layers.Dense(615, activation="relu")(m_inputs)
        m_hide2 = tf.keras.layers.Dense(128, activation="relu")(m_hide1)
        m_output = tf.keras.layers.Dense(3, activation="softmax", name="output")(m_hide2)
        model = tf.keras.Model(inputs= [m_input_board, m_input_move], outputs= m_output)
    elif type == ModelType.CUT:
        m_input = tf.keras.Input(shape=615)
        m_hide1 = tf.keras.layers.Dense(32, activation="relu")(m_input)
        m_hide2 = tf.keras.layers.Dense(8, activation="relu")(m_hide1)
        m_output = tf.keras.layers.Dense(3, activation="softmax", name="output")(m_hide2)
        model = tf.keras.Model(inputs= m_input, outputs= m_output)
    return model

def compileModel(model: tf.keras.Model):
    model.compile(loss='mean_squared_error', optimizer='sgd', metrics=['mae', 'acc'])

def inputModel(path: str) -> tf.keras.Model:
    return tf.keras.models.load_model(path)

def learning(model: tf.keras.Model, epochs: int, path: str, input_board: pd.DataFrame, input_move: pd.DataFrame, output: pd.DataFrame):
    history = model.fit([
        input_board,
        input_move
    ], output, epochs=epochs)

    model.save(path)

    return history

def printLearnData(history):
    fig = plt.figure(figsize = (20, 16), facecolor= 'lightblue')
    ax1 = fig.add_subplot(2, 2, 1)
    ax2 = fig.add_subplot(2, 2, 2)

    ax1.plot(history.history['loss'], label='loss')
    ax1.set_xlabel('Epoch')
    ax1.set_ylabel('Loss')
    ax1.legend(loc='upper right')

    ax2.plot(history.history['acc'], label='output_accuracy')
    ax2.plot(history.history['mae'], label='output_mae')
    ax2.set_xlabel('Epoch')
    ax2.set_ylabel('Accuracy')
    ax2.legend(loc='lower right')

def predict(model: tf.keras.Model, input_board: pd.DataFrame, input_move: pd.DataFrame):
    return model.predict([
        input_board,
        input_move
    ], verbose=0)

def autoMatch(first, second, board: Board = cshogi.Board(), isDisplay: bool = True):
    winner: sn.Winner
    remain = 500 - board.move_number
    for i in range(remain):
        if isDisplay:
            print(f"\rturn{board.move_number}", end="")
        selectedMove = second(board) if not board.turn else first(board)
        board.push(selectedMove)
        if board.is_game_over():
            winner = sn.Winner.FIRST if not board.turn else sn.Winner.SECOND
            break
        elif board.move_number == 500:
            winner = sn.Winner.DRAW
            break
    if isDisplay:
        print(board)
    return winner

# ランダムに手を指し続ける
def ramdomMove(board: Board, timefor: timedelta = timedelta(seconds=1)) -> int:
    endtime = datetime.now() + timefor
    moves = list(board.legal_moves)
    boards = []
    for move in moves:
        test_board = copy.copy(board)
        test_board.push(move)
        boards.append(test_board)

    indexs = [i for i in moves]
    results = pd.DataFrame(0.0, columns=['draw', 'first', 'second', 'total', 'winning percentage'], index=[indexs])
    count = 0
    while True:
        selected = random.randint(0, len(moves) - 1)
        copiedBoard = copy.copy(boards[selected])
        result = sn.fight(copiedBoard, False)
        targetColumn = ''
        if result == sn.Winner.DRAW:
            targetColumn = 'draw'
        elif result == sn.Winner.FIRST:
            targetColumn = 'first'
        else:
            targetColumn = 'second'
        results.loc[moves[selected], targetColumn] += 1.0
        results.loc[moves[selected], 'total'] += 1.0
        count += 1
        if datetime.now() > endtime:
            break
    wins = results.loc[:, 'second'] if board.turn else results.loc[:, 'first']
    for index in results.index.values:
        results.loc[index[0], 'winning percentage'] = wins[index[0]] / results.loc[index[0], 'total']
    print(results)
    return results.loc[:,'winning percentage'].idxmax()[0]

def serchBestMove(board: Board, path: str = 'saved_model/20230912') -> int:
    moves = list(board.legal_moves)
    np_input_board = np.zeros((len(moves), 615), dtype= float)
    np_input_move = np.zeros((len(moves), 32), dtype= float)
    for index in range(len(moves)):
        np_input_board[index] = sc.convertForBoard(board.sfen())
        np_input_move[index] = sc.convertForMove(moves[index])
    input_board = pd.DataFrame(np_input_board)
    input_move = pd.DataFrame(np_input_move)
    model = inputModel(path)
    predictions = predict(model, input_board, input_move)
    result = pd.DataFrame(predictions, columns=['draw', 'first', 'second'])
    for index in range(len(result)):
        target = result.loc[:, 'second'] if board.turn else result.loc[:, 'first']
    selected = target.idxmax()
    return moves.loc[selected]

def serchMoveValue(board: Board, layer: int, head: int = 5, model: tf.keras.Model = inputModel('saved_model/cut')):
    moves = list(board.legal_moves)
    if len(moves) == 0:
        return pd.DataFrame(0.0, columns=['draw', 'first', 'second', 'total'], index=[0])
    boards = []
    for move in moves:
        test_board = copy.copy(board)
        test_board.push(move)
        boards.append(test_board)
    input_board = pd.DataFrame([sc.convertForBoard(i.sfen()) for i in boards])
    predictions = model.predict(input_board, verbose=0)
    result = pd.DataFrame(predictions, columns=['draw', 'first', 'second'])
    result['move'] = moves
    result['board'] = boards
    targetColumn = 'second' if board.turn else 'first'
    header = result.sort_values(by=targetColumn, ascending=False).head(head)
    r = pd.DataFrame(0.0, columns=['draw', 'first', 'second', 'total'], index=[0])
    r.loc[0, 'draw'] = np.sum(result.loc[:,'draw'])
    r.loc[0, 'first'] = np.sum(result.loc[:,'first'])
    r.loc[0, 'second'] = np.sum(result.loc[:,'second'])
    r.loc[0, 'total'] = len(moves)
    if layer > 1:
        for b in header.loc[:,'board']:
            r.loc[0] += serchMoveValue(board= b, layer= layer - 1, model= model).loc[0]
    return r

def lightMcts(board: Board, layer: int) -> int:
    moves = list(board.legal_moves)
    boards = []
    for move in moves:
        test_board = copy.copy(board)
        test_board.push(move)
        boards.append(test_board)

    indexs = [i for i in moves]
    results = pd.DataFrame(0.0, columns=['draw', 'first', 'second', 'total', 'winning percentage'], index=[indexs])
    count = 0
    for move, b in zip(moves, boards):
        copiedBoard = copy.copy(b)
        results.loc[move, ['draw', 'first', 'second', 'total']] += serchMoveValue(copiedBoard, layer).loc[0]
    wins = results.loc[:, 'second'] if board.turn else results.loc[:, 'first']
    for index in results.index.values:
        results.loc[index[0], 'winning percentage'] = wins[index[0]] / results.loc[index[0], 'total']
    print(results)
    return results.loc[:,'winning percentage'].idxmax()[0]