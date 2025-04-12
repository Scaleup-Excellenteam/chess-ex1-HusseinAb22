// Chess
#include "Board.h"
#include "Chess.h"
#include "GameValidator.h"


int main()
{
	string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
	//string board = "##########KP#n############################R#############r#r#####";
	Chess a(board);
	int codeResponse = 0;
	string res = a.getInput();
	bool isWhite = true;
	while (res != "exit")
	{
		codeResponse = validateMove(res, board,isWhite);
		if (codeResponse == 42)
			isWhite = !isWhite;
		a.setCodeResponse(codeResponse);
		res = a.getInput();
	}

	cout << endl << "Exiting " << endl;
	return 0;
}