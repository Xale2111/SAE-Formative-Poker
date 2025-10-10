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


enum class BetAction
{
	kNone,
	kBet,
	kCheck,
	kFold,
	kAllIn
};

class Player
{
private:
	std::string _name;
	int _money;
	std::array<Card, 2> _hand;
	int _betAmountThisRound;
	HandValue _currentHandValue = HandValue::kHighCard;
	std::vector<Card> _finalHand;
	BetAction _currentBetAction;

	void SetBetAction(BetAction newAction);
	std::string ConvertHandValueToString();

	void AddBetAmountThisRound(int amountToAdd);

public:
	Player(std::string name,int StartMoneyAmount);
	void SetMoney(int newAmount);
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

	int GetBetAmountThisRound();
	void ResetBetAmountThisRound();

	void Bet(int betAmount);
	void Check();
	void AllIn();
	void Fold();

	void ChangeMoneyAmount(int amountToAdd);

	BetAction GetCurrentBetAction();
};

