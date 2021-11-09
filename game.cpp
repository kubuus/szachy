#include "types.h"

// Initializes game class, checks if there were any previous moves, inserts them into
// the position tree, and then inserts the position.
void Game::Init(Position Pos)
{
	if(!Pos.Undo)
		Init(*Pos.Undo);

	PositionMap.insert({ Pos.GetHash(), *Pos.Undo });
}

// Generates all moves from a position
void Game::MoveGen(Position Pos)
{

}

