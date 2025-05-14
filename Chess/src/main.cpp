// Chess
#include "Board.h"
#include "Chess.h"
#include "GameValidator.h"
#include "Exceptions.h"

int main()
{
	//string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
	string board = "##########KP#n############################R##########P##r#r#####";

	Chess a(board);
	int codeResponse = 0;
	string res;
	bool isWhite = true;
	while (res != "exit")
	{
		res = a.getInput();
		if (res == "exit") break;
		try {
			codeResponse = validateMove(res, board, isWhite,false);
			if (codeResponse == 42) isWhite = !isWhite;
			a.setCodeResponse(codeResponse);
			a.setBoard(board);

			}
		catch (const IllegalMoveException& e) {
			std::cout << e.what() << std::endl;
			continue;
			}
		catch (const BoardStateException& e) {
			std::cout << e.what() << std::endl;
			continue;
			}
		run(board, isWhite);

	}

	cout << endl << "Exiting " << endl;
	return 0;
}