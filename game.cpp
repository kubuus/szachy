#include "types.h"

// Initializes game class, checks if there were any previous moves, inserts them into
// the position tree, and then inserts the position.
void Game::Init(Position Pos)
{
	if(Pos.Undo)
		Init(*Pos.Undo);

	PositionMap.insert({ Pos.GetHash(), Pos });
}

// Generates all moves from a position
void Game::MoveGen(Position Pos)
{
	eColour Col = eColour(Pos.Turn);
	std::vector<Move> NextMoves;

	// Iterating through all piece types
	for (int i = 0; i < 6; i++)
	{
		ePieceType Pi = ePieceType(i);
		U64 bb = Pos.GetPos(Col, Pi);
		switch (Pi) {
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
				} while (attacks &= ~(-attacks));
				do
				{
					Move mv;
					eSquares Dest = eSquares(TrailingZeros(pushes));
					if (abs(sq - Dest) > North)
						mv.Init(sq, Dest, DOUBLE_PAWN_PUSH, P, Pos.Turn ? eSquares(Dest + a2) : eSquares(Dest - a2));
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
			do
			{
				eSquares sq = eSquares(TrailingZeros(bb));
				U64 attacks = BB_Misc.GetKingAttacks(sq);
				do
				{
					Move mv;
					mv.Init(sq, eSquares(TrailingZeros(attacks)), QUIET, K, n_sq);
					NextMoves.push_back(mv);
				} while (attacks &= ~(-attacks));
				
				// Castling logic, assuming we're not playing Fischer chess
				if (Pos.CastlingRights(Col) & U64(1))
				{
					Move mv;
					mv.Init(sq, eSquares(sq + 2 * East), K_CASTLE, K, n_sq);
					NextMoves.push_back(mv);
				}
				if (Pos.CastlingRights(Col) & U64(2))
				{
					Move mv;
					mv.Init(sq, eSquares(sq + 2 * West), Q_CASTLE, K, n_sq);
					NextMoves.push_back(mv);
				}
			} while (bb &= ~(-bb));
			break;
		default:
			do
			{
				eSquares sq = eSquares(TrailingZeros(bb));
				U64 attacks = BB_Misc.GetAttacks(Pi, sq, Pos.GetPos(NC, NPT), Col) & ~(Pos.GetPos(Pos.Turn, NPT));
				do
				{
					Move mv;
					mv.Init(sq, eSquares(TrailingZeros(attacks)), QUIET, Pi, n_sq);
					NextMoves.push_back(mv);
				} while (attacks &= ~(-attacks));
			} while (bb &= ~(-bb));
			break;
		}
				
	}

	std::vector<U64> temp;
	for (int i = 0; i < NextMoves.size(); i++)
	{
		if (Pos.IsLegal(NextMoves[i]))
		{
			Position newPos = Pos;
			newPos.MakeMove(NextMoves[i]);
			newPos.Undo = &Pos;
			temp.push_back(newPos.GetHash());
			PushPos(newPos);
		}
	}
	MoveList.push_back(temp);
}

void Game::PrintGen(int dep)
{
	for (int i = 0; i < MoveList[dep].size(); i++)
	{
		Position bb = FindPos(MoveList[dep][i]);
		bb.PrintBB(bb.GetPos(NC, NPT));
	}
}

