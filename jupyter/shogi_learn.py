import shogi_convert as sc
import shogi_node as sn
from sql_setting import *
from sqlalchemy import Row, select
import pandas as pd
from typing import List
from enum import IntEnum
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
import cshogi
import random

class LearnData(IntEnum):
    BOARD = 1
    MOVE = 2
    OUTPUT = 3

def getLearnData(limit: int = 1000) -> List[Row]:
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
    
def createModel() -> tf.keras.Model:
    m_input_board = tf.keras.Input(shape=615)
    m_input_move = tf.keras.Input(shape=32)
    m_inputs = tf.keras.layers.concatenate([m_input_board, m_input_move])
    m_hide1 = tf.keras.layers.Dense(615, activation="relu")(m_inputs)
    m_hide2 = tf.keras.layers.Dense(128, activation="relu")(m_hide1)
    m_output = tf.keras.layers.Dense(3, activation="relu", name="output")(m_hide2)
    model = tf.keras.Model(inputs= [m_input_board, m_input_move], outputs= m_output)
    return model

def compileModel(model: tf.keras.Model):
    model.compile(loss='mean_squared_error', optimizer='sgd', metrics=['accuracy'])

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

    ax2.plot(history.history['accuracy'], label='output_accuracy')
    ax2.set_xlabel('Epoch')
    ax2.set_ylabel('Accuracy')
    ax2.legend(loc='lower right')

def predict(model: tf.keras.Model, input_board: pd.DataFrame, input_move: pd.DataFrame):
    return model.predict([
        input_board,
        input_move
    ], verbose=0)

def serchBestMove(board, path: str) -> int:
    moves = list(board.legal_moves)
    np_input_board = np.zeros((len(moves), 615), dtype= float)
    np_input_move = np.zeros((len(moves), 32), dtype= float)
    for index in range(len(moves)):
        np_input_board[index] = sc.convertForBoard(board.sfen())
        np_input_move[index] = sc.convertForMove(moves[index])
    input_board = pd.DataFrame(np_input_board)
    input_move = pd.DataFrame(np_input_move)
    model = inputModel(path)
    predictions = predict(model, input_board, input_move, )
    result = pd.DataFrame(predictions, columns=['draw', 'first', 'second'])
    result['move'] = ''
    result['deviation'] = 0
    for index in range(len(result)):
        target: float = result.loc[index, 'second'] if board.turn else result.loc[index, 'first']
        l = [result.loc[index, 'draw'], result.loc[index, 'first'], result.loc[index, 'second']]
        stdDeviation = np.std(l)
        avg = np.average(l)
        result.loc[index, 'deviation'] = (target - avg) * 10 / stdDeviation
        result.loc[index, 'move'] = cshogi.move_to_usi(moves[index])
    print(result)
    selected = result['deviation'].idxmax()
    return moves[selected]