#include "types.h"

// TODO: Create a method for checking if a square is attacked, then implement it
//       for king moves. Implement moves of the rest of pieces.

bool Position::IsLegal(Move Move)
{
    Bitboards BB;
    BB.Init();
    int Col = Rest & U64(1);

    if(!(Move.MoveFrom & Colour_BB[Col]) || !(Move.MoveFrom & Piece_BB[Move.PieceType]))
        return false;
    
    if(Move.MoveTo & Colour_BB[Col])
        return false;

    if (Move.PieceType == K)
    {
        if(!(BB.GetKingAttacks(Move.MoveFrom) & Move.MoveTo))
        {
            
            if(Move.MoveType > QUIET)
            {
            int CR = CastlingRights(Col);
            if(CR == 0)
                return false;
            
            if((Move.MoveTo == g8 - (!Col * a8)) && (CR & (1)) && ((GetPos(NC, NPT) & (K_CASTLE_MASK << North * 7 * !Col)) == 0))
                return true;
            
            if((Move.MoveTo == c8 - (!Col * a8)) && (CR & (2)) && ((GetPos(NC, NPT) & (Q_CASTLE_MASK << North * 7 * !Col)) == 0))
                return true;
            }
            return false;
        }
        
    }

    return true;
}

void Position::MakeMove(Move Move)
{
    U64 bb = sq((U64)(Move.MoveFrom)) | sq((U64)(Move.MoveTo));
    Colour_BB[Rest & U64(1)] ^= bb;
    Piece_BB[Move.PieceType] ^= bb;
    Rest ^= U64(1);
}