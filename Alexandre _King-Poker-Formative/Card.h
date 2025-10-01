#pragma once
#include <string>

enum class Value
{
	k2,
	k3,
	k4,
	k5,
	k6,
	k7,
	k8,
	k9,
	k10,
	kJack,
	kQueen,
	kKing,
	kAce,

	ValueCounter
};

enum class Color
{
	kClub,
	kHearts,
	kSpades,
	kSquares,

	ColorCounter
};



class Card
{
private:
	Value _value;
	Color _color;
public:
	Card();
	Card(Value value, Color color);
	std::string ToString();
	Value GetValue();
	Color GetColor();
	std::string ColorToString(Color color);
	std::string ValueToString(Value value);

};



