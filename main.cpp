#define DEFAULT_PITS 6
#define DEFAULT_STONES 4

#include <iostream>
#include "mancala.h"

using namespace std;

int main() {
	cout << "################MANCALA################" << endl << endl;
	int pits, stones;
	do {
		cout << "Cuantos pozos p? 1<p<8 (for default type 0)" << endl;
		cin >> pits;
		if (pits == 0) {
			pits = DEFAULT_PITS;
		}
	} while (!((pits > 1) && (pits < 8)));
	do {
		cout << "Cuantas semillas s? 1<s<7 (for default type 0)" << endl;
		cin >> stones;
		if (stones == 0) {
			stones = DEFAULT_STONES;
		}
	} while (!((stones > 1) && (stones < 7)));

	
	MancalaBoard mancala(pits, stones);
	
	Computer cpuplayer(mancala);
	Human humanplayer(mancala);

	int begin;
	do {
		cout << "Quien inicia? 1=USUARIO 2=CPU" << endl;
		cin >> begin;
		if (begin == 1) {
			humanplayer.setTurn(true);
		} else if (begin == 2) {
			cpuplayer.setTurn(true);
		} else {
			cout << "Error, elija 1 o 2 y presione ENTER" << endl;
		}
	} while (!(begin == 1 || begin == 2));

	
	mancala.printBoard();

	do {
		MancalaPlayer::Lastpit reply;
		if (humanplayer.getTurn() == true) {
			int frompit;
			do {
				cout << "Usuario: " << flush;
				cin >> frompit;

				reply = humanplayer.sow(mancala, MancalaPlayer::HUMAN, frompit);
			} while(reply == MancalaPlayer::TRYAGAIN);

			if (reply == MancalaPlayer::STORE) { 
				cout << "Usuario recibe otro turno" << endl;
			} else {
				humanplayer.setTurn(false);
				cpuplayer.setTurn(true);
			}

		} else if (cpuplayer.getTurn() == true) {
			int frompit;

			
			cpuplayer.chooseMove(frompit, MancalaPlayer::COMPUTER, mancala, MancalaPlayer::LOOKAHEAD);

			cout << "CPU " << frompit << endl;

			reply = cpuplayer.sow(mancala, MancalaPlayer::COMPUTER, frompit);
			if (reply == MancalaPlayer::STORE) { 
				cout << "CPU tiene otro turno!" << endl;
			} else {
				humanplayer.setTurn(true);
				cpuplayer.setTurn(false);
			}
		}
		
		mancala.printBoard();
	} while (mancala.gameOver() == 0);

	cout << (mancala.gameOver() == 1 ? "El ganador es: USUARIO" : mancala.gameOver() == 2 ? "El ganador es: CPU" : "EMPATE!") << endl;

	cin.get();
	cin.get();
	return 0;
}
