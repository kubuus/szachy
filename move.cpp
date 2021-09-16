#include "types.h"

bool Position::IsLegal(eSquares StartingSq, eSquares TargetSq, ePieceType PieceType)
{
    if(!(StartingSq & Colour_BB[Rest & U64(1)]) || !(StartingSq & Piece_BB[PieceType]))
        return false;
    
    if(TargetSq & Colour_BB[Rest & U64(1)])
        return false;
}

void Position::Move(eSquares StartingSq, eSquares TargetSq, ePieceType PieceType)
{
    U64 bb = sq((U64)(StartingSq)) | sq((U64)(TargetSq));
    Colour_BB[Rest & U64(1)] ^= bb;
    Piece_BB[PieceType] ^= bb;
    Rest ^= U64(1);
}