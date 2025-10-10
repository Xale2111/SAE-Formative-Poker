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


void Player::AddBetAmountThisRound(int amountToAdd)
{
	if (GetMoney()>0)
	{
		_betAmountThisRound += amountToAdd;
		SetBetAction(BetAction::kBet);
	}
	else
	{
		_betAmountThisRound += GetMoney();
		SetBetAction(BetAction::kAllIn);
	}
}

void Player::SetBetAction(BetAction newAction)
{
	_currentBetAction = newAction;
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

void Player::SetMoney(int newAmount)
{
	_money = newAmount;
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


//Betting related Functions

void Player::Bet(int betAmount)
{
	AddBetAmountThisRound(betAmount);
	ChangeMoneyAmount(-betAmount);
}

void Player::Check()
{
	SetBetAction(BetAction::kCheck);
}

void Player::AllIn()
{
	Bet(GetMoney());
}

void Player::Fold()
{
	SetBetAction(BetAction::kFold);
}

void Player::ChangeMoneyAmount(int amountToAdd)
{
	SetMoney(GetMoney()+amountToAdd);
}

int Player::GetBetAmountThisRound()
{
	return _betAmountThisRound;
}

void Player::ResetBetAmountThisRound()
{
	_betAmountThisRound = 0;
}


BetAction Player::GetCurrentBetAction()
{
	return _currentBetAction;
}
