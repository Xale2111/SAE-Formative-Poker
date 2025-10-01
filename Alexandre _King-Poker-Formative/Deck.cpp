#include <algorithm>
#include "Deck.h"

#include <random>

//Constructor
Deck::Deck()
{
	InitializeDeck();
}

//Private Functions
void Deck::InitializeDeck()
{
	for (int indC = 0; indC < (int)Color::ColorCounter; ++indC)
	{
		for (int indV = 0; indV < (int)Value::ValueCounter; ++indV)
		{
			_deck.emplace_back(Card{ static_cast<Value>(indV),static_cast<Color>(indC)});
		}
	}
	ShuffleDeck();
}

void Deck::ShuffleDeck()
{
	std::random_device rd;
	std::mt19937 shuffler{ rd() };
	std::shuffle(_deck.begin(), _deck.end(), shuffler);
}


//Public Functions

std::vector<Card> Deck::GetDeck()
{
	return _deck;
}

Card Deck::PickCard()
{
	Card card = _deck.back();
	_deck.pop_back();

	return card;
}

