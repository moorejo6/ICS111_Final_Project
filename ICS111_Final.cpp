// ICS111_Final.cpp : My final project for ICS 111
//BY: JORDAN MOORE
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

class Character{
private:
	string name;
	int health;
	int magic;

public:
	Character();
	string getName();
	void setName(string n);
	int getHealth();
	void setHealth(int h){health = h;};
	void addHealth(int h){health += h;};
	int getMagic();
	void setMagic(int m){magic = m;};
	void addMagic(int m){magic += m;};
	void displayStats();
	void takeDamage();
	void heal();
};

class Monster : public Character{
public:
	Monster();
};

class Player : public Character{
private:
	int steps;
	int monstersKilled;
public:
	Player();
	Player(string n, int hp, int mp, int st, int ki);
	int getSteps(){return steps;};
	void addStep(){steps++;};
	int getMonstersKilled(){return monstersKilled;};
	void addMonstersKilled(){monstersKilled++;};
	void extraStats()
	{cout << "  Steps: " << steps << " Monsters Killed: " << monstersKilled << endl;};
};

void mainMenu();
void load();
void newGame();
void save(Player &player);
void play(Player player);
void step(Player *p);
void fight(Player *player);
void exit();

/////////MAIN////////////
int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(0));

	mainMenu();
	exit();

	return 0;
}

///PROGRAM FUNCTIONS///

//mainMenu() handles what the user can do in the main menu of the program
void mainMenu(){

	bool quit = false;
	char choice;

	//display title
	cout << "ICS111 Final Project" << endl << "By: Jordan Moore" << endl << endl;

	do{
		//ask user for input
		cout << "What would you like to do?" << endl;
		cout << "Load [L] | New Game [N] | Quit [Q]" << endl;
		cin >> choice;
		cout << endl; //line break for formatting

		switch(choice){
		case 'L': case 'l':
			load();
			break;

		case 'N': case 'n':
			newGame();
			break;

		case 'Q': case 'q':
			quit = true;
			break;

		default:
			cout << choice << " is not a valid input" << endl << endl;
			cin.clear();
			cin.ignore(1000, '\n');
		}


	}while(!quit);
}


//load() loads saved character data, creates a player object with that data,
// and starts the game via a call to play()
void load(){
	
	//initialize variables 
	string name;
	int health;
	int magic;
	int steps;
	int monstersKilled;
	ifstream inFile;
	bool validName;
	string choice;


	do{
		//ask user for character name or q to quit
		cout << "Type the name of the character you want to load." << endl;
		cout << "Type Q to quit" << endl;
		cin >> choice;

		if(choice == "q" || choice == "Q"){
			//if user inputs q, exit loop
			validName = true;
		}else{

			//else open [choice].txt
			inFile.open(choice + ".txt");
			
			//check if file exists
			if(!inFile){

				//if files does not exist, go to beginning of the loop
				cout << "Could not find the character " << choice <<endl << endl;
				validName = false;
				inFile.close();

			}else{

				//if the file does exist, input contents from file and use them to create a new player character
				// and set validName to true so the load() function will end after the play() function ends
				validName = true;
				inFile >> name;
				inFile >> health;
				inFile >> magic;
				inFile >> steps;
				inFile >> monstersKilled;
				inFile.close();
				cout << endl << endl; //line breaks for formatting

				Player newPlayer(name, health, magic, steps, monstersKilled);
				play(newPlayer);
			}
		}


	}while(!validName);

	
}


//newGame() creates a new player character and starts the game via a call to play()
void newGame(){
	
	//create a new player character
	Player newPlayer;

	//display intro story text
	cout << newPlayer.getName() << " wakes up in a cave lit by a single torch laying on the ground." << endl;
	cout << "The only way out is a narrow passage extending into the darkness." << endl;
	cout << newPlayer.getName() << " picks up the torch and faces the passage...\n" << endl;

	//start game
	play(newPlayer);
}


//save() saves the players data to a file named [playername].txt
void save(Player &player){

	//open or create the file [playername].txt and write player attributes to the text file
	ofstream outFile;
	outFile.open(player.getName() + ".txt");
	outFile << player.getName() << endl;
	outFile << player.getHealth() << endl;
	outFile << player.getMagic() << endl;
	outFile << player.getSteps() << endl;
	outFile << player.getMonstersKilled() << endl;
	outFile.close();

	cout << "Save successful!" << endl;

}

//play handles the actions the player can take out of combat
void play(Player player){
	
	bool quitPlay = false;
	char playInput;
	Player *playerPointer = &player;

	do{

		//display player health, magic, steps taken, and monstersKilled
		player.displayStats();
		player.extraStats();
		cout << endl; //line break for formatting

		//prompt the user for input
		cout << "Save [S] | Step Forward [F] | Heal[H] | Quit [Q]" << endl;
		cin >> playInput;
		cout << endl; //line break for formatting

		switch(playInput){
			//case S: save
		case 'S': case 's':
			save(player);
			cin.clear();
			cin.ignore(1000, '\n');
			break;

		case 'F': case 'f':
			//case F: step forwared
			step(playerPointer);
			cin.clear();
			cin.ignore(1000, '\n');
			break;

		case 'H': case 'h':
			//case h: heal
			player.heal();
			cin.clear();
			cin.ignore(1000, '\n');
			break;

		case 'Q': case 'q':
			//case q: quit
			quitPlay = true;
			cin.clear();
			cin.ignore(1000, '\n');
			break;

		default:
			//else tell user input is invalid and return to top of loop
			cout << playInput << " is not a valid input\n" << endl;
			cin.clear();
			cin.ignore(1000, '\n');
			break;
		}

		//quit if player health is <= 0
		if(player.getHealth() <= 0){
			quitPlay = true;
		}

	}while(!quitPlay);



}

//step determines if a fight starts
void step(Player *p){
	
	int number = 1 + rand() % 10; //generate a random number between 1 and 10

	if(number > 6){
		//if the randomly generated number is > 6, a fight starts
		fight(p);	
	}else{

		//else add to steps taken, add health, and add magic
		p->addStep();
		
		if(p->getHealth() + 5 <= 100){
			p->addHealth(5);
		}

		if(p->getMagic() + 2 <= 40){
			p->addMagic(2);
		}
	}
}

//fight handles combat between the player and a monster.
void fight(Player *player){

	bool combatOngoing = true;
	bool validInput = false;
	char combatInput;

	Character *m = new Monster();
	Character *p = player;

	//tell player they are being attacked
	cout << p->getName() << " is being attacked by " << m->getName()<< "\n" << endl;

	do{

		//display player stats and monster stats
		p->displayStats();
		cout << endl;
		m->displayStats();
		cout << "\n" << endl;

		do{
			//ask the user what they want to do
			cout << "What do you want to do?" << endl << "Attack [A] | Heal [H]" << endl;
			cin >> combatInput;
			cout << endl; //extra line break for formatting

			switch(combatInput){

				//if user chooses to attack, monster takes damage
			case 'A': case 'a':
				validInput = true;
				m->takeDamage();
				cin.clear();
				cin.ignore(1000, '\n');
				break;
				
				//if user chooses to heal, user gains hp
			case 'H': case 'h':
				p->heal();
				validInput = true;
				cin.clear();
				cin.ignore(1000, '\n');
				break;

				//else, tell user invalid input
			default:
				cout << combatInput << " is not a valid input" << endl;
				validInput = false;
				cin.clear();
				cin.ignore(1000, '\n');
			}
			
		}while(!validInput);

		//check monster health
		if(m->getHealth() > 0){

			//if monster health is > 0, player takes damage
			p->takeDamage();

		}else{

			//if monster health is <= 0 combat ends
			cout << p->getName() << " defeated " << m->getName() << endl;
			player->addStep();
			player->addMonstersKilled();
			combatOngoing = false;
		}

		//check player health
		if(p->getHealth() > 0){

			//if player health is > 0, nothing happens and combat continues

		}else{

			//if player health is < 0, game ends
			cout << m->getName() << " defeated " << p->getName() << endl;
			cout << "Game over" << endl;
			combatOngoing = false;
		}

		cout << endl; //extra line break for formatting
	}while(combatOngoing);


}

//prompt user to quit the program
void exit(){
	cout << "\n\nProgram has ended. Press enter to quit.";
	cin.ignore();
	cin.get();
}





///Character class functions///
Character::Character(){
	name = "test";
	health = 100;
	magic = 40;
}

string Character::getName(){
	return name;
}

void Character::setName(string n){
	name = n;
	return;
}

int Character::getHealth(){
	return health;
}

int Character::getMagic(){
	return magic;
}

void Character::displayStats(){
	cout << name << " HP: " << health << "/100  MP:" << magic << "/40";
	return;
}

void Character::takeDamage(){
	int damage = rand() % 51; //generate a random number between 0 and 50
	health -= damage;          //subtract random number from character health
	cout << name << " took " << damage << " damage" << endl;
	return;
}

void Character::heal(){
	int totalHealed = 5 + rand() % 45; //total healed is a number between 5 and 45

	if(magic >= 20){

		//subtract cost of using heal
		magic -= 20;

		if(health + totalHealed> 100){

			//if the result of health + totalHealed is > 100, health = 100
			health = 100;

			//tell the user what happened
			cout << name << " is now at full health" << endl;

		}else{

			//else, add totalHealed to health
			health += totalHealed;

			//and tell the user what happened
			cout << name << " gained " << totalHealed << " hp" << endl;
		}

	}else{

		//if magic is < 20, tell user they dont have enough magic to heal
		cout << "Not enough magic to heal" << endl;
	}
	return;
}

///Monster Class Functions///
Monster::Monster(){
	//randomly choose a monster name
	string nameArray[] = {"Monster1","Monster2","Monster3"};
	setName(nameArray[rand() % 3]);
}

///Player Class Functions///
Player::Player(){
	bool validName = false;
	string n;

	do{

		//prompt user for name
		cout << "What is your name?" << endl;
		cin >> n;

		if(n.length() >= 10){
			//if name not valid, tell user and restart loop
			cout << "name must be 10 characters or less" << endl;
			validName = false;
			cin.clear();
			cin.ignore(1000, '\n');

		}else{

		setName(n);
		steps = 0;
		monstersKilled = 0;
		validName = true;
		
		}

	}while(!validName);

	cout<< endl << endl; //line breaks for formatting
}

Player::Player(string n, int hp, int mp, int st, int ki){
	setName(n);
	setHealth(hp);
	setMagic(mp);
	steps = st;
	monstersKilled = ki;
}