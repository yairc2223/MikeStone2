//
// Created by gil on 21/1/20.
//

#define DELIM "@"

#include <thread>
#include "MatrixBuilder.h"
#include "FileCacheManager.h"

#define T pair<int, int>

PairsMatrix* MatrixBuilder::createMatrix(vector<string> matrixofnums) {
    vector<State<T>*> states;
    map<T, State<T>*> mapopenstates;
    map<State<T>*, vector<State<T>*>> validStatesmap;
    int numofRows = 0;
    for (int i = 0; i < matrixofnums.size(); ++i) {
        if (matrixofnums[i] == DELIM) {
            ++numofRows;
        }
    }
    numofRows -= 2;
    int numofColums = 0;
    int index2 = 0;
    while (matrixofnums[index2] != DELIM) {
        ++numofColums;
        ++index2;
    }
    vector<string> matrixofnumsCopy = matrixofnums;
    for (int i3 = 0; i3 < matrixofnumsCopy.size(); ++i3) {
        if (matrixofnumsCopy[i3] == DELIM) {
            matrixofnumsCopy.erase(matrixofnumsCopy.begin() + i3);
        }
    }
    int marixofints[numofRows][numofColums];
    for (int current_row = 0; current_row < numofRows; ++current_row) {
        for (int current_colum = 0; current_colum < numofColums; ++current_colum) {
            marixofints[current_row][current_colum] = stoi(matrixofnumsCopy[((current_row * numofColums) + current_colum)]);
        }
    }

    cout <<endl;
    for (int current_row = 0; current_row < numofRows; ++current_row) {
        for (int current_colum = 0; current_colum < numofColums; ++current_colum) {
            cout<< marixofints[current_row][current_colum] <<" ";
        }
        cout <<endl;
    }
    for (int current_row = 0; current_row < numofRows; ++current_row) {
        for (int current_colum = 0; current_colum < numofColums; ++current_colum) {
            T curPos = T(current_row, current_colum);
            State<T>* stateone = new State<T>(curPos);
            stateone->setCost(marixofints[current_row][current_colum]);
            states.push_back(stateone);
            mapopenstates[curPos] = stateone;
        }
    }
    for (int current_row = 0; current_row < numofRows; ++current_row) {
        for (int current_colum = 0; current_colum < numofColums; ++current_colum) {
            T curPos = T(current_row, current_colum);
            State<T>* curState = mapopenstates.at(curPos);
            T leftPos = T(current_row, current_colum-1);
            T rightPos = T(current_row, current_colum+1);
            T upPos = T(current_row-1, current_colum);
            T downPos = T(current_row+1, current_colum);
            vector<T> positions = {rightPos, downPos, leftPos, upPos};
            vector<State<T>*> possibleNeighbors;
            for (int i = 0; i < positions.size(); ++i) {
                if (mapopenstates.count(positions[i]) && (mapopenstates.at(positions[i])->getCost() != -1)) {
                    possibleNeighbors.push_back(mapopenstates.at(positions[i]));
                }
            }
            validStatesmap[curState] = possibleNeighbors;
        }
    }
    int numOfMatrixLineSeparators = numofRows;
    int afterMatrixPos = numofRows*numofColums + numOfMatrixLineSeparators;
    int startJ = stoi(matrixofnums[afterMatrixPos + 1]);
    int startI = stoi(matrixofnums[afterMatrixPos]);
    int goalI = stoi(matrixofnums[afterMatrixPos + 3]);
    int goalJ = stoi(matrixofnums[afterMatrixPos + 4]);
    pair<int, int> startPos = pair<int, int>(startI, startJ);
    pair<int, int> goalPos = pair<int, int>(goalI, goalJ);
    State<T>* startState = mapopenstates.at(startPos);
    State<T>* goalState = mapopenstates.at(goalPos);
    PairsMatrix *resultMatrix;
    resultMatrix = new PairsMatrix(states, startState, goalState, validStatesmap);
    return resultMatrix;
}
