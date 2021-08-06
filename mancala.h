#include <vector>

using namespace std;

class MancalaBoard {
public:
	typedef vector<int> Field;

	
	MancalaBoard(int pits, int stones);

	
	void printBoard(void) const;
	int gameOver(void) const;
	int getNumberOfPits(void) const { return pits; };
	int getNumberOfStones(void) const { return stones; };
	int getField(int index) const;
	Field getField(void) const;
	
	void setField(int index, int value);
	void addField(int index, int add);

private:
	Field field; // vector<int>
	int pits; // numer de semillas por usuario
	int stones; // piedras por cuadro
};

class MancalaPlayer {
public:
	enum Side { HUMAN, COMPUTER };
	enum Lastpit { TRYAGAIN, STORE, EMPTY, PIT };
	enum Tactic { GREEDY, LOOKAHEAD };

	
	MancalaPlayer(MancalaBoard board, Side side);

	// get status
	virtual bool getTurn() const = 0;
	virtual void setTurn(bool turn) = 0;
	// elegir moviemiento es distinto para el usuario y CPU
	virtual int chooseMove(int& bestMove, Side side, MancalaBoard board, Tactic tactic) = 0;
	
	
	Lastpit sow(MancalaBoard& board, Side side, int frompit);
	void steal(MancalaBoard& board, Side side, int lastpit);
	
private:
	Side side; 
	MancalaBoard board;
};

class Human: public MancalaPlayer {
public:
	Human(MancalaBoard& board);

	// pensando jugada
	int chooseMove(int& bestMove, Side side, MancalaBoard board, Tactic tactic);
	
	// turno
	bool getTurn() const { return turn; }
	void setTurn(bool t) { if (t==true) { turn=true; } if (t==false) { turn=false; } return;}

private:
	bool turn; 
};

class Computer: public MancalaPlayer {
public:
	Computer(MancalaBoard& board);
	
	// pensando jugada
	int chooseMove(int& bestMove, Side side, MancalaBoard board, Tactic tactic);
	
	// Quien sigue
	bool getTurn() const { return turn; }
	void setTurn(bool t) { if (t==true) { turn=true; } if (t==false) { turn=false; } return;}

private:
	bool turn; // turno
	int depth; // numero de intentos
	int step; // numero de pasos
};
