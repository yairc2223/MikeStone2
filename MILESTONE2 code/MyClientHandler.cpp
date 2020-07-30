//
// Created by yair on 16/1/20.
//
#include "MyClientHandler.h"
/**
 * this function handles the client requests.
 * @param socket This is the file descriptor of the socket.
 */
void MyClientHandler::handleClient(int clientfd) {
    char buffer[1024];
    ssize_t n;
    int clientds = clientfd;
    string problem;
    vector<string> matrixofnums;
    while (true) {
        bzero(buffer, 1024);
        n = read(clientds, buffer, 1023);

        if (n < 0) {
            perror("Couldn't read from socket");
            exit(1);
        }
        string strBuffer = buffer;
        if ((strBuffer == "end\n") || (strBuffer == "end")) {
            break;
        }
        strBuffer += ",";
        vector<string> lexedBuffer = lexer(strBuffer, ',');
        lexedBuffer.push_back(LINEdelim);
        matrixofnums.insert(matrixofnums.end(), lexedBuffer.begin(),lexedBuffer.end());
        lexedBuffer.clear();
    }
    int probLength = (int) matrixofnums.size();
    for (int i = 0; i < probLength; ++i) {
        problem += matrixofnums[i];
    }
    string solution;
    if (cacheManager->isThereSolution(problem)) {
        solution = cacheManager->getSolution(problem);
    } else {
        MatrixBuilder* matrixBuilder = new MatrixBuilder;
        PairsMatrix *matrixOfPairs = matrixBuilder->createMatrix(matrixofnums);
        solution = solver->solve(matrixOfPairs);
        cacheManager->saveSolution(problem, solution);
        delete matrixOfPairs;
        delete matrixBuilder;
        matrixofnums.clear();
    }
    const char *msg = solution.c_str();
    bzero(buffer, 1024);
    strcpy(buffer, msg);
    n = write(clientds, buffer, strlen(buffer));
    if (n < 0) {
        perror("couldn't write to socket");
        exit(1);
    }
    problem.clear();
    solution.clear();
    printf("Done\n");
    return;
}