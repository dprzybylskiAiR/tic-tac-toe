#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Player {
    int index;
    char symbol;
    int wins;
    int draws;
    int losses;
};

vector<Player> readPlayers(const string& filename) {
    vector<Player> players;
    ifstream file(filename);
    int index;
    char symbol;
    while (file >> index >> symbol) {
        players.push_back({ index, symbol, 0, 0, 0 });
    }
    file.close();
    return players;
}

vector<string> readResults(const string& filename) {
    vector<string> results;
    ifstream file(filename);
    string line;
    string match;
    while (getline(file, line)) {
        match += line;
        if (match.size() == 9) {
            results.push_back(match);
            match.clear();
        }
    }
    file.close();
    return results;
}

void updateStats(vector<Player>& players, vector<string>& results) {
    for (const string& match : results) {
        char winner = '.';
        bool draw = true;
        for (size_t i = 0; i < 3; ++i) {
            if (match[i * 3] == match[i * 3 + 1] && match[i * 3 + 1] == match[i * 3 + 2] && match[i * 3] != '.') {
                winner = match[i * 3];
                draw = false;
                break;
            }
            if (match[i] == match[i + 3] && match[i + 3] == match[i + 6] && match[i] != '.') {
                winner = match[i];
                draw = false;
                break;
            }
        }
        if ((match[0] == match[4] && match[4] == match[8] && match[0] != '.') ||
            (match[2] == match[4] && match[4] == match[6] && match[2] != '.')) {
            winner = match[4];
            draw = false;
        }
        for (Player& player : players) {
            if (winner == player.symbol) {
                player.wins++;
            }
            else if (draw && winner == '.') {
                bool playerInvolved = false;
                for (char symbol : match) {
                    if (player.symbol == symbol) {
                        playerInvolved = true;
                        break;
                    }
                }
                if (playerInvolved) {
                    player.draws++;
                }
            }
            else if (winner != '.' && winner != player.symbol) {
                bool playerInvolved = false;
                for (char symbol : match) {
                    if (player.symbol == symbol) {
                        playerInvolved = true;
                        break;
                    }
                }
                if (playerInvolved) {
                    player.losses++;
                }
            }
        }
    }
}


void showTable(const vector<Player>& players) {
    vector<Player> sortedPlayers = players;
    sort(sortedPlayers.begin(), sortedPlayers.end(), [](const Player& a, const Player& b) {
        return (a.wins * 3 + a.draws) > (b.wins * 3 + b.draws);
        });

    cout << "Index\tSymbol\tWins\tDraws\tLosses\tPoints\n";
    for (const Player& player : sortedPlayers) {
        cout << player.index << "\t" << player.symbol << "\t" << player.wins << "\t"
            << player.draws << "\t" << player.losses << "\t" << (player.wins * 3 + player.draws) << "\n";
    }
}

void showResults(const vector<string>& results) {
    cout << "Match Results:\n";
    for (const string& match : results) {
        cout << match.substr(0, 3) << "\n" << match.substr(3, 3) << "\n" << match.substr(6, 3) << "\n";

        char winner = '.';
        bool draw = true;
        for (size_t i = 0; i < 3; ++i) {
            if (match[i * 3] == match[i * 3 + 1] && match[i * 3 + 1] == match[i * 3 + 2] && match[i * 3] != '.') {
                winner = match[i * 3];
                draw = false;
                break;
            }
            if (match[i] == match[i + 3] && match[i + 3] == match[i + 6] && match[i] != '.') {
                winner = match[i];
                draw = false;
                break;
            }
        }
        if ((match[0] == match[4] && match[4] == match[8] && match[0] != '.') ||
            (match[2] == match[4] && match[4] == match[6] && match[2] != '.')) {
            winner = match[4];
            draw = false;
        }
        if (draw && winner == '.') {
            cout << "Draw\n";
        }
        else {
            cout << "Winner: " << winner << "\n";
        }
        cout << endl;
    }
}

void showPlayerStats(const vector<Player>& players, int index) {
    for (const Player& player : players) {
        if (player.index == index) {
            cout << "Player " << player.index << " Stats:\n";
            cout << "Symbol: " << player.symbol << "\n";
            cout << "Wins: " << player.wins << "\n";
            cout << "Draws: " << player.draws << "\n";
            cout << "Losses: " << player.losses << "\n";
            cout << "Points: " << (player.wins * 3 + player.draws) << "\n";
            return;
        }
    }
    cout << "Player with index " << index << " not found.\n";
}

int main() {
    vector<Player> players = readPlayers("players.txt");
    vector<string> results = readResults("tictactoe.txt");

    updateStats(players, results);

    int choice;
    do {
        cout << "1 - Show table\n";
        cout << "2 - Show results\n";
        cout << "3 - Show player stats\n";
        cout << "4 - Exit\n";
        cout << "Choose option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            showTable(players);
            break;
        case 2:
            showResults(results);
            break;
        case 3:
            int index;
            cout << "Enter player index: ";
            cin >> index;
            showPlayerStats(players, index);
            break;
        case 4:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please choose again.\n";
            break;
        }
    } while (choice != 4);

    return 0;
}
