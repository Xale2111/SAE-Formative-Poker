#pragma once
#include <vector>
#include <array>
#include "Card.h"

enum class HandValue
{
	//Who's the winner in case of same hand value?
	kHighCard,			//Highest card
	kPair,				//Highest pair ->  highest card
	kTwoPairs,			//Highest pair -> highest second pair -> highest card
	kThreeOfAKind,		//Highest 3 of a kind -> highest card
	kStraight,			//highest card IN the straight
	kFlush,				//highest card IN the flush
	kFull,				//Highest card IN the full (so in the 3 of a kind (first) THEN in the pair (second))
	kFourOfAKind,		//Highest 4 of a king -> Highest card
	kStraightFlush,		//Highest card IN the straight flush
	kRoyalFlush			//perfect equality, can't decide (if happens, royal flush is in the center of table and everyone win)

};

class Player
{
private:
	std::string _name;
	int _money;
	std::array<Card, 2> _hand;
	HandValue _currentHandValue = HandValue::kHighCard;
	std::vector<Card> _finalHand;

	std::string ConvertHandValueToString();

public:
	Player(std::string name,int StartMoneyAmount);
	int GetMoney();
	std::string GetName();
	//Pick a card for the player
	void GetNewCard(Card newCard);
	std::string ToString();
	//Get the cards of the player
	std::array<Card,2> GetHand();

	void SetHandValue(HandValue newHandValue);
	HandValue GetHandValue();
	std::string HandValueToString();

	void SetFinalHand(std::vector<Card> finalHand);
	std::vector<Card> GetFinalHand();

	//TODO : Create those functions
	//Bet
	//Check
	//AllIn
	//Fold
	//Win/Lose (ChangeMoneyAmount)



};

