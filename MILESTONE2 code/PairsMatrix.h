//
// Created by gil on 21/1/20.
//

#ifndef MILESTONE2_PAIRSMATRIX_H
#define MILESTONE2_PAIRSMATRIX_H
//#define t pair<int, int>

#include "Searchable.h"
#include <vector>

class PairsMatrix : public Searchable<pair<int, int>> {
    vector<State<pair<int, int>>*> allStates;
    State<pair<int, int>>* initialState;
    State<pair<int, int>>* goalState;
    map <State<pair<int, int>>*, vector<State<pair<int, int>>*>> stateAndValidNeighborsMap;
public:

    PairsMatrix(const vector<State<pair<int, int>> *> &allStates,State<pair<int, int>> *initialState,State<pair<int, int>> *goalState,const map<State<pair<int, int>> *, vector<State<pair<int, int>> *>> &stateAndValidNeighborsMap);
    vector<State<pair<int, int>> *> getAllPossibleStates(State<pair<int, int>> *state) override;
    State<pair<int, int>> *getInitialState() override;
    State<pair<int, int>> *getGoalState() override;
    ~PairsMatrix() {
        for (auto& it : allStates) {
            delete it;
        }
    }
};


#endif //MILESTONE2_PAIRSMATRIX_H
