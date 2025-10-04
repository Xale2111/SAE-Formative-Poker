#pragma once
#include <vector>
#include <array>
#include "Card.h"

class Player
{
private:
	std::string _name;
	int _money;
	std::array<Card, 2> _hand;
public:
	Player(std::string name,int StartMoneyAmount);
	int GetMoney();
	std::string GetName();
	//Pick a card for the player
	void GetNewCard(Card newCard);
	std::string ToString();
	//Get the cards of the player
	std::array<Card,2> GetHand();

	//TODO : Create those functions
	//Bet
	//Check
	//AllIn
	//Fold
	//Win/Lose (ChangeMoneyAmount)



};

