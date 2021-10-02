#include "types.h"

// TODO: Implement undo move method.


// Method only changes bitboards getting the Move class 
// This method DOES NOT check whether the move is legal
void Position::MakeMove(Move MoveDo, Move *Undo)
{
    // Saving undo data only if it's at NULL value
    if(!Undo)
        Undo->Init(MoveDo.MoveTo, MoveDo.MoveFrom, QUIET, MoveDo.PieceType, EPsq);

    U64 bb = sq(MoveDo.MoveFrom) | (sq(MoveDo.MoveTo));
    Colour_BB[Turn] ^= bb;
    Piece_BB[MoveDo.PieceType] ^= bb;

    if(Colour_BB[~Turn] & sq(MoveDo.MoveTo))
    {
        Piece_BB[PieceList[MoveDo.MoveTo] - 6] ^= sq(MoveDo.MoveTo);
        Colour_BB[~Turn] ^= sq(MoveDo.MoveTo);
    }
    if(CastlingRights(Turn) != 0)
    {
        if(MoveDo.PieceType == K)
            CastRights ^= 3 << (2 * Turn);

        if(MoveDo.PieceType == R)
        {
            if((CastlingRights(Turn) & 1) && (MoveDo.MoveFrom == (a1 << (56 * Turn))))
                CastRights ^= 1 << (2 * Turn);
            
            if((CastlingRights(Turn) & 2) && (MoveDo.MoveFrom == (h1 << (56 * Turn))))
                CastRights ^= 2 << (2 * Turn);
        }
    }

    if((MoveDo.MoveType != CAPTURE) && (MoveDo.PieceType != P))
        ply++; else ply = 0;
    
    Turn = ~Turn;
    if(Turn == White) move_no++;
    EPsq = n_sq;

    if(MoveDo.MoveType == DOUBLE_PAWN_PUSH)
        EPsq = eSquares(MoveDo.MoveTo - a2);
}

bool Position::IsLegal(Move MoveDo)
{
    if(!(MoveDo.MoveFrom & Colour_BB[Turn]) || !(MoveDo.MoveFrom & Piece_BB[MoveDo.PieceType]))
        return false;
    
    if((MoveDo.MoveTo & Colour_BB[Turn]) || !(MoveDo.MoveTo & BB_Misc.GetAttacks(MoveDo.PieceType, MoveDo.MoveFrom, Colour_BB[NC], Turn)))
        return false;
    
    if(MoveDo.PieceType == K && AttackedSquare(MoveDo.MoveTo, ~Turn))
        return false;

    else {
    Colour_BB[Turn] ^= sq(MoveDo.MoveFrom);

    if(AttackedSquare(eSquares(TrailingZeros(GetPos(Turn, K))), ~Turn))
    {
        Colour_BB[Turn] ^= sq(MoveDo.MoveFrom);
        return false;
    }
    
    Colour_BB[Turn] ^= sq(MoveDo.MoveFrom);
    }

    switch(MoveDo.MoveType)
    {
        case(QUIET):
            return true;
        case(DOUBLE_PAWN_PUSH):

            if(MoveDo.PieceType != P)
                return false;
            
            if(Turn == White)
                if(!(MoveDo.MoveFrom & RANK_2_BB) || (ShiftNorth(MoveDo.MoveFrom) & Colour_BB[NC]))
                    return false;

            if(Turn == Black)
                if(!(MoveDo.MoveFrom & RANK_7_BB) || (ShiftSouth(MoveDo.MoveFrom) & Colour_BB[NC]))
                    return false;

            return true;

        case(K_CASTLE):

            if((MoveDo.PieceType != K) || !(CastlingRights(Turn) & 1))
                return false;
            
            if(Turn == White)
                if(Colour_BB[NC] & K_CASTLE_MASK)
                    return false;
                else return true;
                
            if(Turn == Black)
                if(Colour_BB[NC] & (K_CASTLE_MASK) << 56)
                    return false;
                else return true;
            
            return true;

        case(Q_CASTLE):

            if((MoveDo.PieceType != K) || !(CastlingRights(Turn) & 2))
                return false;
            
            if(Turn == White)
                if(Colour_BB[NC] & Q_CASTLE_MASK)
                    return false;
                else return true;
                
            if(Turn == Black)
                if(Colour_BB[NC] & (Q_CASTLE_MASK) << 56)
                    return false;
                else return true;
            return true;
        
        default:
            return true;
    };

    return true;
}
