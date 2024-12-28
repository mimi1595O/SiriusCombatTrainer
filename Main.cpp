#include <iostream>
#include <cstdlib> // For random number generation
#include <ctime>   // For time function (seeds random generator)
#include <chrono>  // For time-related functionality
#include <thread>  // For sleep functionality
#include <limits>  // For clearing input buffer

using namespace std;

class Player {
public:
    int plasmacharge;
    int health;
    const int maxHealth = 100;

    Player() : plasmacharge(12), health(100) {}
};

class Enemy {
public:
    int health;
    int attack;

    void randomStats() {
        health = rand() % 70 + 20;  // Random health between 20 and 90
        attack = rand() % 5 + 10;   // Random attack between 10 and 15
    }

    bool isAlive() {
        return health > 0;  // Check if enemy is still alive
    }
};

void combat(Player &sirius, Enemy &enemy, int &enemyCounter, int &turnCounter) {
    if (!enemy.isAlive()) {
        cout << "No enemy to fight or the enemy is already defeated.\n";
        return;
    }

    turnCounter++;
    cout << "\nTurn " << turnCounter << " begins!\n";
    cout << "Combat started!\n";
    cout << "Enemy Stats: Health = " << enemy.health << ", Attack = " << enemy.attack << "\n";
    cout << "Plasma charge: " << sirius.plasmacharge << "\n";
    cout.flush(); // Ensure immediate output

    // Display available attack options
    cout << "\nChoose your action:\n";
    cout << "1. Gun Fire (Damage: 30, Free)\n";
    if (sirius.plasmacharge >= 3) cout << "2. Plasma Beam (Damage: 50, Consumes 3 Plasma Charges)\n";
    if (sirius.plasmacharge >= 4) cout << "3. Ion Charge (Damage: 120, Consumes 4 Plasma Charges)\n";
    if (sirius.plasmacharge >= 10) cout << "4. Railgun Ray (Damage: 300, Consumes 10 Plasma Charges)\n";
    cout << "5. Skill Charge (Increase Plasma Charge by 4)\n";
    cout << "6. Armor Regeneration (Restore 30% health or consume 3 Plasma Charges if health is full)\n";

    int choice;
    cin >> choice;

    // Clear input buffer in case of invalid input
    if(cin.fail()) {
        cin.clear();  // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore the invalid input
        cout << "Invalid input. Please enter a valid action number.\n";
        return;  // Exit the function to retry input
    }

    int damage = 0;

    switch (choice) {
        case 1:
            damage = 30;
            cout << "Sirius uses Gun Fire!\n";
            break;

        case 2:
            if (sirius.plasmacharge >= 3) {
                damage = 50;
                sirius.plasmacharge -= 3;
                cout << "Sirius uses Plasma Beam!\n";
            } else {
                cout << "Not enough plasma charge for Plasma Beam.\n";
                return;  // Skip turn
            }
            break;

        case 3:
            if (sirius.plasmacharge >= 4) {
                damage = 120;
                sirius.plasmacharge -= 4;
                cout << "Sirius uses Ion Charge!\n";
            } else {
                cout << "Not enough plasma charge for Ion Charge.\n";
                return;  // Skip turn
            }
            break;

        case 4:
            if (sirius.plasmacharge >= 10) {
                damage = 300;
                sirius.plasmacharge -= 10;
                cout << "Sirius uses Railgun Ray!\n";
            } else {
                cout << "Not enough plasma charge for Railgun Ray.\n";
                return;  // Skip turn
            }
            break;

        case 5:
            sirius.plasmacharge += 4;
            cout << "Sirius uses Skill Charge. Plasma charge increased by 4.\n";
            cout << "Total Plasma Charge: " << sirius.plasmacharge << "\n";
            break;

        case 6:
            if (sirius.health < sirius.maxHealth) {
                int regen = 45;
                sirius.health = min(sirius.health + regen, sirius.maxHealth);
                cout << "Sirius regenerates health by 30%. Current health: " << sirius.health << "\n";
            } else if (sirius.plasmacharge >= 3) {
                sirius.plasmacharge -= 3;
                cout << "Sirius uses Plasma-Charged Armor Regeneration. Health remains at 100%.\n";
            } else {
                cout << "Not enough plasma charge for Armor Regeneration.\n";
                return;  // Skip turn
            }
            break;

        default:
            cout << "Invalid input.\n";
            return;  // Skip turn if invalid input
    }

    // Deal damage to the enemy
    cout << "Dealing " << damage << " damage to the enemy.\n";
    enemy.health -= damage;

    if (enemy.health <= 0) {
        enemy.health = 0;
        enemyCounter++;
        cout << "Enemy defeated! Total enemies defeated: " << enemyCounter << "\n";
        sirius.plasmacharge += rand() % 3 + 1; // Gain 1 to 3 plasma charges
        cout << "Sirius gains plasma charges. Total plasma charge: " << sirius.plasmacharge << "\n";
        return;
    }

    // Wait for a short moment
    cout << "Enemy health after Sirius' attack: " << enemy.health << "\n";
    cout << "Processing...\n";
    cout.flush();
    this_thread::sleep_for(chrono::seconds(1));

    // Enemy attacks back
    cout << "Enemy attacks! Dealing " << enemy.attack << " damage to Sirius.\n";
    sirius.health -= enemy.attack;

    if (sirius.health <= 0) {
        sirius.health = 0;
        cout << "Sirius has taken heavy damage. Retreat!\n";
        return;
    }

    cout << "Sirius' health: " << sirius.health << "\n";
}

int main() {
    // Initialize player (Sirius)
    Player sirius;
    int enemyCounter = 0;
    int turnCounter = 0;

    // Seed the random number generator
    srand(time(0));

    cout << "Welcome to Sirius Drone Combat Simulator.\n";
    cout << "Condition: " << sirius.health << "%\n";
    cout << "Plasma charge: " << sirius.plasmacharge << "\n";

    // Game loop
    Enemy enemy;
    enemy.randomStats(); // Initialize the first enemy

    while (true) {
        if (!enemy.isAlive()) {
            cout << "Generating a new enemy...\n";
            enemy.randomStats();
        }

        combat(sirius, enemy, enemyCounter, turnCounter);

        if (sirius.health <= 0) {
            cout << "Game Over\n";
            break;
        }

        // Wait before continuing to the next turn
        cout << "Preparing for the next turn...\n";
        this_thread::sleep_for(chrono::seconds(3));
    }

    cout << "Final Stats:\n";
    cout << "Total turns: " << turnCounter << "\n";
    cout << "Total enemies defeated: " << enemyCounter << "\n";

    return 0;
}
