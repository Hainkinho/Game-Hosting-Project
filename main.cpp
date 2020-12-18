#include<iostream>
#include"memory"
#include"player.h"
#include"game.h"
#include"gamekey.h"

using namespace std;


int main() {
    // shared_ptr<Player> player1 = std::make_shared<Player>(Player("Leia", 100));
    // shared_ptr<Player> player2 = std::make_shared<Player>(Player("Lara", 109));
    // shared_ptr<Player> player3 = std::make_shared<Player>(Player("Oscar", 1000));
    // shared_ptr<Player> player4 = std::make_shared<Player>(Player("Geschi", 1050));

    shared_ptr<Player> player1 = std::make_shared<Player>(Player("Flo", 100, "Hainkinhos"));
    shared_ptr<Player> player5 = std::make_shared<Player>(Player("Leo", 100, "Hainkinhos"));
    shared_ptr<Player> player2 = std::make_shared<Player>(Player("Simon", 100, "Bks"));
    shared_ptr<Player> player3 = std::make_shared<Player>(Player("Oscar", 100, "Schmits"));
    shared_ptr<Player> player4 = std::make_shared<Player>(Player("Lorenz", 100, "Dos Santos"));
    player1->host_game("Survival Games", Mode::Ranked);
    player1->invite_players({player1, player2, player3, player4, player5});


    player1->get_hosted_game()->play(0);
    player1->get_hosted_game()->play(0);
    player1->get_hosted_game()->play(1);
    player1->get_hosted_game()->play(2);
    player1->get_hosted_game()->play(3);
    player3->change_team("Hainkinhos");
    // player1->close_game();
    player1->get_hosted_game()->play(2);
    player1->get_hosted_game()->play(3);
    player1->get_hosted_game()->play(1);
    player1->get_hosted_game()->play(1);
    player1->get_hosted_game()->play(0);


    cout << "Close Game" << endl;
    player1->close_game();
    cout << player1->get_hosted_game() << endl;
    cout << "Count: "<< player1.use_count() << endl;
    cout << "Game Closed" << endl;

    cout << *player1->get_hosted_game() << endl;

    player1->get_hosted_game()->printHallOfFame();
    auto kv = player1->get_hosted_game()->best_team();
    cout << "The Team \"" << kv.first << "\" wins the game with " << kv.second << " wins!" << endl;

    auto [bestPlayer, wins] = player1->get_hosted_game()->best_player();
    cout << "Best Player: " << bestPlayer.get_name() << " mit " << wins << " wins!" << endl;



    // cout << (*player1 == *player2 ? "True" : "False") << endl;

    // cout << *player1 << endl;



    return 0;
}