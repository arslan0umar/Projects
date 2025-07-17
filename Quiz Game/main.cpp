/*This code is for fully functional quiz game called "Quiz Master" that is both for solo and two player there is three different level 
of the questions easy, medium, hard.*/

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <conio.h>

using namespace std;


int checkinput, easy, medium, hard, newstate = 0, gamemode, score1 = 0, score2 = 0;
string  mainheading, player1, player2;
time_t timeofquiz;

void showdifficultylvl();
void questionloadingdisplaying();
void calculatescoreandother(int, int, int, int[], int[]);
void wrongquestionfileshow(int[], int[]);
void quizlogging(int, int, int);
void quizloggingtwoplayer();
void leaderboardshow();
void review();
int highscore(int, string);

int main() {
	int select;
	checkinput = 0, gamemode = 0;
	// Code for Main menu

	system("CLS");
	mainheading = "Welcome to QuizMaster!";
	cout << "__________________________________________________________________________________\n\n";
	cout << setw(45) << mainheading << "\n\n";
	cout << "__________________________________________________________________________________\n\n";
	cout << "1. Start Quiz.\n2. View High score\n3. Exit Game.\n\nPlease select an option (1-3): ";
	do {
		cin >> select;
		if ((select >= 1 && select <= 3) && (cin)) {
			checkinput = 1;
		}
		else {
			cout << "Invalid Input.\nPlease select an option (1-3): ";
			cin.clear();
			cin.ignore(1000, '\n');
		}
	} while (checkinput != 1);


	switch (select)
	{
	case 1:
		// Code for selecting gamemode

		system("CLS");
		mainheading = "Welcome to QuizMaster!";
		cout << "__________________________________________________________________________________\n\n";
		cout << setw(45) << mainheading << "\n\n";
		cout << "__________________________________________________________________________________\n\n";
		checkinput = 0;
		cout << "Select Gamemode: \n1.  Solo\n2.  Two Player\nPlease select an option (1 or 2): ";
		do {
			cin >> gamemode;
			if ((gamemode == 1 || gamemode == 2) && (cin)) {
				checkinput = 1;
			}
			else {
				cout << "Invalid Input.\nPlease select an option (1-3): ";
				cin.clear();
				cin.ignore(1000, '\n');
			}
		} while (checkinput != 1);

		// Code for displaying Instruction and to get usernames
		system("CLS");
		mainheading = "Instruction";
		cout << "__________________________________________________________________________________\n\n";
		cout << setw(45) << mainheading << "\n\n";
		cout << "__________________________________________________________________________________\n\n";
		cout << "1.  The quiz consits of 10 quetsions.\n\n2.  Each question is multiple-choice with four options (1, 2, 3, or 4).\n\n3.  You will have 10 seconds to answer each question.\n        ~ If you fail to respond within this time, the question will be marked as unattempted.\n\n4.  For every correct answer, you earn 10 points.\n        ~ No points will be deducted for wrong answers.\n        ~ However, unattempted questions will deduct 5 points each from your final score.\n\n" << setw(50) << "Tips for Success:\n\n" << "~ Read each question carefully before selecting an answer.\n\n~ Ensure you select only one option for each question.\n\n~ Stay focused, and manage your time effectively.\n\n" << setw(45) << "Good Luck!\n\n" << "Press any key to continue....";
		_getch();
		system("CLS");
		mainheading = "Welcome to QuizMaster!";
		cout << "__________________________________________________________________________________\n\n";
		cout << setw(45) << mainheading << "\n\n";
		cout << "__________________________________________________________________________________\n\n";
		if (gamemode == 1) {
			cout << "Enter your name: ";
			cin >> player1;
		}
		else {
			cout << "Enter Player 1 name: ";
			cin >> player1;
			cout << "Enter Player 2 name: ";
			cin >> player2;
		}
		showdifficultylvl();
		break;


	case 2:
		leaderboardshow();
		break;


	case 3:
		// Code for show exit
		system("CLS");
		mainheading = "Exit Game";
		cout << "__________________________________________________________________________________\n\n";
		cout << setw(45) << mainheading << "\n\n";
		cout << "__________________________________________________________________________________\n\n";
		char exitsel;
		do {
			cout << "Are sure you want to exit? (Y/N): ";
			cin >> exitsel;
			if (exitsel == 'Y' || exitsel == 'y') {
				cout << "\nThank you for playing QuizMaster! Goodbye!" << endl;
				exit(0);
			}
			else if (exitsel == 'N' || exitsel == 'n') {
				main();
			}
			else {
				cout << "Invalid Input" << endl;
				cin.clear();
				cin.ignore(1000, '\n');
			}
		} while (exitsel != 'Y' && exitsel != 'y');
	}


	system("pause");
	return 0;
}

/*showdifficultylvl() function show the menu for selcting difficulty level and then according to diffculty update the value easy, medium, hard and call the 
questionloading function*/
void showdifficultylvl() {
	int sel_dif_lvl;
	easy = medium = hard = 0;
	system("CLS");
	mainheading = "Select Difficulty Level";
	cout << "__________________________________________________________________________________\n\n";
	cout << setw(45) << mainheading << "\n\n";
	cout << "__________________________________________________________________________________\n\n";
	cout << "1. Easy.\n2. Medium.\n3. Hard.\n\nPlease select an option (1-3): ";
	checkinput = 0;
	do {
		cin >> sel_dif_lvl;
		if ((sel_dif_lvl >= 1 && sel_dif_lvl <= 3) && (cin)) {
			checkinput = 1;
		}
		else {
			cout << "Invalid Input.\nPlease select an option (1-3): ";
			cin.clear();
			cin.ignore(1000, '\n');
		}
	} while (checkinput != 1);
	switch (sel_dif_lvl)
	{
	case 1:
		easy = 1;
		questionloadingdisplaying();
		break;
	case 2:
		medium = 1;
		questionloadingdisplaying();
		break;
	case 3:
		hard = 1;
		questionloadingdisplaying();
	}
}

/*questionloadingdisplaying() function is the most important function this function load and display question according to difficulty and then check the 
answer as well*/
void questionloadingdisplaying() {
	string gettext, opt1, opt2, opt3, opt4, correct, opt[4], filename;
	int option, alreadyquestion[20] = { 0 }, wrongquestionfile[10] = { 0 }, wrongquestionruntime[10] = { 0 }, filequestno = 0, randomno, corrcount = 0, wrongcount = 0, randomindex, unattempted = 0, turn = 1, questionsize = 0;
	bool correctflag, chkrepeat;
	timeofquiz = time(0); // store the time when quiz start
	srand(time(0));

	// Code for Opening the file to read questions
	ifstream questionfile;
	if (newstate == 0) {
		if (easy == 1)
			filename = "questionseasy.txt";
		else if (medium == 1)
			filename = "questionsmedium.txt";
		else
			filename = "questionshard.txt";
	}
	else if (newstate == 1) { // check if newstate active mean you have to open new question files
		if (easy == 1)
			filename = "newquestionseasy.txt";
		else if (medium == 1)
			filename = "newquestionsmedium.txt";
		else
			filename = "newquestionshard.txt";
	}
	questionfile.open(filename);

	if (gamemode == 1) // check the gamemode then update the value of questions
		questionsize = 10;
	else
		questionsize = 20;


	int k = 0, qno = 1;
	while (k < questionsize) {
		chkrepeat = false;
		randomno = rand() % 30 + 1;
		for (int m = 0; m < 20; m++) { // for checking that no random value repeat
			if (alreadyquestion[m] == randomno)
				chkrepeat = true;
		}
		if (chkrepeat == true) {
			continue;
		}
		alreadyquestion[k] = randomno;
		questionfile.seekg(0);

		// Code for displaying question and there four options on screen
		system("CLS");
		mainheading = "Quiz Time!";
		cout << "__________________________________________________________________________________\n\n";
		cout << setw(45) << mainheading << "\n\n";
		cout << "__________________________________________________________________________________\n\n";
		if (gamemode == 2) {
			if (turn == 1)
				cout << player1 << "'s turn!\n";
			else
				cout << player2 << "'s turn!\n";
		}
		int g = 1;
		while (getline(questionfile, gettext)) {
			correctflag = false;
			if (g == 1 || (g - 1) % 7 == 0) { // it can check that only question no. can be check for further processing
				filequestno = stoi(gettext);
				if (randomno == filequestno) {
					cout << "Question " << qno << ":  ";
					int i = 1;
					while (1) {
						if (i == 1) {
							getline(questionfile, gettext);
							cout << gettext << endl;
						}
						else if (i == 2) {
							int j = 0;
							while (j < 4) {
								getline(questionfile, gettext);
								opt[j] = gettext;
								j++;
							}
							for (int b = 0; b < 4; b++) {
								randomindex = rand() % 4;
								string temp = opt[b];
								opt[b] = opt[randomindex];
								opt[randomindex] = temp;
							}
							for (int b = 0; b < 4; b++) {
								cout << b + 1 << ". " << opt[b] << endl;
							}
						}
						else if (i == 3)
						{
							getline(questionfile, gettext);
							correct = gettext;
							break;
						}
						i++;
					}
					checkinput = 0;
					cout << "You have 10 seconds to Answer.\n";
					cout << "Enter your answer (1-4): ";

					// Code for setting the timer if user enter answer after 10 sec code show Time passed and break the loop and moves towards next question 
					time_t start = time(0);
					do {
						cin >> option;
						if ((option >= 1 && option <= 4) && (cin)) {
							checkinput = 1;
						}
						else {
							cout << "Invalid Input.\nPlease select an option (1-4): ";
							cin.clear();
							cin.ignore(1000, '\n');
						}
					} while (checkinput != 1);
					time_t current = time(0);
					time_t covertime = difftime(current, start);
					if (covertime >= 10) {
						cout << "\nTime Passed." << endl;
						if (gamemode == 2) {
							if (turn == 1) 
								score1 -= 5;
							else 
								score2 -= 5;
						}
						else {
							score1 -= 5;
						}
						wrongquestionfile[k] = randomno;
						wrongquestionruntime[k] = k + 1;
						cout << "Press any key for next question: ";
						_getch();
						unattempted++;
						break;
					}

					// Code for checking the answer is correct or not then accoring to this provide feedback to user
					if (opt[option - 1] == correct) {
						correctflag = true;
						if (gamemode == 2) {
							if (turn == 1) 
								score1 += 10;
							else 
								score2 += 10;
						}
						else {
							score1 += 10;
						}
					}
					else {
						correctflag = false;
						wrongquestionfile[k] = randomno;
						wrongquestionruntime[k] = k + 1;
					}

					system("CLS");
					if (correctflag == true) {
						corrcount++;
						mainheading = "Correct!";
						cout << "__________________________________________________________________________________\n\n";
						cout << setw(45) << mainheading << "\n\n";
						cout << "__________________________________________________________________________________\n\n";
						cout << "\nGreat job! You've earned 10 points." << endl;
					}
					else {
						wrongcount++;
						mainheading = "Wrong!";
						cout << "__________________________________________________________________________________\n\n";
						cout << setw(45) << mainheading << "\n\n";
						cout << "__________________________________________________________________________________\n\n";
						cout << "\nTHe correct answer is: " << correct << endl;
					}
					cout << "Press any key for next question: ";
					_getch();
					break;
				}
			}
			g++;
		}
		if (gamemode == 2) { // if for changing the terms between player 1 and 2
			if (turn == 1)
				turn = 2;
			else
			{
				turn = 1;
				qno++;
			}
		}
		else
		{
			qno++;
		}
		k++;
	}
	questionfile.close();
	calculatescoreandother(corrcount, wrongcount, unattempted, wrongquestionfile, wrongquestionruntime);
}

/* calculatescoreandother() this function display the total score and other on the user screen */
void calculatescoreandother(int corrcount, int wrongcount, int unattempted, int wrongquestionfile[], int wrongquestionruntime[]) {
	char wrongsel, againsel, reviewsel, highscoreplayer1, highscoreplayer2;
	system("CLS");
	mainheading = "Quiz Completed";
	cout << "__________________________________________________________________________________\n\n";
	cout << setw(45) << mainheading << "\n\n";
	cout << "__________________________________________________________________________________\n\n";

	// Code for displaying the final score and to decide which player won
	if (gamemode == 1) {
		cout << "Your Final Score: " << score1 << " points\n";
		if (highscore(score1, player1))
			cout << "Congratulations You made a HighScore!\n";
	}
	else {
		cout << "Final Scores:\n";
		cout << player1 << ": " << score1 << " points\n";
		cout << player2 << ": " << score2 << " points\n";

		if (score1 > score2)
			cout << "Congratulations " << player1 << "! You won the game!\n";
		else if (score2 > score1)
			cout << "Congratulations " << player2 << "! You won the game!\n";
		else
			cout << "It's a tie! Well played both!\n";
		for (int i = 1; i <= 2; i++) {
			if (i == 1)
				highscoreplayer1 = highscore(score1, player1);
			else
				highscoreplayer2 = highscore(score2, player2);
		}
		if (highscoreplayer1 == 1) 
			cout << "Congratulations "<< player1 <<  " made a HighScore!\n";

		else if (highscoreplayer2 == 1)
			cout << "Congratulations " << player2 << " made a HighScore!\n";
		else if (highscoreplayer1 == 1 && highscoreplayer2 == 1)
			cout << "Congratulations " << player1 << " and " << player2 << " made a HighScore!\n";
	}

	// Code for displaying the count correct answer and wrong answer and unattempted question only work in solo game
	if (gamemode == 1){
		cout << "Correct Answers: " << corrcount << endl;
		cout << "Wrong Answers: " << wrongcount << endl;
		cout << "Unattempted Question: " << unattempted << endl;
	}

	// Code for displaying wrong and unattempted question only work in solo game
	if (gamemode == 1){
		do {
			cout << "Want to check the unattempted and wrong questions? (Y/N): ";
			cin >> wrongsel;
			if (wrongsel == 'Y' || wrongsel == 'y') {
				wrongquestionfileshow(wrongquestionfile, wrongquestionruntime);
			}
			else if (wrongsel == 'N' || wrongsel == 'n') {
				break;
			}
			else {
				cout << "Invalid Input" << endl;
				cin.clear();
				cin.ignore(1000, '\n');
			}
		} while (wrongsel != 'Y' && wrongsel != 'y');
	}

	// code for storing quizlog
	if (gamemode == 1){
		quizlogging(corrcount, wrongcount, unattempted);
	}
	else {
		quizloggingtwoplayer();
	}

	// code for asking the user to want to play again if user user press yess moves the user to difficulty menu and show new questions
	if (newstate == 0){
		do {
			cout << "\nDo you want to play again with new questions? (Y/N): ";
			cin >> againsel;
			if (againsel == 'Y' || againsel == 'y') {
				newstate = 1;
				showdifficultylvl();
			}
			else if (againsel == 'N' || againsel == 'n') {
				break;
			}
			else {
				cout << "Invalid Input" << endl;
				cin.clear();
				cin.ignore(1000, '\n');
			}
		} while (againsel != 'Y' && againsel != 'y');
	}

	// code for getting review from user
	do {
		cout << "\nDid you enjoy using Quiz Master? Would you like to leave us a review or suggest a new question to add to the quiz? (Y/N): ";
		cin >> reviewsel;
		if (reviewsel == 'Y' || reviewsel == 'y') {
			review();
		}
		else if (reviewsel == 'N' || reviewsel == 'n') {
			break;
		}
		else {
			cout << "Invalid Input" << endl;
			cin.clear();
			cin.ignore(1000, '\n');
		}
	} while (reviewsel != 'Y' && reviewsel != 'y');


	cout << "\nThank you for playing QuizMaster!" << endl;
	cout << "Press any key to return to the main menu";
	_getch();
	main();
}

/* wrongquestionfileshow() this function display the wrong and unattempet answer on the screen along with there correct options*/
void wrongquestionfileshow(int wrongquestionfile[], int wrongquestionruntime[]) {
	string gettext, fileplayer1;
	int filequestno;
	system("CLS");
	mainheading = "Wrong Questions! & Unattempted Questions";
	cout << "__________________________________________________________________________________\n\n";
	cout << setw(55) << mainheading << "\n\n";
	cout << "__________________________________________________________________________________\n\n";

	ifstream forwrongquestionfile;
	if (easy == 1)
		fileplayer1 = "questionseasy.txt";
	else if (medium == 1)
		fileplayer1 = "questionsmedium.txt";
	else
		fileplayer1 = "questionshard.txt";
	forwrongquestionfile.open(fileplayer1);

	// code for loadding and displaying only wrong question from the file
	int k = 0;
	for (int i = 0; i < 10; i++) {
		if (wrongquestionfile[i] == 0)
			continue;
		forwrongquestionfile.seekg(0);
		k++;
		int g = 1;
		while (getline(forwrongquestionfile, gettext)) {
			if (g == 1 || (g - 1) % 7 == 0) {
				filequestno = stoi(gettext);
				if (wrongquestionfile[i] == filequestno) { // match the question no with the wrong question no.
					cout << "Question " << wrongquestionruntime[i] << ":  ";
					getline(forwrongquestionfile, gettext);
					cout << gettext << endl;
					for (int k = 1; k <= 4; k++) {
						getline(forwrongquestionfile, gettext);
						cout << k << ".  " << gettext << endl;
					}
					getline(forwrongquestionfile, gettext);
					cout << "Correct Answer is: " << gettext << endl << endl;
					break;
				}
			}
			g++;
		}
	}
}

/* quizlogging() this function write data in the quiz logging file for check which play game at which time*/
void quizlogging(int corrcount, int wrongcount, int unattempted) {
	char quiztime[26];
	ctime_s(quiztime, 26, &timeofquiz);
	ofstream quizlog("quizlog.txt", ios::app);
	quizlog << "Name: " << player1 << "\t Score: " << score1 << "\nDate and Time: " << quiztime << "Correct Answer: " << corrcount << "  |  Wrong Answer: " << wrongcount << "  |  Unattempted Answer: " << unattempted << "\n\n";
	quizlog.close();
}

/*  quizloggingtwoplayer() this function also write data in quiz logging file but when two player mode is activate*/
void quizloggingtwoplayer() {
	char quiztime[26];
	ctime_s(quiztime, 26, &timeofquiz);
	ofstream quizlog("quizlog.txt", ios::app);
	quizlog << "Twoplayer\nPlayer1 name: " << player1 << "\t Score: " << score1 << "\nPlayer2 name: " << player2 << "\t Score: " << score2 << "\nDate and Time: " << quiztime << "\n\n";
	quizlog.close();
}

/* highscore() this function is used to update the leaderboard by comparing the current score with the exciting scores in leaderboard*/
int highscore(int score, string name) {
	int pointshighscore[5] = { 0 }, updatepointshighscore[5] = { 0 }, checkhighscore = 0;
	string player1high[5], updateplayer1high[5], gettext;
	ifstream readhighscore("highscores.txt");
	int j = 0, k = 0;
	for (int i = 1; i <= 10; i++) { // read the exciting records in leaderboard
		getline(readhighscore, gettext);
		if (i % 2 != 0) {
			pointshighscore[j] = stoi(gettext);
			j++;
		}
		else {
			player1high[k] = gettext;
			k++;
		}
	}
	readhighscore.close();
	int b;
	for (b = 0; b < 5; b++) {
		if (pointshighscore[b] < score) {
			checkhighscore = 1;
			break;
		}
	}
	if (checkhighscore == 1) { // compare the current score with leaderboard scores then make an update according to this in such a way that the person at the bottom out and the remaining names moves downward
		for (int m = 0; m < 5; m++) {
			updatepointshighscore[m] = pointshighscore[m];
			updateplayer1high[m] = player1high[m];
		}
		updatepointshighscore[b] = score;
		updateplayer1high[b] = name;
		if (!(b == 4)) {
			updatepointshighscore[b + 1] = pointshighscore[b];
			updateplayer1high[b + 1] = player1high[b];
		}
		if (!(b == 3 || b == 4)) {
			updatepointshighscore[b + 2] = pointshighscore[b + 1];
			updateplayer1high[b + 2] = player1high[b + 1];
		}
		if (b == 1 || b == 0) {
			updatepointshighscore[b + 3] = pointshighscore[b + 2];
			updateplayer1high[b + 3] = player1high[b + 2];
		}
		if (b == 0) {
			updatepointshighscore[b + 4] = pointshighscore[b + 3];
			updateplayer1high[b + 4] = player1high[b + 3];
		}
		ofstream writehighscore("highscores.txt");
		for (int i = 0; i < 5; i++) {
			writehighscore << updatepointshighscore[i] << "\n" << updateplayer1high[i] << "\n";
		}
		writehighscore.close();
	}
	if (b == 0) { // if user make a highscore it return 1
		return 1;
	}
	return 0;
}

/* leaderboardshow() display leaderboard on screen*/
void leaderboardshow() {
	string gettext;
	system("CLS");
	mainheading = "LeaderBoard!";
	cout << "__________________________________________________________________________________\n\n";
	cout << setw(45) << mainheading << "\n\n";
	cout << "__________________________________________________________________________________\n\n";
	ifstream readhighscores("highscores.txt");
	for (int i = 1; i <= 5; i++) {
		getline(readhighscores, gettext);
		cout << i << ".  Points: " << gettext;
		getline(readhighscores, gettext);
		cout << "  |  Name: " << gettext << endl;
	}
	readhighscores.close();
	cout << "\nPress any Key to return to main menu: ";
	_getch();
	main();
}

/* review() open review file and write review of the user*/
void review() {
	string reviewinput;
	cin.ignore();
	cout << "\nWe would love to hear from you! Enter your review or suggest a new quiz question here: ";
	getline(cin, reviewinput);
	ofstream reviewpurpose("review.txt");
	reviewpurpose << reviewinput << "\n\n";
	reviewpurpose.close();
	cout << "Review Submited Successfully" << endl;
}