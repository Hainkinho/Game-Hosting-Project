#include<iostream>
#include<string>
#include<memory>
#include<map>
#include "player.h"
#include "game.h"
#include "gamekey.h"

using namespace std;

size_t Player::numberOfPlayers = 0;

Player::Player(string name, int mmr) {
    if (name.empty()) throw std::runtime_error("Player konnte nicht erstellt werden, da der name leer ist!");
    if (mmr < 0 || mmr > 9999) throw std::runtime_error("Player konnte nicht erstellt werden, da mmr nicht im richigen Bereich ist!");
    this->id = Player::numberOfPlayers++;
    this->name = name;
    this->mmr = mmr;
}

size_t Player::get_id() const { return id; }
string Player::get_name() const { return name; }
int Player::get_mmr() const { return mmr; }
shared_ptr<Game> Player::get_hosted_game() const { return hosted_game; }

void Player::change_mmr(int n) {
    int modified = mmr + n;
    if (modified < 0 || modified > 9999) return;
    mmr = modified;
}

bool Player::host_game(string s, Mode m) {
    if (s.empty()) throw std::runtime_error("host_game() konnte nicht ausgeführt werden!");
    if (hosted_game == nullptr) {
        if (m == Mode::Ranked) {
            hosted_game = make_shared<RGame>(RGame(s, shared_from_this()));
            return true;
        } else if (m == Mode::Unranked) {
            hosted_game = make_shared<UGame>(UGame(s, shared_from_this()));
            return true;
        }
    }
    return false;
}

bool Player::join_game(shared_ptr<Game> g) {
    bool success = g->add_player(GameKey{}, shared_from_this());
    if (success) {
        games[g->get_name()] = g;
    }
    return success;
}

bool Player::leave_game(shared_ptr<Game> g) {
    size_t count = games.erase(g->get_name());
    bool removed = g->remove_player(GameKey{}, shared_from_this());
    return (count > 0 && removed);
}

vector<weak_ptr<Player>> Player::invite_players(const vector<weak_ptr<Player>>& v) {
    vector<weak_ptr<Player>> res;
    for (const auto& playerPtr : v) {
        bool success = false;
        if (!playerPtr.expired()) {
            success = playerPtr.lock()->join_game(hosted_game);
        }
        if (!success){
            res.push_back(playerPtr);     
        }
    }
    return res;
}

bool Player::close_game() {
    if (hosted_game != nullptr) {
        hosted_game = nullptr;
        return true;
    }
    return false;
}

//TODO: Changed
ostream& Player::print(ostream& o) const {
    o << "[" << name << ", " << mmr << ", hosts: " << (hosted_game != nullptr ? hosted_game->get_name() : "nothing") << ", games: {";

  	bool found = false;
    for (const auto& [key, valPtr] : games) {
        if (!valPtr.expired()) {
          	if (found) {
                o << ", ";            
            }
            o << valPtr.lock()->get_name();
          	found = true;
        }
    }
    o << "}]";
    return o;
}

ostream& operator<<(ostream& o, const Player& player) {
    return player.print(o);
}

//Zusatz --------

Player::Player(string name, int mmr, string team) : Player(name, mmr) {
    if (team.empty()) throw std::runtime_error("Player konnte nicht erstellt werden, da team leer ist!");
    this->team = team;
}

string Player::get_team() const {
    return team;
}

void Player::change_team(string t) {
    if (t.empty()) throw std::runtime_error("change_team() konnte nicht ausgeführt werden, da t leer ist!");
    this->team = t;
}

bool Player::operator==(const Player& rhs) const {
    return (name == rhs.get_name() && team == rhs.get_team());
}

bool Player::operator!=(const Player& rhs) const {
    return !(*this == rhs);
}