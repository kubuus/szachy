#include "types.h"

// TODO: Implement moves of the rest of pieces.


// Method only changes bitboards getting the Move class 
// This method DOES NOT check whether the move is legal
void Position::MakeMove(Move MoveDo, Move *Undo)
{
    // Saving undo data to only if it's at NULL value
    if(!Undo)
        Undo->Init(MoveDo.MoveTo, MoveDo.MoveFrom, UNDO, MoveDo.PieceType, n_sq);

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
    Bitboards BB;
    BB.Init();

    if(!(MoveDo.MoveFrom & Colour_BB[Turn]) || !(MoveDo.MoveFrom & Piece_BB[MoveDo.PieceType]))
        return false;
    
    if(MoveDo.MoveTo & Colour_BB[Turn])
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
        case(DOUBLE_PAWN_PUSH):
            return true;
        case(EN_PASSANT_CAPTURE):
            return true;
        case(K_CASTLE):
            return true;
        case(Q_CASTLE):
            return true;
        default:
            return true;
    };

    return true;
}
