
#include <algorithm>
#include <iostream>
#include <vector>
#include <windows.h>
#include "Deck.h"
#include "Table.h"


float BotCheckChanceOfWinning(std::array<Card,2> botCards, Table table, bool canTakeRisk = false);

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
     *Change startingPlayerIndex
     *New round
     *
     * --end loop--
     */

    bool playingHand = true;
    //int startingPlayerIndex;
    int highestBetAmountThisRound = 0;
    int currentRound = 0;
    bool inGame = true;
    bool inHand = true;
    bool skipRemainingTurns = false;

    std::string temp;

    while (inGame)
    {
        table.ResetTable();
        deck.ResetDeck();
        for (int inx = 0; inx < amountOfPlayer; inx++)
        {
            player.GetNewCard(deck.PickCard());
            bot.GetNewCard(deck.PickCard());
        }
        do
        {

            playingHand = true;
            do
            {
                system("cls");

                std::cout << player.GetName() + " \t\t:" + player.ToString() << std::endl;
                std::cout << bot.GetName() + " \t:" + bot.ToString() << std::endl;

	            for (auto card : table.GetCenterCards())
	            {
		            std::cout << "Center card : " << card.ToString() << std::endl;   
	            }

                player.ResetBetAmountThisRound();
                bot.ResetBetAmountThisRound();

#pragma region PlayerTurn
        //Take and verify input
                int playerInput = 0;
                bool validInput;
                do
                {
                    //Get player input
                    std::cout << "What do you want to do ?" << std::endl;
                    std::cout << "1 : Bet" << std::endl;
                    std::cout << "2 : Check / Follow current bet" << std::endl;
                    std::cout << "3 : ALL IN BABY !!!" << std::endl;
                    std::cout << "4 : Fold" << std::endl;

                    std::cin >> playerInput;

                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cin.ignore();
                        validInput = false;
                    }
                    else if (playerInput < 1 || playerInput > 4)
                    {
                        validInput = false;
                    }
                    else
                    {
                        validInput = true;
                    }
                } while (!validInput);

                switch (playerInput)
                {
                case 1:
                    //Let player enter bet amount (bet amount can't be lower if there's already
                    do
                    {
                        //Get player input
                        std::cout << "Enter bet amount (must be equal or higher then highest bet amount on the table this round)" << std::endl;

                        std::cin >> playerInput;

                        if (std::cin.fail())
                        {
                            std::cin.clear();
                            std::cin.ignore();
                            validInput = false;
                        }
                        else if (playerInput < highestBetAmountThisRound || playerInput > player.GetMoney())
                        {
                            std::cout << "Can't bet less then then the highest amount bet on the table this round or more then the amount of money you have" << std::endl;
                            validInput = false;
                        }
                        else
                        {
                            validInput = true;
                        }
                    } while (!validInput);


                    if (playerInput > highestBetAmountThisRound)
                    {
                        highestBetAmountThisRound = playerInput;
                    }

                    player.Bet(playerInput);

                    break;
                case 2:
                    //Check Or follow based on if current player bet is lower or equal to current bet amount
                    if (highestBetAmountThisRound == 0)
                    {
                        player.Check();
                    }
                    else
                    {
                        player.Bet(highestBetAmountThisRound);
                        std::cout << "You followed the current bet";
                        player.Check();
                    }
                    break;
                case 3:
                    player.AllIn();
                    break;
                case 4:
                    //Fold
                    player.Fold();
                    break;
                }

                table.AddToTotalBet(player.GetBetAmountThisRound());
#pragma endregion PlayerTurn


                //Decide what bot needs to do:
               //check current bot cards
               //check player bet action
               //calculate the risks/rewards with the current info
               //calculation is made by checking if the hand is a pair, same color, straight (check if (highest card - lowest card).abs <= 3 , high cards (sum of both card is higher then 15 i.e.)
               //add a small chance of bluff (5%)
#pragma region BotTurn
            //bot turn
                if (player.GetCurrentBetAction() != BetAction::kFold)
                {
                    std::array<Card, 2> botCards = bot.GetHand();
                    BetAction actionToExecute = BetAction::kNone;
                    bool shouldBluff = false;
                    bool followPlayer = false;
                    int betAmount = 0;

                    switch (player.GetCurrentBetAction())
                    {
                    case BetAction::kBet:
                    case BetAction::kAllIn:
                        //if player bet is higher then half our money, check if risk is less then 30%
                        if (player.GetBetAmountThisRound() >= bot.GetMoney() / 2)
                        {
                            float chanceOfWinning = BotCheckChanceOfWinning(botCards, table);
                            if (chanceOfWinning >= 75.f)
                            {
                                if (player.GetCurrentBetAction() == BetAction::kAllIn)
                                {
                                    if (player.GetBetAmountThisRound() >= bot.GetMoney())
                                    {
                                        actionToExecute = BetAction::kAllIn;
                                    }
                                    else
                                    {
                                        actionToExecute = BetAction::kBet;
                                        betAmount = player.GetBetAmountThisRound();
                                        followPlayer = true;
                                    }
                                }
                                else
                                {
                                    actionToExecute = BetAction::kBet;
                                    betAmount = player.GetBetAmountThisRound();
                                    followPlayer = true;
                                }
                            }
                            else if (chanceOfWinning > 50.f)
                            {
                                //follow (add bluff here in case, check or raise)
                                //random for a bluff
                                srand(time(0));
                                int randomNum = rand() % 20;
                                randomNum == 0 ? shouldBluff = true : shouldBluff = false;

                                if (!shouldBluff)
                                {
                                    if (chanceOfWinning >= 70.f)
                                    {
                                        betAmount = floor(player.GetBetAmountThisRound() + bot.GetMoney() * ((100 - chanceOfWinning + randomNum) / 100));
                                        actionToExecute = BetAction::kBet;
                                    }
                                    else
                                    {
                                        betAmount = player.GetBetAmountThisRound();
                                        actionToExecute = BetAction::kBet;
                                        followPlayer = true;
                                    }
                                }
                                else
                                {
                                    actionToExecute = BetAction::kAllIn;
                                }
                            }
                            else
                            {
                                actionToExecute = BetAction::kFold;
                            }
                        }
                        else
                        {
                            float chanceOfWinning = BotCheckChanceOfWinning(botCards, table, true);
                            if (chanceOfWinning >= 45.f)
                            {
                                //raise by X (calculate with all percentage above 85) (if all in -> commit)
                                if (player.GetCurrentBetAction() == BetAction::kAllIn)
                                {
                                    if (player.GetBetAmountThisRound() >= bot.GetMoney())
                                    {
                                        actionToExecute = BetAction::kAllIn;
                                    }
                                    else
                                    {
                                        actionToExecute = BetAction::kBet;
                                        betAmount = player.GetBetAmountThisRound();
                                        followPlayer = true;
                                    }
                                }
                                else
                                {
                                    srand(time(0));
                                    int randomNum = rand() % 20;
                                    randomNum == 0 ? shouldBluff = true : shouldBluff = false;

                                    if (!shouldBluff)
                                    {
                                        if (chanceOfWinning >= 65.f)
                                        {
                                            betAmount = floor(player.GetBetAmountThisRound() + bot.GetMoney() * ((100 - chanceOfWinning + randomNum) / 100));
                                            actionToExecute = BetAction::kBet;
                                        }
                                        else
                                        {
                                            betAmount = player.GetBetAmountThisRound();
                                            actionToExecute = BetAction::kBet;
                                            followPlayer = true;
                                        }
                                    }
                                    else
                                    {
                                        actionToExecute = BetAction::kAllIn;
                                    }
                                }
                            }
                            else if (chanceOfWinning > 10.f)
                            {
                                //follow (add bluff here in case, check or raise)
                                srand(time(0));
                                int randomNum = rand() % 20;
                                randomNum == 0 ? shouldBluff = true : shouldBluff = false;

                                if (!shouldBluff)
                                {
                                    if (chanceOfWinning >= 30.f)
                                    {
                                        betAmount = floor(player.GetBetAmountThisRound() + bot.GetMoney() * ((100 - chanceOfWinning / 2 + randomNum) / 100));
                                        actionToExecute = BetAction::kBet;
                                    }
                                    else
                                    {
                                        betAmount = player.GetBetAmountThisRound();
                                        actionToExecute = BetAction::kBet;
                                        followPlayer = true;
                                    }
                                }
                                else
                                {
                                    actionToExecute = BetAction::kAllIn;
                                }
                            }
                            else
                            {
                                //fold
                                actionToExecute = BetAction::kFold;
                            }
                        }

                        //check bet amount and calculate risk
                        break;
                    case BetAction::kCheck:
                        float chanceOfWinning = BotCheckChanceOfWinning(botCards, table, true);
                        if (chanceOfWinning >= 65.0f)
                        {
                            //raise by X (calculate with all percentage above 85) (if all in -> commit) (add bluff here in case, check or raise)

                            srand(time(0));
                            int randomNum = rand() % 20;
                            randomNum == 0 ? shouldBluff = true : shouldBluff = false;

                            if (!shouldBluff)
                            {
                                if (chanceOfWinning >= 80.f)
                                {
                                    betAmount = floor(player.GetBetAmountThisRound() + bot.GetMoney() * ((100 - chanceOfWinning + randomNum) / 100));
                                    actionToExecute = BetAction::kBet;
                                }
                                else
                                {
                                    actionToExecute = BetAction::kCheck;
                                }
                            }
                            else
                            {
                                actionToExecute = BetAction::kAllIn;
                            }

                        }
                        else if (chanceOfWinning > 27.5f)
                        {
                            //follow (add bluff here in case, check or raise)
                            srand(time(0));
                            int randomNum = rand() % 20;
                            randomNum == 0 ? shouldBluff = true : shouldBluff = false;

                            if (!shouldBluff)
                            {
                                if (chanceOfWinning >= 45.f)
                                {
                                    betAmount = floor(player.GetBetAmountThisRound() + bot.GetMoney() * ((100 - chanceOfWinning / 2 + randomNum) / 100));
                                    actionToExecute = BetAction::kBet;
                                }
                                else
                                {
                                    actionToExecute = BetAction::kCheck;
                                }
                            }
                            else
                            {
                                actionToExecute = BetAction::kAllIn;
                            }
                        }
                        else
                        {
                            actionToExecute = BetAction::kCheck;
                        }
                        break;

                    }

                    switch (actionToExecute)
                    {
                    case BetAction::kAllIn:
                        bot.AllIn();
                        std::cout << bot.GetName() << " WENT ALL IN BABY !!!!\n";
                        break;
                    case BetAction::kBet:
                        bot.Bet(betAmount);
                        if (followPlayer)
                        {
                            std::cout << bot.GetName() << " followed your bet \n";
                        }
                        else
                        {
                            std::cout << bot.GetName() << " raised the bet to " << betAmount << std::endl;
                        }
                        break;
                    case BetAction::kCheck:
                        bot.Check();
                        std::cout << bot.GetName() << " checked\n";

                        break;
                    default:
                        bot.Fold();
                        std::cout << bot.GetName() << " folded\n";
                        break;
                    }
                    highestBetAmountThisRound = betAmount;

                    table.AddToTotalBet(bot.GetBetAmountThisRound());

#pragma endregion BotTurn

                }

                if (player.GetCurrentBetAction() == BetAction::kFold)
                {
                    //bot win the pot
                    playingHand = false;
                    skipRemainingTurns = true;
                }
                else if (bot.GetCurrentBetAction() == BetAction::kFold)
                {
                    //player win the pot
                    playingHand = false;
                    skipRemainingTurns = true;
                }
                else if (player.GetCurrentBetAction() == BetAction::kAllIn || bot.GetCurrentBetAction() == BetAction::kAllIn)
                {
                    //skip all future turns
                    skipRemainingTurns = true;
                    playingHand = false;
                }
                else if (player.GetCurrentBetAction() == bot.GetCurrentBetAction() && player.GetBetAmountThisRound() == highestBetAmountThisRound && bot.GetBetAmountThisRound() == highestBetAmountThisRound)
                {
                    playingHand = false;
                }
                else
                {
                    playingHand = true;
                }

                if (!playingHand)
                {
                    std::cout << "Pot in the middle of the table is now : " << table.GetTotalBet() << std::endl;
                }
            } while (playingHand);

            if (!skipRemainingTurns)
            {
                switch (currentRound)
                {
                case 0:
                    table.Flop();
                    break;
                case 1:
                    table.FourthStreet();
                    break;
                case 2:
                    table.FifthStreet();
                    break;
                default:
                    break;
                }
            }
            else
            {
                switch (currentRound)
                {
                case 0:
                    table.Flop();
                    table.FourthStreet();
                    table.FifthStreet();
                    break;
                case 1:
                    table.FourthStreet();
                    table.FifthStreet();
                    break;
                case 2:
                    table.FifthStreet();
                    break;
                default:
                    break;
                }
                currentRound += 3;
            }

            if (currentRound >= 3)
            {
	           

                //end of hand
                inHand = false;
                player.SetHandValue(table.CheckPlayerHand(&player));
                bot.SetHandValue(table.CheckPlayerHand(&bot));

                std::cout << "\n--------RESULTS--------\n\n";

                std::cout << player.GetName() + "\t\t" + player.HandValueToString() << std::endl;
                std::cout << bot.GetName() + "\t" + bot.HandValueToString() << std::endl;

                Player* winnerPtr = table.DefineWinner();

                if (player.GetCurrentBetAction() == BetAction::kFold)
                {
                    std::cout << bot.GetName() << " WON !" << std::endl;
                    bot.ChangeMoneyAmount(table.GetTotalBet());

                }
                else if (bot.GetCurrentBetAction() == BetAction::kFold)
                {
                    std::cout << player.GetName() << " WON !" << std::endl;
                    player.ChangeMoneyAmount(table.GetTotalBet());
                }
                else
                {
                    if (winnerPtr == nullptr)
                    {
                        std::cout << "Nobody won, it's an equality. The pot is split between all remaining players." << std::endl;
                        player.ChangeMoneyAmount(table.GetTotalBet() / 2);
                        bot.ChangeMoneyAmount(table.GetTotalBet() / 2);
                    }
                    else
                    {

                        std::cout << winnerPtr->GetName() << " WON !" << std::endl;
                        if (winnerPtr->GetName() == player.GetName())
                        {
                            player.ChangeMoneyAmount(table.GetTotalBet());
                        }
                        else
                        {
                            bot.ChangeMoneyAmount(table.GetTotalBet());
                        }
                    }
                }

                std::cout << player.GetName() << " now has " << player.GetMoney() << " Dollars\n";
                std::cout << bot.GetName() << " now has " << bot.GetMoney() << " Dollars\n";
                table.ResetTotalBet();

                if (player.GetMoney() <= 0)
                {
                    inGame = false;
                    std::cout << bot.GetName() << " WON THE GAME !!!" << std::endl;
                }
                else if (bot.GetMoney() <= 0)
                {
                    inGame = false;
                    std::cout << player.GetName() << " WON THE GAME !!!" << std::endl;
                }
            }

            currentRound++;
            std::cout << "Press anything to continue" << std::endl;
            std::cin >> temp;

        } while (inHand);


        std::cout << "Press anything to start new hand"<<std::endl;
        std::cin >> temp;
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

float BotCheckChanceOfWinning(std::array<Card, 2> botCards, Table table, bool canTakeRisk)
{
    float winningChance = 0.f;
    float chanceThreshold[5] = { 5,10,25,50,80 };

    std::sort(botCards.begin(), botCards.end(), [](Card cardA, Card cardB)
        {
            return cardA.GetValue() < cardB.GetValue();
        });

    Card firstCard = botCards[0];
    Card secondCard = botCards[1];

    std::vector<Card> allCards = table.GetCenterCards();


    float multiplier = 1;     //based on the total amount of cards, the multiplier will be higher or lower. fewer cards -> higher multiplier / more cards -> lower multiplier

    switch (allCards.size())
    {
    case 5:
        multiplier = 1.1;
        break;
    case 4:
        multiplier = 1.7;
        break;
    case 3:
        multiplier = 2.2;
        break;
    default:
        multiplier = 1;
        break;
    }


    allCards.emplace_back(firstCard);
    allCards.emplace_back(secondCard);


    auto occurenceOfEachValue = table.FindAllOccurencesOfEachValue(allCards);

    for (auto occurence : occurenceOfEachValue)
    {

        switch (occurence.second)
        {
        case 4:
            winningChance += chanceThreshold[4] * multiplier;
            break;
        case 3:
            winningChance += chanceThreshold[3] * multiplier;
            break;
        case 2:
            winningChance += chanceThreshold[2] * multiplier;
            break;
        }
    }

    std::unordered_map<Color, int> occurenceOfEachColor;

    for (auto card : allCards)
    {
        occurenceOfEachColor[card.GetColor()]++;
    }

    switch (allCards.size())
    {
    case 2:
        multiplier = 1.5f;
        break;
    case 5:
        multiplier = 1.4f;
        break;
    case 6:
        multiplier = 1.2f;
        break;
    case 7:
        multiplier = 1;
        break;

        default:
            break;
    }

    //Check flush
    for (auto occurence : occurenceOfEachColor)
    {
	    switch (occurence.second)
	    {
	    case 2:
            winningChance += chanceThreshold[1] * multiplier;
            break;
	    case 3:
            winningChance += chanceThreshold[2] * multiplier;
            break;
	    case 4 :
            winningChance += chanceThreshold[3] * multiplier;
            break;
	    case 5:
	    case 6:
	    case 7:
            winningChance += chanceThreshold[4] * multiplier;
            break;
        default:
            winningChance += 0;
            break;
	    }
    }

    //Check Straight
    std::sort(allCards.begin(), allCards.end(), [](Card cardA, Card cardB)
        {
            return cardA.GetValue() < cardB.GetValue();
        });

    int straightCounter = 0;
    std::vector<int> straightCounters;
    //won't check the straight with an ACE to simplify
    for (int i = 1; i < allCards.size(); ++i)
    {
	    if (allCards[i].GetValue() == allCards[i-1].GetValue())
	    {
            straightCounters[straightCounter]++;
	    }
	    else
	    {
            straightCounter++;
	    }
    }


    //Get the highest straight amount
    auto highestStraight = std::max_element(straightCounters.begin(), straightCounters.end());
    if (highestStraight != straightCounters.end())
    {
	    switch (*highestStraight)
	    {
        case 2:
            winningChance += chanceThreshold[1] * multiplier;
            break;
        case 3:
            winningChance += chanceThreshold[2] * multiplier;
            break;
        case 4:
            winningChance += chanceThreshold[3] * multiplier;
            break;
        case 5:
        case 6:
        case 7:
            winningChance += chanceThreshold[4] * multiplier;
            break;
        default:
            winningChance += 0;
            break;
	    }
    }

    if (winningChance <= 30)
    {
        int handTotalValue = (int)botCards[0].GetValue() + (int)botCards[1].GetValue();
        //HighestTotalValue is 24 (Ace is 12, so 12*2 = 24)
        if (handTotalValue >= 18)
        {
            winningChance += chanceThreshold[1] + handTotalValue;
        }
        else if (handTotalValue >= 9)
        {
            winningChance += chanceThreshold[0] + handTotalValue;
        }
        else
        {
            winningChance += handTotalValue;
        }
    }


    //highest value based on a real hand (straight flush/royal flush) -> 240
    //Normalize the value to have a percentage between 0 and 100%


    float normalized = 0;
    canTakeRisk ? normalized = (winningChance * 1.15f / 240.f) * 100 : normalized = (winningChance / 240.f) * 100;
  

    

    return normalized;
}

//calculation is made by checking if the hand is a pair, same color, straight (check if (highest card - lowest card).abs <= 3 , high cards (sum of both card is higher then 15 i.e.)
