#pragma once
#include <unordered_map>

#include "Deck.h"
#include "Player.h"



class Table
{
private:
	std::vector<Card> _tableCards;
	int _totalBets;
	int _playerStartIndex;
	std::vector<Card> _currentPlayerFinalHand;
	Player* _player1;
	Player* _player2;
	Deck* _deck;	

	void AddCardToCenter();
	std::vector<Card> SortByColor(std::vector<Card> cards);
	std::vector<Card> SortByValue(std::vector<Card> cards);
	std::unordered_map<Value, int> FindAllOccurencesOfEachValue(std::vector<Card> cards);


	//For Royal flush, since this is either an equality or a win for the player, no need to return the cards
	//for all of the others, if two or more player has the concerned hand, we need the cards to check the highest card of each player 
	//every time we return a vector of card, the vector should contains the 5 final cards (easier to check the highest card afterwards)
	bool CheckRoyalFlush(std::vector<Card> cards);
	std::vector<Card> CheckStraightFlush(std::vector<Card> cards);	
	std::vector<Card> CheckFourOfAKind(std::vector<Card> cards);
	std::vector<Card> CheckFull(std::vector<Card> cards);
	std::vector<Card> CheckFlush(std::vector<Card> cards);
	std::vector<Card> CheckStraight(std::vector<Card> cards);
	std::vector<Card> CheckThreeOfAKind(std::vector<Card> cards);
	std::vector<Card> CheckTwoPairs(std::vector<Card> cards);
	std::vector<Card> CheckPair(std::vector<Card> cards);


	Card FindPlayerHighestCard(std::vector<Card> cards);

public:
	//Takes the reference of each
	Table(Player* player1, Player* player2, Deck* deck);

	//Check Player Hands
	HandValue CheckPlayerHand(Player* player);

	Player GetPlayerOne();
	Player GetPlayerTwo();
	Deck GetDeck();

	//Flop (add the first 3 cards in the center)
	void Flop();

	//Fourth Street (add the fourth card)
	void FourthStreet();

	//Fifth Street (add the fifth and final card to the table)
	void FifthStreet();

	void CheatCenterCards();

	Player* DefineWinner();

	//TODO: Create those functions 
	//Play turn
};

