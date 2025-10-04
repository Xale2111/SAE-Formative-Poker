#include "Table.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
using namespace std;
//Constructor
Table::Table(Player* player1, Player* player2, Deck* deck)
{
	_player1 = player1;
	_player2 = player2;
	_deck = deck;
}

//Private Functions
//Add one card to the center of the table (doesn't burn a card)
void Table::AddCardToCenter()
{
	Card pickedCard = _deck->PickCard();
	_tableCards.emplace_back(pickedCard);
	std::cout << "Picked card : " << pickedCard.ToString() << std::endl;
}

//Sort given cards by color
std::vector<Card> Table::SortByColor(std::vector<Card> cards)
{

	sort(cards.begin(), cards.end(), [](Card cardA, Card cardB)
	{
		return cardA.GetColor() < cardB.GetColor();
	});
	return cards;
}

//sort given cards by value
std::vector<Card> Table::SortByValue(std::vector<Card> cards)
{
	sort(cards.begin(), cards.end(), [](Card cardA, Card cardB)
		{
			return cardA.GetValue() < cardB.GetValue();
		});
	return cards;
}

//Find all occurences of the same value and returns an unordered map where the Key is the card value and value is the amount of occurence 
unordered_map<Value, int> Table::FindAllOccurencesOfEachValue(std::vector<Card> cards)
{
	vector<Card> tempCopy = cards;
	unordered_map<Value, int> occurencesOfEachValue;

	//filter to have a temp vector with only 1 occurence of each value
	auto newEnd = std::unique(tempCopy.begin(), tempCopy.end(),
		[](Card a, Card b) {
			return a.GetValue() == b.GetValue();
		});

	tempCopy.erase(newEnd, tempCopy.end());
	for (auto card : tempCopy)
	{
		occurencesOfEachValue.insert(pair<Value, int>(card.GetValue(), 0));
	}

	//for each card, increase the occurence amount for each value
	Value currentValue = cards[0].GetValue();
	for (auto card : cards)
	{
		if (card.GetValue() == currentValue)
		{
			occurencesOfEachValue.at(currentValue)++;
		}
		else
		{
			currentValue = card.GetValue();
			occurencesOfEachValue.at(currentValue)++;
		}

	}

	return occurencesOfEachValue;
}



bool Table::CheckRoyalFlush(std::vector<Card> cards)
{
	//Check if there's a Flush, then if there's a straight and finally, if the straight begins with an Ace and finishes with a 10
	std::vector<Card> royalFlushCards = CheckFlush(cards);
	if (royalFlushCards.size() >= 5)
	{
		royalFlushCards = CheckStraight(royalFlushCards);
		if (royalFlushCards.size() >= 5)
		{
			reverse(royalFlushCards.begin(), royalFlushCards.end());
			if (royalFlushCards[0].GetValue() == Value::kAce && royalFlushCards[4].GetValue() == Value::k10)
			{
				return true;
			}
		}
	}
	return false;
}


std::vector<Card> Table::CheckStraightFlush(std::vector<Card> cards)
{
	//First check is there's a flush, then check if there's a straight
	std::vector<Card> straightFlushCards = CheckFlush(cards);
	if (straightFlushCards.size() >= 5)
	{
		straightFlushCards = CheckStraight(straightFlushCards);
		if (straightFlushCards.size() >= 5)
		{
			return straightFlushCards;
		}
	}

	return straightFlushCards;

}

//Sort by color and check if the same color appends 5 consecutive times 
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

	//resize to only have the 5 highest cards of the flush
	if (returnedCards.size() >5)
	{
		returnedCards = SortByValue(returnedCards);
		returnedCards.resize(5);
	}
	

	return returnedCards;
}

//check if there's more the 5 different value,
//if so, check if the straight starts with 2 and end with 5 and if there's an ACE (for the straight from Ace to 5)
//If not, just checks if there's a straight by checking if card value = last card value +1
//TODO:Still need to return only 5 cards
std::vector<Card> Table::CheckStraight(std::vector<Card> cards)
{
	vector<Card> returnedCards;
	unordered_map<Value, int> occurencesOfEachValue = FindAllOccurencesOfEachValue(cards);

	if (occurencesOfEachValue.size() < 5)
	{
		return returnedCards;
	}

	cards = SortByValue(cards);

	
	int currentValue = (int)cards[0].GetValue();

	for (auto card : cards)
	{
		if ((int)card.GetValue() == currentValue + 1)
		{
			returnedCards.emplace_back(card);
			currentValue++;
		}
		else
		{
			returnedCards.emplace_back(card);
			currentValue = (int)card.GetValue();
		}

		if (returnedCards.size() == 4 && card.GetValue() == Value::k5)
		{
			if (cards[cards.size() - 1].GetValue() == Value::kAce)
			{
				returnedCards.emplace_back(cards[cards.size() - 1]);
				return returnedCards;
			}
		}

	}

	return returnedCards;
}

//Uses the "FindAllOccurencesOfEachValue" function to check if there's 4 time the same value of a card
//If so, returns all 4 cards + the highest card (if there's four Aces, takes the highest card after the Aces)
std::vector<Card> Table::CheckFourOfAKind(std::vector<Card> cards)
{
	vector<Card> returnedCards;
	cards = SortByValue(cards);
	unordered_map<Value, int> occurencesOfEachValue = FindAllOccurencesOfEachValue(cards);

	bool has4OfAKind = false;
	Value valueOf4OfAKind;

	for (auto occurence : occurencesOfEachValue)
	{
		if (occurence.second == 4)
		{
			has4OfAKind = true;
			valueOf4OfAKind = occurence.first;
		}
	}

	if (has4OfAKind)
	{
		for (auto card : cards)
		{
			if (card.GetValue() == valueOf4OfAKind)
			{
				returnedCards.emplace_back(card);
			}
		}

		reverse(cards.begin(), cards.end());
		int counterComplete = 1;
		for (auto card : cards)
		{
			if (card.GetValue() != valueOf4OfAKind && counterComplete > 0)
			{
				returnedCards.emplace_back(card);
				counterComplete--;
			}
		}
	}
	

	return returnedCards;
}
//Uses the "FindAllOccurencesOfEachValue" function to check if there's at least 3 of a kind and a pair. If so, returns the 5 cards
//This should take the highest 3 of a kind and the highest pair
std::vector<Card> Table::CheckFull(std::vector<Card> cards)
{
	vector<Card> returnedCards;
	unordered_map<Value, int> occurencesOfEachValue = FindAllOccurencesOfEachValue(cards);

	bool hasPair = false;
	Value valueOfPair;
	bool has3OfAKind = false;
	Value valueOf3OfAKind;

	for (auto occurence : occurencesOfEachValue)
	{
		if (occurence.second == 2)
		{
			hasPair = true;
			valueOfPair = occurence.first;
		}
		else if (occurence.second == 3)
		{
			has3OfAKind = true;
			valueOf3OfAKind = occurence.first;
		}
	}

	if (hasPair && has3OfAKind)
	{
		for (auto card : cards)
		{
			if (card.GetValue() == valueOfPair || card.GetValue() == valueOf3OfAKind)
			{
				returnedCards.emplace_back(card);
			}
		}
	}

	return returnedCards;

}

//Uses the "FindAllOccurencesOfEachValue" function to check if there's 3 cards with the same value
//If so, complete with the two highest cards (takes the two highest card that are NOT the value of the 3 of a kind)
std::vector<Card> Table::CheckThreeOfAKind(std::vector<Card> cards)
{
	vector<Card> returnedCards;
	cards = SortByValue(cards);
	unordered_map<Value, int> occurencesOfEachValue = FindAllOccurencesOfEachValue(cards);

	bool has3OfAKind = false;
	Value valueOf3OfAKind;

	for (auto occurence : occurencesOfEachValue)
	{
		if (occurence.second == 3)
		{
			valueOf3OfAKind = occurence.first;
		}
	}
	if (has3OfAKind)
	{
		for (auto card : cards)
		{
			if (card.GetValue() == valueOf3OfAKind)
			{
				returnedCards.emplace_back(card);
			}
		}

		reverse(cards.begin(), cards.end());
		int counterComplete =  2;
		for (auto card : cards)
		{
			if (counterComplete > 0)
			{
				if (card.GetValue() != valueOf3OfAKind)
				{
					returnedCards.emplace_back(card);
					counterComplete--;
				}
			}
			else
			{
				break;
			}
		}
	}


	return returnedCards;
}


//Uses the "FindAllOccurencesOfEachValue" function to check if there's a pair twice
//If so, the second pair will be the highest pair available.
//(the first pair could be lower than another pair available but since the only pair that matters is the highest one we don't care)
//Complete with the highest card that's NOT the value of the pairs
std::vector<Card> Table::CheckTwoPairs(std::vector<Card> cards)
{
	vector<Card> returnedCards;
	cards = SortByValue(cards);
	unordered_map<Value, int> occurencesOfEachValue = FindAllOccurencesOfEachValue(cards);

	bool hasFirstPair = false;
	Value valueOfFirstPair;

	bool hasSecondPair = false;
	Value valueOfSecondPair;

	for (auto occurence : occurencesOfEachValue)
	{
		if (occurence.second == 2 && !hasFirstPair)
		{
			hasFirstPair = true;
			valueOfFirstPair = occurence.first;
		}
		else if (occurence.second == 2)
		{

			hasSecondPair = true;
			valueOfSecondPair = occurence.first;
		}
	}

	if (hasFirstPair && hasSecondPair)
	{
		for (auto card : cards)
		{
			if (card.GetValue() == valueOfFirstPair || card.GetValue() == valueOfSecondPair)
			{
				returnedCards.emplace_back(card);
			}
		}

		reverse(cards.begin(), cards.end());
		int counterComplete = 1;
		for (auto card : cards)
		{
			if (counterComplete > 0)
			{
				if (card.GetValue() != valueOfFirstPair && card.GetValue() != valueOfSecondPair)
				{
					returnedCards.emplace_back(card);
					counterComplete--;
				}
			}
			else
			{
				break;
			}
		}
	}


	return returnedCards;
}


//Uses the "FindAllOccurencesOfEachValue" function to check if there's a pair
//if so, complete with the 3 highest cards that are NOT the value of the pair
std::vector<Card> Table::CheckPair(std::vector<Card> cards)
{
	vector<Card> returnedCards;
	cards = SortByValue(cards);
	unordered_map<Value, int> occurencesOfEachValue = FindAllOccurencesOfEachValue(cards);

	bool hasPair = false;
	Value valueOfPair;

	for (auto occurence : occurencesOfEachValue)
	{
		if (occurence.second == 2)
		{
			hasPair = true;
			valueOfPair = occurence.first;
		}
	}

	if (hasPair)
	{
		for (auto card : cards)
		{
			if (card.GetValue() == valueOfPair)
			{
				returnedCards.emplace_back(card);
			}
		}

		reverse(cards.begin(), cards.end());
		int counterComplete = 3;
		for (auto card : cards)
		{
			if (counterComplete > 0)
			{
				if (card.GetValue() != valueOfPair)
				{
					returnedCards.emplace_back(card);
					counterComplete--;
				}
			}
			else
			{
				break;
			}
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
	else if (CheckStraightFlush(allCards).size() >= 5)
	{
		playerHandValue = HandValue::kStraightFlush;
	}
	else if (CheckFourOfAKind(allCards).size() >= 5)
	{
		playerHandValue = HandValue::kFourOfAKind;
	}
	else if (CheckFull(allCards).size() >= 5)
	{
		playerHandValue = HandValue::kFull;
	}
	else if (CheckFlush(allCards).size() >= 5)
	{
		playerHandValue = HandValue::kFlush;
	}
	else if (CheckStraight(allCards).size() >= 5)
	{
		playerHandValue = HandValue::kStraight;
	}
	else if (CheckThreeOfAKind(allCards).size() >= 5)
	{
		playerHandValue = HandValue::kThreeOfAKind;
	}
	else if (CheckTwoPairs(allCards).size() >= 5)
	{
		playerHandValue = HandValue::kTwoPairs;
	}
	else if (CheckPair(allCards).size() >= 5)
	{
		playerHandValue = HandValue::kPair;
	}
	else
	{
		//If no hand was found, we can just find the highest card of the player with the "SortByValue" function
		playerHandValue = HandValue::kHighCard;
	}
	
	
	//returns the player hand value.
	//TODO:Needs to be stocked somewhere (in the table perhaps ?)
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

//Functions to draw the cards in the center of the table (possibility to cheat for now to test the different hand)

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
	_tableCards.emplace_back(Card(Value::k2,Color::kHearts));
	_tableCards.emplace_back(Card(Value::kAce, Color::kClub));
	_tableCards.emplace_back(Card(Value::kKing, Color::kSpades));
	_tableCards.emplace_back(Card(Value::kQueen, Color::kSquares));
	_tableCards.emplace_back(Card(Value::kJack, Color::kHearts));

}
