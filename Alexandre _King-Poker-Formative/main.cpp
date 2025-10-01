
#include <iostream>
#include <vector>
#include <windows.h>
#include "Deck.h"
#include "Table.h"

int defaultStartMoney = 500;

int main()
{
    SetConsoleOutputCP(CP_UTF8); // passe la console en UTF-8
    //std::cout << reinterpret_cast<const char*>(u8"♥ ♦ ♣ ♠") << std::endl;

    int amountOfPlayer = 2;

    Deck deck;   
    Player player("PLAYER", defaultStartMoney);
    Player bot("REALLY BAD BOT",defaultStartMoney);
    Table table(&player,&bot,&deck);
   
    for (int inx = 0; inx < amountOfPlayer; inx++)
    {
        player.GetNewCard(deck.PickCard());
        bot.GetNewCard(deck.PickCard());
    }
    table.CheckPlayerHand(table.GetPlayerOne());
    std::cout << "\n----\n" << std::endl;
    table.CheckPlayerHand(table.GetPlayerTwo());
    std::cout << "\n----\n" << std::endl;

    /*for (auto card : table.GetDeck().GetDeck())
    { 
        std::cout << card.ToString() << std::endl;
    }*/

    table.Flop();

    table.FourthStreet();
    table.FifthStreet();

    //Should I create a Menu ?
    
    // Create Deck
    // Create player
    // Create Table 
    // Pass everything to the Table
    // distribute card to the players
    // first round (define who has to bet first with the bet token)
    // let player bet or check (end round when every player played and either checked/folded/bet)   
    // end of round 1
    // Pick and show Flop (3 first cards)
    // new round (same as before)
    // pick and show Fourth Street
    // new round 
    // Pick and show Fifth Street
    // last round
    // Check both players hands and evaluate who wins
    // winner win the whole table
    // in case of equality (both player has a pair of 2 i.e.), money is split in half and distribute to the players
    // 
    // in case both player bet everything, no rounds are played but all cards in the center are directly picked and displayed
    // since there's only 2 player, if someone folds, other player instantly wins
    //

    
}
