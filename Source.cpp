//T01_GXX

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

double const PI = 3.1415926;

class Person {
public:
	int Hi;
	int Hj;
	char Hdead;

	Person(int i=0, int j=0, char dead = 'H') {
		Hi = i;
		Hj = j;
		Hdead = dead;
	}
};

void resetScreen() {
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

void changePlace(Person& hero, char mov) {
	if (mov == 'q') {
		hero.Hi--;
		hero.Hj--;
	}
	else if (mov == 'w')
		hero.Hi--;
	else if (mov == 'e') {
		hero.Hi--;
		hero.Hj++;
	}
	else if (mov == 'a')
		hero.Hj--;
	else if (mov == 'd')
		hero.Hj++;
	else if (mov == 'z') {
		hero.Hi++;
		hero.Hj--;
	}
	else if (mov == 'x')
		hero.Hi++;
	else if (mov == 'c') {
		hero.Hi++;
		hero.Hj++;
	}
}

void checkDeathHero(Person& hero, vector< vector <char> > maze) {
	if (maze[hero.Hi][hero.Hj] == '*' || tolower(maze[hero.Hi][hero.Hj]) == 'r') {
		hero.Hdead = 'h';
	}
}

void moveRobots(Person hero, vector< vector <char> > &maze, vector< vector <int> > &robotList) {
	int i = 0;
	while (i < robotList.size()) {
		//Finding the shortest way to the hero
		float y=hero.Hi - robotList[i][0], x=hero.Hj - robotList[i][1], angle;
		angle = 180 * acos(x / sqrt((x * x) + (y * y)))/PI;
		if (y < 0)
			angle = 360 - angle;
		maze[robotList[i][0]][robotList[i][1]] = ' ';
		//Moving the robots
		if (angle >= 337.5 || angle < 22.5) {
			//Angle closest to 0º
			robotList[i][1] ++;
		}
		else if (angle >= 22.5 && angle < 67.5) {
			//Angle closest to 45º
			robotList[i][1] ++;
			robotList[i][0] ++;
		}
		else if (angle >= 67.5 && angle < 112.5) {
			//Angle closest to 90º
			robotList[i][0] ++;
		}
		else if (angle >= 112.5 && angle < 157.5) {
			//Angle closest to 135º
			robotList[i][1]--;
			robotList[i][0] ++;
		}
		else if (angle >= 157.5 && angle < 202.5) {
			//Angle closest to 180º
			robotList[i][1]--;

		}
		else if (angle >= 202.5 && angle < 247.5) {
			//Angle closest to 225º
			robotList[i][1]--;
			robotList[i][0]--;

		}
		else if (angle >= 247.5 && angle < 292.5) {
			//Angle closest to 270º
			robotList[i][0]--;
		}
		else {
			//Angle closest to 315º
			robotList[i][0]--;
			robotList[i][1]++;
		}
		//Updating the robot's location in the maze and checking if it's dead
		if (maze[robotList[i][0]][robotList[i][1]] == '*' || maze[robotList[i][0]][robotList[i][1]] == 'r') {
			maze[robotList[i][0]][robotList[i][1]] = 'r';
			for (int index = i; index < robotList.size() - 1; index++) {
				robotList[index] = robotList[index + 1];
			}
			robotList.resize(robotList.size() - 1);
		}
		//If the robot hits another living robot, removing both from the array \"robotList\" and setting their location to 'r' in the matrix
		else if (maze[robotList[i][0]][robotList[i][1]] == 'R') {
			//Setting the place of collision in the maze as a dead robot
			maze[robotList[i][0]][robotList[i][1]] = 'r';
			//col_loc = coordinates of the collision
			vector <int> col_loc = robotList[i];
			int col_index=0;
			//Removing the current robot from the array
			for (int index = i; index < robotList.size() - 1; index++) {
				robotList[index] = robotList[index + 1];
			}
			robotList.resize(robotList.size() - 1);
			//Finding the other robot at the same coordinates
			for (int index = 0; index < robotList.size() - 1; index++) {
				if (robotList[index] == col_loc) {
					col_index = index;
					break;
				}
			}
			//Removing the other robot from the array
			for (int index = col_index; index < robotList.size() - 1; index++) {
				robotList[index] = robotList[index + 1];
			}
			robotList.resize(robotList.size() - 1);
			//Getting the next robot
			if (col_index < i)
				i--;
		}
		else {
			maze[robotList[i][0]][robotList[i][1]] = 'R';
			i++;
		}
	}
}

bool play(vector <vector<char>> &maze, Person &hero, vector <vector <int>> &robotList) {
	while (true) {
		resetScreen();
		char movement;
		//Show current board
		for (int i = 0; i < maze.size(); i++) {
			for (int j = 0; j < maze[0].size(); j++) {
				if (i == hero.Hi && j == hero.Hj)
					cout << hero.Hdead;
				else
					cout << maze[i][j];
			}
			cout << '\n';
		}
		if (hero.Hdead == 'h' || robotList.size() == 0)
			break;
		//Get player movement
		while (true) {
			do {
				cout << "Input valid movement(q/w/e/a/s/d/z/x/c): \n";
				cin >> movement;
				if (!cin.eof())
					movement = tolower(movement);
				else {
					return 0;
				}
			} while (movement != 'q' && movement != 'w' && movement != 'e' && movement != 'a' && movement != 's' && movement != 'd' && movement != 'z' && movement != 'x' && movement != 'c');
			//Changing the internal placement of the hero
			int OGHeroi = hero.Hi, OGHeroj = hero.Hj;
			changePlace(hero, movement);
			//Checking if the movement was valid
			if (tolower(maze[hero.Hi][hero.Hj]) == 'r') {
				cout << "Invalid movement(Cannot move to place occupied by robot)";
				hero.Hi = OGHeroi;
				hero.Hj = OGHeroj;
			}
			else
				break;
		}
		//Checking if the hero has stepped unto a fence
		if (maze[hero.Hi][hero.Hj] == '*')
			hero.Hdead = 'h';
		//Moving the robots
		moveRobots(hero, maze, robotList);
		//Checking if the hero has been caught by a robot
		checkDeathHero(hero, maze);
	}
	if (hero.Hdead == 'h') {
		cout << "GameOver, you LOST: ";
		if (maze[hero.Hi][hero.Hj] == '*')
			cout << "You have been electrecuted by the fence!\n";
		else
			cout << "You have been caught by a robot!\n";
	}
	else
		cout << "GameOver, you WON!\n";
	return 1;
}

bool playSelect() {
	//Selecting the maze file
	vector < vector < int > > robotList(0);
	Person hero;
	ifstream maze_file;
	string number;
	int n_lines, n_cols;
	resetScreen();
	cout << "Which maze would you like to use:\n";
	cin >> number;
	if (cin.eof())
		return 0;
	maze_file.open("MAZE_" + number + ".txt");
	resetScreen();
	//Extracting the contents of the maze file
	if (maze_file.is_open()) {
		//Extracting the dimentions of the maze
		maze_file >> n_lines;
		maze_file.ignore(3);
		maze_file >> n_cols;
		maze_file.ignore(1);
		vector <vector <char>> maze(n_lines, vector<char>(n_cols));
		int nRobots = 0;
		//Extracting the actual maze to the vector \"maze\"
		for (int i = 0; i < maze.size(); i++) {
			string line;
			getline(maze_file, line);
			for (int j = 0; j < line.size(); j++) {
				//Extracting the location of the hero to the variable \"hero\"
				if (line[j] == 'H') {
					hero.Hi = i;
					hero.Hj = j;
					maze[i][j] = ' ';
				}
				else if (line[j] == 'R') {
					//Extracting the locations of the Robots to the vector robotList, in this way, the robots are arranged sequentially
					nRobots++;
					robotList.push_back(vector<int>(1));
					robotList[nRobots - 1].resize(2);
					robotList[nRobots - 1][0] = i;
					robotList[nRobots - 1][1] = j;
					maze[i][j] = line[j];
				}
				else
					maze[i][j] = line[j];
			}
		}
		return (play(maze, hero, robotList));
	}
	else {
		cout << "Unable to open file\n\n";
		return 1;
	}
}

bool rules() {
	int selection;
	resetScreen();
	cout << "Rules:\n\n+Type \"2\" on the main menu screen to play, then type a two digit number from \"01\" to \"99\" to select a maze, which will then be shown to you.\n\n+Your location on the maze is represented by the symbol \"H\", if you die, that symbol becomes \"h\"\n\n+The maze is composed of high-voltage fences and posts represented by the symbol \"*\", be careful not to touch them or you'll lose the game immediately\n\n+Each \"R\" on the maze represents an evil robot. The robots are blind, unable to see the fences or posts. However they can detect you by your smell and will try to catch you always running after you in the direction of your scent. If a robot hits an electric fence or post it is destroyed, becoming unable to move, if a robot hits a dead robot it is also destroyed, if two robots hit each other, they are both destroyed. A dead robot is represented by the symbol \"r\"\n\n+Your objective is to make it so that all robots are destroyed, either by running into the electric fences/posts or into one another. You can do this by manipulating their inability to see the maze or each other as well as their determination to always follow your scent to get them to be destroyed. If all the robots are destroyed, you win, if you either hit a fence/post or a robot catches you, you lose\n\n+At the start of each round you will be shown the maze as it currently is and will have the choice to move in one of eight directions...\n\nNW  N  NE\nW       E\nSW  S  SE\n\n...by pressing the following keys respectively:\n\nq   w   e\na       d\nz   x   c\n\nyou may also stay in your place by pressing \"s\"\n\n+After you move, each of the robots will move by id number, always in the direction of your scent\n\nGood game and good luck!\n\n0)Back to Main Menu\n";
	do {
		cin >> selection;
		if (cin.eof())
			return 0;
	}
	while (selection != 0);
	resetScreen();
	return 1;
}

void mainMenu() {
	bool game_running = 1;
	int selection;
	while (game_running) {
		cout << "\n1)Rules\n2)Play\n0)Exit\n";
		cin >> selection;
		if (cin.eof())
			break;
		if (selection == 1) {
			game_running = rules();
		}
		else if (selection == 2) {
			game_running = playSelect();
		}
		else if (selection == 0)
			break;
	}
}

int main() {
	mainMenu();
	return 0;
}