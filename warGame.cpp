#include <iostream>
#include <deque>
#include <cstdlib>
#include <ctime>

using namespace std;

//Enumerate value for each card
enum Value {
	VALUE_TWO = 2,
	VALUE_THREE,
	VALUE_FOUR,
	VALUE_FIVE,
	VALUE_SIX,
	VALUE_SEVEN,
	VALUE_EIGHT,
	VALUE_NINE,
	VALUE_TEN,
	VALUE_JACK,
	VALUE_QUEEN,
	VALUE_KING,
	VALUE_ACE,
};

//Enumerate value for each suit
enum Suit {
	SUIT_CLUBS,
	SUIT_DIAMONDS,
	SUIT_HEARTS,
	SUIT_SPADES,
};

//Class for cards
class Card {
private:
	//Member variables
	Value m_value;
	Suit m_suit;
	
	//Arrays to retrieve readable text
	//Global or put into main?
	string strValue[15] = {"NULL_0", "NULL_1", "Two", "Three", "Four", "Five", 
	"Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};
	string strSuit[4] = {"Clubs", "Diamonds", "Hearts", "Spades"};

public:
	//Constructor
	Card(Value value = VALUE_ACE, Suit suit = SUIT_SPADES) {
		m_value = value;
		m_suit = suit;
	}
	
	//Get functions
	Value getValue() {
		return m_value;
	}
	Suit getSuit() {
		return m_suit;
	}
	
	//Return card name
	string cardName() {
		string m_cardName = strValue[m_value] + " of " + strSuit[m_suit];
		return m_cardName;		
	}
};

//Prototype functions
void createDeck(Card deckOfCards[]);
void shuffleDeck(int shuffleIndex[], int numCards);
void dealCards(Card deckOfCards[], int shuffleIndex[],
				deque<Card>& deckPlayer1, deque<Card>& deckPlayer2);
void joustCards(deque<Card>& deckPlayer1, deque<Card>& deckPlayer2);
void warCards(deque<Card>& deckPlayer1, deque<Card>& deckPlayer2, 
				deque<Card>& pilePlayer1, deque<Card>& pilePlayer2);
void moveCardFromTo(deque<Card>& fromGroup, string fromLoc,
				deque<Card>& toGroup, string toLoc);

//Main function
int main() {
	//Initialize a deck of cards
	Card cards[52] = {};
	createDeck(cards);
	
	//Shuffle the deck
	int shuffleIndex[52] = {};
	shuffleDeck(shuffleIndex, 52);
	
	//Deal the cards to the players
	deque<Card> deckP1 = {};
	deque<Card> deckP2 = {};
	dealCards(cards, shuffleIndex, deckP1, deckP2);
	
	//Print the cards in the deck array
	for (int i = 0; i < 52; i++) {
		cout << cards[shuffleIndex[i]].cardName() << "\n";
	}
	
	cout << "\n";
	
	//Print the cards in deck 1
	for (Card card : deckP1) {
		cout << card.cardName() << "\n";
	}
	
	cout << "\n";
	
	//Print the cards in deck 2
	for (Card card : deckP2) {
		cout << card.cardName() << "\n";
	}
	
	//Joust cards while players have enough to continue
	joustCards(deckP1, deckP2);
	
	//Award victory and tell results
	if (deckP1.empty()){
		cout << "Player 2 wins with " << deckP2.size() << " cards.\n";
	}
	else if (deckP2.empty()){
		cout << "Player 1 wins with " << deckP1.size() << " cards.\n";
	}
	else {
		cout << "Draw!\n";
	}
	
	return 0;
}

//Function to fill array with deck of cards
void createDeck(Card poolOfCards[]) {
	int index = 0;
	
	//Loop through all values and suits to make cards
	for (int cardVal = VALUE_TWO; cardVal <= VALUE_ACE; cardVal++) {
		for (int cardSuit = SUIT_CLUBS; cardSuit <= SUIT_SPADES; cardSuit++) {
			poolOfCards[index] = Card(static_cast<Value>(cardVal), 
										static_cast<Suit>(cardSuit));
			index++;
		}
	}
}

//Function to return a shuffled index for cards
void shuffleDeck(int shuffleIndex[], int numCards) {
	//Fill the array sequentially
	for (int i = 0; i < numCards; i++) {
		shuffleIndex[i] = i;
	}
	
	//Seed the RNG
	srand(time(0));
	
	//Shuffle the array
	//Some way to populate and shuffle in the same loop?
	for (int i = 0; i < numCards; i++) {
		int r = rand() % numCards;
		int tempVal = shuffleIndex[i];
		shuffleIndex[i] = shuffleIndex[r];
		shuffleIndex[r] = tempVal;		
	}
}

void dealCards(Card deckOfCards[], int shuffleIndex[],
				 deque<Card>& deckPlayer1, deque<Card>& deckPlayer2) {
				 	
	//Draw two cards and deal the first to P1, second to P2
	for (int i = 0; i < 52; i = i + 2) {
		deckPlayer1.push_front(deckOfCards[shuffleIndex[i]]);
		deckPlayer2.push_front(deckOfCards[shuffleIndex[i + 1]]);
	}
}

//Handle battles, and go to war if necessary
void joustCards(deque<Card>& deckPlayer1, deque<Card>& deckPlayer2) {
	//Prepare piles for when ties occur
	deque<Card> pileP1 = {};
	deque<Card> pileP2 = {};
	
	//While players have enough cards, play on
	while (!deckPlayer1.empty() && !deckPlayer2.empty()) {		
		//If P1 has a better card, win the battle and take the spoils
		if (deckPlayer1.front().getValue() > deckPlayer2.front().getValue()) {
			cout << "Player 1 wins this battle!\n";
			moveCardFromTo(deckPlayer1, "top", deckPlayer1, "bot");
			moveCardFromTo(deckPlayer2, "top", deckPlayer1, "bot");
		}
		
		//If P2 has a better card, win the battle and take the spoils
		else if (deckPlayer1.front().getValue() < deckPlayer2.front().getValue()) {
			cout << "Player 2 wins this battle!\n";
			moveCardFromTo(deckPlayer2, "top", deckPlayer2, "bot");
			moveCardFromTo(deckPlayer1, "top", deckPlayer2, "bot");
		}
		
		//If the battle is a tie, go to war
		else if (deckPlayer1.front().getValue() == deckPlayer2.front().getValue()){
			warCards(deckPlayer1, deckPlayer2, pileP1, pileP2);	
		}
	}
}

//Handle ties by going to war
void warCards(deque<Card>& deckPlayer1, deque<Card>& deckPlayer2,
				deque<Card>& pilePlayer1, deque<Card>& pilePlayer2) {
	cout << "Going to war!\n";
	cout << deckPlayer1.front().cardName() << " vs " << deckPlayer2.front().cardName() << "\n";
	
	//Take the top card to the bounty pile
	moveCardFromTo(deckPlayer1, "top", pilePlayer1, "top");
	moveCardFromTo(deckPlayer2, "top", pilePlayer2, "top");
	
	//Check to see if enough cards are left to war
	if (deckPlayer1.empty() && !deckPlayer2.empty()) {
		cout << "Player 1 is out of cards! Player 2 wins!\n";
	}
	else if (deckPlayer2.empty() && !deckPlayer1.empty()) {
		cout << "Player 2 is out of cards! Player 1 wins!\n";
	}
	else if (deckPlayer1.empty() && deckPlayer2.empty()) {
		cout << "Both players out of cards! Draw!\n";
	}
	
	//Go to war!
	else {
		//Check how many cards are available to war
		int numToWar = 4;
		if (deckPlayer1.size() < numToWar) {
			numToWar = deckPlayer1.size();
			cout << "Player 1 not enough, warring with " << numToWar << "\n";
		}
		if (deckPlayer2.size() < numToWar) {
			numToWar = deckPlayer2.size();
			cout << "Player 2 not enough, warring with " << numToWar << "\n";
		}
		
		//Stack up bounty cards (face down)
		for (numToWar; numToWar > 1; numToWar--) {
			moveCardFromTo(deckPlayer1, "top", pilePlayer1, "top");
			moveCardFromTo(deckPlayer2, "top", pilePlayer2, "top");
		}
	
//		cout << "Pile for player 1: ";
//		for (Card bountyCard : pilePlayer1) {
//			cout << bountyCard.cardName() << ". ";
//		}
//		cout << "\nPile for player 2: ";
//		for (Card bountyCard : pilePlayer2) {
//			cout << bountyCard.cardName() << ". ";
//		}
//		cout << "\n";
//		cout << "Top card P1: " << deckPlayer1.front().cardName() << "\n";
//		cout << "Top card P2: " << deckPlayer2.front().cardName() << "\n";
		
		//If P1 wins, reward the bounty
		if (deckPlayer1.front().getValue() > deckPlayer2.front().getValue()) {
			cout << "Player 1 wins the war!\n";
			while (!pilePlayer1.empty()) {
				moveCardFromTo(pilePlayer1, "top", deckPlayer1, "bot");
			}
			moveCardFromTo(deckPlayer1, "top", deckPlayer1, "bot");
			while (!pilePlayer2.empty()) {
				moveCardFromTo(pilePlayer2, "top", deckPlayer1, "bot");
			}
			moveCardFromTo(deckPlayer2, "top", deckPlayer1, "bot");
		}
		
		//If P2 wins, reward bounty
		else if (deckPlayer1.front().getValue() < deckPlayer2.front().getValue()) {
			cout << "Player 2 wins the war!\n";
			while (!pilePlayer2.empty()) {
				moveCardFromTo(pilePlayer2, "top", deckPlayer2, "bot");
			}
			moveCardFromTo(deckPlayer2, "top", deckPlayer2, "bot");
			while (!pilePlayer1.empty()) {
				moveCardFromTo(pilePlayer1, "top", deckPlayer2, "bot");
			}
			moveCardFromTo(deckPlayer1, "top", deckPlayer2, "bot");
		}
		
		//If tie again, perform recursion
		else if (deckPlayer1.front().getValue() == deckPlayer2.front().getValue()){
			warCards(deckPlayer1, deckPlayer2, pilePlayer1, pilePlayer2);			
		}
	}
	
}

//Handle moving cards from group top or bot to other group top or bot
void moveCardFromTo (deque<Card>& fromGroup, string fromLoc,
						deque<Card>& toGroup, string toLoc) {
	
	//Handle actions moving to top of deck							
	if (toLoc == "top") {
		if (fromLoc == "top") {
			//Move card from top of G1 to top of G2
			toGroup.push_front(fromGroup.front());
			fromGroup.pop_front();
		}
		else if (fromLoc == "bot") {
			//Move card from bot of G1 to top of G2
			toGroup.push_front(fromGroup.back());
			fromGroup.pop_back();
		}	
	}
	
	//Handle actions moving to bottom of deck
	else if (toLoc == "bot") {
		if (fromLoc == "top") {
			//Move card from top of G1 to bot of G2
			toGroup.push_back(fromGroup.front());
			fromGroup.pop_front();
		}
		else if (fromLoc == "bot") {
			//Move card from bot of G1 to bot of G2
			toGroup.push_back(fromGroup.back());
			fromGroup.pop_back();
		}
	}
}
