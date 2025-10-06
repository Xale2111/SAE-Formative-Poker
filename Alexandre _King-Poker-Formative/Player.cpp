#include "Player.h"

//Constructor
Player::Player(std::string name,int StartMoneyAmount)
{
	_name = name;
	_money = StartMoneyAmount;
}

//Private functions

std::string Player::ConvertHandValueToString()
{
	std::string convertToString;
	switch (GetHandValue())
	{
	case HandValue::kHighCard:
		convertToString = "High Card";
		break;
	case HandValue::kPair:
		convertToString = "Pair";
		break;
	case HandValue::kTwoPairs:
		convertToString = "Two Pairs";
		break;
	case HandValue::kThreeOfAKind:
		convertToString = "3 of a kind";
		break;
	case HandValue::kStraight:
		convertToString = "Straight";
		break;
	case HandValue::kFlush:
		convertToString = "Flush";
		break;
	case HandValue::kFull:
		convertToString = "Full House";
		break;
	case HandValue::kFourOfAKind:
		convertToString = "4 of a kind";
		break;
	case HandValue::kStraightFlush:
		convertToString = "Straight flush";
		break;
	case HandValue::kRoyalFlush:
		convertToString = "Royal Flush";
		break;
	default:
		convertToString = "Not a hand value ! This case shouldn't be true ??!";
		break;

	}

	return convertToString;
}


//Public functions
//when picking a new card, this is done this way so we can pick one card for each player one by one (like we would do in real life)
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

void Player::SetHandValue(HandValue newHandValue)
{
	_currentHandValue = newHandValue;
}

HandValue Player::GetHandValue()
{
	return _currentHandValue;
}

std::string Player::HandValueToString()
{
	std::string toString;
	toString =  "hand is a " + ConvertHandValueToString();

	return  toString;
}


void Player::SetFinalHand(std::vector<Card> finalHand)
{
	_finalHand = finalHand;
	_finalHand.resize(5);
}

std::vector<Card> Player::GetFinalHand()
{
	return _finalHand;
}
