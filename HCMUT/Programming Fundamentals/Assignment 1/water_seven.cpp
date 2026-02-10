#include "water_seven.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////
//Task 0
bool readInput(const string &filename, char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER], int &shipHP, int &repairCost) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        return 0;
    }
    for (int i = 0; i < FIXED_CHARACTER; i++) {
        for (int j = 0; j < MAX_NAME; j++) {
            character[i][j] = '\0';
        }
        hp[i]    = 0;
        skill[i] = 0;
    }
    shipHP         = 0;
    repairCost     = 0;
    string names[] = {"LUFFY", "ZORO", "SANJI", "NAMI", "CHOPPER", "USOPP", "ROBIN", "GOING_MERRY"};
    int    id      = -1;
    string name;
    int    x, y;
    while (fin >> name >> x >> y) {
        string tmp;
        for (char c : name) {
            if (c != '_') {
                tmp += c;
            }
        }
        bool is_character = 0;
        for (string nm : names) {
            if (tmp == nm) {
                is_character = 1;
                break;
            }
        }
        if (is_character) {
            name    = tmp;
            int pos = -1;
            for (int i = 0; i < 7; i++) {
                if (strcmp(character[i], name.c_str()) == 0) {
                    pos = i;
                    break;
                }
            }
            if (pos == -1) {
                pos = ++id;
            }
            strcpy(character[pos], name.c_str());
            hp[pos]    = min(1000, max(0, x));
            skill[pos] = min(100, max(0, y));
        } else if (name == "GOING_MERRY") {
            shipHP     = min(1000, max(0, x));
            repairCost = min(3000, max(0, y));
        } else {
            return 0;
        }
    }
    return 1;
}

// Task 1
int damageEvaluation(int shipHP, int repairCost) {
    if (shipHP >= 455) {
        return repairCost;
    }
    int n = 0;
    while (shipHP) {
        n += shipHP % 10;
        shipHP /= 10;
    }
    int sum = 0;
    for (int i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            sum += i;
            if (n / i != i) {
                sum += n / i;
            }
        }
    }
    sum -= n;
    if (n != 0 && sum == n) {
        return min(3000, max(0, int(ceil(1.5 * repairCost))));
    }
    return repairCost;
}

// Task 2
int conflictSimulation(char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER], int shipHP, int repairCost) {
    char *names[]  = {"LUFFY", "USOPP"};
    int   id_LUFFY = -1, id_USOPP = -1;
    for (int i = 0; i < 7; i++) {
        if (strcmp(character[i], names[0]) == 0) {
            id_LUFFY = i;
        }
        if (strcmp(character[i], names[1]) == 0) {
            id_USOPP = i;
            break;
        }
    }
    if (id_LUFFY == -1 || id_USOPP == -1) {
        return 0;
    }
    int conflictIndex = ceil(skill[id_LUFFY] - skill[id_USOPP] + repairCost / 100.0 + (500.0 - shipHP) / 50.0);
    int val[6]        = {255, 20, 50, 70, 90, 100};
    for (int i = 1; conflictIndex < 255 && i <= 10; i++) {
        int id = ((conflictIndex % 6) + 6) % 6;
        conflictIndex += val[id];
    }
    return conflictIndex;
}

// Task 3
void resolveDuel(char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER], int conflictIndex, int repairCost, char duel[FIXED_CHARACTER][MAX_NAME]) {
    //TODO: Output assign to duel parameter
    for (int i = 0; i < FIXED_CHARACTER; i++) {
        for (int j = 0; j < MAX_NAME; j++) {
            duel[i][j] = '\0';
        }
    }
    char *names[]  = {"LUFFY", "USOPP"};
    int   id_LUFFY = -1, id_USOPP = -1;
    for (int i = 0; i < 7; i++) {
        if (strcmp(character[i], names[0]) == 0) {
            id_LUFFY = i;
        }
        if (strcmp(character[i], names[1]) == 0) {
            id_USOPP = i;
        }
    }
    if (id_LUFFY == -1 || id_USOPP == -1) {
        return;
    }
    int ids[5];
    for (int i = 0, id = -1; i < 7; i++) {
        if (i != id_LUFFY && i != id_USOPP) {
            ids[++id] = i;
        }
    }
    int U = ceil(skill[id_USOPP] + conflictIndex / 20.0 + repairCost / 500.0);
    int n = 0;
    for (int i = 0; i < 7; i++) {
        if (character[i][0] != '\0') {
            n++;
        } else {
            break;
        }
    }
    int  res_cost    = INT_MAX;
    int  res_support = (1 << (n - 2)) - 1;
    bool ok          = 0;
    for (int i = 0; i < (1 << (n - 2)); i++) {
        int support = 0, cost = 0;
        for (int j = 0; j < n - 2; j++) {
            if (i >> j & 1) {
                support += skill[ids[j]];
                cost += (hp[ids[j]] % 10) + 1;
            }
        }
        if (skill[id_LUFFY] + support >= U) {
            ok = 1;
            if (cost < res_cost) {
                res_cost    = cost;
                res_support = i;
            } else if (cost == res_cost) {
                int cnt1 = 0, cnt2 = 0;
                for (int j = 0; j < 5; j++) {
                    if (i >> j & 1) {
                        cnt1++;
                    }
                }
                for (int j = 0; j < 5; j++) {
                    if (res_support >> j & 1) {
                        cnt2++;
                    }
                }
                if (cnt1 < cnt2) {
                    res_support = i;
                }
            }
        }
    }
    if (ok) {
        for (int i = 0, id = -1; i < 5; i++) {
            if (res_support >> i & 1) {
                strcpy(duel[++id], character[ids[i]]);
            }
        }
    }
}

// Task 4
void decodeCP9Message(char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER], int conflictIndex, int repairCost, char cipherText[], char resultText[]) {
    //TODO: Output assign to resultText parameter
    for (int i = 0; i < MAX_NAME; i++) {
        resultText[i] = '\0';
    }
    int XY = 0;
    int p  = -1;
    for (int i = strlen(cipherText) - 1; i >= 0; i--) {
        if (cipherText[i] == '#') {
            p = i;
            break;
        }
    }
    if (p != -1 && p + 2 == int(strlen(cipherText)) - 1 && '0' <= cipherText[p + 1] && cipherText[p + 1] <= '9' && '0' <= cipherText[p + 2] && cipherText[p + 2] <= '9') {
        XY = (cipherText[p + 1] - '0') * 10 + (cipherText[p + 2] - '0');
    } else {
        return;
    }
    for (int i = p; i < int(strlen(cipherText)); i++) {
        cipherText[i] = '\0';
    }
    if (!strlen(cipherText)) {
        return;
    }
    int checksum = 0;
    for (int i = 0; i < int(strlen(cipherText)); i++) {
        checksum += int(cipherText[i]);
    }
    checksum %= 100;
    if (checksum != XY) {
        return;
    }
    int key, B;
    key = (conflictIndex + repairCost) % 26;
    B   = (key % 5) + 4;
    string new_message;
    for (int i = 0; i < int(strlen(cipherText)); i += B) {
        string tmp;
        for (int j = i; j < min(i + B, int(strlen(cipherText))); j++) {
            char c = cipherText[j];
            if ('A' <= c && c <= 'Z') {
                c = 'A' + ((c - 'A' - key) % 26 + 26) % 26;
            } else if ('a' <= c && c <= 'z') {
                c = 'a' + ((c - 'a' - key) % 26 + 26) % 26;
            } else if ('0' <= c && c <= '9') {
                c = '0' + ((c - '0' - key) % 10 + 10) % 10;
            }
            tmp = c + tmp;
        }
        new_message += tmp;
    }
    if (new_message.find("CP9") != -1 || new_message.find("ENIESLOBBY") != -1) {
        new_message += "_TRUE";
    } else {
        new_message += "_FALSE";
    }
    strcpy(resultText, new_message.c_str());
}

// Task 5
int analyzeDangerLimit(int grid[MAX_GRID][MAX_GRID], int rows, int cols) {
    int maxRowSum = INT_MIN / 2, maxCell = INT_MIN / 2;
    for (int i = 0; i < rows; i++) {
        int rowSum = 0;
        for (int j = 0; j < cols; j++) {
            maxCell = max(maxCell, grid[i][j]);
            if (grid[i][j] != -1) {
                rowSum += grid[i][j];
            }
        }
        maxRowSum = max(maxRowSum, rowSum);
    }
    return maxRowSum + maxCell;
}

bool evaluateRoute(int grid[MAX_GRID][MAX_GRID], int rows, int cols, int dangerLimit) {
    int dp[MAX_GRID][MAX_GRID];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dp[i][j] = INT_MAX / 2;
        }
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] != -1) {
                if (!i && !j) {
                    dp[0][0] = grid[0][0];
                } else {
                    dp[i][j] = min(INT_MAX / 2, min(i ? dp[i - 1][j] : INT_MAX / 2, j ? dp[i][j - 1] : INT_MAX / 2) + grid[i][j]);
                }
            }
        }
    }
    return (dp[rows - 1][cols - 1] <= dangerLimit);
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
