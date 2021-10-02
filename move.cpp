#include "types.h"

// TODO: Implement moves of the rest of pieces.


// Method only changes bitboards getting the Move class 
// This method DOES NOT check whether the move is legal
void Position::MakeMove(Move MoveDo, Move *Undo)
{
    // Saving undo data to only if it's at NULL value
    if(!Undo)
        Undo->Init(MoveDo.MoveTo, MoveDo.MoveFrom, QUIET, MoveDo.PieceType, n_sq);

    U64 bb = sq((U64)(MoveDo.MoveFrom)) | (sq((U64)(MoveDo.MoveTo)));
    Colour_BB[Turn] ^= bb;
    if(Colour_BB[~Turn] & sq((U64)(MoveDo.MoveTo))){
        
        Colour_BB[~Turn] ^ sq((U64)(MoveDo.MoveTo));
        }
    Piece_BB[MoveDo.PieceType] ^= bb;
    Turn = ~Turn;
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
    
    Move *Undo = NULL;
    MakeMove(MoveDo, Undo);

    if(AttackedSquare(eSquares(TrailingZeros(GetPos(Turn, K))), ~Turn))
    {
        MakeMove(*Undo, &MoveDo);
        return false;
    }
    
    MakeMove(*Undo, &MoveDo);

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
