//
// Created by yair on 18/1/20.
//
#ifndef MILESTONE2_SEARCHSOLVEROA_H
#define MILESTONE2_SEARCHSOLVEROA_H

#include "Solver.h"
#include "SearcherInterface.h"
template <class Problem, class Solution, class T>
class SearchSolverOA : public Solver<Problem, Solution> {
    SearcherInterface<T, Solution>* searcherinterface;
public:
    SearchSolverOA(SearcherInterface<T, Solution> *searcherinterface) : searcherinterface(searcherinterface) {}

    Solution solve(Problem problem) override {
        return searcherinterface->search(problem);
    }
};


#endif //MILESTONE2_SEARCHSOLVEROA_H
