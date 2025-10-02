#include "Table.h"
#include <iostream>
#include <algorithm>
using namespace std;
//Constructor
Table::Table(Player* player1, Player* player2, Deck* deck)
{
	_player1 = player1;
	_player2 = player2;
	_deck = deck;
}

//Private Functions
void Table::AddCardToCenter()
{
	Card pickedCard = _deck->PickCard();
	_tableCards.emplace_back(pickedCard);
	std::cout << "Picked card : " << pickedCard.ToString() << std::endl;
}

std::vector<Card> Table::SortByColor(std::vector<Card> cards)
{

	sort(cards.begin(), cards.end(), [](Card cardA, Card cardB)
	{
		return cardA.GetColor() < cardB.GetColor();
	});
	return cards;
}

std::vector<Card> Table::SortByValue(std::vector<Card> cards)
{
	sort(cards.begin(), cards.end(), [](Card cardA, Card cardB)
		{
			return cardA.GetValue() < cardB.GetValue();
		});
	return cards;
}


bool Table::CheckRoyalFlush(std::vector<Card> cards)
{
	

	std::vector<Card> royalFlushCards = CheckFlush(cards);
	if (royalFlushCards.size() >= 5)
	{
		royalFlushCards = CheckStraight(royalFlushCards);
		if (royalFlushCards.size() >= 5)
		{
			reverse(royalFlushCards.begin(), royalFlushCards.end());
			if (royalFlushCards[0].GetValue() == Value::kAce && royalFlushCards[2].GetValue() == Value::kQueen)
			{
				return true;
			}
		}
	}


	return false;

}

bool Table::CheckStraightFlush(std::vector<Card> cards)
{
	std::vector<Card> straightFlushCards = CheckFlush(cards);
	if (straightFlushCards.size() >= 5)
	{
		straightFlushCards = CheckStraight(straightFlushCards);
		if (straightFlushCards.size() >= 5)
		{
			return true;
		}
	}


	return false;

}

std::vector<Card> Table::CheckFlush(std::vector<Card> cards)
{
	vector<Card> returnedCards;

	cards = SortByColor(cards);

	vector<Card> sameColor;
	bool isFlush = false;
	int currentColor = (int)cards[0].GetColor();
	for (auto card : cards)
	{
		if ((int)card.GetColor() == currentColor)
		{
			sameColor.emplace_back(card);
		}
		else
		{
			sameColor.clear();
			sameColor.emplace_back(card);
			currentColor++;
		}

		if (sameColor.size() >=5)
		{
			isFlush = true;
			break;
		}
	}

	if (isFlush)
	{
		for (auto card : cards)
		{
			if ((int)card.GetColor() == currentColor)
			{
				returnedCards.emplace_back(card);
			}
		}
	}
	return returnedCards;
}

std::vector<Card> Table::CheckStraight(std::vector<Card> cards)
{
	vector<Card> returnedCards;

	cards = SortByValue(cards);

	vector<Card> sameValue;
	int currentValue = (int)cards[0].GetValue();
	bool straightStartingFromAce = false;
	if (currentValue == (int)Value::k2)
	{
		if (cards[cards.size()-1].GetValue() == Value::kAce )
		{
			straightStartingFromAce = true;
		}
	}

	for (auto card : cards)
	{
		if ((int)card.GetValue() == currentValue+1)
		{
			returnedCards.emplace_back(card);
			currentValue++;
		}
		else
		{
			if (straightStartingFromAce)
			{
				if (returnedCards.size() < 4 && card.GetValue() < Value::k5)
				{
					returnedCards.clear();
				}
				else
				{
					returnedCards.push_back(cards[cards.size() - 1]);
					return returnedCards;

				}
			}
			else
			{
				if (returnedCards.size() < 5)
				{
					returnedCards.clear();
				}
				else
				{
					return returnedCards;
				}
			}
			returnedCards.emplace_back(card);
			currentValue = (int)card.GetValue();

		}
	}

	return returnedCards;
}





//Public Functions
HandValue Table::CheckPlayerHand(Player player)
{
	HandValue playerHandValue = HandValue::kHighCard;
	vector<Card> allCards;
	vector<Card> finalCards;

	allCards.emplace_back(player.GetHand()[0]);
	allCards.emplace_back(player.GetHand()[1]);

	for (auto card : _tableCards)
	{
		allCards.emplace_back(card);
	}

	if (CheckRoyalFlush(allCards))
	{
		playerHandValue = HandValue::kRoyalFlush;
	}
	else if (CheckStraightFlush(allCards))
	{
		playerHandValue = HandValue::kStraightFlush;
	}
	
	if (finalCards.size()<5)
	{
		
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

void Table::Flop()
{
	for (int i = 0; i < 3; i++)
	{		
		AddCardToCenter();

	}
}

void Table::FourthStreet()
{
	AddCardToCenter();
}
void Table::FifthStreet()
{
	AddCardToCenter();
}


void Table::CheatCenterCards()
{
	_tableCards.emplace_back(Card(Value::kAce,Color::kHearts));
	_tableCards.emplace_back(Card(Value::k2, Color::kHearts));
	_tableCards.emplace_back(Card(Value::k3, Color::kHearts));
	_tableCards.emplace_back(Card(Value::k4, Color::kHearts));
	_tableCards.emplace_back(Card(Value::k5, Color::kHearts));

}
