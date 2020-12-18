#include<iostream>
#include"memory"
#include"player.h"
#include"game.h"
#include"gamekey.h"

using namespace std;


int main() {
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

    cout << *player1->get_hosted_game() << endl;

    return 0;
}