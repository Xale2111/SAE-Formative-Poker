#pragma once
#include "Deck.h"
#include "Player.h"

enum class HandValue
{
	kHighCard,
	kPair,
	kTwoPairs,
	kThreeOfAKind,
	kStraight,
	kFlush,
	kFull,
	kFourOfAKing,
	kStraightFlush,
	kRoyalFlush

};

class Table
{
private:
	std::vector<Card> _tableCards;
	int _totalBets;
	int _playerStartIndex;
	Player* _player1;
	Player* _player2;
	Deck* _deck;	

	void AddCardToCenter();
public:
	Table(Player* player1, Player* player2, Deck* deck);


	HandValue CheckPlayerHand(Player player);

	Player GetPlayerOne();
	Player GetPlayerTwo();
	Deck GetDeck();
	void Flop();
	void FourthStreet();
	void FifthStreet();
	//Play turn
	//Flop (add the first 3 cards in the center)
	//Fourth Street (add the fourth card)
	//Fifth Street (add the fifth and final card to the table)
	//Check Player Hands
	//DefineWinner
};

