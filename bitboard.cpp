#include "types.h"
#include "magicmoves.h"
#include <math.h>
#include <iostream>

void Bitboards::Init()
{
    initmagicmoves();

    // initialazing pawn, king and knight moves

    for(int i = 0; i < 64; i++)
    {
        PawnAttacks[0][i] = ShiftNE(sq(i)) | ShiftNW(sq(i));
        PawnAttacks[1][i] = ShiftSE(sq(i)) | ShiftSW(sq(i));

        if(i % 8 != RANK_2)
            PawnMoves[0][i] = ShiftNorth(sq(i));
        else
            PawnMoves[0][i] = ShiftNorth(sq(i)) | sq(i+2*North);

        if(i % 8 != RANK_7)
            PawnMoves[1][i] = ShiftSouth(sq(i));
        else
            PawnMoves[1][i] = ShiftSouth(sq(i)) | sq(i + 2 * South);

        KingAttacks[i] = ShiftSideways(sq(i));
        KingAttacks[i] |= ShiftNorth(KingAttacks[i] | sq(i)) | ShiftSouth(KingAttacks[i] | sq(i));
        
        KnightAttacks[i] = ShiftNW(ShiftWest(sq(i))) | ShiftSW(ShiftWest(sq(i))) | ShiftNE(ShiftEast(sq(i))) | ShiftSE(ShiftEast(sq(i))) |
                           ShiftNW(ShiftNorth(sq(i))) | ShiftSW(ShiftSouth(sq(i))) | ShiftNE(ShiftNorth(sq(i))) | ShiftSE(ShiftSouth(sq(i)));
    }
}

U64 Bitboards::GetPawnAttacks(eSquares sq, eColour col)
{
    return PawnAttacks[col][sq];
}

U64 Bitboards::GetPawnMoves(eSquares sq, eColour col, U64 occ)
{
    return PawnMoves[col][sq] & (~occ);
}

U64 Bitboards::GetKnightAttacks(eSquares sq)
{
    return KnightAttacks[sq]; 
}

U64 Bitboards::GetKingAttacks(eSquares sq)
{
    return KingAttacks[sq]; 
}

U64 Bitboards::GetBishopAttacks(eSquares sq, U64 occ)
{
    return Bmagic(sq, occ);
}

U64 Bitboards::GetRookAttacks(eSquares sq, U64 occ)
{
    return Rmagic(sq, occ);
}

U64 Bitboards::GetQueenAttacks(eSquares sq, U64 occ)
{
    return Rmagic(sq, occ) | Bmagic(sq, occ);
}

U64 Bitboards::GetAttacks(ePieceType piece, eSquares sq, U64 occ, eColour col)
{
    switch (piece)
    {
    case P:
        return GetPawnAttacks(sq, col) | GetPawnMoves(sq, col, occ);
    
    case N:
        return GetKnightAttacks(sq);
    
    case B:
        return GetBishopAttacks(sq, occ);
    
    case R:
        return GetRookAttacks(sq, occ);
    
    case Q:
        return GetQueenAttacks(sq, occ);
    
    case K:
        return GetKingAttacks(sq);

    default:
        break;
    }
    return 0;
}

void Position::PrintBB(U64 bb)
{
    for (int j = 0; j < 8; j++)
    {
        for(int i = 0; i < 8; i++){
            char n = (bb >> (a8 + i - (j * 8)) & (1));
            std::cout << (n ? '1' : '.');
            }

    std::cout << "\n";
    }
    std::cout << "\n";
}

U64 Position::GetPos(eColour Col, ePieceType PieceType)
{
    return (Col == NC ? Colour_BB[White] | Colour_BB[Black] : Colour_BB[Col])
         & (PieceType == NPT ? (U64)(-1) : Piece_BB[PieceType]);
}

// Method checking for castling rights of given colour,
// 0 for none, 1 for kingside, 2 for queenside, 3 for both
U64 Position::CastlingRights(int Col)
{
    return ((CastRights >> (Col*2)) & U64(3));
}

void Position::Init(std::string FEN)
{
    PieceList.fill(no_Piece);
    
    int i = 0, j = 0, count = 0;
    do {
    switch (FEN[i])
    {
    case 'p':
        Colour_BB[1] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[P] |= (sq(a8 + (j % 8) - (count * 8)));
        PieceList[a8 + (j++ % 8) - (count * 8)] = bP;
        i++; break;

    case 'P':
        Colour_BB[0] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[P] |= (sq(a8 + (j % 8) - (count * 8)));
        PieceList[a8 + (j++ % 8) - (count * 8)] = wP;
        i++; break;

    case 'b':
        Colour_BB[1] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[B] |= (sq(a8 + (j % 8) - (count * 8)));
        PieceList[a8 + (j++ % 8) - (count * 8)] = bB;
        i++; break;

    case 'B':
        Colour_BB[0] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[B] |= (sq(a8 + (j % 8) - (count * 8)));
        PieceList[a8 + (j++ % 8) - (count * 8)] = wB;
        i++; break;

    case 'n':
        Colour_BB[1] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[N] |= (sq(a8 + (j % 8) - (count * 8)));
        PieceList[a8 + (j++ % 8) - (count * 8)] = bN;
        i++; break;

    case 'N':
        Colour_BB[0] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[N] |= (sq(a8 + (j % 8) - (count * 8)));
        PieceList[a8 + (j++ % 8) - (count * 8)] = wN;
        i++; break;

    case 'r':
        Colour_BB[1] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[R] |= (sq(a8 + (j % 8) - (count * 8)));
        PieceList[a8 + (j++ % 8) - (count * 8)] = bR;
        i++; break;

    case 'R':
        Colour_BB[0] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[R] |= (sq(a8 + (j % 8) - (count * 8)));
        PieceList[a8 + (j++ % 8) - (count * 8)] = wR;
        i++; break;
        
    case 'q':
        Colour_BB[1] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[Q] |= (sq(a8 + (j % 8) - (count * 8)));
        PieceList[a8 + (j++ % 8) - (count * 8)] = bQ;
        i++; break;

    case 'Q':
        Colour_BB[0] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[Q] |= (sq(a8 + (j % 8) - (count * 8)));
        PieceList[a8 + (j++ % 8) - (count * 8)] = wQ;
        i++; break;

    case 'k':
        Colour_BB[1] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[K] |= (sq(a8 + (j % 8) - (count * 8)));
        PieceList[a8 + (j++ % 8) - (count * 8)] = bK;
        i++; break; 

    case 'K':
        Colour_BB[0] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[K] |= (sq(a8 + (j % 8) - (count * 8)));
        PieceList[a8 + (j++ % 8) - (count * 8)] = wK;
        i++; break;
        
    case '1':
        j++;
        i++; break;
       
    case '2':
        j += 2;
        i++; break;
       
    case '3':
        j += 3;
        i++; break;
       
    case '4':
        j += 4;
        i++; break;
       
    case '5':
        j += 5;
        i++; break;
       
    case '6':
        j += 6;
        i++; break;
        
    case '7':
        j += 7;
        i++; break;
       
    case '8':
        j += 8;
        i++; break;
       
    case '/':
        count++;
        i++; break;
       
    case ' ':
        i++; count++; break;

    default:
        std::cout << "Error while reading FEN, unexpected character!\n";
        return;
    }
    }while(count < 8);

    count = 0;
    do{
    switch (FEN[i])
    {
    case 'w':
        Turn = White;
        i++; break;

    case 'b':
        Turn = Black;
        i++; break;

    case 'K':
        CastRights |= 1;
        i++; break;

    case 'Q':
        CastRights |= 2;
        i++; break;

    case 'k':
        CastRights |= 4;
        i++; break;

    case 'q':
        CastRights |= 8;
        i++; break;

    case '-':
        i++; break;

    case ' ':
        count++;
        i++; break; 

    default:
        std::cout << "Error while reading FEN, unexpected character!\n";
        return;
    }
    }while(count < 2);

    // Reading En Passant Square
    if(FEN[i] != '-')
    {
        EPsq = (eSquares)((FEN[i] - 'a') + (FEN[i + 1] -'1')* 8);
        i += 3;
    }
    else i += 2;
    
    // Reading 50 move rule
    if(FEN[i + 1] != ' ')
    {
        ply = ((FEN[i] -'0')*10 + (FEN[i + 1] -'0'));
        i += 3;
    }
    
    else
    {
        ply = (U64)(FEN[i] - '0');
        i += 2;
    }
    

    // Reading move number
    U64 num = 0;
    for(int j = FEN.length(); i < j; i++)
        num += (FEN[i] - '0')*(pow(10, j-i-1));
    move_no = num;

    InitHashKey();
}