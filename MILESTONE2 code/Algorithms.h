//
// Created by gil on 01/02/2020.
//

#ifndef MILESTONE2_ALGORITHMS_H
#define MILESTONE2_ALGORITHMS_H
#include "Searcher.h"
#include <set>
#include "State.h"
#include <algorithm>
#include <unordered_set>
#include <math.h>
#include "Searcher.h"
#include <set>
#include "State.h"
#include <algorithm>
#include "State.h"
#include "Searcher.h"
#include <set>
#include "State.h"
#include <algorithm>
#include <unordered_set>
#include "Searcher.h"
#include <unordered_set>
#include "State.h"
#include "Searcher.h"

template<class T, class Solution>
class Astar : public Searcher<T, Solution> {

    unordered_set<State<T> *> is_close;

public:

    Solution search(Searchable<T> *searchable) override {
        typename unordered_set<State<T>*>::iterator itclose;
        vector<State<T>*> possiblestates;
        static State<T>* goalState = searchable->getGoalState();
        vector<State<T>*> pathresult;
        State<T>* tstate;
        class LessThanOfStateCostPlusDist
        {
        public:
            bool operator()(State<T>* a, State<T>* b)
            {
                double ra = h(a, goalState, distFunction);
                double rb = h(b, goalState, distFunction);
                return (a->getTotalCost() + ra) > (b->getTotalCost() + rb);
            }
        };

        priority_queue<State<T>*, vector<State<T>*>, LessThanOfStateCostPlusDist> listOfOpen;
        priority_queue<State<T>*, vector<State<T>*>, LessThanOfStateCostPlusDist> listOfOpenCopy;
        listOfOpen.push(searchable->getInitialState());
        while (!listOfOpen.empty()) {
            tstate = listOfOpen.top();
            listOfOpen.pop();
            this->nodescounter++;
            is_close.insert(tstate);
            if (tstate == goalState) {
                string strPath;
                pathresult.push_back(tstate);
                while (tstate->getCameFrom() != nullptr) {
                    State<T>* temp = tstate->getCameFrom();
                    pathresult.push_back(temp);
                    tstate = temp;
                }
                reverse(pathresult.begin(), pathresult.end());
                int pathresultSize = (int) pathresult.size();
                for (int a = 0; a < pathresultSize; ++a) {
                    strPath += pathresult[a]->findDirection(pathresult[a], pathresult[a]->directionOfPairs);
                    if ((a != (pathresultSize -1)) && (a != 0)) {
                        strPath += ", ";
                    }
                }
                return strPath;
            }

            possiblestates = searchable->getAllPossibleStates(tstate);
            if (possiblestates.empty()) {
                continue;
            }

            int possibleStatesSize = (int) possiblestates.size();
            for (int l = 0; l< possibleStatesSize; ++l) {
                State<T>* sstate = possiblestates[l];


                itclose = is_close.find(sstate);
                if (itclose != is_close.end()) {
                    continue;
                }
                listOfOpenCopy = listOfOpen;
                bool foundInOpen = false;
                while (!listOfOpenCopy.empty()) {
                    State<T>* temp = listOfOpenCopy.top();
                    listOfOpenCopy.pop();
                    if (temp == sstate) {
                        foundInOpen = true;
                    }
                }
                if ((itclose == is_close.end()) && (!foundInOpen)) {

                    sstate->setCameFrom(tstate);
                    listOfOpen.push(sstate);
                } else {
                    if ((tstate->getTotalCost() + sstate->getCost()) < sstate->getTotalCost()) {
                        if (!foundInOpen) {
                            listOfOpen.push(sstate);
                        } else {
                            sstate->setCameFrom(tstate);
                            double newTotalCost = (tstate->getTotalCost() + sstate->getCost());
                            sstate->setTotalCost(newTotalCost);
                            State<T>* temp = sstate;
                            vector<State<T>*> tempQ;
                            int i=0;
                            State<T>* state;
                            int openSize = (int) listOfOpen.size();
                            while(i < openSize)
                            {
                                if (listOfOpen.top() == sstate) {
                                    break;
                                }
                                tempQ.push_back(listOfOpen.top());
                                listOfOpen.pop();
                                ++i;
                            }
                            listOfOpen.pop();
                            int tempQsize = (int) tempQ.size();
                            for (int m = 0;  m < tempQsize; ++m) {
                                listOfOpen.push(tempQ[m]);
                            }
                            listOfOpen.push(temp);
                        }
                    }
                }
            }
        }
        State<T> *initialState = searchable->getInitialState();
        cout << "Total cost Astar: " << initialState->getTotalCost() << endl;
        cout << "Number Of Nodes: " << this->nodescounter << endl;

    }

    static double distFunction(State<pair<int, int>> *s, State<pair<int, int>> *goal) {
        int sX = s->getState().first;
        int sY = s->getState().second;
        int goalX = goal->getState().first;
        int goalY = goal->getState().second;
        double distanceX = pow(sX - goalX, 2);
        double distanceY = pow(sY - goalY, 2);
        double gdistance = sqrt(distanceX + distanceY);
        return gdistance;
    }
    template<class function>
    static double h(State<T> *s, State<T> *goal, function distFunction) {
        double dist = distFunction(s, goal);
        return dist;
    }
};





template <class T, class Solution>
class BestFirstSearch : public Searcher<T, Solution> {

    unordered_set<State<T>*> close;

public:

    Solution search(Searchable<T>* searchable) override {
        typename unordered_set<State<T>*>::iterator itclose;
        string pathstr;
        State<T>* goalState = searchable->getGoalState();
        State<T>* tstate;
        vector<State<T>*> possiblestates;
        vector<State<T>*> pathresult;
        this->openList.push(searchable->getInitialState());
        while (!this->openList.empty()) {
            tstate = this->popOpenList();
            close.insert(tstate);
            if (tstate == goalState) {
                pathresult.push_back(tstate);
                while (tstate->getCameFrom() != nullptr) {
                    State<T>* temp = tstate->getCameFrom();
                    pathresult.push_back(temp);
                    tstate = temp;
                }
                reverse(pathresult.begin(), pathresult.end());
                int pathresultSize = (int) pathresult.size();
                for (int a = 0; a < pathresultSize; ++a) {
                    pathstr += pathresult[a]->findDirection(pathresult[a], pathresult[a]->directionOfPairs);
                    if ((a != (pathresultSize -1)) && (a != 0)) {
                        pathstr += ", ";
                    }
                }
                return pathstr;
            }

            possiblestates = searchable->getAllPossibleStates(tstate);
            if (possiblestates.empty()) {
                continue;
            }

            int possibleStatesSize = (int) possiblestates.size();
            for (int l = 0; l< possibleStatesSize; ++l) {
                State<T>* s = possiblestates[l];
                itclose = close.find(s);
                if (itclose != close.end()) {
                    continue;
                }
                this->openListCopy = this->openList;
                bool foundInOpen = false;
                while (!this->openListCopy.empty()) {
                    State<T>* temp = this->openListCopy.top();
                    this->openListCopy.pop();
                    if (temp == s) {
                        foundInOpen = true;
                    }
                }
                if ((itclose == close.end()) && (!foundInOpen)) {
                    s->setCameFrom(tstate);
                    this->openList.push(s);
                } else {
                    if ((tstate->getTotalCost() + s->getCost()) < s->getTotalCost()) {

                        if (!foundInOpen) {
                            this->openList.push(s);
                        } else {
                            s->setCameFrom(tstate);
                            double newTotalCost = (tstate->getTotalCost() + s->getCost());
                            s->setTotalCost(newTotalCost);
                            State<T>* temp = s;
                            vector<State<T>*> tempQ;
                            int i=0;
                            State<T>* state;
                            int openSize = (int) this->openList.size();
                            while(i < openSize)
                            {
                                if (this->openList.top() == s) {
                                    break;
                                }
                                tempQ.push_back(this->openList.top());
                                this->openList.pop();
                                ++i;
                            }
                            this->openList.pop();
                            int tempQsize = (int) tempQ.size();
                            for (int m = 0;  m < tempQsize; ++m) {
                                this->openList.push(tempQ[m]);
                            }
                            this->openList.push(temp);
                        }
                    }
                }
            }
        }
    }
};



template <class T, class Solution>
class BFS : public Searcher<T, Solution> {

public:
    int getNumberOfNodesEvaluated() override {
        return this->nodescounter;
    }

    Solution search(Searchable<T> *searchable) override {
        State<T>* goalState = searchable->getGoalState();
        State<T>* initialState = searchable->getInitialState();
        list<State<T>*> queue;
        initialState->setVisited(true);
        queue.push_back(initialState);
        while(!queue.empty())
        {
            State<T>* s = queue.front();
            cout << s->getCost() << " ";
            queue.pop_front();
            ++this->nodescounter;
            if (s == goalState) {
                cout<<endl<<"total cost BFS: " << s->getTotalCost() << endl;
                cout<<"numOfNodes: "<<getNumberOfNodesEvaluated()<<endl;
                vector<State<T> *> resPath;
                string strPath;
                State<T> *n = s;
                resPath.push_back(n);
                while (n->getCameFrom() != nullptr) {
                    State<T> *temp = n->getCameFrom();
                    resPath.push_back(temp);
                    n = temp;
                }
                reverse(resPath.begin(), resPath.end());
                int resPathSize = (int) resPath.size();
                for (int a = 0; a < resPathSize; ++a) {
                    strPath += resPath[a]->findDirection(resPath[a],resPath[a]->directionOfPairs);
                    if ((a != (resPathSize - 1)) && (a != 0)) {
                        strPath += ", ";
                    }
                }
                return strPath;
            }
            vector<State<T>*> curPossibleStates;
            curPossibleStates = searchable->getAllPossibleStates(s);
            int possibleStatesSize = (int) curPossibleStates.size();
            for (int i = 0; i < possibleStatesSize; ++i) {
                if (!(curPossibleStates[i]->isVisited())) {
                    curPossibleStates[i]->setCameFrom(s);
                    curPossibleStates[i]->setVisited(true);
                    queue.push_back(curPossibleStates[i]);
                }
            }
        }



        return "end of BFS";
    }
};




#include "State.h"
#include "Searcher.h"

template<class T, class Solution>
class DFS : public Searcher<T, Solution> {

public:
    int getNumberOfNodesEvaluated() override {
        return this->nodescounter;
    }

    Solution search(Searchable<T> *searchable) override {
        State<T> *goalState = searchable->getGoalState();
        State<T> *initialState = searchable->getInitialState();
        string strPath = DFSUtil(initialState, goalState, searchable);

        return strPath;
    }

    string DFSUtil(State<T> *initialState, State<T> *goalState,Searchable<T> *searchable) {
        initialState->setVisited(true);
        ++this->nodescounter;
        if (initialState == goalState) {
            cout << initialState->getCost() << endl;
            cout << "Total cost of DFS: " << initialState->getTotalCost() << endl;
            cout << "Number Of Nodes: " << getNumberOfNodesEvaluated() << endl;
            vector<State<T> *> resPath;
            string strPath;
            State<T> *n = initialState;
            resPath.push_back(n);
            while (n->getCameFrom() != nullptr) {
                State<T> *temp = n->getCameFrom();
                resPath.push_back(temp);
                n = temp;
            }
            reverse(resPath.begin(), resPath.end());
            int resPathSize = (int) resPath.size();
            for (int a = 0; a < resPathSize; ++a) {
                strPath += resPath[a]->findDirection(resPath[a],
                                                     resPath[a]->directionOfPairs);
                if ((a != (resPathSize - 1)) && (a != 0)) {
                    strPath += ", ";
                }
            }
            return strPath;
        }
        cout << initialState->getCost() << " ";
        vector<State<T> *> curPossibleStates;
        curPossibleStates = searchable->getAllPossibleStates(initialState);
        int possibleStatesSize = (int) curPossibleStates.size();
        for (int i = 0; i < possibleStatesSize; ++i) {
            if (!(curPossibleStates[i]->isVisited())) {
                curPossibleStates[i]->setCameFrom(initialState);
                return DFSUtil(curPossibleStates[i], goalState, searchable);
            }
        }
    }

};

#endif //MILESTONE2_ALGORITHMS_H
