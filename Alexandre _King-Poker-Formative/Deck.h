#pragma once
#include <vector>

#include "Card.h"

class Deck
{
private:
	void InitializeDeck();
	std::vector<Card> _deck;
	void ShuffleDeck();
public:
	Deck();
	std::vector<Card> GetDeck();
	Card PickCard();

	void ResetDeck();

};

