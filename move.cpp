#include "types.h"

// TODO: Create a method for checking if a square is attacked, then implement it
//       for king moves. Implement moves of the rest of pieces.

bool Position::IsLegal(eSquares StartingSq, eSquares TargetSq, ePieceType PieceType)
{
    Bitboards BB;
    BB.Init();
    int Col = Rest & U64(1);

    if(!(StartingSq & Colour_BB[Col]) || !(StartingSq & Piece_BB[PieceType]))
        return false;
    
    if(TargetSq & Colour_BB[Col])
        return false;

    eMoveType movetype;

    if (PieceType == K)
    {
        if(!(BB.GetKingAttacks(StartingSq) & TargetSq))
        {
            int CR = CastlingRights(Col);
            if(CR == 0)
                return false;
            
<<<<<<< HEAD
            if((TargetSq == g8 - (!Col * a8)) && (CR & (1)) && ((GetPos(NC, NPT) & (K_CASTLE_MASK << North * 7 * !Col)) == 0))
                return true;
            
            if((TargetSq == c8 - (!Col * a8)) && (CR & (2)) && ((GetPos(NC, NPT) & (Q_CASTLE_MASK << North * 7 * !Col)) == 0))
=======
            if((TargetSq == g8 - (Col * a8)) && (CR & (1)) && (GetPos(NC, NPT) && (K_CASTLE_MASK << North * 7 * (!Col)) == 0))
                return true;
            
            if((TargetSq == c8 - (Col * a8)) && (CR & (2)) && (GetPos(NC, NPT) && (Q_CASTLE_MASK << North * 7 * (!Col)) == 0))
>>>>>>> 1ae76f60385f23f2ff5adafbb176e92faa4ff96d
                return true;
            
            return false;
        }
        
    }

    return true;
}

void Position::Move(eSquares StartingSq, eSquares TargetSq, ePieceType PieceType)
{
    U64 bb = sq((U64)(StartingSq)) | sq((U64)(TargetSq));
    Colour_BB[Rest & U64(1)] ^= bb;
    Piece_BB[PieceType] ^= bb;
    Rest ^= U64(1);
}