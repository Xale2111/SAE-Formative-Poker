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
	void GetNewCard(Card newCard);
	std::string ToString();
	std::array<Card,2> GetHand();


	//Bet
	//Check
	//AllIn
	//Fold
	//Win/Lose (ChangeMoneyAmount)



};

