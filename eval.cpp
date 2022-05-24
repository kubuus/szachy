#include "types.h"

int Vertex::Mobility()
{
	U64 bAttacks, wAttacks;
	bAttacks = wAttacks = 0;
	for (int i = 0; i < 64; i++)
	{
		ePiece pc = Pos.PieceList[i];
		if (pc != no_Piece) 
			(pc / 2 == 1 ? bAttacks : wAttacks) |= BB_Misc.GetAttacks(ePieceType(pc % 6), eSquares(i), Pos.GetPos(NC, NPT), eColour(pc / 2)) & ~(Pos.GetPos(eColour(pc / 2), NPT));
	}
	int mob = PopCount(wAttacks) - PopCount(bAttacks);
	return mob*4;
	
}

void Vertex::Evaluate()
{
	int PieceValues[6] = { 100, 300, 300, 500, 800, 20000 };
	for (ePieceType i = P; i < NPT; ~i)
		Score += PopCount(Pos.GetPos(White, i)) * PieceValues[i] - PopCount(Pos.GetPos(Black, i)) * PieceValues[i];
	Score += Mobility();
}