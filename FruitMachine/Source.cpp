/**
 * @file Source.cpp
 * @author Vasco Pinto
 * @brief Fruit Machine
 * @version 2.0
 * @date 2019-10-15
 * @modified 2019-11-16
 *
 * @copyright Copyright (c) 2019
 *
 */
#include <iostream>
#include <string> 
#include <vector>
#include <map>

#include <cstdlib> // To use srand()
#include <ctime> // To use srand()

#include <curses.h> // External library to control console screen (e.g. clear just one column of the screen without needing to clear the whole screen and print everything again)

#include <thread> // To define the rotational speed of the columns
#include <chrono> // To define the rotational speed of the columns

using namespace std;

constexpr auto speed = 50; // Defining the speed of the slot machine in miliseconds
constexpr auto price = 15; // Price to play the game

int credit = 100; // Initial credit. Declared as global variable so every function can access it without having to receive it as an argument
map<string, int> stats; // Pair of values to store nr of occurrences of a result and cash-flow. Declared as global variable so every function can access it without having to receive it as an argument

/**
 * @brief Clears specific lines in the console while keeping everything else untouched.
 *
 * @param line to clean
 */
void clearLine(int line) {
	int y, x;            // To store current position of the cursor
	getyx(stdscr, y, x); // Store current position of the cursor
	move(line, 0);       // Move to begining of line to clear
	clrtoeol();          // Clear line
	move(y, x);          // Move back to saved position
}

/**
 * @brief Prints the ASCII art on the top and shows the amount of credits left on the bottom left corner.
 *
 */
void banner() {

	mvaddstr(0, 0, R"(
		 _____                  _   _      __  __                  _       _
		|  ___|  _ __   _   _  (_) | |_   |  \/  |   __ _    ___  | |__   (_)  _ __     ___
		| |_    | '__| | | | | | | | __|  | |\/| |  / _` |  / __| | '_ \  | | | '_ \   / _ \
		|  _|   | |    | |_| | | | | |_   | |  | | | (_| | | (__  | | | | | | | | | | |  __/
		|_|     |_|     \__,_| |_|  \__|  |_|  |_|  \__,_|  \___| |_| |_| |_| |_| |_|  \___|


)");
	mvaddstr(6, 86, "by Vasco Pinto");

	clearLine(LINES - 2);

	mvprintw(LINES - 2, COLS - 15, "Credit = %d", credit);

	refresh();
}

/**
 * @brief Prints the structer of the slot as well as the ASCII art representing a lever.
 *
 */
void printFrame() {

	clear();

	banner();

	for (unsigned short int i = 8; i < 19; i++) {
		if (i == 8 || i == 18) { // If it's the top or bottom
			mvaddstr(i, 33, "#################################");
		}
		else if (i == 13) { // If it's the middle
			mvaddstr(i, 33, "#==>|                       |<==#");
		}
		else {
			mvaddstr(i, 33, "#                               #");
		}
	}


	mvaddstr(9, 73, R"( ___ (@) )");
	mvaddstr(10, 73, R"(|.-.|/ )");
	mvaddstr(11, 73, R"(|| |/ )");
	mvaddstr(12, 73, R"(|| /|)");
	mvaddstr(13, 73, R"(||/||)");
	mvaddstr(14, 73, R"(|| ||)");
	mvaddstr(15, 73, R"(|| ||)");
	mvaddstr(16, 73, R"('---')");

	mvaddstr(17, 68, "Press [ENTER] to");
	mvaddstr(18, 68, "lock each column");

	refresh();
}

/**
 * @brief Waits for a key to be pressed in order to continue.
 *
 */
void waitForKey() {

	curs_set(1);
	move(LINES - 2, 1);// Move cursor to the bottom left of the screen
	refresh();
	system("pause");
	curs_set(0);
}

/**
 * @brief Displays text right in the middle of the screen.
 *
 * @param message1 Message to be displayed.
 * @param message2 Optional parameter. This message will be displayed right below the first one.
 */
void displayCentralMessage(const char* message1, const char* message2 = NULL) {

	string str1 = message1;
	int center1 = str1.length() / 2; // Calculates the center of the message

	clear();
	banner();

	mvaddstr((LINES / 2), (COLS / 2) - center1, message1);

	if (message2 != NULL) { // If there are two messages to be displayed, do the same as for the first message but on the line below.
		string str2 = message2;
		int center2 = str2.length() / 2;
		mvaddstr((LINES / 2 + 1), (COLS / 2) - center2, message2);
	}

	refresh();
}

/**
 * @brief Generates a random fruit from a pre-defined list.
 *
 * @return string Random slot machine fruit symbol.
 */
string slotSymbols() {

	string symbols[13] = { "APPLES", "BANANA", "CHERRY", "LEMONS", "GRAPES", "ORANGE", "DIAMOND", "MELONS", "APRICOT", "KIWIS", "MANGO", "PEACH", "PEARS" };

	return symbols[rand() % 13]; // Random number between 0-12
}

/**
 * @brief Controls the speed of the rotating columns.
 *
 */
void colsRotating() {
	this_thread::sleep_for(chrono::milliseconds(speed));
}

/**
 * @brief Clears specific columns of the slot machine.
 *
 * @param firstColLocked In case this parameter is true, the function will only clear the second and third columns.
 * @param secondColLocked In case this parameter is true, the function will only clear the third column.
 */
void clearSlot(bool firstColLocked = false, bool secondColLocked = false) {

	if (!firstColLocked && !secondColLocked) { // If both the first and second columns haven't been locked yet, clear all columns.
		for (unsigned short int i = 10; i < 17; i++) {

			mvaddstr(i, 38, "       ");
			mvaddstr(i, 46, "       ");
			mvaddstr(i, 54, "       ");
		}
	}
	else if (firstColLocked && !secondColLocked) { // If the first has been locked, clear only the second and third columns.
		for (unsigned short int i = 10; i < 17; i++) {

			mvaddstr(i, 46, "       ");
			mvaddstr(i, 54, "       ");
		}
	}
	else if (firstColLocked && secondColLocked) { // If both the first and second columns have been locked, clear only the third column.
		for (unsigned short int i = 10; i < 17; i++) {

			mvaddstr(i, 54, "       ");
		}
	}
	refresh();
}

/**
 * @brief Based on the result of the slot machine, updates the user's credits according to the prize.
 *
 * @param firstCol Vector of strings containing all the symbols of the first column.
 * @param secondCol Vector of strings containing all the symbols of the second column.
 * @param thirdCol Vector of strings containing all the symbols of the third column.
 * @return int Returns the amount of points the user got.
 */
int updateCredits(vector<string> firstCol, vector<string> secondCol, vector<string> thirdCol) {

	/*
	50pts for each DIAMOND

	DIAMOND DIAMOND DIAMOND - 1000pts
	BANANA BANANA BANANA		- 150pts
	DIAMOND DIAMOND BANANA	- 110pts
	BANANA BANANA DIAMOND	- 60pts
	BANANA APPLES DIAMOND	- 50pts
	BANANA BANANA APPLES		- 10pts
	BANANA APPLES ORANGE		- 0pts
	 */

	int points = 0;

	if ((firstCol[3] == secondCol[3]) && (firstCol[3] == thirdCol[3])) {

		stats["Jackpots"]++;

		if (firstCol[3] == "DIAMOND") {// DIAMOND DIAMOND DIAMOND - 1000pts

			stats["Diamonds"] += 3;

			points += 1000;
		}
		else {// BANANA BANANA BANANA - 150pts
			points += 150;
		}
	}
	else if ((firstCol[3] == secondCol[3]) || (firstCol[3] == thirdCol[3]) || (secondCol[3] == thirdCol[3])) {// BANANA BANANA APPLES - 10pts

		stats["2Symbols"]++;
		points += 10;

		if (firstCol[3] == "DIAMOND") {// DIAMOND BANANA BANANA - 60pts
			stats["Diamonds"]++;
			points += 50;
			if (secondCol[3] == "DIAMOND" || thirdCol[3] == "DIAMOND") {// DIAMOND DIAMOND BANANA - 110pts
				stats["Diamonds"]++;
				points += 50;
			}
		}
		else if (secondCol[3] == "DIAMOND") {// BANANA DIAMOND BANANA - 60pts
			stats["Diamonds"]++;
			points += 50;
			if (thirdCol[3] == "DIAMOND") {// BANANA DIAMOND DIAMOND - 110pts
				stats["Diamonds"]++;
				points += 50;
			}
		}
		else if (thirdCol[3] == "DIAMOND") {// BANANA BANANA DIAMOND - 60pts
			stats["Diamonds"]++;
			points += 50;
		}
	}
	else if (firstCol[3] == "DIAMOND" || secondCol[3] == "DIAMOND" || thirdCol[3] == "DIAMOND") {// BANANA APPLES DIAMOND - 50pts
		stats["Diamonds"]++;
		points += 50;
	}
	stats["Total"]++;
	return points;
}

/**
 * @brief Displays the slot machine columns, calls colsRotating to wait the specified amount of time, then clears all columns.
 *
 */
void printRotCols() {

	for (unsigned short int i = 10; i < 17; i++) { // 3 columns with 7 lines each
		mvaddstr(i, 38, slotSymbols().c_str()); // We need to convert to C string to use with mvaddstr()
		mvaddstr(i, 46, slotSymbols().c_str());
		mvaddstr(i, 54, slotSymbols().c_str());
	}
	refresh();

	colsRotating();

	clearSlot();

}

/**
 * @brief Locks the first column, i.e. the symbols in the first column don't change while the ones in the other columns are changing.
 *
 * @param firstCol Vector of strings containing all the symbols of the first column.
 */
void lockFirstCol(vector<string> firstCol) {

	for (unsigned short int i = 10; i < 17; i++) {
		mvaddstr(i, 38, firstCol[i - 10].c_str());
		mvaddstr(i, 46, slotSymbols().c_str());
		mvaddstr(i, 54, slotSymbols().c_str());
	}
	refresh();
	colsRotating();

	clearSlot(true);
}

/**
 * @brief Locks the second column, i.e. the symbols in the first and second columns don't change while the ones in the third column are changing.
 *
 * @param firstCol Vector of strings containing all the symbols of the first column.
 * @param secondCol Vector of strings containing all the symbols of the second column.
 */
void lockSecondCol(vector<string> firstCol, vector<string> secondCol) {

	for (unsigned short int i = 10; i < 17; i++) {
		mvaddstr(i, 38, firstCol[i - 10].c_str());
		mvaddstr(i, 46, secondCol[i - 10].c_str());
		mvaddstr(i, 54, slotSymbols().c_str());
	}
	refresh();
	colsRotating();

	clearSlot(true, true);
}

/**
 * @brief Locks the third column so that all of the columns are now stopped. Also, it evaluates every column to check if the user got any prize.
 *
 * @param firstCol Vector of strings containing all the symbols of the first column.
 * @param secondCol Vector of strings containing all the symbols of the second column.
 * @param thirdCol Vector of strings containing all the symbols of the third column.
 * @return int Returns the amount of points the user got so that the points can be later evaluated and converted into prizes.
 */
int lockThirdCol(vector<string> firstCol, vector<string> secondCol, vector<string> thirdCol) {

	for (unsigned short int i = 10; i < 17; i++) {
		mvaddstr(i, 38, firstCol[i - 10].c_str());
		mvaddstr(i, 46, secondCol[i - 10].c_str());
		mvaddstr(i, 54, thirdCol[i - 10].c_str());
	}
	refresh();

	return updateCredits(firstCol, secondCol, thirdCol);
}

/**
 * @brief Prints a message with the result of the game below the slot machine frame.
 *
 * @param message Message to be displayed.
 */
void displayResult(const char* message) {
	string str = message;
	int center = str.length() / 2;

	mvaddstr(22, (COLS / 2) - center, message);

	refresh();
}

/**
 * @brief Adds the amount of points the user got to their credits and displays a message accordingly to their result.
 *
 * @param result Amount of points the user got.
 */
void evalResult(int result) {

	/*
	50pts for each DIAMOND

	DIAMOND DIAMOND DIAMOND - 1000pts
	BANANA BANANA BANANA		- 150pts
	DIAMOND DIAMOND BANANA	- 110pts
	BANANA BANANA DIAMOND	- 60pts
	BANANA APPLES DIAMOND	- 50pts
	BANANA BANANA APPLES		- 10pts
	BANANA APPLES ORANGE		- 0pts
	 */


	switch (result)
	{
	case 0: displayResult("Bad luck... Maybe next time!");

		banner();
		waitForKey();
		break;

	case 10: displayResult("Congrats!! You got two symbols!");

		credit += result;
		stats["Earned"] += result;

		banner();
		waitForKey();
		break;

	case 50: displayResult("At least you got a DIAMOND!");

		credit += result;
		stats["Earned"] += result;

		banner();
		waitForKey();
		break;

	case 60: displayResult("Congrats!! You got two symbols AND a DIAMOND!");

		credit += result;
		stats["Earned"] += result;

		banner();
		waitForKey();
		break;

	case 110: displayResult("Congrats!! You got two symbols and they are both a DIAMOND!");

		credit += result;
		stats["Earned"] += result;

		banner();
		waitForKey();
		break;

	case 150: displayResult("=====>   JACKPOT!!!   <=====");

		credit += result;
		stats["Earned"] += result;

		banner();
		waitForKey();
		break;

	case 1000: displayResult("=====>   OMG A DIAMOND JACKPOT!!!   <=====");

		credit += result;
		stats["Earned"] += result;

		banner();
		waitForKey();
		break;

	default:
		mvaddstr(20, 30, "Something went wrong with the Slot Machine...");
		mvaddstr(21, 30, "        You didn't lose credits.");

		credit += price;
		stats["Spent"] -= price;


		banner();
		waitForKey();
		break;
	}
}

/**
 * @brief One of the core functions of this program. It's responsible for calling the function to print the slot frame, for calling the function that makes the columns rotate, to catch the user input to stop each column and to call the function that checks if user get a prize or not.
 *
 * @return int Returns the amount of points the user got so that the points can be later evaluated and converted into prizes.
 */
int slotMachine() {

	nodelay(stdscr, TRUE);  // Causes getch to be a non-blocking call. If no input is ready, getch returns ERR. If disabled (bf is FALSE), getch waits until a key is pressed.

	int key;

	printFrame();

	while (true) {
		if ((key = getch()) == ERR) { // If no key is pressed then continue showing the rotating columns

			printRotCols();
		}
		else { // When key is pressed, lock first column

			vector<string> firstCol;

			for (unsigned short int i = 0; i < 7; i++) {
				firstCol.push_back(slotSymbols());
			}

			while (true) {
				if ((key = getch()) == ERR) {

					lockFirstCol(firstCol);
				}
				else { // When key is pressed, lock second column*/

					vector<string> secondCol;

					for (unsigned short int i = 0; i < 7; i++) {
						secondCol.push_back(slotSymbols());
					}

					while (true) {
						if ((key = getch()) == ERR) {

							lockSecondCol(firstCol, secondCol);
						}
						else { // When key is pressed, lock third column*/
							vector<string> thirdCol;

							for (unsigned short int i = 0; i < 7; i++) {
								thirdCol.push_back(slotSymbols());
							}

							return lockThirdCol(firstCol, secondCol, thirdCol);
						}
					}
				}
			}
		}
	}
}

//function prototype needed because the function is being called before being defined.
void printRules();

//function prototype needed because the function is being called before being defined.
void loopGame();

/**
 * @brief Prints all the possible prizes of the game and waits for user input to return to the previous menu.
 *
 */
void printPrizes() {

	clear();
	banner();

	/*
	50pts for each DIAMOND

	DIAMOND DIAMOND DIAMOND - 1000pts
	BANANA BANANA BANANA		- 150pts
	DIAMOND DIAMOND BANANA	- 110pts
	BANANA BANANA DIAMOND	- 60pts
	BANANA APPLES DIAMOND	- 50pts
	BANANA BANANA APPLES		- 10pts
	BANANA APPLES ORANGE		- 0pts
	 */
	mvaddstr(9, 30, "Prizes:");
	mvaddstr(12, 35, "Each DIAMOND            - 50 credits");
	mvaddstr(14, 35, "DIAMOND DIAMOND DIAMOND - 1000 credits");
	mvaddstr(15, 35, "BANANA BANANA BANANA    - 150 credits");
	mvaddstr(16, 35, "DIAMOND DIAMOND BANANA  - 110 credits");
	mvaddstr(17, 35, "BANANA BANANA DIAMOND   - 60 credits");
	mvaddstr(18, 35, "BANANA APPLES DIAMOND   - 50 credits");
	mvaddstr(19, 35, "BANANA BANANA APPLES    - 10 credits");
	mvaddstr(20, 35, "BANANA APPLES ORANGE    - 0 credits");


	char option; // 'r' to return to the previous menu
	do {

		mvaddstr(22, 30, "Press 'r' to return to the previous menu.");

		option = getch();

		if (option == 'r') printRules();

	} while (option != 'r');

}

/**
 * @brief Prints the game rules and waits for user input to return to the previous menu or to view the prizes.
 *
 */
void printRules() {

	clear();
	banner();

	mvaddstr(9, 20, "Rules:");
	mvprintw(12, 25, "# You start with 100 credits and each game costs you %d credits.", price);
	mvaddstr(14, 25, "# You can play the Normal Mode, the Fast Mode and the Ultra-Fast Mode:");
	mvaddstr(16, 30, "* Normal Mode(1) requires full user interaction like a normal slot machine.");
	mvaddstr(18, 30, "* Fast Mode(2) requires less user interaction but you can still control");
	mvaddstr(19, 33, "when each column stops.");
	mvaddstr(21, 30, "* Ultra-Fast Mode(3) requires no user interaction and the game will run until");
	mvprintw(22, 33, "you have less than %d credits.", price);

	char option; // 'p' for prizes or 'r' to return to the previous menu
	do {

		mvaddstr(24, 20, "Press 'p' to see the prizes or 'r' to return to the previous menu.");

		option = getch();

		if (option == 'p') printPrizes();
		else if (option == 'r') return;

	} while (option != 'p' && option != 'r');
}

/**
 * @brief Depending on the user's credit, it displays if the user is leaving with or without credits.
 *
 */
void cashOut() {

	clear();
	banner();

	if (credit > 0) {
		mvprintw(LINES / 2, 50, "Cashing out %d credits!", credit);
		mvaddstr((LINES / 2) + 1, 53, "Congratulations!!");

		waitForKey();
	}
	else {
		displayCentralMessage("You're leaving with empty pockets... Sorry!");

		waitForKey();
	}
}

/**
 * @brief Checks if user has enough credits to keep playing. Otherwise, it shows a message saying so and calls the cashOut function.
 *
 * @return char Returns '0' if user doesn't have enough credits to keep playing or '1' in case it does.
 */
char checkCredit() {
	if (credit < price) {
		displayCentralMessage("Sorry but you don't have enough credits to play.");

		cashOut();

		return '0';
	}
	else return '1';
}

/**
 * @brief Quickly fills out the columns with symbols and check if there's any winning combination and updates the credit variable accordingly.
 *
 */
void ultraFastMode() {

	vector<string> firstCol;
	for (unsigned short int i = 0; i < 7; i++) {
		firstCol.push_back(slotSymbols());
	}

	vector<string> secondCol;
	for (unsigned short int i = 0; i < 7; i++) {
		secondCol.push_back(slotSymbols());
	}

	vector<string> thirdCol;
	for (unsigned short int i = 0; i < 7; i++) {
		thirdCol.push_back(slotSymbols());
	}

	int value = updateCredits(firstCol, secondCol, thirdCol);

	credit += value;
	stats["Earned"] += value;
}

/**
 * @brief Another core function of the program. It's responsible to check if the user still has credit to play, to ask which game mode the user wants to play or to check the rules/prizes, to call the function to evaluate the game's result and even to decrement the credit variable each time the user plays a game.
 *
 * @return int Returns always 1. The function is called in a while condition, therefore the function must return a value different from '0', otherwise the while condition wouldn't be met.
 */
int playGame() {

	stats["Jackpots"] = 0; // Initialise nr occurrences of Jackpots
	stats["2Symbols"] = 0; // Initialise nr occurrences of 2 Symbols
	stats["Diamonds"] = 0; // Initialise nr occurrences of Diamonds
	stats["Total"] = 0;    // Initialise nr of games played
	stats["Spent"] = 0;    // Initialise nr of credits spent
	stats["Earned"] = 0;   // Initialise nr of credits earned

	char game = '0';

	game = checkCredit();

	if (game == '1') {// If player can play (has enough credits)
		do {

			displayCentralMessage("Which mode do you want to play? Normal: 1  ||  Fast: 2  ||  Ultra-Fast: 3", "Press 0 to see the rules of the game.");

			game = getch();

			if (game == '0') printRules();

		} while (game != '1' && game != '2' && game != '3');
	}

	if (game == '1') { // If user chose to play Normal Mode

		displayCentralMessage("You are playing Normal Mode...", "Let's start...");

		credit -= price;
		stats["Spent"] += price;

		banner();

		waitForKey();

		evalResult(slotMachine());
	}

	else if (game == '2') { // If user chose to play Fast Mode

		displayCentralMessage("You are playing Fast Mode...", "Let's hurry up!");

		banner();

		waitForKey();

		int tempGameCounter = 0; // To count each games until reach n-hundred
		char again; // To check if user wants to keep playing after X attempts

		do
		{
			tempGameCounter++;


			credit -= price;
			stats["Spent"] += price;

			evalResult(slotMachine());

			if (tempGameCounter == 10) {

				clear();
				banner();
				mvprintw(LINES / 2, 28, "You already played %d games. Do you want to continue? (No: 0  OR  Yes: 1)", stats["Total"]);

				do
				{
					again = getch();
				} while (again != '0' && again != '1');

				if (again == '0') break;
				else tempGameCounter = 0;
			}

		} while (credit >= price);

		clear();
		banner();

		mvprintw(LINES / 2, 50, "You played %d games!", stats["Total"]);
		mvaddstr((LINES / 2) + 1, 51, "Congratulations!!");

		waitForKey();
	}
	else if (game == '3') { // If user chose to play Ultra-Fast Mode
		displayCentralMessage("You are playing Ultra-Fast Mode!!!", "Let's goooo...");

		banner();

		waitForKey();

		int tempGameCounter = 0; // To count each games until reach n-hundred
		char again; // To check if user wants to keep playing after X attempts

		do
		{
			printFrame();
			printRotCols();

			tempGameCounter++;

			credit -= price;
			stats["Spent"] += price;

			ultraFastMode();

			if (tempGameCounter == 100) {

				clear();
				banner();
				mvprintw(LINES / 2, 28, "You already played %d games. Do you want to continue? (No: 0  OR  Yes: 1)", stats["Total"]);

				do
				{
					again = getch();
				} while (again != '0' && again != '1');

				if (again == '0') break;
				else tempGameCounter = 0;
			}

		} while (credit >= price);


		clear();
		banner();

		mvprintw(LINES / 2, 50, "You played %d games!", stats["Total"]);
		mvaddstr((LINES / 2) + 1, 51, "Congratulations!!");

		waitForKey();
	}
	return 1;
}

/**
 * @brief Prints some stats about the game, like how many times the user played, how many Jackpots, etc
 *
 */
void displayStats() {
	clear();
	banner();

	displayCentralMessage("Here's the Statistics of your game:", "Press [ENTER] to see");
	waitForKey();

	clear();
	banner();

	mvprintw(14, 45, "You played a Total of %d game(s).", stats["Total"]);
	mvprintw(15, 45, "You got a Jackpot in %d of them.", stats["Jackpots"]);
	mvprintw(16, 45, "You got 2 Symbols in %d of them.", stats["2Symbols"]);
	mvprintw(17, 45, "And you got Diamonds %d time(s).", stats["Diamonds"]);

	mvprintw(19, 45, "You spent %d credits.", stats["Spent"]);
	mvprintw(20, 45, "And you earned %d credits.", stats["Earned"]);

	waitForKey();
}

/**
 * @brief Another core function of the program and the first one to start the game. It's responsible for asking the user if they want to keep playing as long as they can afford it. If the user wants to quit, the function calls the cashOut function and exits.
 *
 */
void loopGame() {

	char again = '1';

	while ((playGame()) && (credit >= price) && (again == '1')) {

		do {
			displayCentralMessage("Do you want to play again? Maybe it's your chance to win the Jackpot! (No: 0  OR  Yes: 1) ");

			nodelay(stdscr, FALSE); // Causes getch() to be a blocking call. the program stops until some input is given
			again = getch();

			if (again == '0') {
				displayStats();
				cashOut();
				return;
			}
		} while (again != '0' && again != '1');
	}
	displayStats();
	cashOut();
}

/**
 * @brief Prints a goodbye message on the screen and terminates the ncurses instance.
 *
 */
void exitGame() {
	displayCentralMessage("Bye!");
	waitForKey();

	clear();
	refresh();
	endwin(); // Closes PDCurses
}

/**
 * @brief Main function. It initialises ncurses and the random seed, and sets some options related to the screen. Then it starts the main game by calling the loopGame function. After the game has ended, it calls the exitGame function and exits the program.
 *
 * @return int
 */
int main() {

	initscr();		// initialise pdcurses
	noecho();       // don't print character pressed to end the loop
	cbreak();       // don't wait for user to press ENTER after pressing a key
	curs_set(0);		// hide the cursor

	srand((unsigned int)time(NULL));

	loopGame();
	exitGame();

	return 0;
}