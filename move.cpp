#include "types.h"

// TODO: Create a method for checking if a square is attacked, then implement it
//       for king moves. Implement moves of the rest of pieces.


void Position::MakeMove(Move MoveDo, Move *Undo)
{
    // Saving move data to undo move only if it's at NULL value to avoid repetition
    if(!Undo)
        Undo->Init(MoveDo.MoveTo, MoveDo.MoveFrom, UNDO, MoveDo.PieceType, n_sq);

    U64 bb = sq((U64)(MoveDo.MoveFrom)) | (sq((U64)(MoveDo.MoveTo)));
    Colour_BB[Rest & U64(1)] ^= bb;
    if(Colour_BB[!(Rest & U64(1))] & sq((U64)(MoveDo.MoveTo))){
        
        Colour_BB[!(Rest & U64(1))] ^ sq((U64)(MoveDo.MoveTo));
        }
    Piece_BB[MoveDo.PieceType] ^= bb;
    Rest ^= U64(1);
}

bool Position::IsLegal(Move MoveDo)
{
    Bitboards BB;
    BB.Init();
    eColour Col = eColour(Rest & U64(1));
    eColour oppCol = InvertCol(Col);

    if(!(MoveDo.MoveFrom & Colour_BB[Col]) || !(MoveDo.MoveFrom & Piece_BB[MoveDo.PieceType]))
        return false;
    
    if(MoveDo.MoveTo & Colour_BB[Col])
        return false;

    if (MoveDo.PieceType == K)
    {
        if(AttackedSquare(MoveDo.MoveTo, oppCol))
            return false;

        if(!(BB.GetKingAttacks(MoveDo.MoveFrom) & MoveDo.MoveTo))
        {
               
            if(MoveDo.MoveType == K_CASTLE || MoveDo.MoveType == Q_CASTLE)
            {
            int CR = CastlingRights(Col);
            if(CR == 0)
                return false;
            
            if((MoveDo.MoveTo == g8 - (oppCol * a8)) && !(AttackedSquare(eSquares(g8 - (oppCol * a8)), oppCol)) && (CR & (1)) && ((GetPos(NC, NPT) & (K_CASTLE_MASK << North * 7 * Col)) == 0))
                return true;
            
            if((MoveDo.MoveTo == c8 - (oppCol * a8)) && !(AttackedSquare(eSquares(c8 - (oppCol * a8)), oppCol)) && (CR & (2)) && ((GetPos(NC, NPT) & (Q_CASTLE_MASK << North * 7 * Col)) == 0))
                return true;
            }
            return false;
        }
        return true;
    }

    Move *Undo = NULL;
    MakeMove(MoveDo, Undo);
    
    if(AttackedSquare(eSquares(TrailingZeros(GetPos(Col, K))), oppCol))
    {
        MakeMove(*Undo, &MoveDo);
        return false;
    }
    
    MakeMove(*Undo, &MoveDo);

    if(MoveDo.PieceType == P);

    return true;
}
