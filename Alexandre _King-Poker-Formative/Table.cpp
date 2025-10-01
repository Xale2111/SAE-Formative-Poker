#include "Table.h"
#include <iostream>


//Constructor
Table::Table(Player* player1, Player* player2, Deck* deck)
{
	_player1 = player1;
	_player2 = player2;
	_deck = deck;
}

//Public Functions
HandValue Table::CheckPlayerHand(Player player)
{
	HandValue playerHandValue = HandValue::kHighCard;
	std::vector<Card> allCards;
	allCards.emplace_back(player.GetHand()[0]);
	allCards.emplace_back(player.GetHand()[1]);

	for (auto card : _tableCards)
	{
		allCards.emplace_back(card);
	}

	for(auto card : allCards)
	{
		std::cout << card.ToString() << std::endl;
	}


	return playerHandValue;
}


Player Table::GetPlayerOne()
{
	return *_player1;
}

Player Table::GetPlayerTwo()
{
	return *_player2;
}

Deck Table::GetDeck()
{
	return *_deck;
}