#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>
#include<iomanip>
#include<windows.h>
using namespace std;

const bool debug = false;

int roundCount = 0;	//counter of rounds
int rowStatus[3][4];	//the type of cards in each position of rows
int cardType[77];	//store the types of cards waiting to be drawn
bool gottenCards[4];	//whether the user has drawn cards of any row
int lastGotPlayer = 0;	//remember the last player to draw cards
int player;	//the current player
int cardNum = 4;	//store the next card to be drawn
bool endCardAppeared = false;
bool roundEnd;
int action;	//1:Draw a coloretto  2:Get the coloretto on some row
int score[4];
int gottenCardCount[10][4];	//count how many cards each one has gotten
string input;

void generate_cardType();
void printCurrent();
void selectByPlayer();
void putCardIntoRow(int);
void getTheWholeRow(int);
void selectByAI();
void computeScore();

int main() {
	// initialize gottenCardCount[10][4] //
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			gottenCardCount[j][i] = 0;
		}
	}
	generate_cardType();
	player = 0;

	// the whole game //
	while (!endCardAppeared) {
		roundCount++;

		// initialize rowStatus[3][4] //
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				rowStatus[j][i] = 0;
			}
		}
		player = lastGotPlayer;
		for (int i = 0; i < 4; i++) {
			gottenCards[i] = false;
		}

		roundEnd = false;
		// a round //
		while (!roundEnd) {
			//playing by player and Bot //
			switch (player) {
			case 0:
				selectByPlayer();
				break;
			case 1:
			case 2:
			case 3:
				selectByAI();
				break;
			}

			// check if all rows are gotten //
			if ((rowStatus[2][0] == -2 && rowStatus[2][1] == -2) && (rowStatus[2][2] == -2 && rowStatus[2][3] == -2)) {
				roundEnd = true;
			}

			// decide the next player //
			else {
				while (1) {
					if (player <= 2) {
						player++;
					}
					else {
						player = 0;
					}
					if (gottenCards[player] == false) {
						break;
					}
				}
			}
		}
	}

	// count score //
	computeScore();
	system("cls");
	cout << "Score:" << endl << endl;
	cout << "Your score is " << score[0] << endl;
	cout << "Bot(1)'s score is " << score[1] << endl;
	cout << "Bot(2)'s score is " << score[2] << endl;
	cout << "Bot(3)'s score is " << score[3] << endl;

	// show the winner //
	bool win = true;
	for (int i = 1; i < 4; i++) {
		if (score[0] < score[i]) {
			win = false;
		}
	}
	if (win) {
		cout << "You Win!!" << endl << endl;
	}
	else {
		cout << "You lose..." << endl << endl;
	}

	system("pause");
	return 0;
}

void generate_cardType() {
	cardType[61] = -1;	//the end card
	int counter[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	// count cards of each type //
	srand(time(NULL));
	int gotten;
	for (int i = 0; i < 4; i++) {
		gotten = rand() % 7 + 1;
		if (counter[gotten] != 0) {
			i--;
			continue;
		}
		gottenCardCount[gotten][i]++;
		counter[gotten]++;
	}

	int i = 4;	//counter of the while loop below
	while (i < 77) {
		if (i == 61) {
			i++;
			continue;
		}
		cardType[i] = rand() % 9 + 1;
		counter[cardType[i]]++;	//update the total amount of each type
		switch (cardType[i]) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			if (counter[cardType[i]] > 9) {
				counter[cardType[i]] --;
				break;
			}
			else {
				i++;
				break;
			}
		case 8:
			if (counter[cardType[i]] > 10) {
				counter[cardType[i]] --;
				break;
			}
			else {
				i++;
				break;
			}
		case 9:
			if (counter[cardType[i]] > 3) {
				counter[cardType[i]] --;
				break;
			}
			else {
				i++;
				break;
			}
		}
	}

	// Debug //
	if (debug) {
		cout << "============Debug============" << endl << endl;
		cout << "Values of variables in generate_cardType():" << endl << endl;
		for (int i = 0; i < 10; i++) {
			cout << "counter[" << i << "] = " << setw(2) << counter[i] << "   ";
			if (i == 4 || i == 9) {
				cout << endl;
			}
		}
		cout << endl;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 10; j++) {
				cout << "Count[" << j << "][" << i << "] = " << setw(2) << gottenCardCount[j][i] << "   ";
				if (j == 4 || j == 9) {
					cout << endl;
				}
			}
			cout << endl;
		}
		cout << "============Debug============" << endl << endl;
		system("pause");
	}
}

void printCurrent() {
	// show the current round //
	cout << "Round " << setw(2) << roundCount << endl << endl;
	if (debug) {
		cout << "============Debug============" << endl << endl;
		cout << "cardNum = " << cardNum << endl << endl;
		cout << "============Debug============" << endl << endl;
	}
	// show cards of each player //
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 38; j++) {
			cout << "¡X";
		}
		cout << endl;

		cout << "|  ";
		switch (i) {
		case 0:cout << "You   "; break;
		case 1:cout << "Bot(1)"; break;
		case 2:cout << "Bot(2)"; break;
		case 3:cout << "Bot(3)"; break;
		}
		cout << setw(67) << "|" << endl;

		cout << "|  Red  Blue  Green  Yellow  Gray  Brown  Pink  \"+2\"  Colorful  GottenRow  |" << endl;

		cout << "| ";
		for (int j = 1; j < 10; j++) {
			switch (j) {
			case 1:cout << "   "; break;
			case 2:cout << "     "; break;
			case 3:cout << "      "; break;
			case 4:cout << "       "; break;
			case 5:cout << "     "; break;
			case 6:cout << "      "; break;
			case 7:cout << "     "; break;
			case 8:cout << "     "; break;
			case 9:cout << "         "; break;
			}
			cout << gottenCardCount[j][i];
		}
		if (gottenCards[i]) {
			cout << setw(11) << "Yes";
		}
		else {
			cout << setw(11) << "No";
		}
		cout << "  |" << endl;

		for (int j = 0; j < 38; j++) {
			cout << "¡X";
		}
		cout << endl << endl;
	}

	// show cards on the table //
	for (int i = 0; i < 4; i++) {
		cout << "Row " << i + 1 << ": ";
		for (int j = 0; j < 3; j++) {
			switch (rowStatus[j][i]) {
			case 0:break;
			case 1:cout << "Red  "; break;
			case 2:cout << "Blue  "; break;
			case 3:cout << "Green  "; break;
			case 4:cout << "Yellow  "; break;
			case 5:cout << "Gray  "; break;
			case 6:cout << "Brown  "; break;
			case 7:cout << "Pink  "; break;
			case 8:cout << "\"+2\"  "; break;
			case 9:cout << "Colorful  "; break;
			case -2:break;
			default:cout << "Line 292 error" << endl << endl;
			}
		}
		if (rowStatus[0][i] == -2) {
			cout << "(gotten)";
		}
		cout << endl << endl;
	}
}

void selectByPlayer() {
	bool allowDrawCards = false;
	bool allowGetRow = false;
	while (1) {
		action = 0;
		do {
			// print the situation //
			system("cls");
			printCurrent();

			cout << "Please choose an action (1:Draw a coloretto  2:Get the coloretto on some row): ";
			getline(cin, input);
			if (input.length() != 1) {
				cout << "Input Error!!" << endl << endl;
				system("pause");
				continue;
			}
			else {
				action = input[0] - 48;
			}
			if (action < 1 || action > 2) {
				cout << "Input Error!!" << endl << endl;
				system("pause");
			}
		} while (action < 1 || action > 2);

		// check if the action legal //
		switch (action) {
		case 1:
			for (int i = 0; i < 4; i++) {
				if (rowStatus[0][i] != -2 && rowStatus[2][i] == 0) {
					allowDrawCards = true;
				}
			}
			break;
		case 2:
			for (int i = 0; i < 4; i++) {
				if (rowStatus[0][i] != 0 && rowStatus[0][i] != -2) {
					allowGetRow = true;
				}
			}

		}

		if (allowDrawCards || allowGetRow) {
			break;
		}
		else {
			cout << "Action Illegal!!" << endl << endl;
			system("pause");
			continue;
		}

	}

	int targetRow;

	switch (action) {
	case 1:
		// show the card which was drawn: cardType[cardNum] //
		cout << "The card you have drawn is: ";
		switch (cardType[cardNum]) {
		case -1:
			cout << "End card!" << endl;
			break;
		case 1:
			cout << "Red!" << endl;
			break;
		case 2:
			cout << "Blue!" << endl;
			break;
		case 3:
			cout << "Green!" << endl;
			break;
		case 4:
			cout << "Yellow!" << endl;
			break;
		case 5:
			cout << "Gray!" << endl;
			break;
		case 6:
			cout << "Brown!" << endl;
			break;
		case 7:
			cout << "Pink!" << endl;
			break;
		case 8:
			cout << "\"+2\" card!" << endl;
			break;
		case 9:
			cout << "Colorful" << endl;
			break;
		}

		// get the number of the row to put card into //
		while (1) {
			cout << "Which row do you want to put at: ";
			getline(cin, input);
			if (input.length() != 1) {
				cout << "Input Error!!" << endl << endl;
				continue;
			}
			else {
				targetRow = input[0] - 48;
			}
			if (targetRow > 4 || targetRow < 1) {
				cout << "Input Error!!" << endl << endl;
				continue;
			}
			if (rowStatus[0][targetRow - 1] != -2 && rowStatus[2][targetRow - 1] == 0) {
				putCardIntoRow(targetRow - 1);
				cardNum++;
				break;
			}
			else {
				cout << "Action Error!!" << endl << endl;
				continue;
			}
		}
		system("cls");
		printCurrent();
		Sleep(1500);
		break;
	case 2:
		// get the number of the row to get //
		while (1) {
			cout << "Which row do you want to get: ";
			getline(cin, input);
			if (input.length() != 1) {
				cout << "Input Error!!" << endl << endl;
				continue;
			}
			else {
				targetRow = input[0] - 48;
			}
			if (targetRow <= 0 || targetRow >= 5) {
				cout << "Input Error!!" << endl << endl;
				continue;
			}
			if (rowStatus[0][targetRow - 1] > 0) {
				getTheWholeRow(targetRow - 1);
				lastGotPlayer = player;
				break;
			}
			else {
				cout << "Action Error!!" << endl << endl;
				continue;
			}
		}
		system("cls");
		printCurrent();
		Sleep(1500);
		break;
	}
}

void selectByAI() {
	action = 0;
	srand(time(NULL));

	// decide what to do //
	while (1) {
		bool allowDrawCards = false;
		bool allowGetRow = false;
		action = rand() % 2 + 1;
		switch (action) {
		case 1:
			for (int i = 0; i < 4; i++) {
				if (rowStatus[0][i] != -2 && rowStatus[2][i] == 0) {
					allowDrawCards = true;
				}
			}
			break;
		case 2:
			for (int i = 0; i < 4; i++) {
				if (rowStatus[0][i] != 0 && rowStatus[0][i] != -2) {
					allowGetRow = true;
				}
			}

		}

		if (allowDrawCards || allowGetRow) {
			break;
		}
		else {
			continue;
		}
	}

	// decide the number of the row to put or get //
	int targetRow;
	switch (action) {
	case 1:
		while (1) {
			targetRow = rand() % 4 + 1;
			if (rowStatus[0][targetRow - 1] != -2 && rowStatus[2][targetRow - 1] == 0) {
				putCardIntoRow(targetRow - 1);
				cardNum++;
				break;
			}
			else {
				continue;
			}
		}
		break;
	case 2:
		while (1) {
			targetRow = rand() % 4;
			if (rowStatus[0][targetRow] > 0) {
				getTheWholeRow(targetRow);
				lastGotPlayer = player;
				break;
			}
			else {
				continue;
			}
		}
		break;
	}
	system("cls");
	printCurrent();
	cout << endl;
	if (action == 1) {
		cout << "Bot(" << player << ") draw a ";
		switch (cardType[cardNum - 1]) {
		case -1:
			cout << "END CARD";
			break;
		case 1:
			cout << "RED";
			break;
		case 2:
			cout << "BLUE";
			break;
		case 3:
			cout << "GREEN";
			break;
		case 4:
			cout << "YELLOW";
			break;
		case 5:
			cout << "GRAY";
			break;
		case 6:
			cout << "BROWN";
			break;
		case 7:
			cout << "PINK";
			break;
		case 8:
			cout << "\"+2\" CARD!";
			break;
		case 9:
			cout << "COLORFUL";
			break;
		}
		if (cardNum - 1 == 61) {
			cout << "." << endl << endl;
		}
		else {
			cout << " and put it into ROW " << targetRow << "." << endl << endl;
		}
	}
	else {
		cout << "Bot(" << player << ") gotten the ROW " << targetRow + 1 << "." << endl << endl;
	}
	Sleep(1500);
}

void putCardIntoRow(int target) {
	int Num = 0;
	if (cardType[cardNum] == -1) {
		endCardAppeared = true;
		return;
	}
	while (rowStatus[Num][target] != 0) {
		Num++;
	}
	if (Num > 2) {
		cout << "There is something wrong in function \"putCardIntoRow\":" << endl << "Num = " << Num << endl << endl;
		system("pause");
	}
	rowStatus[Num][target] = cardType[cardNum];
}

void getTheWholeRow(int target) {
	int Num = 2;
	while (rowStatus[Num][target] == 0) {
		Num--;
	}
	for (int i = 0; i <= Num; i++) {
		gottenCardCount[rowStatus[i][target]][player]++;
	}
	for (int i = 0; i < 3; i++) {
		rowStatus[i][target] = -2;
	}
	gottenCards[player] = true;
}

void computeScore() {
	int max[3][4];
	int min[4][4];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			max[j][i] = 0;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			min[j][i] = 10;
		}
	}

	// count each type of cards //
	for (int i = 0; i < 4; i++) {
		for (int k = 1; k < 8; k++) {
			if (gottenCardCount[k][i] >= max[0][i]) {
				max[2][i] = max[1][i];
				max[1][i] = max[0][i];
				max[0][i] = gottenCardCount[k][i];
			}
			else if (gottenCardCount[k][i] >= max[1][i]) {
				max[2][i] = max[1][i];
				max[1][i] = gottenCardCount[k][i];
			}
			else if (gottenCardCount[k][i] >= max[2][i]) {
				max[2][i] = gottenCardCount[k][i];
			}
		}

		for (int k = 1; k < 8; k++) {
			if (gottenCardCount[k][i] <= min[0][i]) {
				min[3][i] = min[2][i];
				min[2][i] = min[1][i];
				min[1][i] = min[0][i];
				min[0][i] = gottenCardCount[k][i];
			}
			else if (gottenCardCount[k][i] <= min[1][i]) {
				min[3][i] = min[2][i];
				min[2][i] = min[1][i];
				min[1][i] = gottenCardCount[k][i];
			}
			else if (gottenCardCount[k][i] <= min[2][i]) {
				min[3][i] = min[2][i];
				min[2][i] = gottenCardCount[k][i];
			}
			else if (gottenCardCount[k][i] < min[3][i]) {
				min[3][i] = gottenCardCount[k][i];
			}
		}

	}

	for (int i = 0; i < 4; i++) {
		if (max[0][i] < 6) {
			max[0][i] += gottenCardCount[9][i];
		}
		else if (max[1][i] < 6) {
			max[1][i] += gottenCardCount[9][i];
		}
		else if (max[2][i] < 6) {
			max[2][i] += gottenCardCount[9][i];
		}
	}

	// compute total score //
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			switch (max[j][i]) {
			case 1:max[j][i] = 1; break;
			case 2:max[j][i] = 3; break;
			case 3:max[j][i] = 6; break;
			case 4:max[j][i] = 10; break;
			case 5:max[j][i] = 15; break;
			case 6:
			case 7:
			case 8:
			case 9:max[j][i] = 21; break;
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			switch (min[j][i]) {
			case 1:min[j][i] = -1; break;
			case 2:min[j][i] = -3; break;
			case 3:min[j][i] = -6; break;
			case 4:min[j][i] = -10; break;
			case 5:min[j][i] = -15; break;
			case 6:
			case 7:
			case 8:
			case 9:min[j][i] = -21; break;
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			score[i] += max[j][i];
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			score[i] += min[j][i];
		}
	}

	for (int i = 0; i < 4; i++) {
		score[i] += gottenCardCount[8][i] * 2;
	}
}