//
// Created by gil on 21/1/20.
//


#include "PairsMatrix.h"

PairsMatrix::PairsMatrix(const vector<State<pair<int, int>> *> &allStates,State<pair<int, int>> *initialState,State<pair<int, int>> *goalState,const map<State<pair<int, int>> *, vector<State<pair<int, int>> *>> &stateAndValidNeighborsMap): allStates(allStates), initialState(initialState),goalState(goalState),stateAndValidNeighborsMap(stateAndValidNeighborsMap) {}

State<pair<int, int>> *PairsMatrix::getInitialState() {
    return initialState;
}

State<pair<int, int>> *PairsMatrix::getGoalState() {
    return goalState;
}

vector<State<pair<int, int>> *>
PairsMatrix::getAllPossibleStates(State<pair<int, int>> *state) {
    return stateAndValidNeighborsMap.at(state);
}
