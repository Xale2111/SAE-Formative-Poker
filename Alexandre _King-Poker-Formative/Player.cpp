#include "Player.h"

//Constructor
Player::Player(std::string name,int StartMoneyAmount)
{
	_name = name;
	_money = StartMoneyAmount;
}

//Private functions

//Public functions
void Player::GetNewCard(Card newCard)
{
	if (_hand[0].GetValue() == Value::ValueCounter)
	{
		_hand[0] = newCard;
	}
	else if (_hand[1].GetValue() == Value::ValueCounter)
	{
		_hand[1] = newCard;
	}
}
std::string Player::ToString()
{
	std::string toString;
	toString += _hand[0].ToString() + " and " + _hand[1].ToString();
	return toString;
}

std::string Player::GetName()
{
	return _name;
}

int Player::GetMoney()
{
	return _money;
}

std::array<Card,2> Player::GetHand()
{
	return _hand;
}