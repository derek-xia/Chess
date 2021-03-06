#include "Piece.h"
#include "King.h"
#include "Pawn.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"

#include <iostream>
#include <iomanip>
#include <cctype>
#include <cmath>

using namespace std;

const int CHESS_SIZE = 8;
int turn = 0;

const string black_piece = "prnbkq";
const string white_piece = "PRNBKQ";
string player;

//Game board variable
Piece* x[8][8];
char y[8][8];
string fake_board[8][8];

void play();
void move(string player);
void location(int &xCur, int &yCur, int &xTar, int &yTar);
void valid_location(int &xCur, int &yCur);
bool valid_dest(int xCur, int yCur, int &xTar, int &yTar);
void starting_board();
bool horizontal();
bool vertical();
bool diagonal();
bool kingcheck(int player);
void findking(int player, int& kingrow, int& kingcol);
bool checkmate();
void printing_board();
void checkValid();
bool continue_playing();
void copyboard();
void move_copy();

void copyboard(){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(x[i][j] != NULL){
				y[i][j] = x[i][j]->get_piece();
			}else{
				y[i][j] = ' ';
			}
		}
	}
}
void move_copy(int xCur, int yCur, int xTar, int yTar){
	copyboard();
	y[xTar][yTar] = y[xCur][yCur];
	y[xCur][yCur] = ' ';
}

bool checkmate()
{
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if(x[i][j] != NULL && 
				(isupper(x[i][j]->get_piece()) && turn%2 == 0 ||
				islower(x[i][j]->get_piece()) && turn%2 == 1))
			{
				for(int r = 0; r < 8; r++)
				{
					for(int c = 0; c < 8; c++)
					{
						if(valid_dest(i, j, r, c))
						{
							cout << "valid" << endl;
							move_copy(i, j, r, c);
							if(!kingcheck(turn+1))
							{
								cout << x[i][j]->get_piece() << "from" << i << " " << j << endl << "to" << r << " " << c <<endl;
								return false;
							}
						}							
					}
				}
			}			
		}
	}
	return true;
}

void printing_board(){
	cout << "  +------------------------+" << endl;
	for(int row = 0; row < 8; row++){
		cout << 8-row << " |";
		for(int col = 0; col < 8; col++){
			if(x[row][col] != NULL){
				char piece = x[row][col]->get_piece();
				if(piece >= 65 && piece <= 90){
					//Check if piece is white's piece
					cout << "<" << piece << ">";
				}else if(piece >= 97 && piece <= 122){
					//Black's piece
					cout << "[" << piece << "]";
				}
			}else{
				cout << fake_board[row][col];
			}
			
		}
		cout << "|" << endl;
	}
	
	cout << "  +------------------------+" << endl;
	cout << "    a  b  c  d  e  f  g  h" << endl;
}


void play(){
	//DO EVERYTHING
	cout << "Welcome! Before you start your game please note" << endl;
	cout << "That the common notation is letter then number" << endl;
	cout << "Such as b6 all other input would be invalid thank you." << endl;
	
	bool bcheckmate = false;
	while(!bcheckmate){
		if(turn%2 == 0)
			player = "White";
		else
			player = "Black";
		cout << player << "'s move:" << endl;
		printing_board();
		
		move(player);
		
		if(kingcheck(turn)){
			cout << player << " checked the king" << endl;
		}
		turn++;
		if(kingcheck(turn+1) && checkmate()){
			printing_board();
			cout << player << " has checkmated the king !!!" << endl;
			bcheckmate = true;
		}
		
	}
}
void valid_location(int &x, int &y){
	string temp;
	
	//Ensures location on array
	while(x < 0 || x > 7 || y < 0 || y > 7){
		cout << "Invalid location. Please reenter another." << endl;
		cin >> temp;
		y = temp[0]-97;
		x = (temp[1]-56)*-1;
	}
}

bool valid_dest(int xCur, int yCur, int &xTar, int &yTar)
{
	string temp;
	Piece* piece = x[xCur][yCur];

	if((xTar == xCur && yTar == yCur) || !piece->checkValid(xTar, yTar, x)
		|| x[xTar][yTar] != NULL && 
		((islower(x[xTar][yTar]->get_piece()) && islower(piece->get_piece())) 
		|| (isupper(x[xTar][yTar]->get_piece()) && isupper(piece->get_piece()))))
	{
		return false;
	}
	else
	{
		return true;
	}
}

void location(int &xCur, int &yCur, int &xTar, int &yTar){
	//Select the piece to move
	string start;
	cout << player << ": select a piece" << endl;
	cin >> start;
	yCur = start[0]-97;
	xCur = (start[1]-56)*-1;
	
	valid_location(xCur, yCur);
	//Check if it is turn's player piece
	if(turn%2 == 0){
		//White players
		while(x[xCur][yCur] == NULL || x[xCur][yCur]->get_piece()  < 65 || x[xCur][yCur]->get_piece() > 90){
			cout << "Not your piece. Please enter another location" << endl;
			cin >> start;
			yCur = start[0]-97;
			xCur = (start[1]-56)*-1;
			valid_location(xCur, yCur);
		}
	}else{
		while(x[xCur][yCur] == NULL || x[xCur][yCur]->get_piece()  < 97 || x[xCur][yCur]->get_piece() > 122){
			cout << "Not your piece. Please enter another location" << endl;
			cin >> start;
			yCur = start[0]-97;
			xCur = (start[1]-56)*-1;
			valid_location(xCur, yCur);
		}
	}
	
	//Select the destination
	string end;
	cout << player << ": choose a destination" << endl;
	cin >> end;
	
	//Changing location to array notation (x=row, y=col)
	yTar = end[0]-97;
	xTar = (end[1]-56)*-1;
	
	valid_location(xTar, yTar);
	
}


void move(string player){
	int xCur = -1, yCur = -1, xTar = -1, yTar = -1;
	bool move_valid = false;
	
	location(xCur, yCur, xTar, yTar);
	
	if(!valid_dest(xCur, yCur, xTar, yTar)){
		cout << "Not A Valid Move" << endl;
		move(player);
		return;
	}
	move_copy(xCur, yCur, xTar, yTar);
	if(kingcheck(turn+1)){
		cout << "Your King is in Check You Bonobo" << endl;
		move(player);
		return;
	}
	
	//moved
	x[xTar][yTar] = x[xCur][yCur];
	x[xCur][yCur] = NULL;

	x[xTar][yTar]->set_moved();
	
	//move_valid = checkMoveValid(xCur, yCur, xTar, yTar);
	
}


/**
	Creates the starting chess board
	@param 8x8 array for entire board
	@return void (NULL)
*/
void starting_board()
{
	//Black is lower case, White is upper
	x[0][0] = new Rook(0,0,'r'); 
	x[0][1] = new Knight(0,1,'n');
	x[0][2] = new Bishop(0,2,'b');
	x[0][3] = new Queen(0,3,'q');
	x[0][4] = new King(0,4,'k');
	x[0][5] = new Bishop(0,5,'b');
	x[0][6] = new Knight(0,6,'n');
	x[0][7] = new Rook(0,7,'r');
	x[1][0] = new Pawn(1,0,'p');
	x[1][1] = new Pawn(1,1,'p');
	x[1][2] = new Pawn(1,2,'p');
	x[1][3] = new Pawn(1,3,'p');
	x[1][4] = new Pawn(1,4,'p');
	x[1][5] = new Pawn(1,5,'p');
	x[1][6] = new Pawn(1,6,'p');
	x[1][7] = new Pawn(1,7,'p');
	x[6][0] = new Pawn(6,0,'P');
	x[6][1] = new Pawn(6,1,'P');
	x[6][2] = new Pawn(6,2,'P');
	x[6][3] = new Pawn(6,3,'P');
	x[6][4] = new Pawn(6,4,'P');
	x[6][5] = new Pawn(6,5,'P');
	x[6][6] = new Pawn(6,6,'P');
	x[6][7] = new Pawn(6,7,'P');
	x[7][0] = new Rook(7,0,'R');
	x[7][1] = new Knight(7,1,'N');
	x[7][2] = new Bishop(7,2,'B');
	x[7][3] = new Queen(7,3,'Q');
	x[7][4] = new King(7,4,'K');
	x[7][5] = new Bishop(7,5,'B');
	x[7][6] = new Knight(7,6,'N');
	x[7][7] = new Rook(7,7,'R');

	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(i%2 == 0 && j%2 == 0){
				fake_board[i][j] = ":::";
			}else if(i%2 == 1 && j%2 == 1){
				fake_board[i][j] = ":::";
			}else{
				fake_board[i][j] = "   ";
			}
		}
	}
}

/**
	Asks player if wants to play another game_board
	@param 
	@return true or false
*/
bool continue_playing(){
	//Ask if the player wants to play another match
	string input;
	cout << "Do you want to play another game? (Y/N)" << endl;
	cin >> input;
	
	if(input == "Y" || input == "y" || input == "yes"){
		turn = 0;
		return true;
	}else{
		return false;
	}
}

bool horizontal(int row, int col, int kingrow, int kingcol){
    bool clear = true;
	//cout << "Passed" << endl;
    int start_x = row;
    int end_x = kingrow;
	//cout << "xpos " << start_x << " " << end_x << endl;
    if(start_x > end_x){
        start_x = kingrow;
        end_x = row;
    }
    if(col != kingcol){
        return false;
    }
    else {
        for(int i = start_x+1; i < end_x; i++){
            if(y[i][col] != ' '){
                clear = false;
            }
        }
    }
    return clear;
}

bool vertical(int row, int col, int kingrow, int kingcol){
    bool clear = true;
	//cout << "Passed" << endl;
    int start_y = col;
    int end_y = kingcol;
	//cout << "xpos " << start_y << " " << end_y << endl;
    if(start_y > end_y){
        start_y = kingcol;
        end_y = col;
    }
    if(row != kingrow){
        return false;
    }
    else {
        for(int i = start_y+1; i < end_y; i++){
            if(y[row][i] != ' '){
                clear = false;
            }
        }
    }
    return clear;
    
}

bool diagonal(int row, int col, int kingrow, int kingcol){
    bool clear = true;
    int x_d = 1;
    int y_d = 1;
    if(row - kingrow < 0){
        x_d = -1;
    }
    if(col - kingcol < 0){
        y_d = -1;
    }
    if((col - kingcol) == 0 || abs(row - kingrow)/abs(col - kingcol) != 1){
        return false;
    }else{
        for(int i = 1; i < abs(row - kingrow); i++){
            if(y[kingrow+(i*x_d)][kingcol+(i*y_d)] != ' '){
                return false;
            }
        }
    }
    return clear;
}

bool kingcheck(int player){
    bool check = false;
	//White's turn and black's king
    char king = 'k';
    char rook = 'R';
    char knight = 'N';
    char bishop = 'B';
    char queen = 'Q';
    char pawn = 'P';
    
	//Change if black's turn, king is white's
    if (player % 2 == 1){
        king = 'K';
        rook = 'r';
        knight = 'n';
        bishop = 'b';
        queen = 'q';
        pawn = 'p';
    }

	//Locates all the pieces on artifical board
	int kingrow, kingcol;
	findking(player+1, kingrow, kingcol);
    
	cout << "King is at " << kingrow << " " << kingcol << endl;
	
    // rook
    for (int row = 0; row < 8; row ++){
        for (int col = 0; col < 8; col ++){
            if (y[row][col] == rook){
				//cout << "is jason a bonobo" << endl;
				//cout << row << " " << col << endl<< endl;
                if (horizontal(row, col, kingrow, kingcol) || vertical(row, col, kingrow, kingcol)){
					//cout << "jason is not a bonobo";
                    return true;
                }
            }
        }
    }
    
    // knight
    if (kingrow < 7 && kingcol < 6){
        if (y[kingrow+1][kingcol+2] == knight){
            return true;
        }
    }

    if (kingrow < 7 && kingcol > 1){
        if (y[kingrow+1][kingcol-2] == knight){
            return true;
        }
    }
    
    if (kingrow > 0 && kingcol < 6){
        if (y[kingrow-1][kingcol+2] == knight){
            return true;
        }
    }
    
    if (kingrow > 0 && kingcol > 1) {
        if (y[kingrow-1][kingcol-2] == knight){
            return true;
        }
    }
    
    if (kingrow < 6 && kingcol < 7){
        if (y[kingrow+2][kingcol+1] == knight){
            return true;
        }
    }

    if (kingrow < 6 && kingcol > 0){
        if (y[kingrow+2][kingcol-1] == knight){
            return true;
        }
    }

    if (kingrow > 1 && kingcol <7){
        if (y[kingrow-2][kingcol+1] == knight){
            return true;
        }
    }

    if (kingrow > 1 && kingcol > 0){
        if (y[kingrow-2][kingcol-1] == knight){
            return true;
        }
    }

    // bishop
    for (int row = 0; row < 8; row ++){
        for (int col = 0; col < 8; col ++){
            if (y[row][col] == bishop){
                if (diagonal(row, col, kingrow, kingcol)){
                    return true;
                }
            }
        }
    }
    
    // pawn
    for (int row = 0; row < 8; row ++){
        for (int col = 0; col < 8; col ++){
            if (y[row][col] == pawn && abs(row-kingrow) == 1){
                if (diagonal(row, col, kingrow, kingcol)){
                    return true;
                }
            }
        }
    }
    
    // queen
    for (int row = 0; row < 8; row ++){
        for (int col = 0; col < 8; col ++){
            if (y[row][col] == queen){
                if (diagonal(row, col, kingrow, kingcol) || horizontal(row, col, kingrow, kingcol) || vertical(row, col, kingrow, kingcol)){
                    return true;
                }
            }
        }
    }
    
    return check;
}

void findking(int player, int& row, int& col)
{
	char king = 'K';
	if(player % 2 == 1)
	{
		king = 'k';
	}
	//cout << "bye";
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(y[i][j] != ' ' && y[i][j] == king){
				//cout << "hi";
				row = i;
				col = j;
				return;
			}
		}
	}
}

int main(){
    bool cont = false;
    do{
        //Fake board for printing purposes
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                if(i%2 == 0 && j%2 == 0){
                    fake_board[i][j] = ":::";
                }else if(i%2 == 1 && j%2 == 1){
                    fake_board[i][j] = ":::";
                }else{
                    fake_board[i][j] = "   ";
                }
            }
        }
        
        //Create the starting game board
        starting_board();
        
        //Playing Chess game
        play();
        
        //Remove memory of the game board
        //delete x;
        
        //Ask user if they want to play again
        cont = continue_playing();
    }while(cont);
    
    cout << "Thanks for playing!";
    return 0;
}
