#ifndef PLAYER_H
#define PLAYER_H
#include<iostream>
#include<vector>
#include<string>
#include<memory>
#include<map>
// #include"gamekey.h"
// #include"game.h"

using namespace std;

class Game;
enum class Mode;

class Player : public enable_shared_from_this<Player> {
public:
    static size_t numberOfPlayers;
private:
    size_t id;
    string name;
    int mmr;    //Matchmakingrating eines Players.
    shared_ptr<Game> hosted_game; //Gestartetes Spiel von diesem Spieler.
    map<string,weak_ptr<Game>> games; //Map von Spielen an denen Player teilnimmt.
    string team;

public:
    Player(string name, int mmr);
    Player(string name, int mmr, string team);

    //*Zusatz
    string get_team() const;
    void change_team(string t);
    bool operator==(const Player& rhs) const;
    bool operator!=(const Player& rhs) const;
    //*-----

    size_t get_id() const;
    string get_name() const;
    int get_mmr() const;
    shared_ptr<Game> get_hosted_game() const;
    void change_mmr(int n);
    bool host_game(string s, Mode m);
    bool join_game(shared_ptr<Game> g);
    bool leave_game(shared_ptr<Game> g);
    vector<weak_ptr<Player>> invite_players(const vector<weak_ptr<Player>>& v);
    bool close_game();

    ostream& print(ostream& o) const;
};

ostream& operator<<(ostream& o, const Player& player);

#endif