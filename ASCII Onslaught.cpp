/*
	ASCII Onslaught
	Copyright 2004 - Distributed under the GNU General Public License (http://www.gnu.org)
	
	Win32 Version Compiled with Borland C++ Compiler Free Edition
	Linux Version Compiled with g++
*/

#include <iostream>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <fstream.h>
using namespace std;


//******** STRUCTS ********
//**************************

struct game_settings
{
	int speed;	//1 to 499	 1-slow, 499 - insanely fast
	int numBaddies;
	int numLasers;
	int difficulty;
	bool twoPlayers;
	int highscores[3];
	string highscorenames[3];
};

struct bad_ascii
{
	int x;
	int y;
	bool beenHit;
	char symbol;
};

struct projectile
{
	int x,y;
	char symbol;
	bool isActive;
};

struct playerinfo
{
	int health,level;
	long score;
	int movement;	//holds the direction the ship is travelling in. (0 - no movement, 1 - left, 2 - right)
	int x,y;
	string ship;
	string blank;
};


//******** GLOBAL VARS ********
//*******************************
//declare the baddies and other vars
const int maxBaddies=200;	//set to max amount of baddies
const int maxLasers=100;	//set to max amount of lasers 
const char version[]="1.11";
char key;
int j;
struct bad_ascii baddies[maxBaddies];
struct playerinfo player;
struct playerinfo player2;
struct projectile laser[maxLasers];
struct projectile laser2[maxLasers];
struct game_settings settings;


//******** METHODS *********
//*****************************

//checkLaserHit()
void checkLaserHit()
{
	for (j=0;j<settings.numBaddies;j++)
	{
		for (int k=0;k<settings.numLasers; k++)
		{
			//laser1
			if (laser[k].isActive && baddies[j].y==laser[k].y-1 && baddies[j].x==laser[k].x)
			{
				baddies[j].beenHit=true;
				player.score=player.score+100+(2*settings.difficulty);
			}
			else if (laser[k].isActive && baddies[j].y==laser[k].y && baddies[j].x==laser[k].x)
			{
				baddies[j].beenHit=true;
				player.score=player.score+100+(2*settings.difficulty);
			}
			//laser2
			if (settings.twoPlayers)
			{
				if (laser2[k].isActive && baddies[j].y==laser2[k].y-1 && baddies[j].x==laser2[k].x)
				{
					baddies[j].beenHit=true;
					player2.score=player2.score+100+(2*settings.difficulty);
				}
				else if (laser2[k].isActive && baddies[j].y==laser2[k].y && baddies[j].x==laser2[k].x)
				{
					baddies[j].beenHit=true;
					player2.score=player2.score+100+(2*settings.difficulty);
				}
			}
		}
	}
}

//showMenu()
void showMenu()
{
	cout<<"\tWELCOME TO ASCII ONSLAUGHT: VERSION "<<version<<endl;
	cout<<"\tProgrammed by dracflamloc@phreaker.net"<<endl;
	cout<<"\t  http://silentunseen.netfirms.com/"<<endl<<endl;
	cout<<"Player 1 Controls: Left/Right - Move ship, Up - Fire, Down - Stop Moving"<<endl;
	cout<<"Player 2 Controls: A/D - Move ship, W - Fire, S - Stop Moving"<<endl<<endl;
	cout<<"Enter # of Players (1/2): "<<endl;
	if (getch()=='2')
		settings.twoPlayers=true;
	else
		settings.twoPlayers=false;
	
	cout<<"Select a difficulty: "<<endl;
	cout<<"1. Pathetic"<<endl;
	cout<<"2. Wimp"<<endl;
	cout<<"3. Normal"<<endl;
	cout<<"4. Hard"<<endl;
	cout<<"5. OH S***!!!"<<endl;
	cout<<"6. Customize"<<endl;
	cout<<"7. High Scores"<<endl;
	cout<<"Enter your choice: ";
	
	key=0;	
	while(key>'6' || key<'1')
	{
		key=getch();
		if (key=='1')	//difficulty level 1
		{
			settings.numLasers=6;
			settings.speed=350;
			settings.numBaddies=3;
			settings.difficulty=1;
		}
		else if (key=='2')	//difficulty level 2
		{
			settings.numLasers=5;
			settings.speed=400;
			settings.numBaddies=3;
			settings.difficulty=2;
		}
		else if (key=='3')	//difficulty level 3
		{
			settings.numLasers=4;
			settings.speed=430;
			settings.numBaddies=4;
			settings.difficulty=2;
		}
		else if (key=='4')	//difficulty level 4
		{
			settings.numLasers=3;
			settings.speed=450;
			settings.numBaddies=5;
			settings.difficulty=3;
		}
		else if (key=='5')
		{
			settings.numLasers=2;
			settings.speed=470;
			settings.numBaddies=5;
			settings.difficulty=4;
		}
		else if (key=='6')
		{
			cout<<"Max lasers you can fire (1-10): ";
			cin>>settings.numLasers;
			cout<<"Speed (1-499): ";
			cin>>settings.speed;
			cout<<"Starting # of Baddies (1-30): ";
			cin>>settings.numBaddies;
			cout<<"Baddies to add at each level: ";
			cin>>settings.difficulty;
		}
		else if (key=='7')
		{
			cout<<"\nHigh Scores:\n";
			cout<<settings.highscorenames[0]<<"    "<<settings.highscores[0]<<endl;
			cout<<settings.highscorenames[1]<<"    "<<settings.highscores[1]<<endl;
			cout<<settings.highscorenames[2]<<"    "<<settings.highscores[2]<<endl;
			cout<<"Enter your choice: ";
		}
		else
			cout<<"\nNot a valid option, choose again: ";
	}
}

//checkKeypress(char)
void checkKeypress(char key)
{
	if (key==_KB_UP)	//set laser on
	{
		for (j=0; j<settings.numLasers; j++)
		{
			if (!laser[j].isActive)
			{
				laser[j].isActive=true;
				laser[j].x=player.x+1;
				laser[j].y=player.y;
				break;
			}
		}
	}
	else if (key==_KB_LEFT)	//go left
		player.movement=1;
	else if (key==_KB_RIGHT) 	//go right
		player.movement=2;
	else if (key==_KB_DOWN)	//stop
		player.movement=0;
	
	if (settings.twoPlayers)
	{
		if (key=='w')	//set laser on
		{
			for (j=0; j<settings.numLasers; j++)
			{
				if (!laser2[j].isActive)
				{
					laser2[j].isActive=true;
					laser2[j].x=player2.x+1;
					laser2[j].y=player2.y;
					break;
				}
			}
		}
		else if (key=='a')	//go left
			player2.movement=1;
		else if (key=='d') 	//go right
			player2.movement=2;
		else if (key=='s')	//stop
			player2.movement=0;
	}
	
	if (key=='p')
		while(kbhit()==0) {}
}

//drawStatus();
void drawStatus()
{
	gotoxy(62,2); cout<<"Health:\t "<<player.health;
	gotoxy(62,3); cout<<"Score:\t "<<player.score<<endl;
	gotoxy(62,4); cout<<"Level:\t "<<player.level<<endl;
	
	if (settings.twoPlayers)
	{
		gotoxy(62,6); cout<<"Health2:\t "<<player2.health;
		gotoxy(62,7); cout<<"Score2:\t "<<player2.score<<endl;
		gotoxy(62,8); cout<<"Level2:\t "<<player2.level<<endl;
	}
}

//drawBaddies()
void drawBaddies()
{
	for (j=0; j<settings.numBaddies; j++)
	{
		if (baddies[j].y<25 && baddies[j].y>0)
		{
			gotoxy(baddies[j].x,baddies[j].y);
			cout<<baddies[j].symbol;
		}
	}
}

//drawShip()
void drawShip()
{
	gotoxy(player.x,player.y); 
	cout<<player.ship;
	
	if (settings.twoPlayers)
	{
		gotoxy(player2.x,player2.y); 
		cout<<player2.ship;
	}
}

//movePlayer()
void movePlayer()
{
	if (player.movement==1 && player.x>1)
		player.x--;
	else if (player.movement==2 && player.x<57)
		player.x++;
	
	if (settings.twoPlayers)
	{
		if (player2.movement==1 && player2.x>1)
			player2.x--;
		else if (player2.movement==2 && player2.x<57)
			player2.x++;
	}
}

//moveLaser()  - draws the laser too
void moveLasers()
{
	for (j=0;j<settings.numLasers;j++)
	{
		if (laser[j].isActive)
		{
			if (laser[j].y==2)
				laser[j].isActive=false;
			else
			{
				laser[j].y--;
				gotoxy(laser[j].x,laser[j].y); cout<<laser[j].symbol;
			}
		}
		
		if (settings.twoPlayers)
		{
			if (laser2[j].isActive)
			{
				if (laser2[j].y==2)
					laser2[j].isActive=false;
				else
				{
					laser2[j].y--;
					gotoxy(laser2[j].x,laser2[j].y);
					cout<<laser2[j].symbol;
				}
			}
		}			
	}
}

//moveBaddies() - checks for laser hit too
void moveBaddies()
{
	int k;
	for (j=0; j<settings.numBaddies; j++)
	{
		if (baddies[j].beenHit)
		{
			baddies[j].beenHit=false;
			baddies[j].y--;
			if (baddies[j].y==1)
				baddies[j].x=random(56)+2;
		}
		else if (baddies[j].y>24)
		{
			baddies[j].y=1;
			baddies[j].x=random(56)+2;
			player.score=player.score-10-(settings.difficulty*2);
			if (settings.twoPlayers)
				player2.score=player2.score-10-(settings.difficulty*2);
		}
		else
			baddies[j].y++;
	}
}

//blankAll()
void blankAll()
{
	gotoxy(player.x,player.y);
	cout<<player.blank;
	
	if (settings.twoPlayers)
	{
		gotoxy(player2.x,player2.y); 
		cout<<player2.blank;
	}
	
	for (j=0; j<settings.numBaddies; j++) 
	{	// for some reason loop wasnt working without the brackets
		gotoxy(baddies[j].x,baddies[j].y); cout<<' ';	
	}
	
	for (j=0; j<settings.numLasers;j++)
	{
		if (laser[j].isActive)
		{
			gotoxy(laser[j].x,laser[j].y); cout<<' ';
		}
		if (laser2[j].isActive)
		{
			gotoxy(laser2[j].x,laser2[j].y); cout<<' ';
		}
	}
}

//checkCollision()
bool checkCollision()
{
	bool result=false;
	
	for (j=0; j<settings.numBaddies; j++)
	{
		if ((player.x==baddies[j].x || player.x+1==baddies[j].x || player.x+2==baddies[j].x) && (player.y==baddies[j].y))
		{
			player.health--;
			result=true;
		}
		
		if (settings.twoPlayers)
		{
			if ((player2.x==baddies[j].x || player2.x+1==baddies[j].x || player2.x+2==baddies[j].x) && (player2.y==baddies[j].y))
			{
				player2.health--;
				result=true;
			}
		}
	}
	
	return result;
}

//checkScore()
void checkScore()
{
	if ((player.score/10000)+1>player.level)
	{
		settings.numBaddies=settings.numBaddies+settings.difficulty;
		player.level++;
		if (settings.twoPlayers)
			player2.level++;
		
	}
	else if (settings.twoPlayers && (player2.score/10000)+1>player2.level)
	{
		settings.numBaddies=settings.numBaddies+settings.difficulty;
		player2.level++;
		player.level++;
	}
}
	
//initializeVariables()
void initializeVariables()
{
	settings.speed=400;
	settings.numBaddies=3;
	settings.numLasers=2;
	settings.difficulty=2;
	settings.twoPlayers=false;
	//initialize baddies
	for (j=0; j<maxBaddies; j++)
	{		
		baddies[j].symbol=random(8)+232;
		baddies[j].x=random(56)+2;
		baddies[j].y=random(5)+1;
		baddies[j].beenHit=false;
	}
	
	//initialize lasers
	for (j=0; j<maxLasers; j++)
	{
		laser[j].x=0;
		laser[j].y=0;
		laser[j].isActive=false;
		laser[j].symbol='|';
	}
	for (j=0; j<maxLasers; j++)
	{
		laser2[j].x=0;
		laser2[j].y=0;
		laser2[j].isActive=false;
		laser2[j].symbol='|';
	}
	
	//initialize player
	player.score=0;
	player.level=1;
	player.movement=0;
	player.health=5;
	player.x=20;
	player.y=25;
	player.ship="/=\\";
	player.blank="   ";
	//initialize player2
	player2.score=0;
	player2.level=1;
	player2.movement=0;
	player2.health=5;
	player2.x=10;
	player2.y=25;
	player2.ship="\\^/";
	player2.blank="   ";
	
	//read in high scores from highscor.sco
	ifstream fp_in("highscor.sco", ios::in);  // declare and open
	
	for (j=0;j<3;j++)
	{
		fp_in>>settings.highscorenames[j];
		fp_in>>(int)settings.highscores[j];
	}
	
	fp_in.close();
}
	
//endGame(int)
void endGame(int score)
{
	clrscr();
	gotoxy(1,23); cout<<"You've died, your final score was: "<<score<<endl;
	
	//if highscore then get name and write score to file.
	ofstream fp_out("highscor.sco", ios::out);  // declare and open
	
	//player 1 check
	if (player.score>settings.highscores[0])
	{
		settings.highscorenames[2]=settings.highscorenames[1];
		settings.highscores[2]=settings.highscores[1];
		settings.highscorenames[1]=settings.highscorenames[0];
		settings.highscores[1]=settings.highscores[0];
		gotoxy(1,24);
		cout<<"\nCongrats player 1, enter your name for the high score list: ";
		cin>>settings.highscorenames[0];
		settings.highscores[0]=player.score;
	}
	else if (player.score>settings.highscores[1])
	{
		settings.highscorenames[2]=settings.highscorenames[1];
		settings.highscores[2]=settings.highscores[1];
		gotoxy(1,24);
		cout<<"\nCongrats player 1, enter your name for the high score list: ";
		cin>>settings.highscorenames[1];
		settings.highscores[1]=player.score;		
	}
	else if (player.score>settings.highscores[2])
	{
		gotoxy(1,24);
		cout<<"\nCongrats player 1, enter your name for the high score list: ";
		cin>>settings.highscorenames[2];
		settings.highscores[2]=player.score;
	}
	
	//player 2 check
	if (player2.score>settings.highscores[0])
	{
		settings.highscorenames[2]=settings.highscorenames[1];
		settings.highscores[2]=settings.highscores[1];
		settings.highscorenames[1]=settings.highscorenames[0];
		settings.highscores[1]=settings.highscores[0];
		gotoxy(1,25);
		cout<<"\nCongrats player 2, enter your name for the high score list: ";
		cin>>settings.highscorenames[0];
		settings.highscores[0]=player2.score;
	}
	else if (player2.score>settings.highscores[1])
	{
		settings.highscorenames[2]=settings.highscorenames[1];
		settings.highscores[2]=settings.highscores[1];
		gotoxy(1,25);
		cout<<"\nCongrats player 2, enter your name for the high score list: ";
		cin>>settings.highscorenames[1];
		settings.highscores[1]=player2.score;		
	}
	else if (player2.score>settings.highscores[2])
	{
		gotoxy(1,25);
		cout<<"\nCongrats player 2, enter your name for the high score list: ";
		cin>>settings.highscorenames[2];
		settings.highscores[2]=player2.score;
	}
	
	fp_out<<settings.highscorenames[0]<<endl;
	fp_out<<settings.highscores[0]<<endl;
	fp_out<<settings.highscorenames[1]<<endl;
	fp_out<<settings.highscores[1]<<endl;
	fp_out<<settings.highscorenames[2]<<endl;
	fp_out<<settings.highscores[2]<<endl;
	
	fp_out.close();
	
	//print high score table
	cout<<endl<<settings.highscorenames[0]<<"    "<<settings.highscores[0]<<endl;
	cout<<settings.highscorenames[1]<<"    "<<settings.highscores[1]<<endl;
	cout<<settings.highscorenames[2]<<"    "<<settings.highscores[2]<<endl;
}

//Main method
int main()
{
	//initial commands
	initializeVariables();	//set starting values for variables
	showMenu();		//show menu to select difficulty
	_setcursortype(0);	//set to no cursor
	clrscr();			//clear screen
	randomize();		//start randomize timer
	
	//draw seperator bar
	for (j=1; j<26;j++)
	{
		gotoxy(60,j); cout<<'|';
	}
		
	//main loop
	while (true)
	{
		//calculation loop, only break out when a key is pressed
		while (kbhit()==0)
		{
			//check the score checkpoints
			checkScore();
			
			//check for collision
			if (checkCollision() && (player.health<1||player2.health<1))
			{
				//player is dead, end game, halt program
				endGame(player.score+player2.score);
				cout<<"Press space to quit...";
				while (getch()!=' '){}
				return 0;
			}
			
			//blank the ship/characters/laser
			blankAll();
			
			//check laser hit
			checkLaserHit();
			
			//do movements				
			movePlayer();
			moveLasers();
			moveBaddies();
			
			//draw player/baddies/status
			drawShip();
			drawBaddies();
			drawStatus();
			
			//pause code
			//for (j=0;j<100000;j++)
			//	cout<<"";
			Sleep((500-settings.speed));
		}
		
		//set movement of ship
		key=getch();
		checkKeypress(key);
		
		if (key=='q'||key=='Q')
			break;
		
		key=0;	//set to an unused key.
	}
	
	endGame(player.score);
	
	//end of program
	cout<<"Press space to quit...";
	while (getch()!=' '){}
	return 0;
}