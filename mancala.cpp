#define CPU_WIN 10000 
#define HUMAN_WIN -10000 
#define DRAW -5000 
#define NO_MOVES -100000
#define MAX_DEPTH 4
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>
#include <climits>
#include "mancala.h"

using namespace std;


MancalaBoard::MancalaBoard(int pits, int stones) : pits(pits), stones(stones) {
	if (pits <= 0) cout << "Numero de pozos debe ser > 0" << endl;
	if (stones <= 0) cout << "Numero de piedras debe ser > 0" << endl;
	for (int i=0; i<=(2*pits+1); i++) {
		field.push_back(stones);
	}
	// reset stores to 0
	field[pits] = 0;
	field[2*pits+1] = 0;
}

// Funcion que imprime el tablero
void MancalaBoard::printBoard(void) const {
	//Default board
	//CPU: 12|11|10|9 |8 |7 |
	// -------------------------
	// |  |4 |4 |4 |4 |4 |4 |  |
	// |0 |--+--+--+--+--+--|0 |
	// |  |4 |4 |4 |4 |4 |4 |  |
	// -------------------------
	//HUM: 0 |1 |2 |3 |4 |5 |
	
	//line 1
	cout << "CPU: ";
	for(int i=(2*pits); i>pits; i--) {
		if (i < 10) {
			cout <<i<<" |";
		} else {
			cout <<i<<"|";
		}
	}
	cout<<endl;
	//line 2
	cout << " ----";
	for(int i=pits; i>0; i--) {
		cout << "---";
	}
	cout << "---" <<  endl;
	//line 3
	cout << " |  |";
	for(int i=(2*pits); i>pits; i--) {
		cout << field[i];
		if (field[i]<10) {
			cout << " |";
		} else {
			cout << "|";
		}
	} 
	cout << "  |" << endl;
	//line 4
	cout << " |" << field[(2*pits+1)];
	if (field[2*pits+1] < 10) {
		cout << " |";
	} else {
		cout << "|";
	}
	for(int i=pits; i>1; i--) {
		cout << "--+";
	}
	cout << "--|" << field[pits];
	if (field[pits]<10) {
		cout << " |";
	} else {
		cout << "|";
	}
	cout << endl;
	//line 5
	cout << " |  |";for(int i=0; i<pits; i++) {
		cout << field[i];
		if (field[i]<10) {
			cout << " |";
		} else{
			cout << "|";
		}
	}
	cout << "  |" << endl;
	//line 6
	cout << " ----";
	for(int i=pits; i>0; i--) {
		cout << "---";
	}
	cout << "---" << endl;
	//line 7
	cout << "HUM: ";
	for(int i=0; i<pits; i++) {
		if (i < 10) {
			cout <<i<<" |";
		} else {
			cout <<i<<"|";
		}
	} cout<<endl;
}

//Funcion del ciclo del juego, Gameover o no?
int MancalaBoard::gameOver(void) const {
	//revisando si hay gameove<< endl;
	if (field[pits] > (stones*pits)) return 1; // mas de la mitad del total es que hay un ganador
	if (field[(2*pits+1)] > (stones*pits)) return 2; // mas de la mitad del total es que hay un ganador
	
	int human = 0, cpu = 0, endgame = 0;
	for (int i=0 ; i<pits ; i++) {
		human += field[i];
	}
	if (human == 0) {
		cpu = 0;
		for (int i=pits+1 ; i<(2*pits+2) ; i++) {
			cpu += field[i];
		}
		endgame = 1;
		human = field[pits];
	}

	for (int i=pits+1 ; i<(2*pits+1) ; i++) {
		cpu += field[i];
	}
	if (cpu == 0) {
		human = 0;
		for (int i=0 ; i<pits+1 ; i++) {
			human += field[i];
		}
		endgame = 1;
		cpu = field[(2*pits+1)];
	}

	if ((human==cpu) && (endgame==1)) return 3; // empate
	if ((cpu>human) && (endgame==1)) return 2; // cpu = ganador
	if ((human>cpu) && (endgame==1)) return 1; // usuario = ganador
	return 0; // no winner
}

// retorna el numero de piedras por pozo
int MancalaBoard::getField(int index) const {
	//cout << "getting value of field[" << index << "]" << endl;
	return field[index];
}

// retorna una copia 
MancalaBoard::Field MancalaBoard::getField(void) const {
	//cout << "copying the field..." << endl;
	return field;
}

// set el numero de piedras en un pozo
void MancalaBoard::setField(int index, int value) {
	//cout << "setField field[" << index << "] with value: " << value << endl;
	field[index] = value;
}

// incrementa el valor field(indx) con el valor a sumar
void MancalaBoard::addField(int index, int add) {
	//cout << "addField field[" << index << "] with add value: " << add << endl;
	int temp = getField(index);
	temp += add;
	setField(index, temp);
}

MancalaPlayer::MancalaPlayer(MancalaBoard board, Side side) : side(side), board(board) {}


MancalaPlayer::Lastpit MancalaPlayer::sow(MancalaBoard& board, Side side, int frompit) {
	
	if ((side == HUMAN) && (frompit > board.getNumberOfPits())) {cout << "Movimiento Ilegal" << endl; return TRYAGAIN;}
	if (frompit < 0) { cout << "Moviemiento Ilegal" << endl; return TRYAGAIN; }
	if ((frompit == board.getNumberOfPits()) || (frompit == (2*board.getNumberOfPits()+1))) { cout << "No se puede sembrar aca" << endl; return TRYAGAIN; }
	if (board.getField(frompit) == 0) {cout << "No se puede sembrar en un pozo vacio" << endl; return TRYAGAIN;}

	int sowstones = board.getField(frompit);
	int pit = frompit;
	int previouspit = frompit;
	board.setField(frompit, 0); 
	while (sowstones > 0) {
		
		if(pit != -1) { 
			previouspit = pit;
			pit++;
		} else if (pit == -1) {
			previouspit = (2*board.getNumberOfPits()+1);
			pit = 0;
		} else { 
			cout << "Algo salio Mal" << endl;
		}

		if (side == COMPUTER) {
			if (pit == (2*board.getNumberOfPits()+1)) {
				board.addField(pit, 1); 
				sowstones--;
				pit = -1; 
			} else {
				board.addField(pit, 1); 
				sowstones--; 
			}
		} else if (side == HUMAN) {
			if (pit == (2*board.getNumberOfPits()+1)) {
				
				pit = -1; 
			} else {
				board.addField(pit, 1); 
				sowstones--;
			}
		}
	}
	
	if (pit == -1) {
		pit = (2*board.getNumberOfPits()+1);
	}
	
	
	if ((board.getField(pit) == 1) && !((pit == board.getNumberOfPits()) || (pit == (2*board.getNumberOfPits()+1))) ) {
		// steal!
		steal(board, side, pit);
		return EMPTY;
	}
	
	if ( ((side == HUMAN) && (pit == board.getNumberOfPits())) || ((side == COMPUTER) && (pit == (2*board.getNumberOfPits()+1))) ) {
		return STORE;
	}
	
	return PIT;
};


void MancalaPlayer::steal(MancalaBoard& board, Side side, int lastpit) {
	int stealpit;
	if (side == COMPUTER) {
		//cout << "CPU is stealing" << endl;
		if (lastpit > board.getNumberOfPits()) {
			stealpit = lastpit - board.getNumberOfPits();
			stealpit = board.getNumberOfPits() - stealpit;
			//cout << "Steal from: " << stealpit << " number: " << board.getField(stealpit) << endl;
			board.addField((2*board.getNumberOfPits()+1), board.getField(stealpit));
			board.addField((2*board.getNumberOfPits()+1), 1);
			board.setField(lastpit, 0);
			board.setField(stealpit, 0);
		}
	} else if (side == HUMAN) {
		//cout << "HUMAN is stealing" << endl;
		if (lastpit < board.getNumberOfPits()) {
			stealpit = board.getNumberOfPits() - lastpit;
			stealpit = board.getNumberOfPits() + stealpit;
			//cout << "Steal from: " << stealpit << " number: " << board.getField(stealpit) << endl;
			board.addField(board.getNumberOfPits(), board.getField(stealpit));
			board.addField(board.getNumberOfPits(), 1);
			board.setField(lastpit, 0);
			board.setField(stealpit, 0);
		}
	}
}



Human::Human(MancalaBoard& board) : MancalaPlayer(board, HUMAN), turn(false) {}


int Human::chooseMove(int& bestMove, Side side, MancalaBoard board, Tactic tactic) {
	
	return 0;
}


Computer::Computer(MancalaBoard& board) : MancalaPlayer(board, COMPUTER), turn(false), depth(0), step(0) {}


int Computer::chooseMove(int& bestMove, Side side, MancalaBoard board, Tactic tactic) {
	//cout << "CPU is in chooseMove" << endl;
	depth++;
	int* a = NULL;
	a = new int[board.getNumberOfPits()];
	MancalaBoard boardcopy = board;
	int dummy;

	
	if (tactic == GREEDY) {
		int score = board.getField(2*board.getNumberOfPits()+1); 
		int computerpits = 0; 
		for (int i=board.getNumberOfPits()+1; i<(2*board.getNumberOfPits()+1);i++){ 
			int movevalue = score;
			if (board.getField(i) != 0){ 
				Lastpit reply = sow(boardcopy, side, i);
				movevalue = boardcopy.getField(2*board.getNumberOfPits()+1) - score;
				if (reply == STORE) {
					a[computerpits] = movevalue + chooseMove(dummy, side, boardcopy, GREEDY); 
				} else {
					a[computerpits] = movevalue;
				}
			} else { 
				a[computerpits] = NO_MOVES;
			}
			computerpits++;
			boardcopy = board; 
		}

	
	} else if (tactic == LOOKAHEAD) {
		if (side == HUMAN) {
			int score = board.getField(board.getNumberOfPits()); 
			for (int i=0; i<board.getNumberOfPits(); i++){ 
				int movevalue = score;
				if (board.getField(i) != 0) {
					Lastpit reply = sow(boardcopy, side, i);
					movevalue =  boardcopy.getField(board.getNumberOfPits()) - score;
					if (reply == STORE) {
						depth--; 
						step++;
						a[i] = movevalue + chooseMove(dummy, HUMAN, boardcopy, LOOKAHEAD); 
						step--;
						depth++;
					} else {
						if (depth <= MAX_DEPTH) {
							a[i] = movevalue - chooseMove(dummy, COMPUTER, boardcopy, LOOKAHEAD); 
						} else {
							a[i] = movevalue;
						}
					}
				} else {
					a[i] = NO_MOVES;
				}
				boardcopy = board; 
			}
		} else if (side == COMPUTER) {
			int score = board.getField(2*board.getNumberOfPits()+1);
			int computerpits = 0; 
			for (int i=(board.getNumberOfPits()+1); i<(2*board.getNumberOfPits()+1); i++){ 
				int movevalue = score;
				if (board.getField(i) != 0){
					Lastpit reply = sow(boardcopy, side, i);
					movevalue = boardcopy.getField(2*board.getNumberOfPits()+1) - score;
					if (reply == STORE) {
						depth--; 
						step++; 
							a[computerpits] = movevalue + chooseMove(dummy, COMPUTER, boardcopy, LOOKAHEAD); 
						step--;
						depth++;
					} else {
						if (depth <= MAX_DEPTH) {
							a[computerpits] = movevalue - chooseMove(dummy, HUMAN, boardcopy, LOOKAHEAD); 
						}
						else{
							a[computerpits] = movevalue;
						}
					}
				}
				else{ 
					a[computerpits] = NO_MOVES;
				}
				computerpits++;
				boardcopy = board;
			}
		}	
	}
	
	int max = INT_MIN;
	for (int i=0; i<board.getNumberOfPits(); i++) {
		if (a[i] > max) {
			max = a[i];
			bestMove = i+board.getNumberOfPits()+1;
		}
	}
	
	
	if (max == NO_MOVES) {
		if (side == COMPUTER) {
			if (boardcopy.getField(2*board.getNumberOfPits()+1) > (board.getNumberOfPits() * board.getNumberOfStones()))
				max = CPU_WIN;
			else if (boardcopy.getField(2*board.getNumberOfPits()+1) == (board.getNumberOfPits() * board.getNumberOfStones()))
				max = DRAW;
			else
				max = HUMAN_WIN;
		} else if (side == HUMAN) {
			if (boardcopy.getField(board.getNumberOfPits()) > (board.getNumberOfPits() * board.getNumberOfStones()))
				max = -HUMAN_WIN;
			else if (boardcopy.getField(board.getNumberOfPits()) == (board.getNumberOfPits() * board.getNumberOfStones()))
				max = -DRAW;
			else
				max = -CPU_WIN;
		}
	}
		
	depth--;
	delete [] a;
	return max;

}
