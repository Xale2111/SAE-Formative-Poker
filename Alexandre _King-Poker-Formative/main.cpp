
#include <iostream>
#include <vector>
#include <windows.h>
#include "Deck.h"
#include "Table.h"


int main()
{
    //Test for special chars (maybe we can draw each cards instead of just writing it ?)
    SetConsoleOutputCP(CP_UTF8); // passe la console en UTF-8
    std::string test = reinterpret_cast<const char*>(u8"♥ ♦ ♣ ♠");
    //std::cout << test << std::endl;

	int defaultStartMoney = 500;
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

    std::cout << player.GetName() +" \t\t:"+ player.ToString()<<std::endl;

    std::cout << bot.GetName() + " \t:" + bot.ToString()<<std::endl;

    //Full Hand :
    /*--loop
     *Players Pick cards
     *
     * --Loop--
     *Player bet
     *Flop
     *Player bet
     *4th street
     *player bet
     *5th street
     *player bet
     *--End Loop--
     *
     *Check winner
     *
     *Distribute pot
     *New round
     * --end loop--
     */

    bool playingHand = true;
    do
    {
        system("cls");

        //Take and verify input
        int playerInput = 0;
        bool validInput;
    	do
        {
            //Get player input
            std::cout << "What do you want to do ?" << std::endl;
            std::cout << "1 : Bet (only enter amount)" << std::endl;
            std::cout << "2 : Follow current bet" << std::endl;
            std::cout << "3 : Check" << std::endl;
            std::cout << "4 : ALL IN BABY !!!" << std::endl;
            std::cout << "5 : Fold" << std::endl;

            std::cin >> playerInput;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore();
                validInput = false;
            }
            else if (playerInput < 1 || playerInput > 5)
            {
                validInput = false;
            }
            else
            {
                validInput = true;
            }
        }
        while (!validInput);

        switch (playerInput)
        {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        }



    }
    while (playingHand);

    do
    {
	    
    }
    while (player.GetMoney()>0 && bot.GetMoney()>0);


    //Play first turn (preflop)
    //end turn -> flop

    //Play second turn (flop)
    //end turn -> 4th S

    //Play third turn (4th street)
	//end turn 5th street

    //Play Last turn


    table.Flop();

    table.FourthStreet();
    table.FifthStreet();
	



    player.SetHandValue(table.CheckPlayerHand(&player));
    bot.SetHandValue(table.CheckPlayerHand(&bot));

    std::cout << "\n--------RESULTS--------\n\n";

    std::cout << player.GetName() + "\t\t" + player.HandValueToString() << std::endl;
    std::cout << bot.GetName() + "\t" + bot.HandValueToString()<< std::endl;

    Player* winnerPtr = table.DefineWinner();
    

    if (winnerPtr == nullptr)
    {
	    std::cout << "Nobody won, it's an equality. The pot is split between all remaining players." << std::endl;
    }
    else
    {
        std::cout << winnerPtr->GetName() << " WON !" << std::endl;
    }

    //Should I create a Menu ?


    //How a full game should work :

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

/*
 table.CheatCenterCards();

player.GetNewCard(Card(Value::k6, Color::kSpades));
player.GetNewCard(Card(Value::k3, Color::kHearts));*/
