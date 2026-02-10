/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2026
* Date: 27.01.2026
*/

//The library here is concretely set, students are not allowed to include any other libraries.

#include "water_seven.h"

using namespace std;

char character[FIXED_CHARACTER][MAX_NAME];
int  hp[FIXED_CHARACTER];
int  skill[FIXED_CHARACTER];
int  shipHP;
int  repairCost;
int  conflictIndex;
char duel[FIXED_CHARACTER][MAX_NAME];
char cipherText[MAX_NAME];
char resultText[MAX_NAME];
int  dangerLimit;
bool route;
int  rows, cols;
int  grid[MAX_GRID][MAX_GRID];

int main(int argc, const char *argv[]) {
    int n = 6;
    for (int i = n; i <= n; i++) {
        for (int i = 0; i < FIXED_CHARACTER; i++) {
            for (int j = 0; j < MAX_NAME; j++) {
                character[i][j] = '\0';
            }
            hp[i]    = 0;
            skill[i] = 0;
        }
        shipHP        = 0;
        repairCost    = 0;
        conflictIndex = 0;
        for (int i = 0; i < FIXED_CHARACTER; i++) {
            for (int j = 0; j < MAX_NAME; j++) {
                duel[i][j] = '\0';
            }
        }
        for (int i = 0; i < MAX_NAME; i++) {
            cipherText[i] = '\0';
            resultText[i] = '\0';
        }
        dangerLimit = 0;
        route       = 0;
        rows        = 0;
        cols        = 0;
        for (int i = 0; i < MAX_GRID; i++) {
            for (int j = 0; j < MAX_GRID; j++) {
                grid[i][j] = 0;
            }
        }
        cout << "Test " << (i < 10 ? "0" : "") + to_string(i) << ":\n";
        // cout << readInput("opw_tc_" + string(i < 10 ? "0" : "") + to_string(i) + "_input", character, hp, skill, shipHP, repairCost) << "\n";
        // for (int i = 0; i < 7; i++) {
        //     cout << character[i] << " " << hp[i] << " " << skill[i] << "\n";
        // }
        // cout << "GOING_MERRY " << shipHP << " " << repairCost << "\n";
        // cout << (repairCost = damageEvaluation(shipHP, repairCost)) << "\n";
        // cout << (conflictIndex = conflictSimulation(character, hp, skill, shipHP, repairCost)) << "\n";
        // resolveDuel(character, hp, skill, conflictIndex, repairCost, duel);
        // for (int i = 0; i < 7; i++) {
        //     if (duel[i][0] != '\0') {
        //         cout << duel[i] << " ";
        //     }
        // }
        // cout << "\n";
        char cipherText[] = "nPLCSLfXesw 8IV lI#77";
        conflictIndex     = 227;
        repairCost        = 1898;
        decodeCP9Message(character, hp, skill, conflictIndex, repairCost, cipherText, resultText);
        cout << resultText << "\n";
        // cout << (dangerLimit = analyzeDangerLimit(grid, rows, cols)) << "\n";
        // cout << (route = evaluateRoute(grid, rows, cols, dangerLimit)) << "\n";
        cout << "\n";
    }
    return 0;
}
