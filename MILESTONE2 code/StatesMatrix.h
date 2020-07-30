//
// Created by gil on 21/1/20.
//

#ifndef MILESTONE2_STATESMATRIX_H
#define MILESTONE2_STATESMATRIX_H

#include "State.h"
#include "Searchable.h"
#include <map>
#include <list>
using  namespace std;
template <class T>
class StatesMatrix : public Searchable<T> {
    State<T>* initialState;
    State<T>* goalState;
    struct cmpByState {
        bool operator()(State<T>* a, State<T>* b) {
            return a->getState() < b->getState();
        }
    };
    typedef map<State<T>*, list<State<T>*>, cmpByState> state_map;
    state_map statemap;

public:
    StatesMatrix(list<State<T> *> &alls, State<T> *initialStatet,State<T> *goalStatet,state_map& map) {
        initialState = initialStatet;
        goalState = goalStatet;
        statemap = map;
    }

    virtual State<T>* getInitialState() {
        return initialState;
    }
    virtual State<T>* getGoalState() {
        return goalState;
    }
    virtual list<State<T>*> getAllPossibleStates(State<T>* state) {
        if (statemap.count(state)) {
            list<State<T>*> result = statemap[state];
            return result;
        } else {
            return list<State<T>*>();
        }
    }
};


#endif //MILESTONE2_STATESMATRIX_H
