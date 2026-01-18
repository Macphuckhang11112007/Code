#include <iomanip>
#include <iostream>

using namespace std;

class Env;
class Player;
class Demon;

class Env {
public:
    char timeOfDay;

    Env() { timeOfDay = 'D'; }
};

class Demon {
public:
    int demonPresence;
    int demonRank;
    int bossHP;
    int finalHP;

    Demon() {
        demonPresence = 0;
        demonRank     = 1;
        bossHP        = 100;
        finalHP       = 100;
    }
};

class Player {
public:
    int    slayerLevel;
    int    hp;
    double breathingMastery;
    int    hasTalisman;
    double swordSharpness;
    int    allyCount;
    int    totalDamage;
    int    specialMoveReady;

    Player() {
        slayerLevel      = 1;
        hp               = 100;
        breathingMastery = 0.0;
        hasTalisman      = 0;
        swordSharpness   = 0.0;
        allyCount        = 0;
        totalDamage      = 0;
        specialMoveReady = 0;
        env              = Env();
        demon            = Demon();
        power            = 20;
        adv              = 126;
        rank             = "Novice";
    }

    void setRef(Env &envRef, Demon &demonRef) {
        env   = envRef;
        demon = demonRef;
    }

    void setPower() { power = slayerLevel * 10.0 + hp / 10.0 + breathingMastery * 50.0; }

    void displayScene() {
        for (int sc = 1; sc <= 4; sc++) {
            cout << "[Scene " << sc << "] ";
            if (sc == 1) {
                setRank();
                getRank();
            } else if (sc == 2) {
                canOpenGate();
            } else if (sc == 3) {
                setAdvantage();
                battleAdvantage();
            } else if (sc == 4) {
                setBossFinalHP();
                finalOutcome();
            }
        }
    }

private:
    Env    env;
    Demon  demon;
    double power;
    double adv;
    string rank;

    void setRank() {
        if (power >= 120.0) {
            rank = "Hashira";
        } else if (power >= 80.0) {
            rank = "Elite";
        } else {
            rank = "Novice";
        }
    }

    void setAdvantage() { adv = (101.0 - demon.demonRank * 15.0) + swordSharpness * 0.4 + allyCount * 5.0; }

    void setBossFinalHP() { demon.finalHP = demon.bossHP - totalDamage; }

    void getRank() { cout << fixed << setprecision(1) << "Rank: " << rank << " (power = " << power << ")" << endl; }

    void canOpenGate() {
        if (hasTalisman == 0) {
            cout << "Denied: No talisman." << endl;
        } else if (env.timeOfDay != 'D' && env.timeOfDay != 'N') {
            cout << "Warning: invalid timeOfDay." << endl;
        } else if (env.timeOfDay == 'N' && demon.demonPresence == 1) {
            cout << "Open silently." << endl;
        } else {
            cout << "Open cautiously." << endl;
        }
    }

    void battleAdvantage() {
        if (adv >= 100.0) {
            cout << "Engage head-on" << " ";
        } else if (adv >= 60.0) {
            cout << "Harass and probe" << " ";
        } else {
            cout << "Retreat and regroup" << " ";
        }
        cout << fixed << setprecision(1) << "(adv = " << adv << ")" << endl;
    }

    void finalOutcome() {
        if (demon.finalHP <= 0) {
            cout << "Boss defeated! ";
        } else if (demon.finalHP > 0 && specialMoveReady == 1 && demon.finalHP <= 50) {
            cout << "Use special move to finish! ";
        } else {
            cout << "Withdraw to heal. ";
        }
        cout << fixed << setprecision(1) << "(finalHP = " << max(demon.finalHP, 0) << ")" << endl;
    }
};

int main() {
    Env    env;
    Demon  demon;
    Player player;

    cin >> player.slayerLevel >> player.hp >> player.breathingMastery >> player.hasTalisman >> env.timeOfDay >> demon.demonPresence >> demon.demonRank >> player.swordSharpness >> player.allyCount >> demon.bossHP >> player.totalDamage >> player.specialMoveReady;

    player.setRef(env, demon);
    player.setPower();
    player.displayScene();
}