#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include <ctime>

using namespace std;

#define BOARDSIZE 4
#define NEXTCOLUMN 1
#define NEXTROW 4


void printboard(vector<int>& board);
void defaultt(vector <int>& defaultt);
void shift (vector <int>& board, string direction);
void addnum(vector <int>& board);
bool possiblemoves(vector <int> board);
void shiftnums(vector<int>& board, int startlocation, int nexttile, int nextset, int rowcol);
void mergenums(vector<int>& board, int startlocation, int nexttile);
int definestart(string direction, int rowcol);
void nextilenextset(string direction, int& nexttile, int& nextset); 

int main() {

	vector <int> board;

	//-------------------------SETUP------------------------//

	//GET FILENAME
	string filename;
	cout << "enter initial configuration file name: " << endl;
	cin >> filename;

	//OPEN FILE
	ifstream infile;
	infile.open(filename.c_str());

	//CHECK FILE COULD BE OPENED
	if(!infile.is_open()) {
		cout << "file not found, using default start configuration" << endl;
		defaultt(board);
	}

	else if (infile.is_open()) {
		int tmpp;

		//READ FILE, STORE IN VECTOR
		while (infile >> tmpp){
		board.push_back(tmpp);
	}

	}

	//CLOSE FILE
	infile.close();

	//PRINT BOARD
	printboard(board);

	//-------------------------SETUP------------------------//

	//-------------------------GAMEPLAY------------------------//
	
	vector <int> boardcopy;
	string direction;
	
	while(possiblemoves(board)){
		cin >> direction;
		boardcopy = board;
		shift(board, direction);
		//IF SHIFT HAS CAUSED A CHANGE IN THE BOARD, ADD A 2	
		if (boardcopy != board){
			addnum(board);
			printboard(board);
		}
	}

	cout << "game over" << endl;

	//-------------------------GAMEPLAY------------------------//

	return 0;

}

//PRINTBOARD
void printboard(vector<int>& board){

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 4; j++){
			cout << board[i] << '\t';
			i++;
		}
	cout << endl;
	i--;
	}
	return;
}

//CREATE DEFAULT GAMEBOARD VECTOR
void defaultt(vector <int>& defaultt){

	for (int i = 0; i < 15; i++) {
		defaultt.push_back(0);
	}
	defaultt.push_back(2);
	return;
}

//PERFORMS THE SHIFTING OF NUMBERS IN THE SPECIFIED DIRECTION
void shift(vector <int>& board, string direction){

	int nexttile = 0; //Denotes the shift in index to reach the adjacent tile in 
	int nextset = 0;  //Denotes the shift in index to move to the next set i.e. next column if we are moving in the direction of rows and vice versa.
	int startlocation; //Holds the tile we start from when traversing rows/columns
	
	nextilenextset(direction, nexttile, nextset);
	
	//Perform the shift once for every row or column (depending on the direction we are moving in). 
	//Rowcol holds the row number if moving left/right. Column number if moving up/down.
	for (int rowcol = 0; rowcol < BOARDSIZE; rowcol++){
		startlocation = definestart(direction, rowcol);
		shiftnums(board, startlocation, nexttile, nextset, rowcol);
		mergenums(board, startlocation, nexttile);
		shiftnums(board, startlocation, nexttile, nextset, rowcol);
	}
	return;
}

//ADDS A 2 TO A RANDOM FREE SPACE
void addnum(vector <int>& board){

	int tile  = 0;
	while (board[tile] != 0) { 
		tile = rand() % 16; // random tile selected
		}
	board[tile] = 2;
	return;
}

//TRUE IF THERE ARE ANY POSSIBLE MOVES. FALSE IF GAME OVER.
bool possiblemoves(vector <int> board){
	
	vector <int> boardcopy;
	boardcopy = board;
	
	shift(boardcopy, "w");
	if(boardcopy != board)
		return true;
	
	boardcopy = board;
	shift(boardcopy, "s");
	if(boardcopy != board)
		return true;
		
	boardcopy = board;
	shift(boardcopy, "d");
	if(boardcopy != board)
		return true;
		
	boardcopy = board;
	shift(boardcopy, "a");
	if(boardcopy != board)
		return true;
	
	return false;
	
}

//PERFORMS THE SHIFTING OF THE NUMBERS
void shiftnums(vector<int>& board, int startlocation, int nexttile, int nextset, int rowcol){
		int tile;
		int tileahead;
		//SHIFTS ALL NUMBERS AS FAR IN THE "DIRECTION" AS POSSIBLE
		for (int i = 1; i < 4; i++){
			tile = startlocation - i*nexttile;
			tileahead = tile + nexttile;
			while(board[tileahead] == 0 && tileahead >= abs(nextset)*rowcol && tileahead <= (abs(nextset)*rowcol + 3*abs(nexttile)) && board[tile] != 0){
				board[tileahead] = board[tile];
				board[tile] = 0;
				tile += nexttile;
				tileahead += nexttile;
			}
		}
	return;
}

//MERGES TWO NUMBERS IF ADJACENT IN DIRECTION TILES ARE SHIFTING
void mergenums(vector<int>& board, int startlocation, int nexttile){
	int tile;
	int tileahead;
	for (int i = 1; i < 4; i++){
			tile = startlocation - i*nexttile;
			tileahead = tile + nexttile;
			if (board[tileahead] == board[tile]){
				board[tileahead] = 2*board[tile];
				board[tile] = 0;	
			}
		}
	return;
}

//DETERMINES WHICH TILE SHIFT FUNCTION SHOULD START FROM DEPENDING ON SHIFT DIRECTION
int definestart(string direction, int rowcol){
	int startlocation;
	if (direction == "w")
			startlocation = rowcol*NEXTCOLUMN; //move up, start at the top
		else if (direction == "s")
			startlocation = rowcol*NEXTCOLUMN + 3*NEXTROW; //move down: start at the bottom
		else if (direction == "a")
			startlocation = rowcol*NEXTROW; //move left: Start at the very left
		else if (direction == "d")
			startlocation = rowcol*NEXTROW + 3*NEXTCOLUMN; //move right: start at very right
	return startlocation;
}

//DEFINES THE DIRECTION THE NEXT TILE AND NEXT SET ARE WITH RESPECT TO THE TILE. DEPENDS ON DIRECTION OF ATTEMPTED SHIFT.
void nextilenextset(string direction, int& nexttile, int& nextset) {

	if (direction == "w"){
		nexttile = -NEXTROW;
		nextset = NEXTCOLUMN;
	}
	else if (direction == "s"){
		nexttile = NEXTROW;
		nextset = NEXTCOLUMN;
	}
	else if (direction == "d"){
		nexttile = NEXTCOLUMN;
		nextset = NEXTROW;
	}

	else if (direction == "a"){
		nexttile = -NEXTCOLUMN;
		nextset = NEXTROW;
	}
	
	return;
}

