#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
/*
BLACKJACK GAME
Sarah O'Connor
G00423847
*/

//VARIABLES

//decks of cards
int oneDeck[52];
int twoDecks[104];
int fourDecks[208];

//player specific variables
int dealerCard1, dealerCard2, dealerCard1Value, dealerCard2Value, holeCard, holeCardValue, dealerScore = 0, dealerTurnCounter = 0, dealerBlackjack = 0; 
int card1Value, card2Value, player1Score = 0, player1Done = 0, playerTurnCounter = 0, player1Save;	//player 1
int card3Value, card4Value, player2Score = 0, player2Done = 0, player2Save;							//player 2
int card5Value, card6Value, player3Score = 0, player3Done = 0;										//player 3

//general variables
int cardValue, card, cardCounter = 0;
int numPlayers, numDecks, ranNum, playerCounter = 1, initialCards = 0, win = 0;
int playerChoice, newGame;

//file
FILE* outputFile;

//char* variables
char* cardFace;
char* card1Face;
char* card2Face;
char* card3Face;
char* card4Face;
char* card5Face;
char* card6Face;
char* cardSuit;
char* card1Suit;
char* card2Suit;
char* card3Suit;
char* card4Suit;
char* card5Suit;
char* card6Suit;
char* dealerCard1Face;
char* holeCardFace;
char* dealerCard1Suit;
char* holeCardSuit;

//DECLARE FUNCTIONS
char* getCardFace(int cardVar);
char* getCardSuit(int cardVar);
int getCardValue(int cardVar, int playerScore);
void instructions();
void populateDeck();
void shuffleCards();
void play();
void hit();
void checkScore(int score);
void playerTurn();
void dealerTurn();
void checkWin();
void saveGame();
void readFromSave();
void displayDetails();

void main() {	
	instructions();

	do {
		printf("Please enter 1 to continue an Existing Game, or 2 to start a New Game: \n");
		scanf("%d", &newGame);
	} while (newGame != 1 && newGame != 2);

	if (newGame == 1) {
		readFromSave();
		populateDeck();
		shuffleCards();
		displayDetails();
		printf("\n\n");

		if (player1Done == 0 && numPlayers > 1) {
			player2Score = 0;
			player3Score = 0;

			hit(player2Score);
			card3Value = cardValue;
			card3Suit = cardSuit;
			card3Face = cardFace;
			player2Score += cardValue;

			hit(player2Score);
			card4Value = cardValue;
			card4Suit = cardSuit;
			card4Face = cardFace;
			player2Score += cardValue;

			if (numPlayers == 3) {
				hit(player3Score);
				card5Value = cardValue;
				card5Suit = cardSuit;
				card5Face = cardFace;
				player3Score += cardValue;

				hit(player3Score);
				card6Value = cardValue;
				card6Suit = cardSuit;
				card6Face = cardFace;
				player3Score += cardValue;
			}//if 3 players
		}//reinitialise player 2 and 3 starting cards if their turn was not reached
		else if (player2Done == 0 && numPlayers > 2) {
			player3Score = 0;

			hit(player3Score);
			card5Value = cardValue;
			card5Suit = cardSuit;
			card5Face = cardFace;
			player3Score += cardValue;

			hit(player3Score);
			card6Value = cardValue;
			card6Suit = cardSuit;
			card6Face = cardFace;
			player3Score += cardValue;
		}//if player 2 did not finish turn and there is 3 players 

		//player 1 turn
		if (player1Done == 0) {
			printf("\n - - Player 1's Turn - - \n\n");
			printf("Player 1 Score: %d\n", player1Score);
			if (dealerBlackjack == 1) {
				win = 1;
				if (player1Score == 21) {
					printf("Player 1 got a Blackjack, with a score of %d!\n", player1Score);
					printf("\nPlayer 1 and Dealer tie!\n\n");
				}//if dealer and player blackjack
			}//if dealer got a blackjack
			else {
				checkScore(player1Score);

				while (playerChoice == 1 || playerChoice == 3) {
					if (playerChoice == 3)
						displayDetails();
					else {
						hit(player1Score);
						player1Score += cardValue;
						printf("Player 1 score: %d\n", player1Score);
					}//if player choice == 1
					checkScore(player1Score);
				}//while choosing to hit
				if (player1Score >= 21)
					player1Done = 1;

				if (playerChoice == 2 && win != 1) {
					player1Done = 1;
					printf("Player Stands . . .\n");
				}//if standing
				else if (playerChoice == 4) {
					printf("Saving Game File . . .\n");
					saveGame();
					printf("Game Saved!\n");
					exit(0);
				}//if saving game
				else if (playerChoice == 5) {
					printf("Exiting game without saving . . .\n");
					exit(0);
				}//exit game
			}//else no blackjack - continue turn as normal
		}//if player 1 did not finish turn
		else
			printf("Player 1: %d points \n", player1Score); //if player 1 did finish turn
		initialCards = 1;
		if (numPlayers > 1) {
			if (player2Done == 0 && numPlayers > 1) {
				printf("\n - - Player 2's Turn - - \n\n");

				if (dealerBlackjack == 1) {
					win = 1;
					if (player2Score == 21) {
						printf("Player 2 Score: %d\n", player2Score);
						printf("Player 2 got a Blackjack, with a score of %d!\n", player2Score);
						printf("\nPlayer 2 and Dealer tie!\n\n");
					}//if dealer and player blackjack
				}//if dealer got blackjack
				else if (player1Save == 0) {
					printf("Card pulled: %s of %s, worth %d points.\n", card3Face, card3Suit, card3Value);
					printf("Card pulled: %s of %s, worth %d points.\n", card4Face, card4Suit, card4Value);
					printf("Player 2 score: %d\n", player2Score);
					checkScore(player2Score);

					while (playerChoice == 1 || playerChoice == 3) {
						if (playerChoice == 3)
							displayDetails();
						else {
							hit(player2Score);
							player2Score += cardValue;
							printf("Player 2 score: %d\n", player2Score);
						}//if playerChoice == 1
						checkScore(player2Score);
					}//while choosing to hit or display details
					if (playerChoice == 2 && win != 1) {
						player2Done = 1;
						printf("Player Stands . . .\n");
					}//if standing
					else if (playerChoice == 4) {
						printf("Saving Game File . . .\n");
						saveGame();
						printf("Game Saved!\n");
						exit(0);
					}//if saving game
					else if (playerChoice == 5) {
						printf("Exiting game without saving . . .\n");
						exit(0);
					}//exit game
				}//if turn did not start at all - begin from scratch
				else {
					printf("Player 2 Score: %d\n", player2Score);
					checkScore(player2Score);

					while (playerChoice == 1 || playerChoice == 3) {
						if (playerChoice == 3)
							displayDetails();
						else {
							hit(player2Score);
							player2Score += cardValue;
							printf("Player 2 score: %d\n", player2Score);
						}//if playerChoice == 1
						checkScore(player2Score);
					}//while choosing to hit or display details
		
					if (player2Score > 21)
						player2Done = 1;

					if (playerChoice == 2 && win != 1) {
						player2Done = 1;
						printf("Player Stands . . .\n");
					}//if standing
					else if (playerChoice == 4) {
						printf("Saving Game File . . .\n");
						saveGame();
						printf("Game Saved!\n");
						exit(0);
					}//if saving game
					else if (playerChoice == 5) {
						printf("Exiting game without saving . . .\n");
						exit(0);
					}//exit game
				}//if turn had started but not finished
			}//else if player 2 did not finish turn
			else
				printf("Player 2: %d points \n", player2Score); //if player 2 did finish turn

			if(numPlayers == 3) {
				if (player3Done == 0) {
					printf("\n - - Player 3's Turn - - \n\n");

					if (dealerBlackjack == 1) {
						win = 1;
						if (player3Score == 21) {
							printf("Player 3 Score: %d\n", player3Score);
							printf("Player 3 got a Blackjack, with a score of %d!\n", player3Score);
							printf("\nPlayer 3 and Dealer tie!\n\n");
						}//if dealer and player blackjack
					}//if dealer got blackjack
					else if (player2Save == 0) {
						printf("Card pulled: %s of %s, worth %d points.\n", card5Face, card5Suit, card5Value);
						printf("Card pulled: %s of %s, worth %d points.\n", card6Face, card6Suit, card6Value);
						printf("Player 3 score: %d\n", player3Score);
						checkScore(player3Score);

						while (playerChoice == 1 || playerChoice == 3) {
							if (playerChoice == 3)
								displayDetails();
							else {
								hit(player3Score);
								player3Score += cardValue;
								printf("Player 3 score: %d\n", player3Score);
							}//if playerChoice == 1
							checkScore(player3Score);
						}//while choosing to hit or display details

						if (playerChoice == 2 && win != 1) {
							player3Done = 1;
							printf("Player Stands . . .\n");
						}//if standing
						else if (playerChoice == 4) {
							printf("Saving Game File . . .\n");
							saveGame();
							printf("Game Saved!\n");
							exit(0);
						}//if saving game
						else if (playerChoice == 5) {
							printf("Exiting game without saving . . .\n");
							exit(0);
						}//exit game
					}//if turn did not start at all - begin from scratch
					else {
						printf("Player 3 Score: %d\n", player3Score);
						checkScore(player3Score);

						while (playerChoice == 1 || playerChoice == 3) {
							if (playerChoice == 3)
								displayDetails();
							else {
								hit(player3Score);
								player3Score += cardValue;
								printf("Player 3 score: %d\n", player3Score);
							}//if playerChoice == 1
							checkScore(player3Score);
						}//while choosing to hit or display details

						if (player3Score > 21)
							player3Done = 1;

						if (playerChoice == 2 && win != 1) {
							player3Done = 1;
							printf("Player Stands . . .\n");
						}//if standing
						else if (playerChoice == 4) {
							printf("Saving Game File . . .\n");
							saveGame();
							printf("Game Saved!\n");
							exit(0);
						}//if saving game
						else if (playerChoice == 5) {
							printf("Exiting game without saving . . .\n");
							exit(0);
						}//exit game
					}//else if continuing turn
				}//if(player3Done == 0)
				else
					printf("Player 3: %d points \n", player3Score); //if player 3 did finish turn
			}//if 3 players
		}//if more than 1 player

		//dealer turn
		if(dealerBlackjack != 1)
			dealerTurn();
		//check winner
		checkWin();
	}//if - continuing old game file
	
	else {
		do {
			printf("Please enter number of Players (1-3): \n");
			scanf("%d", &numPlayers);
		} while (numPlayers < 1 || numPlayers > 3);
		do {
			printf("Player 1: Please input number of decks to use (1, 2, or 4):\n");
			scanf("%d", &numDecks);
		} while (numDecks != 1 && numDecks != 2 && numDecks != 4);

		populateDeck();
		shuffleCards();
		play();
	}//else start new game
}//main

void instructions() {
	//intro to the game
	printf(" - - - B L A C K J A C K - - -\n");
	printf("- - - - - - - - - - - - - - - -\n\n");
	printf("Welcome to Blackjack!\n");
	printf("Try to beat the Dealer by scoring as close to 21 points as possible - WITHOUT going Bust!\n\n\n");
}//intructions()

void populateDeck() {
	if (numDecks == 1) {
		for (int i = 0; i < 52; i++)
			oneDeck[i] = i + 1;
	}//1 deck being used
	else if (numDecks == 2) {
		for (int i = 0; i < 104; i++)
			twoDecks[i] = i + 1;
	}//2 decks being used
	else {
		for (int i = 0; i < 208; i++)
			fourDecks[i] = i + 1;
	}//4 decks being used
}//populateDeck()

void shuffleCards() {
	srand(time(NULL));// setting time seed
	if (numDecks == 1) {
		printf("Dealer is shuffling the deck . . .\n\n");
		for (int i = 0; i < 52; i++) {
			int randomNum = rand() % 51 + 1;
			int cardToSwap = oneDeck[i];
			oneDeck[i] = oneDeck[randomNum];
			oneDeck[randomNum] = cardToSwap;
		}//for loop - to choose random card to swap with index i - iterates 52 times
	}//if - 1 deck being used
	else if (numDecks == 2) {
		printf("Dealer is shuffling two decks . . .\n\n");
		for (int i = 0; i < 104; i++) {
			int randomNum = rand() % 103 + 1;
			int cardToSwap = twoDecks[i];
			twoDecks[i] = twoDecks[randomNum];
			twoDecks[randomNum] = cardToSwap;
		}//for loop - to choose random card to swap with index i - iterates 104 times
	}//else if - using 2 decks of cards
	else {
		printf("Dealer is shuffling four decks . . .\n\n");
		for (int i = 0; i < 208; i++) {
			int randomNum = rand() % 207 + 1;
			int cardToSwap = fourDecks[i];
			fourDecks[i] = fourDecks[randomNum];
			fourDecks[randomNum] = cardToSwap;
		}//for loop - to choose random card to swap with index i - iterates 208 times
	}//else - using 4 decks of cards
}//shuffleCards

void play() {
	printf("Dealer gives 2 cards to himself, and 2 cards to each player.\n");

	//get all card values first
	hit(player1Score);
	card1Value = cardValue;
	card1Suit = cardSuit;
	card1Face = cardFace;
	player1Score += card1Value;

	hit(player1Score);
	card2Value = cardValue;
	card2Suit = cardSuit;
	card2Face = cardFace;
	player1Score += card2Value;

	if (numPlayers > 1) {
		hit(player2Score);
		card3Value = cardValue;
		card3Suit = cardSuit;
		card3Face = cardFace;
		player2Score += cardValue;

		hit(player2Score);
		card4Value = cardValue;
		card4Suit = cardSuit;
		card4Face = cardFace;
		player2Score += cardValue;

		if (numPlayers == 3) {
			hit(player3Score);
			card5Value = cardValue;
			card5Suit = cardSuit;
			card5Face = cardFace;
			player3Score += cardValue;

			hit(player3Score);
			card6Value = cardValue;
			card6Suit = cardSuit;
			card6Face = cardFace;
			player3Score += cardValue;
		}//if 3 players
	}//if two players

	//dealer cards
	hit(dealerScore);
	dealerCard1Value = cardValue;
	dealerCard1Suit = cardSuit;
	dealerCard1Face = cardFace;
	dealerScore += dealerCard1Value;

	//pull hole card
	hit(dealerScore);
	holeCardValue = cardValue;
	holeCardSuit = cardSuit;
	holeCardFace = cardFace;
	dealerScore += holeCardValue;

	initialCards = 1; //first cards have been initialised

	//dealer turn
	dealerTurn();

	//player turns
	for (int i = 0; i < numPlayers; i++){
		playerTurn();
		playerCounter++;
	}//for loop - iterates through for each player
	
	//dealer turn
	if (dealerBlackjack != 1)
		dealerTurn();

	checkWin();
}//play()

void playerTurn() {
	printf("\n - - Player %d's Turn - - \n", playerCounter);
	if (playerCounter == 1) {
		if (dealerBlackjack == 1) {
			win = 1;
			printf("Player's first card: %s of %s, worth %d points.\n", card1Face, card1Suit, card1Value);
			printf("Player's second card: %s of %s, worth %d points.\n", card2Face, card2Suit, card2Value);
			
			if (player1Score == 21) {
				printf("Player got a Blackjack, with a score of %d!\n", player1Score);
				printf("\nPlayer and Dealer tie!\n\n");
			}//if blackjack
			else
				printf("Player Score: %d\n\n", player1Score);
		}//if blackjack has been found
		else {
			printf("Card pulled: %s of %s, worth %d points.\n", card1Face, card1Suit, card1Value);
			printf("Card pulled: %s of %s, worth %d points.\n", card2Face, card2Suit, card2Value);
			printf("Player %d score: %d\n", playerCounter, player1Score);
			checkScore(player1Score);
			if (player1Score == 21)
				player1Done = 1;

			while (playerChoice == 1 || playerChoice == 3) {
				if (playerChoice == 3)
					displayDetails();
				else {
					hit(player1Score);
					player1Score += cardValue;
					printf("Player 1 score: %d\n", player1Score);
				}//if playerChoice == 1
				checkScore(player1Score);
			}//while choosing to hit or display details
			if (playerChoice == 2 && win != 1) {
				player1Done = 1;
				printf("Player Stands . . .\n");
			}//if standing
			else if (playerChoice == 4) {
				printf("Saving Game File . . .\n");
				saveGame();
				printf("Game Saved!\n");
				exit(0);
			}//if saving game
			else if (playerChoice == 5) {
				printf("Exiting game without saving . . .\n");
				exit(0);
			}//exit game
		}//if not - play as normal
	}//if player 1
	else if (playerCounter == 2) {
		if (dealerBlackjack == 1) {
			win = 1;
			printf("Player's first card: %s of %s, worth %d points.\n", card3Face, card3Suit, card3Value);
			printf("Player's second card: %s of %s, worth %d points.\n", card4Face, card4Suit, card4Value);
			
			if (player2Score == 21) {
				printf("Player got a Blackjack, with a score of %d!\n", player2Score);
				printf("\nPlayer and Dealer tie!\n\n");
			}//if blackjack
			else {
				printf("Player Score: %d\n\n", player2Score);
			}//if no player blackjack
		}//if blackjack has been found
		else {
			printf("Card pulled: %s of %s, worth %d points.\n", card3Face, card3Suit, card3Value);
			printf("Card pulled: %s of %s, worth %d points.\n", card4Face, card4Suit, card4Value);
			printf("Player 2 score: %d\n", player2Score);
			checkScore(player2Score);

			while (playerChoice == 1 || playerChoice == 3) {
				if (playerChoice == 3)
					displayDetails();
				else {
					hit(player2Score);
					player2Score += cardValue;
					printf("Player 2 score: %d\n", player2Score);
				}//if playerChoice == 1
				checkScore(player2Score);
			}//while choosing to hit or display details

			if (playerChoice == 2 && win != 1) {
				player2Done = 1;
				printf("Player Stands . . .\n");
			}//if standing
			else if (playerChoice == 4) {
				printf("Saving Game File . . .\n");
				saveGame();
				printf("Game Saved!\n");
				exit(0);
			}//if saving game
			else if (playerChoice == 5) {
				printf("Exiting game without saving . . .\n");
				exit(0);
			}//exit game
		}//if not - play as normal
	}//player 2
	else  {
		if (dealerBlackjack == 1) {
			win = 1;
			printf("Player's first card: %s of %s, worth %d points.\n", card5Face, card5Suit, card5Value);
			printf("Player's second card: %s of %s, worth %d points.\n", card6Face, card6Suit, card6Value);
			
			if (player3Score == 21) {
				printf("Player got a Blackjack, with a score of %d!\n", player3Score);
				printf("\nPlayer and Dealer tie!\n\n");
			}//if blackjack
			else {
				printf("Player Score: %d\n\n", player3Score);
			}//if no player blackjack
		}//if blackjack has been found
		else {
			printf("Card pulled: %s of %s, worth %d points.\n", card5Face, card5Suit, card5Value);
			printf("Card pulled: %s of %s, worth %d points.\n", card6Face, card6Suit, card6Value);
			printf("Player 3 score: %d\n", player3Score);
			checkScore(player3Score);

			while (playerChoice == 1 || playerChoice == 3) {
				if (playerChoice == 3)
					displayDetails();
				else {
					hit(player3Score);
					player3Score += cardValue;
					printf("Player 3 score: %d\n", player3Score);
				}//if playerChoice == 1
				checkScore(player3Score);
			}//while choosing to hit or display details

			if (playerChoice == 2 && win != 1) {
				player3Done = 1;
				printf("Player Stands . . .\n");
			}//if standing
			else if (playerChoice == 4) {
				printf("Saving Game File . . .\n");
				saveGame();
				printf("Game Saved!\n");
				exit(0);
			}//if saving game
			else if (playerChoice == 5) {
				printf("Exiting game without saving . . .\n");
				exit(0);
			}//exit game
		}//if not - play as normal
	}//player 3
}//playerTurn()

void dealerTurn() {
	printf("\n - - Dealer's Turn - -\n");

	if(dealerTurnCounter == 0) {
		printf("The Dealer turns over his first card. . .\n");
		printf("Card pulled: %s of %s, worth %d points.\n", dealerCard1Face, dealerCard1Suit, dealerCard1Value);
		
		if (dealerCard1Value == 10 || dealerCard1Value == 11) {
			printf("Dealer peaks at Hole Card . . .\n");

			if (holeCardValue == 10 && dealerCard1Value == 11) {
				printf("Dealer's Hole Card was %s of %s, valued at %d points. \n", holeCardFace, holeCardSuit, holeCardValue);
				printf("Dealer has a Blackjack, with a score of %d!\n", dealerScore);
				win = 1;
				dealerBlackjack = 1;
			}//blackjack
			else if (holeCardValue == 11 && dealerCard1Value == 10) {
				printf("Dealer's Hole Card was %s of %s, valued at %d points. \n", holeCardFace, holeCardSuit, holeCardValue);
				printf("Dealer has a Blackjack, with a score of %d!\n", dealerScore);
				win = 1;
				dealerBlackjack = 1;
			}//blackjack
			else {
				printf("Dealer does not turn over Hole Card.\n\n");
			}//not a blackjack
		}//if first card is a 10 or 11 - check for blackjack
		else {
			printf("Dealer Score: %d \n", dealerCard1Value);
		}//else no blackjack
	}//if first turn

	else if(dealerTurnCounter == 1) {
		//dealer will turn over hole card on second turn
		printf("Dealer turns over Hole Card. . .\n");
		if (newGame == 2)
			printf("Dealer's Hole Card is %s of %s, valued at %d points. \n", holeCardFace, holeCardSuit, holeCardValue);
		else
			printf("Dealer's Hole Card is valued at %d points.\n", holeCardValue);

		printf("Dealer Score: %d\n", dealerScore);

		if (dealerScore < 17) {
			printf("Dealer Hits . . .\n");
			hit(dealerScore);
			dealerScore += cardValue;
			printf("Dealer Score: %d\n", dealerScore);
		}//dealer hits
		else {
			printf("Dealer Stands . . .\n");
		}//if dealer stands (score > 16)
	}//if not first turn
	dealerTurnCounter++;
}//dealerTurn()

void hit(int score) {
	if (numDecks == 1) {
		ranNum = rand() % 51 + 1;
		card = oneDeck[ranNum];
	}//if using 1 deck
	else if (numDecks == 2) {
		ranNum = rand() % 103 + 1;
		card = twoDecks[ranNum];
	}//if using 2 decks
	else {
		ranNum = rand() % 207 + 1;
		card = fourDecks[ranNum];
	}//if using 4 decks
	
	cardFace = getCardFace(card);
	cardSuit = getCardSuit(card);
	cardValue = getCardValue(card, score);

	if(initialCards != 0)
		printf("Card pulled: %s of %s, worth %d points.\n", cardFace, cardSuit, cardValue);
}//hit()

char* getCardFace(int cardVar) {
	int result = cardVar % 13 + 1;
	char* face;

	switch (result) {
		case 1:
			face = "Ace";
			break;
		case 2:
			face = "Two";
			break;
		case 3:
			face = "Three";
			break;
		case 4:
			face = "Four";
			break;
		case 5:
			face = "Five";
			break;
		case 6:
			face = "Six";
			break;
		case 7:
			face = "Seven";
			break;
		case 8:
			face = "Eight";
			break;
		case 9:
			face = "Nine";
			break;
		case 10:
			face = "Ten";
			break;
		case 11:
			face = "Jack";
			break;
		case 12:
			face = "Queen";
			break;
		case 13:
			face = "King";
			break;
	}//switch - set card face
	
	return face;
}//cardFace()

char* getCardSuit(int cardVar) {
	int result = cardVar % 4 + 1;
	char* suit;
	switch (result) {
		case 1:
			suit = "Spades";
			break;
		case 2:
			suit = "Clubs";
			break;
		case 3:
			suit = "Hearts";
			break;
		case 4:
			suit = "Diamonds";
			break;
	}//switch - find suit

	return suit;
}//cardSuit()

int getCardValue(int cardVar, int playerScore) {
	int result;
	if (((cardVar % 13) + 1) == 1) {
		if (playerScore < 11)
			result = 11;
		else
			result = 1;
	}//if ace - either add 11 or 1 depending on current score
	else if (((cardVar % 13) + 1) < 11)
		result = cardVar % 13 + 1;
	else
		result = 10;

	return result;
}//cardValue()

void checkScore(int score) {
	if (score < 21) {
		printf("1 - Hit \n2 - Stand \n3 - Display Game Status \n4 - Save and Quit \n5 - Quit without Saving\n");
		printf("What would you like to do?\n");
		scanf("%d", &playerChoice);
	}// if score is less than 21
	else if (score == 21) {
		win = 1;
		if (card1Value == 10 || card2Value == 10) {
			if (card1Value == 11 || card2Value == 11) {
				printf("BlackJack!\n");
			}//if blackjack
		}//checking for blackjack
		playerChoice = 2;
		printf("You got 21!\n");
	}//if score == 21
	else {
		playerChoice = 2;
		printf("Bust! Score: %d \n", score);
		win = 1;
	}//if bust
}//checkScore()

void checkWin() {
	printf("\n");
	//FIND WINNER
	if (numPlayers == 1) {
		if (dealerScore > player1Score && dealerScore < 22)
			printf("Dealer Wins!\n\n"); //dealer wins
		else if (player1Score > dealerScore && player1Score < 22)
			printf("Player 1 Wins!\n\n"); //player 1 wins
		else if (dealerScore > 21 && player1Score > 21)
			printf("Dealer and Player Bust! Nobody wins.\n\n");
		else if (dealerScore == player1Score && dealerScore < 22)
			printf("Dealer and Player 1 Tie!\n\n");
		else if (dealerScore > 21 && player1Score < 22)
			printf("Dealer Bust! Player 1 Wins\n\n");
		else if (player1Score > 21 && dealerScore < 22)
			printf("Player Bust! Dealer Wins\n\n");
	}//if 1 player
	else {
		if (dealerScore > 21 && player1Score > 21 && player2Score > 21) {
			if (numPlayers == 2 || player3Score > 21)
				printf("Everyone Bust! Nobody wins\n\n");
		}//if everyone bust
		else {
			if (dealerScore > 21) {
				printf("Dealer Bust!\n\n");
				dealerScore = 0;
			}//dealer bust
			if (player1Score > 21) {
				printf("Player 1 Bust!\n\n");
				player1Score = 0;
			}//player 1 bust
			if (player2Score > 21) {
				printf("Player 2 Bust!\n\n");
				player2Score = 0;
			}//player 2 bust
			if (player3Score > 21) {
				printf("Player 3 Bust!\n\n");
				player3Score = 0;
			}//player 3 bust
		}//else not everyone bust

		if (player1Score > dealerScore && player1Score > player2Score && player1Score > player3Score && player1Score < 22)
			printf("Player 1 Wins!\n\n"); //player 1 wins
		else if (player2Score > dealerScore && player2Score > player1Score && player2Score > player3Score && player2Score < 22)
			printf("Player 2 Wins!\n\n"); //player 2 wins
		else if (player3Score > dealerScore && player3Score > player1Score && player3Score > player2Score && player3Score < 22)
			printf("Player 3 Wins!\n\n"); //player 3 wins
		else if (dealerScore > player1Score && dealerScore > player2Score && dealerScore > player3Score && dealerScore < 22)
			printf("Dealer Wins!\n\n"); //dealer wins
		
		if (dealerScore == player1Score || dealerScore == player2Score || dealerScore == player3Score && dealerScore != 0) {
			if (dealerScore == player1Score && dealerScore != 0 && dealerScore >= player2Score && dealerScore >= player3Score)
				printf("Tie between Dealer and Player 1!\n\n");
			if (numPlayers > 1) {
				if (dealerScore == player2Score && dealerScore != 0 && dealerScore >= player1Score && dealerScore >= player3Score)
					printf("Tie between Dealer and Player 2!\n\n");
				if (numPlayers > 2) {
					if (dealerScore == player3Score && dealerScore != 0 && dealerScore >= player1Score && dealerScore >= player2Score)
						printf("Tie between Dealer and Player 3!\n\n");
				}//if more than 2 players
			}//if more than 1 players
		}//if dealer ties with players
		if (player1Score == player2Score || player1Score == player3Score && player1Score != 0) {
			if (player1Score == player2Score && player1Score == player3Score && player1Score != 0)
				printf("Tie between All Players!\n\n");
			else if (player1Score == player2Score && player1Score != 0 && player1Score > dealerScore && player1Score > player3Score)
				printf("Tie between Player 1 and Player 2!\n\n");
			else if (player1Score == player3Score && player1Score != 0 && player1Score > dealerScore && player1Score > player2Score)
				printf("Tie between Player 1 and Player 3!\n\n");
		}//tie between players
		else if (player2Score == player3Score && player2Score != 0 && player2Score > dealerScore && player2Score > player1Score)
			printf("Tie between Player 2 and Player 3!\n\n");//tie player2 and player3
	}//else more than 1 players
}//checkWin()

void saveGame() {
	// open file
	outputFile = fopen("SaveFile.txt", "w"); 

	//save all variables into file - line by line
	fprintf(outputFile, "%d\n", numPlayers);
	fprintf(outputFile, "%d\n", numDecks);

	//dealer info
	fprintf(outputFile, "%d\n", dealerScore);
	fprintf(outputFile, "%d\n", dealerTurnCounter);
	fprintf(outputFile, "%d\n", dealerCard1Value);
	fprintf(outputFile, "%d\n", holeCardValue);
	fprintf(outputFile, "%d\n", dealerBlackjack);
	
	//player1 info
	fprintf(outputFile, "%d\n", player1Score);
	fprintf(outputFile, "%d\n", player1Done);

	if (numPlayers > 1) {
		//player2 info
		fprintf(outputFile, "%d\n", player2Score);
		fprintf(outputFile, "%d\n", player2Done);

		if (numPlayers == 3) {
			//player3 info
			fprintf(outputFile, "%d\n", player3Score);
			fprintf(outputFile, "%d\n", player3Done);
		}//if 3 players - print player 3 info to file
	}// if two or more players
	//close file
	fclose(outputFile);
}//saveGame()

void readFromSave() {
	// open file
	outputFile = fopen("SaveFile.txt", "r");
	if (outputFile == NULL) {
		printf("Error - Save File not found\n");
		exit(0);
	}//if no save file
	else {
		//num players and decks
		fscanf(outputFile, "%d", &numPlayers);
		fscanf(outputFile, "%d", &numDecks);

		//dealer info
		fscanf(outputFile, "%d\n", &dealerScore);
		fscanf(outputFile, "%d\n", &dealerTurnCounter);
		fscanf(outputFile, "%d\n", &dealerCard1Value);
		fscanf(outputFile, "%d\n", &holeCardValue);
		fscanf(outputFile, "%d\n", &dealerBlackjack);

		//player1 info
		fscanf(outputFile, "%d\n", &player1Score);
		fscanf(outputFile, "%d\n", &player1Done);

		if (numPlayers > 1) {
			//player2 info
			fscanf(outputFile, "%d\n", &player2Score);
			fscanf(outputFile, "%d\n", &player2Done);

			if (numPlayers == 3) {
				//player3 info
				fscanf(outputFile, "%d\n", &player3Score);
				fscanf(outputFile, "%d\n", &player3Done);
			}//if 3 players - print player 3 info to file
		}// if two or more players
		
		player1Save = player1Done;
		player2Save = player2Done;
		//close file
		fclose(outputFile);
	}//read from file
}//readFromSave()

void displayDetails() {
	printf("\nCurrent Game Status: \n\n");
	printf("Number of Players: %d\n", numPlayers);
	printf("Number of Decks of Cards being used: %d\n", numDecks);

	if (dealerTurnCounter == 1)
		printf("Dealer Score: %d\n", dealerCard1Value);
	else
		printf("Dealer Score: %d\n", dealerScore);

	printf("Player 1 turn finished: ");
	if (player1Done == 1) {
		printf("Yes\n");
		printf("Player 1 Score: %d\n", player1Score);
	}//if player 1 turn is finished
	else
		printf("No\n");


	if (numPlayers > 1) {
		printf("Player 2 turn finished: ");
		if (player2Done == 1) {
			printf("Yes\n");
			printf("Player 2 Score: %d\n", player2Score);
		}//if player 2 turn is finished
		else
			printf("No\n");

		if (numPlayers == 3) {
			printf("Player 3 turn finished: ");
			if (player3Done == 1) {
				printf("Yes\n");
				printf("Player 3 Score: %d\n", player3Score);
			}//if player 3 turn is finished
			else
				printf("No\n");
		}//if 3 players
	}//if more than 1 player
}//displayDetails()