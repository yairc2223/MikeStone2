//
// Created by yair on 18/1/20.
//

#ifndef MILESTONE2_STATE_H
#define MILESTONE2_STATE_H

#include <map>
#include <cfloat>
using namespace std;

template <class T>
class State {

    T state;
    double cost;
    double totalCost;
    string direction = "";
    State<T>* cameFrom;
    bool is_visited = false;
public:
    State(T statet) {
        this->state = statet;
        cost = 0;
        totalCost = cost;
        cameFrom = nullptr;
    }

    bool operator==(State<T> statet){
        return (this->state == statet.getState());
    }

    bool operator==(State<T>* statet){
        return (this->state == statet->getState());
    }

    friend bool operator==(State<T>* statet, State<T> statett){
        return (statet->getState() == statett.getState());
    }

    T getState() {
        return state;
    }

    double getCost()  {
        return cost;
    }

    void setCost(double cost) {
        State::cost = cost;
        totalCost = cost;
    }

    State<T>* getCameFrom() {
        return cameFrom;
    }

    void setCameFrom( State<T>* cameFrom) {
        this->cameFrom = cameFrom;
        setTotalCost(cameFrom->getTotalCost() + cost);
    }

    double getTotalCost() {
        return totalCost;
    }

    void setTotalCost(double totalCost) {
        State::totalCost = totalCost;
    }

    bool isVisited() const {
        return is_visited;
    }

    void setVisited(bool visited) {
        is_visited = visited;
    }

    virtual double distanceTo(State<T>* goal) {}

    const std::string &getDirection() const {
        return direction;
    }

    void setDirection(const std::string &direction) {
        State::direction = direction;
    }

    template<class function>
    static string findDirection(State<T> *son, function dirFunction) {
        string direction = dirFunction(son);
        return direction;
    }

    static string directionOfPairs(State<pair<int, int>> *son) {
        if (son->getCameFrom() == nullptr) {
            return "";
        }
        State<pair<int, int>> *father = son->getCameFrom();
        int sx = son->getState().first;
        int sy = son->getState().second;
        int fx = father->getState().first;
        int fy = father->getState().second;
        if (sx == fx) {
            if (fy == sy) {
                son->setDirection(father->getDirection());
                return father->getDirection();
            } else if (fy > sy) {
                son->setDirection("Left");
                return "Left";
            } else if (fy < sy) {
                son->setDirection("Right");
                return "Right";
            }
        } else if (fx < sx) {
            son->setDirection("Down");
            return "Down";
        } else if (fx > sx) {
            son->setDirection("Up");
            return "Up";
        }
    }
};


#endif //MILESTONE2_STATE_H
