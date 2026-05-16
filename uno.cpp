#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

void clearScreen()
{
	system("CLS");
}
void DrawCards(int &pcards, int phands[][108], int deck[][15], int player, int drawcards, int &topCard, int discardedPile[], int &discardSize);


void arrangedeck(int phands[][108],int player);

void initializeDeck(int deck[][15]);
	
void shuffleDeck(int deck[][15]);

void dealCards(int deck[][15], int phands[][108], int &topcard, int discardedPile[],int &discardSize);

bool isValidPlay(int playerCard, int topCard);

void playTurn(bool playCard, int &topCard, int playerCard,int totalcards[][15], int phands[][108], int& player, int &p1cards, int &p2cards, int discardedPile[],int &discardSize, int &pcards);

void handleSpecialCard(int specialCardType, int phands[][108], int totalcards[][15], int &player, int &pcards, int &topCard);
	
void saveLoadGameResult(int &p1wins, int &p2wins, bool save);

bool callUno(int &p1cards, int &p2cards, int &player);

void UpdateDiscardPile(int discardedPile[], int &discardSize, int deck[][15], int &topCard);

void PrintPlayerHand(int phands[][108],int topcard,int pcard, int player);

void MainMenu();

void print_cards(int phand[][108], int player, int pcard,int topcard);

void setColor(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}
void print_cards(int phand[][108], int player, int pcard);

void print_cards(int topcard);

//global variables & constants
const int BLACK = 0;
const int BLUE = 1;
const int GREEN = 2;
const int CYAN = 3;
const int RED = 4;
const int MAGENTA = 5;
const int YELLOW = 6;
const int WHITE = 7;

int main()
{
	int totalcards[9][15];
	int phands[2][108];
	int discardedPile[108];
	int discardSize=0;
	int p1cards=7;
	int p2cards=7;
	int topCard, playerCard, playercardcolor, playercardtype,player,specialCard, p1wins=0,p2wins=0,p1losses=0,p2losses=0;
	bool playCard;
	player = 0;
	
	MainMenu();

}
void MainMenu()
{
	int deck[9][15];
	int phands[2][108];
	int discardedPile[108];
	int discardSize=0;
	int p1cards=7;
	int p2cards=7;
	int topCard, playerCard, playercardcolor, playercardtype,player,specialCard, p1wins,p2wins,p1losses,p2losses,pcards;
	bool playCard;
	char choice;
	
	
	setColor(BLUE, BLACK);
	cout<<"WELCOME ";
	setColor(YELLOW, BLACK);
	cout<<"TO ";
	setColor(RED, BLACK);
	cout<<"UNO"; 
	setColor(GREEN, BLACK);
	cout<<"!!!\n";
	setColor(WHITE, BLACK);
	flag:
	cout<<"\nP to play\nS to view scoreboard\n\n";
	cin>>choice;
	
	switch (choice)
	{
	case 'p':
	case 'P': 
	clearScreen();
	player = 0;
	initializeDeck(deck);
    shuffleDeck(deck);
    dealCards(deck, phands, topCard, discardedPile, discardSize);
    
    while (p1cards!=0 && p2cards!=0)
    {
    	cout<<endl;
    	cout<<"=====10 SKIP || 11 REVERSE || 12 DRAW +2 || 13 WILD CARD || 14 WILD DRAW +4====="<<endl<<endl;
 		playTurn( playCard,  topCard,  playerCard,  deck, phands,  player, p1cards, p2cards, discardedPile, discardSize,pcards);
 		clearScreen();
   		player++;
   		 if (player>=2)
    	{
           	player=0;
		}
	}
	if (p1cards==0)
	{
		cout<<"PLAYER 1 WINS !!!!\n\n";
		p1wins++;
		cout<<"P1 Wins: "<<p1wins<<"  P2 Wins: "<<p2wins<<endl<<endl;
		saveLoadGameResult(p1wins, p2wins,true);
	}
	else if (p2cards==0)
	{
		cout<<"PLAYER 2 WINS !!!!\n\n";
		p2wins++;
		cout<<"P1 Wins: "<<p1wins<<"  P2 Wins: "<<p2wins<<endl<<endl;
		saveLoadGameResult(p1wins, p2wins,true);
	}
					break;
		case 's':
		case 'S': 	clearScreen();
					saveLoadGameResult(p1wins, p2wins, false);
					cout<<"P1 Wins: "<<p1wins<<"  P2 Wins: "<<p2wins<<endl<<endl;
					break;
		default: 	cout<<"\nInvalid choice. Enter again\n";
					goto flag;
			
	}
}
void saveLoadGameResult(int &p1wins, int &p2wins, bool save) 
{

    if (save) //to check whether we want to save or load the results
	{
        ofstream outFile("winlossrecord.txt");//declaring the ofstream datatype this will allow us to save our data
        if (outFile.is_open()) //checks if file is open (files needs to b open to b worked on)
		{
            outFile <<p1wins<< " "<< p2wins<<endl;//this will save the values in a textfile
            outFile.close();//closes the file
        } 
    } 
	else //this will load the results
	{
        ifstream inFile("winlossrecord.txt");
        if (inFile.is_open()) //checks if file is open
		{
            inFile >> p1wins >> p2wins;//takes the data stored in the text file
            inFile.close();//closes the file
        } 
		else 
		{
            //when file doesnt exist it gives the value 0 to the wins
            p1wins=p2wins= 0;
        }
    }
}

void initializeDeck(int deck[][15])
{
	//rows represent colors, 2 for each color and 1 for wild
	int rowsColors=9;
	//column represents cards; numbered (0-9), special cards(10-14)
	int colType=15;
	
	int temp=0;
	int mod=0;
	int color=100; //100 for red 200 for yellow 300 for blue 400 for green
	

	//for incrementing in color/row
	for (int i=0;i<9;i++)
	{
		//this will assign the deck numbers from 0-14
		for (int r=0; r<15; r++)
		{
			//16 represents null and this will be filtered out
			deck[i][r]=16;
			
			//for the cards excluding wild and drawfour category
			if (i<8)
			{
				//since total type of cards is 15 but for colors red blue yellow and green we do not want to add wild or draw four there.	//since there are only 1 zero card for each color but there are 2 rows for each color, this will allow for storing 0 in alternate row
				mod=i%2;
				if (mod!=0)
				{
					temp=r+1+color;
					if(temp>=13+color)
					{
						deck[i][r]=16;
					}
					else
					{
						deck[i][r]=temp;
					}
				}
				else
				{
					if(r>=13)
					{
						deck[i][r]=16;
					}
					else
					{
						deck[i][r]=r+color;
					}
				}
			}
			else
			{
				for (int k=0;k<8;k++)
				{
					deck[i][k]=13;
					
					if (k>3)
					{
						deck[i][k]=14;
					}
				}
			}

		}
		if ((i+1)%2==0)
		{
			color+=100;		
		}
	}
}	



void print_cards(int phand[][108], int player, int pcard,int topcard)
{
	cout<<"Player "<<player+1<<"'s hand:\n\n"; //printing hand
	for (int row=0;row<5;row++)//since a card is made up of 5 lines outer loop runs 5 times
	{
		for (int r=0; r<pcard; r++)//inner loops runs for how many cards player has
		{
	
			if (phand[player][r]!=16)//checks for valid cards in players hand
			{
				int color=phand[player][r]/100; // getting the color of the card
				int type=phand[player][r]%100;
	
				switch (color)//setting the color
				{
					case 1: setColor(BLACK, RED);
					break;
					case 2: setColor(BLACK, YELLOW);
					break;
					case 3: setColor(BLACK, BLUE);
					break;
					case 4: setColor(BLACK, GREEN);
					break;
					default: setColor(WHITE, BLACK);
				}
				
				switch (row)//decides which row is gonna be printing
				{
					case 0: cout<<"--------";
							setColor(WHITE, BLACK);
							cout<<"  ";
							break;
					case 1: cout<< "-      -";
							setColor(WHITE, BLACK);
							cout<<"  ";
							break;
							
					case 2: if (type<10)
							{
								cout<< "-   "<<type<<"  -";
								setColor(WHITE, BLACK);
								cout<<"  ";
								break;
							}
							else
							{
								cout<<"-  "<<type<<"  -";
								setColor(WHITE, BLACK);
								cout<<"  ";
								break;
							}
							break;
					case 3: cout<<"-      -";
							setColor(WHITE, BLACK);
							cout<<"  ";
							break;
					default:cout<<"--------";
							setColor(WHITE, BLACK);
							cout<<"  ";
					
				}
			}
		}
		cout<<endl;
	}
}
void print_cards(int topcard)//for printing one card only
{
	int color=topcard/100;
	int type=topcard%100;
	cout<<endl<<endl;
	switch (color)
	{
		case 1: setColor(BLACK, RED);
				cout<<"--------";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				setColor(BLACK, RED);
				cout<< "-      -";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				setColor(BLACK, RED);
				if (type<10)
				{
					cout<< "-   "<<type<<"  -";
					setColor(WHITE, BLACK);
					cout<<"  "<<endl;
				}
				else
				{
					cout<<"-  "<<type<<"  -";
					setColor(WHITE, BLACK);
					cout<<"  "<<endl;
				}
				setColor(BLACK, RED);
				cout<< "-      -";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				setColor(BLACK, RED);
				cout<<"--------";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				break;
				
		case 2: setColor(BLACK, YELLOW);
				cout<<"--------";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				setColor(BLACK, YELLOW);
				cout<< "-      -";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				setColor(BLACK, YELLOW);
				if (type<10)
				{
					cout<< "-   "<<type<<"  -";
					setColor(WHITE, BLACK);
					cout<<"  "<<endl;
				}
				else
				{
					cout<<"-  "<<type<<"  -";
					setColor(WHITE, BLACK);
					cout<<"  "<<endl;
				}
				setColor(BLACK, YELLOW);
				cout<< "-      -";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				setColor(BLACK, YELLOW);
				cout<<"--------";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				break;
				
		case 3: setColor(BLACK, BLUE);
				cout<<"--------";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				setColor(BLACK, BLUE);
				cout<< "-      -";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				setColor(BLACK, BLUE);
				if (type<10)
				{
					cout<< "-   "<<type<<"  -";
					setColor(WHITE, BLACK);
					cout<<"  "<<endl;
				}
				else
				{
					cout<<"-  "<<type<<"  -";
					setColor(WHITE, BLACK);
					cout<<"  "<<endl;
				}
				setColor(BLACK, BLUE);
				cout<< "-      -";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				setColor(BLACK, BLUE);
				cout<<"--------";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				break;
				
		case 4: setColor(BLACK, GREEN);
				cout<<"--------";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				setColor(BLACK, GREEN);
				cout<< "-      -";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				setColor(BLACK, GREEN);
				if (type<10)
				{
					cout<< "-   "<<type<<"  -";
					setColor(WHITE, BLACK);
					cout<<"  "<<endl;
				}
				else
				{
					cout<<"-  "<<type<<"  -";
					setColor(WHITE, BLACK);
					cout<<"  "<<endl;
				}
				setColor(BLACK, GREEN);
				cout<< "-      -";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				setColor(BLACK, GREEN);
				cout<<"--------";
				setColor(WHITE, BLACK);
				cout<<"  "<<endl;
				break;
				
		default: setColor(WHITE, BLACK);
				cout<<"--------";
				cout<<"  "<<endl;
				cout<< "-      -";
				cout<<"  "<<endl;
				if (type<10)
				{
					cout<< "-   "<<type<<"  -";
					setColor(WHITE, BLACK);
					cout<<"  "<<endl;
				}
				else
				{
					cout<<"-  "<<type<<"  -";
					setColor(WHITE, BLACK);
					cout<<"  "<<endl;
				}
				cout<< "-      -";
				cout<<"  "<<endl;
				cout<<"--------";
				cout<<"  "<<endl;
	}
	cout<<endl;	
}
void shuffleDeck(int deck[][15])
{
    srand(time(NULL)); //Seed for randomness

    int row,col;

    for (int i=0;i<9;i++)
    {
    for (int j=0;j<15;j++)
        {

            int cardDeck= rand()% 135;
            row= cardDeck/15; //For randomising rows/colours
            col= cardDeck%15; //For randomising columns/card type

            int temp = deck [i][j];
            deck[i][j] = deck[row][col]; //Swapping cards to avoid duplicates
            deck[row][col] = temp;

        }
    }
}
bool isValidPlay(int playerCard, int topCard)
{
	if (playerCard%100==topCard%100 || playerCard/100==topCard/100) //Condition to check if player card's colour or number type matches the top card's colour or number type respectively
	{
		return true;
	}
	else
	{
		cout<<endl<<"Not the right color! Enter again."<<endl;
		return false;
	}
}

void dealCards(int deck[][15], int p[][108], int &topCard, int discardedPile[],int &discardSize) 
{
	//cardno is card number
	int player=0, cardno=0;
	//it works as a flag to break the loop
	bool loopbreak=false;
	
	for (int i=0;i<9;i++)
	{
		for (int j=0;j<15;j++)
		{
			if (deck[i][j]!=16)
			{
				p[player][cardno]=deck[i][j];
				cardno++;
				//this is done in order to 'remove' the card from the deck (replacing it with 16 which is being used as null)
				deck[i][j]=16;
				
				//condition to change player no.
				if (cardno==7)
				{
					player++;
					cardno=0;
				}
				//since we only have 2 players and so only 14 cards are being taken out of deck, we will temrinate loop once both of the players have gotten 7 cards each
				if (player==2)
				{
					loopbreak=true;
					break;
				}
			}
		}
		if (loopbreak)
		{
			break;
		}
	}
	
	loopbreak=false;//assigns the initial topCard
	for (int i=0;i<9;i++)
	{
		for (int j=0;j<15;j++)
		{
			topCard=deck[i][j];
			if (topCard!=16 && topCard%100<=9)
			{
				loopbreak=true;
				deck[i][j]=16;
				break;
			}
		}
		if(loopbreak)
		{
			break;
		}
	}
	discardedPile[0] = topCard; 
	discardSize = 1;

}

void handleSpecialCard(int specialCardType, int phands[][108], int deck[][15], int &player, int &pcards, int &topCard, int discardedPile[], int &discardSize)
{
	int mod=specialCardType%100;
	int color;
	int p=(player+1)%2;
	switch (mod)
	{
		case 10://skip
		
		case 11: topCard=specialCardType;//reverse, since they both work the same here, no need to write another line of code
				 player--;//this makes sure that player who played the card retains their turn
				 break;	
			
		case 12: DrawCards(pcards,phands,deck,p,2, topCard, discardedPile, discardSize);//draw 2 this calls the drawcards function, where the constant '2' determines how many cards are to be drawn
				 topCard=specialCardType;//assigns topcard the value
				 player--;//to retain players turn
				 break;
			
		case 13: cout<<"What color do you want.\n1.Red\n2.Yellow\n3.Blue\n4.Green\n";//wild
				 cin>>color;//asks user for the color they wanna use
				 while (color>4 || color<1)//validation
				 {
				 	cout<<"Invalid entry enter again\n";
				 	cout<<"What color do you want.\n1.Red\n2.Yellow\n3.Blue\n4.Green\n";
				 	cin>>color;
				 }
				 topCard=color*100+13;//colors r stored in first digit so number starting with 1 is red,2 yellow 3 blue etc.
				 break;	
			
		default: DrawCards(pcards,phands,deck,p,4, topCard, discardedPile, discardSize);//draw 4 works as a combination of draw 2 and wild card
				 cout<<"What color do you want.\n1.Red\n2.Yellow\n3.Blue\n4.Green\n";
				 cin>>color;
				 while (color>4 || color<1)
				 {
				 	cout<<"Invalid entry enter again\n";
				 	cout<<"What color do you want.\n1.Red\n2.Yellow\n3.Blue\n4.Green\n";
				 	cin>>color;
				 }
				 player--;
				 topCard=color*100+14;
				 
				
	}
	UpdateDiscardPile(discardedPile, discardSize, deck, topCard);
	
}



void PrintPlayerHand(int phands[][108],int topcard,int pcard, int player)//prints the whole board
{
	print_cards(phands,player, pcard, topcard);	
	cout<<"\n\n";
	cout<<"Top Card:\n\n";
	print_cards(topcard);
}
void DrawCards(int &pcards, int phands[][108], int deck[][15], int player, int drawcards, int &topCard, int discardedPile[], int &discardSize)
{
//function which handles card drawing
	
	for (int x=0;x<drawcards;x++)//determines how many times cards r to be drawn
	{
		bool loopbreak=false;
		
		for (int i=0;i<9;i++)//since main deck/totalcards has 9 rows
		{
			for (int j=0;j<15;j++)//and 15 rows, this will allow us to iterate thru every element
			{
				if (deck[i][j]!=16)//if there is a non 16/ valid card found
				{
					phands[player][pcards]=deck[i][j];//assigns it to player
					deck[i][j]=16;//removes the card from the main deck by making it 16 or null
					pcards+=1;//by drawing one card, player's total number of cards has increased by one
					loopbreak=true;// since we have found the card no need for the loop to work and so it breaks
					break;
				}
			}
			if (loopbreak)//allows us to exit through outer loop
			{
				break;
			}
		}
		if (loopbreak==false)
		{
			UpdateDiscardPile(discardedPile, discardSize, deck, topCard);
		}
	}
}
void playTurn(bool playCard, int &topCard, int playerCard,int deck[][15], int phands[][108], int& player, int &p1cards, int &p2cards, int discardedPile[], int &discardSize, int &pcards)
{
	cout<<endl;
	int specialCard,option2;
	char a;
	int uno;
	bool isUnoCalled=false;
	char option;
    if (player==0) {//player==0 is player 1
    		PrintPlayerHand(phands,topCard,p1cards,player);
    		flag1:
    		cout<<"1. Play a Card (Enter 1)"<<endl<<"2. Draw a Card (Enter 2)"<<endl<<endl<<"Enter 'P' to pause the game."<<endl<<"Enter 'E' to exit the game."<<endl;
    		cin>>option;
    		
    		while(option!='1' && option!='2' && option!='P' && option!='E' && option!='U')//validation checks
    		{
    			cout<<"Invalid entry, enter again."<<endl;
    			cout<<"1. Play a Card (Enter 1)"<<endl<<"2. Draw a Card (Enter 2)"<<endl<<endl<<"Enter 'P' to pause the game."<<endl<<"Enter 'E' to exit the game."<<endl;
    			cin>>option;
			}
			switch (option)
			{
				case 'U': if (isUnoCalled==false && callUno(p1cards, p2cards, player)) //Function called to check if calling uno is valid
							{
								clearScreen();
								cout<<"UNO has been called by player 1!"<<endl; //Correct uno call by player 1 if two cards are remaining
								cout<<"Enter any key to continue."<<endl;
								cin>>a;
								isUnoCalled=true;
								if (isUnoCalled)
								{
									int uno=1;
								}
								player--;						
							}
							else if (isUnoCalled==true)
							{
								cout<<"UNO was already called this turn!"<<endl;
							}
							else
							{
								clearScreen();
								cout<<"Incorrect UNO call! Player 1 will be penalised with two extra cards."<<endl; //Incorrect uno call by player 1 if more than two cards are remaining
								DrawCards(p1cards, phands, deck, player, 2, topCard, discardedPile, discardSize);
								cout<<"Enter any key to continue."<<endl;
								cin>>a;
							}
							break;
				case '1': if (p1cards==2 && isUnoCalled==false && uno==1)//if player chooses to play without calling uno
							{
								clearScreen();
								cout<<"Player 1 has failed to call UNO and will be penalised with two extra cards!"<<endl<<endl; //Two cards will be given if player 1 skips uno call
								DrawCards(p1cards, phands, deck, player, 2, topCard, discardedPile, discardSize);
								PrintPlayerHand(phands,topCard,p1cards,player);

							} 
						//asks player to choose what card to play
						cout<<"Choose card 1-"<<p1cards<<":";
						cin>>option2;
						while(option2<1 || option2>p1cards) //Input validation
    					{
    						cout<<"Invalid entry, enter again."<<endl;
    						cout<<"Choose card 1-"<<p1cards<<":";
							cin>>option2;
						}
						playerCard=phands[player][option2-1];//assign the chosen card to a variable player card
						if (playerCard%100==13 || playerCard%100==14)//if card is either wild or draw 4, it is automatically a valid play
						{
							playCard=1;
						}
						else//else validplay function is called to see if the card can be played or not
						{
							playCard=isValidPlay(playerCard, topCard);
						}
						
						if (playCard)//if the card is deemed to b playable
						{
							if (playerCard%100>=10&&playerCard%100<=14)//checks if card is an action card
							{
								specialCard=playerCard;
								handleSpecialCard(specialCard, phands, deck, player, p2cards, topCard, discardedPile, discardSize);//calls the function which deals with action cards
								if (topCard%100!=13)//if the special card is not wild
								{
									phands[player+1][option2-1]=16;//this means it has decremented the value of 'player', so by doing 'player+1' in index we can avoid causing wrong player getting inputs
									p1cards-=1;//since card is played, number of cards is decreased by one
									arrangedeck(phands,player+1);//calls arrangeddeck function which filters out the null cards/empty spaces
								}
								else//this will run in case of wild card, since it doesnt affect 'player's turn' we dont need to do player+1
								{
									phands[player][option2-1]=16;
									p1cards-=1;
									arrangedeck(phands,player);
								}
								//p1cards-=1;
								//arrangedeck(phands,player+1);
							}
							else//for normal cards
							{
								topCard=playerCard;
								phands[player][option2-1]=16;
								p1cards-=1;
								arrangedeck(phands,player);
							}
							UpdateDiscardPile(discardedPile, discardSize, deck, topCard);
						}
						else
						{
							goto flag1;
						}
						break;
						
				case '2': DrawCards(p1cards,phands,deck,player,1, topCard, discardedPile, discardSize);//if player chooses to draw cards, calls the draw card function
						 playCard=isValidPlay(phands[player][p1cards-1], topCard);//checks if the drawn card is playable or not
						 playerCard=phands[player][p1cards-1];//assigns the drawn card to a variable
						
						if (playCard)//if its a playable card
						{
							int tempoption;
							print_cards(playerCard);//prints the drawn card
							cout<<"Do you want to play the draw card?"<<endl<<"1. Yes"<<endl<<"2. No"<<endl;//gives player the choice either they wanna keep it or play it
							cin>>tempoption;
							
							while (tempoption<1 || tempoption>2)
							{
								cout<<"Invalid entry, enter again."<<endl;
								cout<<"Do you want to play the draw card?"<<endl<<"1. Yes"<<endl<<"2. No"<<endl;
								cin>>tempoption;
							}
							if (tempoption==1)//if player plays
							{
								if (playerCard%100>=10&&playerCard%100<=14)//checks if its an action card
								{
									specialCard=playerCard;
									handleSpecialCard(specialCard, phands, deck, player, p2cards, topCard, discardedPile, discardSize);
									if (topCard%100!=13)
									{
										phands[player+1][p1cards-1]=16;//we do p1cards as an index since we know that is the maximum value/last value and drawn card is always gonna be the last/latest card
										p1cards-=1;
										arrangedeck(phands,player+1);
									}
									else
									{
										phands[player][p1cards-1]=16;
										p1cards-=1;
										arrangedeck(phands,player);
									}	
								}
								else
								{
									topCard=playerCard;
									phands[player][p1cards-1]=16;
									p1cards-=1;
									arrangedeck(phands,player);
								}
							}
						}
						break;
						
				case 'P': char resume; 
						cout<<endl<<"Game Paused!"<<endl; //Pausing the game if P is entered
						cout<<"Enter 'R' to resume."<<endl; 
						cin>>resume;
						
						while (resume!= 'R')
							{
								cout<<"Invalid entry, please enter 'R' to resume the game."<<endl; //Game will not continue unless 'R' is entered
								cin>>resume;
							}
						if (resume=='R')
						{	
							cout<<endl;
							cout<<"The game has resumed! You may continue."<<endl<<endl; //Game will continue is 'R' is entered
							goto flag1;
						}
						break;
						
				case 'E': 	char exit;
						  	cout<<"Are you sure you want to return to the main menu (Enter 'Y' for yes or 'N' for no)?"<<endl; //Game asks for double confirmation if you want to exit
							cin>>exit;
							
						while (exit!= 'Y' && exit!= 'N' )
							{
								cout<<"Invalid entry, please enter either 'Y' for yes or 'N' for no."<<endl; //Input validation
								cin>>exit;
							}
						if (exit=='Y')
						{
							cout<<"Returning to the main menu..."<<endl<<endl; //Goes back to main menu if user enters 'Y'
							clearScreen();
							MainMenu();
						}
						else if (exit=='N')
						{
							cout<<"Continuing the game..."<<endl<<endl; //Resumes the game if user enters 'N'
							goto flag2;
						}
						break;		
			}	
							
						
						
						
		}
		else//player==1 is player 2 and the code is the same
		{
			PrintPlayerHand(phands,topCard,p2cards,player);
			flag2:
			isUnoCalled==false;
    		cout<<"1. Play a Card (Enter 1)"<<endl<<"2. Draw a Card (Enter 2)"<<endl<<endl<<"Enter 'P' to pause the game."<<endl<<"Enter 'E' to exit the game."<<endl;
    		cin>>option;
    		isUnoCalled==false;
    		while(option!='1' && option!='2' && option!='P' && option!='E' && option!='U')
    		{
    			cout<<"Invalid entry, enter again."<<endl;
    			cout<<"1. Play a Card (Enter 1)"<<endl<<"2. Draw a Card (Enter 2)"<<endl<<endl<<"Enter 'P' to pause the game."<<endl<<"Enter 'E' to exit the game."<<endl;
    			cin>>option;
			}
			switch (option)
			{
				case 'U': if (isUnoCalled==false && callUno(p1cards, p2cards, player)) //Function called to check if calling uno is valid
							{
								clearScreen();
								cout<<"UNO has been called by player 2!"<<endl; //Correct uno call by player 2 if two cards are remaining
								cout<<"Enter any key to continue."<<endl;
								cin>>a;
								isUnoCalled==true;
								if (isUnoCalled)
								{
									int uno=1;
								}
								player--;						
							}
							else if (isUnoCalled==true)
							{
								cout<<"UNO was already called this turn!"<<endl;
							}
							else
							{
								clearScreen();
								cout<<"Incorrect UNO call! Player 2 will be penalised with two extra cards."<<endl; //Incorrect uno call by player 2 if more than two cards are remaining
								DrawCards(p2cards, phands, deck, player, 2, topCard, discardedPile, discardSize);
								cout<<"Enter any key to continue."<<endl;
								cin>>a;
							}
							break;
				case '1':if (p2cards==2 && isUnoCalled==false && uno==1)
							{
								clearScreen();
								cout<<"Player 2 has failed to call UNO and will be penalised with two extra cards!"<<endl<<endl; //Two cards will be given if player 2 skips uno call
								DrawCards(p2cards, phands, deck, player, 2, topCard, discardedPile, discardSize);
								PrintPlayerHand(phands,topCard,p2cards,player);
							}  
						cout<<"Choose card 1-"<<p2cards<<":";
						cin>>option2;
						while(option2<1 || option2>p2cards)
    					{
    						cout<<"Invalid entry, enter again."<<endl;
    						cout<<"Choose card 1-"<<p2cards<<":";
							cin>>option2;
						}
						playerCard=phands[player][option2-1];
						if (playerCard%100==13 || playerCard%100==14)
						{
							playCard=1;
						}
						else
						{
							playCard=isValidPlay(playerCard, topCard);
						}
						
						if (playCard)
						{
							if (playerCard%100>=10&&playerCard%100<=14)
							{
								specialCard=playerCard;
								handleSpecialCard(specialCard, phands, deck, player, p1cards, topCard, discardedPile, discardSize);
								if (topCard%100!=13)
								{
									phands[player+1][option2-1]=16;
									p2cards-=1;
									arrangedeck(phands,player+1);	
								}
								else
								{
									phands[player][option2-1]=16;
									p2cards-=1;
									arrangedeck(phands,player);	
								}
							}
							else
							{
								topCard=playerCard;
								phands[player][option2-1]=16;
								p2cards-=1;
								arrangedeck(phands,player);	
							}
							UpdateDiscardPile(discardedPile, discardSize, deck, topCard);
						}
						else
						{
							goto flag2;
						}
						break;
						
				case '2': DrawCards(p2cards,phands,deck,player,1, topCard, discardedPile, discardSize);
						playCard=isValidPlay(phands[player][p2cards-1], topCard);
						playerCard=phands[player][p2cards-1];
						if (playCard)
						{
							int tempoption;
							print_cards(playerCard);
							cout<<"Do you want to play the draw card?"<<endl<<"1. Yes"<<endl<<"2. No"<<endl;
							cin>>tempoption;
							
							while (tempoption<1 || tempoption>2)
							{
								cout<<"Invalid entry, enter again."<<endl;
								cout<<"Do you want to play the draw card?"<<endl<<"1. Yes"<<endl<<"2. No"<<endl;
								cin>>tempoption;
							}
							if (tempoption==1)
							{
								if (playerCard%100>=10&&playerCard%100<=14)
								{
									specialCard=playerCard;
									handleSpecialCard(specialCard, phands, deck, player, p1cards, topCard, discardedPile, discardSize);
									if (topCard%100!=13)
									{
										phands[player+1][p2cards-1]=16;
										p2cards-=1;
										arrangedeck(phands,player+1);
									}
									else
									{
										phands[player][p2cards-1]=16;
										p2cards-=1;
										arrangedeck(phands,player);
									}
									
								}
								else
								{
									topCard=playerCard;
									phands[player][p2cards-1]=16;
									p2cards-=1;
									arrangedeck(phands,player);
								}
							}
						}
						break;
						
				case 'P': 	char resume; 
							cout<<endl<<"Game Paused!"<<endl;
							cout<<"Enter 'R' to resume."<<endl;
							cin>>resume;
						
						while (resume!= 'R')
							{
								cout<<"Invalid entry, please enter 'R' to resume the game."<<endl;
								cin>>resume;
							}
						if (resume=='R')
						{	
							cout<<endl;
							cout<<"The game has resumed! You may continue."<<endl<<endl;
							goto flag2;
						}
						break;
				case 'E': 	char exit;
						  	cout<<"Are you sure you want to return to the main menu (Enter 'Y' for yes or 'N' for no)?"<<endl;
							cin>>exit;
							
						while (exit!= 'Y' && exit!= 'N' )
							{
								cout<<"Invalid entry, please enter either 'Y' for yes or 'N' for no."<<endl;
								cin>>exit;
							}
						if (exit=='Y')
						{
							cout<<"Returning to the main menu..."<<endl<<endl;
							clearScreen();
							MainMenu();
						}
						else if (exit=='N')
						{
							cout<<"Continuing the game..."<<endl<<endl;
							goto flag2;
						}
						break;
			}
		}
	
}
void arrangedeck(int phands[][108],int player)//to arrange the deck after a card has been played
{
	int temp[108];//temporary array which stores the max number of cards
	int count=0;
	for(int i=0;i<108;i++)
	{
		if (phands[player][i]!=16)
		{
			temp[count]=phands[player][i];//temp array only gets valid cards so all the starting elements of temp array are valid cards
			count++;//to increment
		}
	}
		
	while (count<108)//count will start from where ever it left off
	{
		temp[count]=16;//fills remaining array with 16/null cards
		count++;
	}
	
	for(int i=0;i<108;i++)//now elements of temp array are assigned to playes hand with the same exact order
	{
		phands[player][i]=temp[i];
	}
	
}

void UpdateDiscardPile(int discardedPile[], int &discardSize, int deck[][15], int &topCard)
{
	
	bool isDeckEmpty=true; //Used to check if the current deck is empty for conditions to be applied later
	int card,row,col;
	
	discardedPile[discardSize]=topCard; //The current top card is added to the top of discard pile
	discardSize+=1;
	
	for (int i=0;i<9;i++)
	{
		for (int j=0;j<15;j++)
		{
			if (deck[i][j]!=16) //Check if all the cards are empty/set to null value (16)
			{
				isDeckEmpty=false; //Exit loop if deck is not empty
				break;
			}
		}
		if (isDeckEmpty==false)
		{
			break; //Will break the entire function if a valid card was found
		}
	}
	if (isDeckEmpty==true) //Condition to check if the deck is empty
	{
		int reshuffleCount=0,x;
		
		for (int i=0;i<discardSize-1;i++)
		{
			card=discardedPile[i];
			row=reshuffleCount/15; //Randomising rows/colours
			col=reshuffleCount%15; //Randomising columns/card type
			
			deck[row][col]= card; //Moving each card except top card back to the deck
			reshuffleCount++;
		}
		
		discardedPile[0]=discardedPile[discardSize-1]; //Top card remains on top of discard pile
		discardSize=1;
		
		shuffleDeck(deck); //Reshuffling the new deck
		cout<<endl<<"The deck has been reshuffled from the discarded pile!"<<endl<<endl;	
	}
	
}

bool callUno(int &p1cards, int &p2cards, int &player)
{
	if (player==0) //Checking if first player has two cards remaining
	{
		if (p1cards==2) //Condition to check if two cards are remaining
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (player==1) //Checking if second player has two cards remaining
	{
		if (p2cards==2) //Condition to check if two cards are remaining
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
