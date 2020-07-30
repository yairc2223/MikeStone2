//
// Created by yair on 16/1/20.
//

#include <fstream>
#include <vector>
#include "FileCacheManager.h"

bool FileCacheManager::isThereSolution(string problem) {
    bool result;
    lock_guard<mutex> lock(mutexFileCacheM);
    result = (bool) (SolutionByProblemMap.count(problem));
    return result;
}

string FileCacheManager::getSolution(string problem) {
    string  result;
    lock_guard<mutex> lock(mutexFileCacheM);
     result = SolutionByProblemMap.at(problem);
    return  result;
}

void FileCacheManager::saveSolution(string problem, string solution) {
    lock_guard<mutex> lock(mutexFileCacheM);
    SolutionByProblemMap.insert(pair<string, string> (problem, solution));
    writeSolutionsToFile();
}

void FileCacheManager::writeSolutionsToFile() {
    if (SolutionByProblemMap.empty()) {
        return;
    }
    ofstream  FW("solutions.txt");
    if ( FW.is_open()) {
        for (map<string, string>::iterator it = SolutionByProblemMap.begin();it != SolutionByProblemMap.end(); ++it) {
             FW << (it->first) << ";";
             FW << (it->second) << ";\n";
        }
         FW.close();
    } else cout << "Unable to open file";
}

void FileCacheManager::loadSolutions() {
    string problem, solution;
    vector<string> details = {problem, solution};
    string line;
    ifstream fileReader("solutions.txt");
    string delim = ";";
    string token;
    if (fileReader.is_open()) {
        while (fileReader.good()) {
            getline(fileReader, line);
            if (line.empty()) {
                break;
            }
            int pos = 0;
            int Index = 0;
            while ((pos = line.find(delim)) != string::npos) {
                token = line.substr(0, pos);
                details[Index] = token;
                ++Index;
                line.erase(0, pos + delim.length());
            }
            problem = details[0];
            solution = details[1];
            SolutionByProblemMap[problem] = solution;
        }
        fileReader.close();
    } else {
        return;
    }
}
FileCacheManager::FileCacheManager() {
    lock_guard<mutex> lock(mutexFileCacheM);
    loadSolutions();
}



