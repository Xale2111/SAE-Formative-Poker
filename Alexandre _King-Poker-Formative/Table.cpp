#include "Table.h"
#include <iostream>
#include <algorithm>
#include <ranges>
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

std::vector<Card> Table::GetCenterCards()
{
	return _tableCards;
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
				_currentPlayerFinalHand = royalFlushCards;
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
	}

	_currentPlayerFinalHand = straightFlushCards;
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
		sort(returnedCards.begin(), returnedCards.end(), [](Card cardA, Card cardB)
			{
				return cardA.GetValue() > cardB.GetValue();
			});
		returnedCards.resize(5);
	}
	
	_currentPlayerFinalHand = returnedCards;
	return returnedCards;
}

//check if there's more the 5 different value,
//if so, check if the straight starts with 2 and end with 5 and if there's an ACE and that the next card isn't a 6 (for the straight from Ace to 5)
//If not, just checks if there's a straight by checking if card value = last card value +1
std::vector<Card> Table::CheckStraight(std::vector<Card> cards)
{
	//If there's a value twice i.e. two times 3, this won't work

	vector<Card> returnedCards;
	unordered_map<Value, int> occurenceOfEachValue = FindAllOccurencesOfEachValue(cards);

	if (occurenceOfEachValue.size() < 5)
	{
		return returnedCards;
	}
	
	vector<Card> uniqueCards = cards;
	
	uniqueCards = SortByValue(uniqueCards);


	uniqueCards.erase(unique(uniqueCards.begin(), uniqueCards.end(),[](Card cardA, Card cardB)
	{
			return cardA.GetValue() == cardB.GetValue();
	}), uniqueCards.end());


	int currentValue = (int)uniqueCards[0].GetValue();

	for (int idx = 0; idx < uniqueCards.size(); ++idx)
	{
		if ((int)uniqueCards[idx].GetValue() == currentValue + 1)
		{
			returnedCards.emplace_back(uniqueCards[idx]);
			currentValue++;
		}
		else
		{
			if (returnedCards.size()>=5)
			{
				break;
			}
			returnedCards.clear();
			returnedCards.emplace_back(uniqueCards[idx]);
			currentValue = (int)uniqueCards[idx].GetValue();
		}

		if (returnedCards.size() == 4 && uniqueCards[idx].GetValue() == Value::k5)
		{
			if (uniqueCards[idx+1].GetValue() != Value::k6 && uniqueCards[uniqueCards.size() - 1].GetValue() == Value::kAce)
			{
				returnedCards.emplace_back(uniqueCards[uniqueCards.size() - 1]);
				return returnedCards;
			}
		}
	}

	if (returnedCards.size() > 5)
	{
		sort(returnedCards.begin(), returnedCards.end(), [](Card cardA, Card cardB)
			{
				return cardA.GetValue() > cardB.GetValue();
			});
		returnedCards.resize(5);
	}


	_currentPlayerFinalHand = returnedCards;
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
	
	_currentPlayerFinalHand = returnedCards;

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

	_currentPlayerFinalHand = returnedCards;
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


	_currentPlayerFinalHand = returnedCards;
	return returnedCards;
}


//Uses the "FindAllOccurencesOfEachValue" function to check if there's a two pair 
//If so, we need to find the two highest pair, because if both player highest pair is the same, we need to check the second one 
//Complete with the highest card that's NOT the value of the pairs
std::vector<Card> Table::CheckTwoPairs(std::vector<Card> cards)
{
	vector<Card> returnedCards;
	cards = SortByValue(cards);
	unordered_map<Value, int> occurencesOfEachValue = FindAllOccurencesOfEachValue(cards);
	if (occurencesOfEachValue.size()>5)
	{
		return returnedCards;
	}
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
			if (occurence.first > valueOfFirstPair)
			{
				valueOfSecondPair = valueOfFirstPair;
				valueOfFirstPair = occurence.first;
			}
			else
			{
				valueOfSecondPair = occurence.first;
			}
			hasSecondPair = true;
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


	_currentPlayerFinalHand = returnedCards;
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

	_currentPlayerFinalHand = returnedCards;
	return returnedCards;
}


//If 5 different occurence > Find highest card
//if 4 different occurence >
//TODO : Compare the two hand directly since this time we know they have the same hand value REFACTOR THIS FUNCTION
std::array<Card, 2> Table::FindPlayerHighestCard(std::vector<Card> firstPlayerCards, std::vector<Card> secondPlayerCards, HandValue sameHandValue)
{
	std::array<Card, 2> playersHighestCards;
	Card firstPlayerhighestCard;
	firstPlayerCards = SortByValue(firstPlayerCards);
	unordered_map<Value, int> firstPlayerOccurence = FindAllOccurencesOfEachValue(firstPlayerCards);

	Card secondPlayerhighestCard;
	secondPlayerCards = SortByValue(secondPlayerCards);
	unordered_map<Value, int> secondPlayerOccurence = FindAllOccurencesOfEachValue(secondPlayerCards);

	Value firstPlayerValue = Value::k2;
	Value secondPlayerValue = Value::k2;

	int tempCounter = 0;


	switch (sameHandValue)
	{
	#pragma region 4OfAKind and Full
	case HandValue::kFourOfAKind:
	case HandValue::kFull:

		for (auto occurence : firstPlayerOccurence)
		{
			if (occurence.second == 4 || occurence.second == 3)
			{
				firstPlayerValue = occurence.first;
				break;
			}
		}
		for (auto occurence : secondPlayerOccurence)
		{
			if (occurence.second == 4 || occurence.second == 3)
			{
				secondPlayerValue = occurence.first;
				break;
			}
		}

		if (firstPlayerValue == secondPlayerValue)
		{
			erase_if(firstPlayerCards, [firstPlayerValue](Card card)
				{
					return card.GetValue() == firstPlayerValue;
				});

			erase_if(secondPlayerCards, [secondPlayerValue](Card card)
				{
					return card.GetValue() == secondPlayerValue;
				});


			firstPlayerhighestCard = firstPlayerCards[0];
			secondPlayerhighestCard = secondPlayerCards[0];
			
		}
		else
		{
			erase_if(firstPlayerCards, [firstPlayerValue](Card card)
				{
					return card.GetValue() != firstPlayerValue;
				});

			erase_if(secondPlayerCards, [secondPlayerValue](Card card)
				{
					return card.GetValue() != secondPlayerValue;
				});


			firstPlayerhighestCard = firstPlayerCards[0];
			secondPlayerhighestCard = secondPlayerCards[0];
		}
	#pragma endregion 4OfAKind and Full
		break;
	case HandValue::kThreeOfAKind:
	case HandValue::kPair:
	#pragma region 3OfAKind and Pair
		for (auto occurence : firstPlayerOccurence)
		{
			if (occurence.second == 3 || occurence.second == 2)
			{
				firstPlayerValue = occurence.first;
				break;
			}
		}
		for (auto occurence : secondPlayerOccurence)
		{
			if (occurence.second == 3 || occurence.second == 2)
			{
				secondPlayerValue = occurence.first;
				break;
			}
		}

		if (firstPlayerValue == secondPlayerValue)
		{
			erase_if(firstPlayerCards, [firstPlayerValue](Card card)
				{
					return card.GetValue() == firstPlayerValue;
				});

			erase_if(secondPlayerCards, [secondPlayerValue](Card card)
				{
					return card.GetValue() == secondPlayerValue;
				});


			firstPlayerhighestCard = firstPlayerCards[firstPlayerCards.size()-1];
			secondPlayerhighestCard = secondPlayerCards[secondPlayerCards.size() - 1];

		}
		else
		{
			erase_if(firstPlayerCards, [firstPlayerValue](Card card)
				{
					return card.GetValue() != firstPlayerValue;
				});

			erase_if(secondPlayerCards, [secondPlayerValue](Card card)
				{
					return card.GetValue() != secondPlayerValue;
				});


			firstPlayerhighestCard = firstPlayerCards[0];
			secondPlayerhighestCard = secondPlayerCards[0];
		}
		#pragma endregion 3OfAKind and Pair
		break;
	case HandValue::kTwoPairs:

		array<Value, 2> firstPlayerPairValues;
		array<Value, 2> secondPlayerPairValues;
		for (auto occurence : firstPlayerOccurence)
		{
			if (occurence.second == 2)
			{
				firstPlayerPairValues[tempCounter] = occurence.first;
				tempCounter = 1;
			}
		}
		tempCounter = 0;
		for (auto occurence : secondPlayerOccurence)
		{
			if (occurence.second == 2)
			{
				secondPlayerPairValues[tempCounter] = occurence.first;
				tempCounter = 1;
			}
		}

		sort(firstPlayerPairValues.begin(), firstPlayerPairValues.end(), [](Value valueA, Value valueB)
			{
					return valueA < valueB;
			});

		sort(secondPlayerPairValues.begin(), secondPlayerPairValues.end(), [](Value valueA, Value valueB)
			{
				return valueA < valueB;
			});


		//If highest pair is equal -> check second pair
		if (firstPlayerPairValues[1] == secondPlayerPairValues[1])
		{
			//if second pair is equal -> take last card
			if (firstPlayerPairValues[0] == secondPlayerPairValues[0])
			{
				erase_if(firstPlayerCards, [firstPlayerPairValues](Card card)
					{
						return card.GetValue() == firstPlayerPairValues[1] || card.GetValue() == firstPlayerPairValues[0];
					});

				erase_if(secondPlayerCards, [secondPlayerPairValues](Card card)
					{
						return card.GetValue() == secondPlayerPairValues[1] || card.GetValue() == secondPlayerPairValues[0];
					});

			}
			else
			{
				erase_if(firstPlayerCards, [firstPlayerPairValues](Card card)
					{
						return card.GetValue() != firstPlayerPairValues[0];
					});

				erase_if(secondPlayerCards, [secondPlayerPairValues](Card card)
					{
						return card.GetValue() != secondPlayerPairValues[0];
					});
			}
		}
		else
		{
			//highest pair isn't equal
			//take highest value
			erase_if(firstPlayerCards, [firstPlayerPairValues](Card card)
				{
					return card.GetValue() != firstPlayerPairValues[1];
				});

			erase_if(secondPlayerCards, [secondPlayerPairValues](Card card)
				{
					return card.GetValue() != secondPlayerPairValues[1];
				});
		}

		firstPlayerhighestCard = firstPlayerCards[0];
		secondPlayerhighestCard = secondPlayerCards[0];

		break;

	default:
		firstPlayerhighestCard = firstPlayerCards[firstPlayerCards.size() - 1];
		secondPlayerhighestCard = secondPlayerCards[secondPlayerCards.size() - 1];
		break;
	}



	playersHighestCards[0] = firstPlayerhighestCard;
	playersHighestCards[1] = secondPlayerhighestCard;


	return playersHighestCards;
}



//Public Functions


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

	tempCopy = SortByValue(tempCopy);

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



HandValue Table::CheckPlayerHand(Player* player)
{
	HandValue playerHandValue = HandValue::kHighCard;
	vector<Card> allCards;

	allCards.emplace_back(player->GetHand()[0]);
	allCards.emplace_back(player->GetHand()[1]);

	for (auto card : GetCenterCards())
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
		vector<Card> finalCards = SortByValue(allCards);
		reverse(finalCards.begin(), finalCards.end());
		finalCards.resize(5);
		_currentPlayerFinalHand = finalCards;
		//If no hand was found, we can just find the highest card of the player with the "SortByValue" function
		playerHandValue = HandValue::kHighCard;
	}

	player->SetFinalHand(_currentPlayerFinalHand);
	
	//returns the player hand value.
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

Player* Table::DefineWinner()
{
	Player* winner = nullptr;

	if (GetPlayerOne().GetHandValue() == GetPlayerTwo().GetHandValue())
	{
		array<Card, 2> highestCardResult = FindPlayerHighestCard(GetPlayerOne().GetFinalHand(), GetPlayerTwo().GetFinalHand(), GetPlayerOne().GetHandValue());

		//If highestCardResult[0] = highestCardResult[1] -> equality
		//if highestCardResult[0] > highestCardResult[1] -> player one won
		//if highestCardResult[0] < highestCardResult[1] -> player two won
		if (highestCardResult[0].GetValue() != highestCardResult[1].GetValue())
		{
			winner = highestCardResult[0].GetValue() > highestCardResult[1].GetValue() ? _player1 : _player2;
		}

	}
	else
	{
		winner = GetPlayerOne().GetHandValue() > GetPlayerTwo().GetHandValue() ? _player1 : _player2;
	}

	return winner;
}

int Table::GetTotalBet()
{
	return _totalBets;
}

void Table::AddToTotalBet(int amountToAdd)
{
	_totalBets += amountToAdd;
}

void Table::ResetTotalBet()
{
	_totalBets = 0;
}

void Table::ResetTable()
{
	ResetTotalBet();
	_tableCards.clear();
}



void Table::CheatCenterCards()
{
	_tableCards.emplace_back(Card(Value::k10,Color::kSquares));
	_tableCards.emplace_back(Card(Value::k10, Color::kHearts));
	_tableCards.emplace_back(Card(Value::kKing, Color::kSpades));
	_tableCards.emplace_back(Card(Value::k3, Color::kClub));
	_tableCards.emplace_back(Card(Value::k2, Color::kClub));

}


