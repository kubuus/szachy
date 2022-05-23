#include "types.h"

void Vertex::Evaluate()
{
	float PieceValues[6] = { 1, 3, 3, 5, 8, 200 };
	for (ePieceType i = P, i < NPT, i++)
		Score += PopCount(Pos.GetPos(Pos.Turn, i)) * PieceValues[i];
}