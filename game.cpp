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
	eColour Col = eColour(Pos.GetTurn());
	for (ePieceType i = P; i < NPT; ++i)
	{
		U64 bb = Pos.GetPos(Col, i);
		std::vector<Move> NextMoves;
		switch (i) {
		case(P):
			do
			{
				eSquares sq = eSquares(TrailingZeros(bb));
				U64 attacks = BB_Misc.GetPawnAttacks(sq, Col) & (Pos.GetPos(~Col, NPT) | sq(U64(Pos.EPsq)));
				U64 pushes = BB_Misc.GetPawnMoves(sq, Col, Pos.GetPos(NC, NPT));
				do
				{
					Move mv;
					mv.Init(sq, eSquares(TrailingZeros(attacks)), QUIET, P, n_sq);
					NextMoves.push_back(mv);
				} while (attacks &= (-attacks));
				do
				{
					Move mv;
					eSquares Dest = eSquares(TrailingZeros(pushes));
					if (abs(sq(U64(sq)) - sq(U64(Dest)) > 1 << North))
						mv.Init(sq, Dest, DOUBLE_PAWN_PUSH, P, eSquares(Dest - a2));
					else if (pushes & RANK_1_BB || pushes & RANK_8_BB)
					{
						mv.Init(sq, Dest, Q_PROMOTION, P, n_sq);
						NextMoves.push_back(mv);
						mv.Init(sq, Dest, R_PROMOTION, P, n_sq);
						NextMoves.push_back(mv);
						mv.Init(sq, Dest, B_PROMOTION, P, n_sq);
						NextMoves.push_back(mv);
						mv.Init(sq, Dest, N_PROMOTION, P, n_sq);
					}
					else
						mv.Init(sq, Dest, QUIET, P, n_sq);
					NextMoves.push_back(mv);
				} while (pushes &= ~(-pushes));
			} while (bb &= ~(-bb));
			break;

		case(K):

			break;
		}
				
	}
}

