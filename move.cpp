#include "types.h"

// TODO: Implement undo move method.


// Method only changes bitboards getting the Move class 
// This method DOES NOT check whether the move is legal
void Position::MakeMove(Move MoveDo)
{
    U64 bb = sq(MoveDo.MoveFrom) | (sq(MoveDo.MoveTo));

    if(MoveDo.MoveType == K_CASTLE)
    {
        Colour_BB[Turn] ^= bb | ShiftEast(bb);
        Piece_BB[K] ^= bb;
        Piece_BB[R] ^= ShiftEast(bb);
        UpdateHashKey(MoveDo.MoveFrom, MoveDo.MoveTo);
        UpdateHashKey(eSquares(MoveDo.MoveTo + East), eSquares(MoveDo.MoveFrom + East));
        UpdatePieceList(MoveDo.MoveFrom, MoveDo.MoveTo, PieceList[MoveDo.MoveFrom]);
        UpdatePieceList(eSquares(MoveDo.MoveTo + East), eSquares(MoveDo.MoveFrom + East), ePiece(wR + Turn * 6));
        HashKey ^= ZobCR[CastRights];
        CastRights ^= (CastRights & (3 << (Turn * 2)));
        HashKey ^= ZobCR[CastRights];
    }
        
    else if(MoveDo.MoveType == Q_CASTLE)
    {
        Colour_BB[Turn] ^= bb | ((sq(d1) | sq(h1)) << (56 * Turn));
        Piece_BB[K] ^= bb;
        Piece_BB[R] ^= (sq(d1) | sq(h1)) << (56 * Turn);
        UpdateHashKey(MoveDo.MoveFrom, MoveDo.MoveTo);
        UpdateHashKey(eSquares(MoveDo.MoveTo + 2*West), eSquares(MoveDo.MoveFrom + West));
        UpdatePieceList(MoveDo.MoveFrom, MoveDo.MoveTo, ePiece(wK + Turn * 6));
        UpdatePieceList(eSquares(MoveDo.MoveTo + 2*West), eSquares(MoveDo.MoveFrom + West), ePiece(wR + Turn * 6));
        HashKey ^= ZobCR[CastRights];
        CastRights ^= (CastRights & (3 << (Turn * 2)));
        HashKey ^= ZobCR[CastRights];
    }

    else if(MoveDo.MoveType > Q_CASTLE && MoveDo.MoveType != CAPTURE)
    {
        Colour_BB[Turn] ^= bb;
        if(Colour_BB[~Turn] & sq(MoveDo.MoveTo))
        {
            Piece_BB[PieceList[MoveDo.MoveTo] % 6] ^= sq(MoveDo.MoveTo);
            Colour_BB[~Turn] ^= sq(MoveDo.MoveTo);
            HashKey ^= ZobPieces[PieceList[MoveDo.MoveTo]][MoveDo.MoveTo];
        }
        HashKey ^= ZobPieces[PieceList[MoveDo.MoveFrom]][MoveDo.MoveFrom];
        if (MoveDo.MoveType == Q_PROMOTION || MoveDo.MoveType == Q_PROMOTION_CAPTURE)
        {
            Piece_BB[P] ^= MoveDo.MoveFrom;
            Piece_BB[Q] ^= MoveDo.MoveTo;
            UpdatePieceList(MoveDo.MoveFrom, MoveDo.MoveTo, ePiece(wQ + Turn * 6));
        }
        else if (MoveDo.MoveType == R_PROMOTION || MoveDo.MoveType == R_PROMOTION_CAPTURE)
        {
            Piece_BB[P] ^= MoveDo.MoveFrom;
            Piece_BB[R] ^= MoveDo.MoveTo;
            UpdatePieceList(MoveDo.MoveFrom, MoveDo.MoveTo, ePiece(wR + Turn * 6));
        }
        else if (MoveDo.MoveType == B_PROMOTION || MoveDo.MoveType == B_PROMOTION_CAPTURE)
        {
            Piece_BB[P] ^= MoveDo.MoveFrom;
            Piece_BB[B] ^= MoveDo.MoveTo;
            UpdatePieceList(MoveDo.MoveFrom, MoveDo.MoveTo, ePiece(wB + Turn * 6));
        }
        
        else if (MoveDo.MoveType == N_PROMOTION || MoveDo.MoveType == N_PROMOTION_CAPTURE)
        {
            Piece_BB[P] ^= MoveDo.MoveFrom;
            Piece_BB[N] ^= MoveDo.MoveTo;
            UpdatePieceList(MoveDo.MoveFrom, MoveDo.MoveTo, ePiece(wN + Turn * 6));
        }
        HashKey ^= ZobPieces[PieceList[MoveDo.MoveTo]][MoveDo.MoveTo];
    }

    


    else {
    Colour_BB[Turn] ^= bb;
    Piece_BB[MoveDo.PieceType] ^= bb;
    if(MoveDo.MoveType == EN_PASSANT_CAPTURE)
    {
        if(Turn == White)
        { 
            Piece_BB[P] ^= sq(MoveDo.MoveTo + South);
            Colour_BB[~Turn] ^= sq(MoveDo.MoveTo + South);
            HashKey ^= ZobPieces[PieceList[MoveDo.MoveTo + South]][MoveDo.MoveTo + South];
            PieceList[MoveDo.MoveTo + South] = no_Piece;
        }
            
        else
        {
            Piece_BB[P] ^= sq(MoveDo.MoveTo + North);
            Colour_BB[~Turn] ^= sq(MoveDo.MoveTo + North);
            HashKey ^= ZobPieces[PieceList[MoveDo.MoveTo + North]][MoveDo.MoveTo + North];
            PieceList[MoveDo.MoveTo + North] = no_Piece;
        }
            
    }
    if(Colour_BB[~Turn] & sq(MoveDo.MoveTo))
    {
        Piece_BB[PieceList[MoveDo.MoveTo] % 6] ^= sq(MoveDo.MoveTo);
        Colour_BB[~Turn] ^= sq(MoveDo.MoveTo);
        HashKey ^= ZobPieces[PieceList[MoveDo.MoveTo]][MoveDo.MoveTo];
    }

    UpdatePieceList(MoveDo.MoveFrom, MoveDo.MoveTo, PieceList[MoveDo.MoveFrom]);
    UpdateHashKey(MoveDo.MoveFrom, MoveDo.MoveTo);
    }

    if(CastlingRights(Turn) != 0)
    {
        HashKey ^= ZobCR[CastRights];
        if(MoveDo.PieceType == K)
            CastRights ^= 3 << (2 * Turn);

        if(MoveDo.PieceType == R)
        {
            if((CastlingRights(Turn) & 1) && (MoveDo.MoveFrom == (a1 << (56 * Turn))))
                CastRights ^= 1 << (2 * Turn);
            
            if((CastlingRights(Turn) & 2) && (MoveDo.MoveFrom == (h1 << (56 * Turn))))
                CastRights ^= 2 << (2 * Turn);
        }
        HashKey ^= ZobCR[CastRights];
    }

    if((MoveDo.MoveType < CAPTURE) && (MoveDo.PieceType != P))
        ply++; else ply = 0;
    
    Turn = ~Turn;
    if(Turn == White) move_no++;
    if(EPsq != n_sq) 
        HashKey ^= ZobEPsq[EPsq];
    EPsq = n_sq;

    if(MoveDo.MoveType == DOUBLE_PAWN_PUSH) 
    {
        EPsq = eSquares(MoveDo.MoveTo + South);
        HashKey ^= ZobEPsq[EPsq];
    }
    HashKey ^= ~U64(0);
}

// Method for checking legality of moves.
// Works for any move, even not generated inside the program.
bool Position::IsLegal(Move MoveDo)
{
    if(!(sq(MoveDo.MoveFrom) & Colour_BB[Turn]) || !(sq(MoveDo.MoveFrom) & Piece_BB[MoveDo.PieceType]))
        return false;
    
    if(sq(MoveDo.MoveTo) & Colour_BB[Turn])
        return false;

    if (!(sq(MoveDo.MoveTo) & BB_Misc.GetAttacks(MoveDo.PieceType, MoveDo.MoveFrom, GetPos(NC, NPT), Turn)) && MoveDo.MoveType != K_CASTLE && MoveDo.MoveType != Q_CASTLE)
        return false;
    
    if((MoveDo.PieceType == K && AttackedSquare(MoveDo.MoveTo, ~Turn)))
        return false;


    // a bit of piece shuffling, might clean it up later
    else {
        Colour_BB[Turn] ^= sq(MoveDo.MoveFrom) | sq(MoveDo.MoveTo);
        Piece_BB[MoveDo.PieceType] ^= sq(MoveDo.MoveFrom) | sq(MoveDo.MoveTo);
        if (Colour_BB[~Turn] & sq(MoveDo.MoveTo))
        {
            Piece_BB[PieceList[MoveDo.MoveTo] % 6] ^= sq(MoveDo.MoveTo);
            Colour_BB[~Turn] ^= sq(MoveDo.MoveTo);
            if (AttackedSquare(eSquares(TrailingZeros(GetPos(Turn, K))), ~Turn))
            {
                Piece_BB[PieceList[MoveDo.MoveTo] % 6] ^= sq(MoveDo.MoveTo);
                Colour_BB[~Turn] ^= sq(MoveDo.MoveTo);
                Colour_BB[Turn] ^= sq(MoveDo.MoveFrom) | sq(MoveDo.MoveTo);
                Piece_BB[MoveDo.PieceType] ^= sq(MoveDo.MoveFrom) | sq(MoveDo.MoveTo);
                return false;
            }
            Piece_BB[PieceList[MoveDo.MoveTo] % 6] ^= sq(MoveDo.MoveTo);
            Colour_BB[~Turn] ^= sq(MoveDo.MoveTo);
        }
        else if (AttackedSquare(eSquares(TrailingZeros(GetPos(Turn, K))), ~Turn))
            {
                Colour_BB[Turn] ^= sq(MoveDo.MoveFrom) | sq(MoveDo.MoveTo);
                Piece_BB[MoveDo.PieceType] ^= sq(MoveDo.MoveFrom) | sq(MoveDo.MoveTo);
                return false;
            }
            Colour_BB[Turn] ^= sq(MoveDo.MoveFrom) | sq(MoveDo.MoveTo);
            Piece_BB[MoveDo.PieceType] ^= sq(MoveDo.MoveFrom) | sq(MoveDo.MoveTo);
        }

    switch(MoveDo.MoveType)
    {
        case(DOUBLE_PAWN_PUSH):
            if(MoveDo.PieceType != P)
                return false;
            
            if(Turn == White)
                if (sq(MoveDo.MoveFrom) & RANK_2_BB)
                    if (ShiftNorth(sq(MoveDo.MoveFrom)) & GetPos(NC, NPT))
                        return false;

            if(Turn == Black)
                if(sq(MoveDo.MoveFrom) & RANK_7_BB)
                    if(ShiftSouth(sq(MoveDo.MoveFrom)) & GetPos(NC, NPT))
                        return false;

            return true;

        case(EN_PASSANT_CAPTURE):
            if(MoveDo.PieceType != P || EPsq != MoveDo.MoveTo)
                return false;

        case(K_CASTLE):

            if((MoveDo.PieceType != K) || !(CastlingRights(Turn) & 1))
                return false;
            
            if (AttackedSquare(eSquares(MoveDo.MoveFrom + East), ~Turn) || AttackedSquare(eSquares(MoveDo.MoveFrom), ~Turn))
                return false;

            if (GetPos(NC, NPT) & (K_CASTLE_MASK) << (56 * Turn))
                    return false;
            
            return true;

        case(Q_CASTLE):

            if((MoveDo.PieceType != K) || !(CastlingRights(Turn) & 2))
                return false;

            if (AttackedSquare(eSquares(MoveDo.MoveFrom + West), ~Turn) || AttackedSquare(eSquares(MoveDo.MoveFrom), ~Turn))
                return false;
            
            if(GetPos(NC, NPT) & (Q_CASTLE_MASK) << (56 * Turn))
                    return false;

            return true;
        
        default:
            return true;
    };

    return true;
}
