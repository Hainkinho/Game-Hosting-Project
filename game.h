#ifndef GAME_H
#define GAME_H

#include<iostream>
#include<string>
#include<memory>
#include<map>
#include<unordered_map>
#include"player.h"
// #include"gamekey.h"

using namespace std;

class Player;
class GameKey;


enum class Mode { Ranked, Unranked };

//TODO:
class MyHashFunction { 
public:  
    size_t operator()(const Player& p) const { 
        return p.get_id();
        // return p.get_name().length() + p.get_team().length();
    }
};

// Abstract Class
class Game {
protected:
    string name;
    shared_ptr<Player> host;
    map<string, shared_ptr<Player>> players;
    Game(string name, shared_ptr<Player> host);
    unordered_map<Player, int, MyHashFunction> hallOfFame;

    bool findPlayer(const string name) const;
    shared_ptr<Player> getWinner(size_t i) const;

    void addToHallOfFame(Player);
    map<string, int> get_teams() const;

public:
    string get_name() const;
    bool is_allowed(int n) const;
    bool remove_player(const GameKey& gk, shared_ptr<Player> p);
    bool add_player(const GameKey& gk, shared_ptr<Player> p);
    // shared_ptr<Player> best_player() const;
    pair<Player, int> best_player() const;
    pair<string, int> best_team() const;
    size_t number_of_players() const;
    shared_ptr<Player> play(size_t i);
    virtual int change(bool x) const = 0;

    virtual ~Game() = default;
    virtual ostream& print(ostream& o) const;

    //TODO:
    #define DEBUG
    #ifdef DEBUG
    void printHallOfFame() const {
        for (const auto& [key, value] : hallOfFame) {
            cout << key.get_name() << " (" << key.get_team() << ")" << " wins "<< value << " times." << endl;
        }
    }
    #endif
};

ostream& operator<<(ostream& o, const Game& game);

class RGame : public Game {
    private:

    public:
    RGame(string name, shared_ptr<Player> host);
    int change(bool x) const override;
    ostream& print(ostream& o) const override;

    ~RGame() {
        cout << "Removed" << endl;
    }
};

class UGame : public Game {
    private:

    public:
    UGame(string name, shared_ptr<Player> host);
    int change(bool x) const override;
    ostream& print(ostream& o) const override;
};



#endif