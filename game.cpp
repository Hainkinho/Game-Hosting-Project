#include<iostream>
#include<string>
#include<memory>
#include<algorithm>
#include<map>
#include<unordered_map>
#include "game.h"
#include "player.h"

using namespace std;


Game::Game(string name, shared_ptr<Player> host) {
    if (name == "" || host == nullptr) throw std::runtime_error("Game konnte nicht erzeugt werden!");
    this->name = name;
    this->host = host;
}

string Game::get_name() const { return name; }

bool Game::is_allowed(int n) const {
    int minMMR = host->get_mmr() * 0.9;
    int maxMMR = host->get_mmr() * 1.1;
    return ((minMMR < n) && (n < maxMMR));
}

size_t Game::number_of_players() const { return players.size(); }

bool Game::findPlayer(const string name) const {
    return players.find(name) != players.end();
}

bool Game::remove_player(const GameKey&, shared_ptr<Player> p) {
    if (findPlayer(p->get_name())) {
        players.erase(p->get_name());
        return true;
    }
    return false;
}

bool Game::add_player(const GameKey&, shared_ptr<Player> p) {
    if (!findPlayer(p->get_name())) { 
        if (is_allowed(p->get_mmr())) {
            players[p->get_name()] = p;
            return true;
        }
    }
    return false;
}

pair<Player, int> Game::best_player() const {
    if (players.empty()) throw std::runtime_error("bestPlayer(): Es ist kein Spieler in der Runde!");

    auto bestPlayerIt = max_element(hallOfFame.begin(), hallOfFame.end(), [](auto biggestKV, auto newKV) {
        return (biggestKV.second < newKV.second);
    });
    
    return pair<Player, int>{bestPlayerIt->first, bestPlayerIt->second};
}

map<string, int> Game::get_teams() const {
    map<string, int> teams;
    for (const auto& [player, wins] : hallOfFame) {
        if (teams.count(player.get_team()) <= 0) {
            teams[player.get_team()] = wins;
        } else {
            teams[player.get_team()] += wins;
        }
    }
    return teams;
}

pair<string, int> Game::best_team() const {
    if (hallOfFame.empty()) throw runtime_error("best_team() nicht möglich, da hallOfFame leer ist!");
    map<string, int> teams = get_teams();

    auto bestTeamIt = max_element(teams.begin(), teams.end(), [](auto biggestKV, auto newKV) {
        return (biggestKV.second < newKV.second);
    });
    
    return pair<string, int>{bestTeamIt->first, bestTeamIt->second};
}

shared_ptr<Player> Game::getWinner(size_t i) const {
    size_t counter = 0;
    for (auto it = players.begin(); it != players.end(); ++it, ++counter) {
        if (counter == i) {
            return it->second;
        }
    }
    return nullptr;
}

shared_ptr<Player> Game::play(size_t i) {
    shared_ptr<Player> winner = getWinner(i);
    for (auto it = players.begin(); it != players.end(); ++it) {
        shared_ptr<Player>& curPlayer = it->second;
        if (curPlayer != winner) {
            int multiplier = 1;
            if (curPlayer->get_mmr() > winner->get_mmr())
                multiplier = 2;
            curPlayer->change_mmr(multiplier * change(false));
        }
    }
    winner->change_mmr(change(true));
    addToHallOfFame(*winner);
    return winner;
}

void Game::addToHallOfFame(Player p) {
    if (hallOfFame.count(p) > 0) {
        hallOfFame[p] += 1;
    } else {
        hallOfFame[p] = 1;
    }
}

ostream& Game::print(ostream& o) const {
    o << "[" << name << ", " << host->get_name() << ", " << host->get_mmr() << ", player: {";

    size_t counter = 0;
    for (const auto& [key, value] : players) {
        o <<"["<< value->get_name() << ", " << value->get_mmr() << "]";
        if (counter < players.size() -1) {
            o << ", ";
        }
        ++counter;
    }
    o << "}]";
    return o;
}

ostream& operator<<(ostream& o, const Game& game) {
    return game.print(o);
}


// * RGame --------------------------------

RGame::RGame(string name, shared_ptr<Player> host) : Game(name, host) {}

int RGame::change(bool x) const {
    return (x ? 5 : -5);
}

ostream& RGame::print(ostream& o) const {
    o << "Ranked Game: ";
    return Game::print(o);
}

// * UGame --------------------------------

UGame::UGame(string name, shared_ptr<Player> host) : Game(name, host) {}

int UGame::change(bool) const { return 0; }

ostream& UGame::print(ostream& o) const {
    o << "Unranked Game: ";
    return Game::print(o);
}
