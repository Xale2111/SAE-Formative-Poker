#include "Card.h"

#include <iostream>

//Constructor
Card::Card()
{
	_value = Value::ValueCounter;
	_color = Color::ColorCounter;
}
Card::Card(Value value, Color color)
{
	_value = value;
	_color = color;
}

std::string Card::ColorToString(Color color)
{
	switch (color)
	{
	case Color::kClub:
		return "Club";
	case Color::kHearts:
		return "Hearts";
	case Color::kSpades:
		return "Spades";
	case Color::kSquares:
		return "Squares";
	default:
		std::cout << "Error with Color ?? Not supposed to pass here";
		break;
	}
}

std::string Card::ValueToString(Value value)
{
	switch (value)
	{
	case Value::k2:
		return "2";
	case Value::k3:
		return "3";
	case Value::k4:
		return "4";
	case Value::k5:
		return "5";
	case Value::k6:
		return "6";
	case Value::k7:
		return "7";
	case Value::k8:
		return "8";
	case Value::k9:
		return "9";
	case Value::k10:
		return "10";
	case Value::kJack:
		return "Jack";
	case Value::kQueen:
		return "Queen";
	case Value::kKing:
		return "King";
	case Value::kAce:
		return "Ace";
		default:
			std::cout << "Error with Value ?? Not supposed to pass here";
			break;
	}
}

Color Card::GetColor()
{
	return _color;
}

Value Card::GetValue()
{
	return _value;
}


std::string Card::ToString()
{
	std::string toString;
	toString += ValueToString(GetValue()) +" of " + ColorToString(GetColor());

	return toString;
}


