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

        KingAttacks[i] = ShiftSideways(sq(i));
        KingAttacks[i] |= ShiftNorth(KingAttacks[i] | sq(i)) | ShiftSouth(KingAttacks[i] | sq(i));
        
        KnightAttacks[i] = ShiftNW(ShiftWest(sq(i))) | ShiftSW(ShiftWest(sq(i))) | ShiftNE(ShiftEast(sq(i))) | ShiftSE(ShiftEast(sq(i))) |
                           ShiftNW(ShiftNorth(sq(i))) | ShiftSW(ShiftNorth(sq(i))) | ShiftNE(ShiftSouth(sq(i))) | ShiftSE(ShiftSouth(sq(i)));
    }
}

U64 Bitboards::GetPawnAttacks(eSquares sq, eColour col)
{
    return PawnAttacks[col][sq];
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
    return ((Rest << (1 + Col*2)) & U64(3));
}

// Method assumes that the two squares are on the same file, rank or diagonal.
/*bool Position::FreeBetween(eSquares StartingSq, eSquares TargetSq)
{

}*/

void Position::Init(std::string FEN)
{
    int i = 0, j = 0, count = 0;
    do {
    switch (FEN[i])
    {
    case 'p':
        Colour_BB[1] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[P] |= (sq(a8 + (j++ % 8) - (count * 8)));
        i++; break;

    case 'P':
        Colour_BB[0] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[P] |= (sq(a8 + (j++ % 8) - (count * 8)));
        i++; break;

    case 'b':
        Colour_BB[1] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[B] |= (sq(a8 + (j++ % 8) - (count * 8)));
        i++; break;

    case 'B':
        Colour_BB[0] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[B] |= (sq(a8 + (j++ % 8) - (count * 8)));
        i++; break;

    case 'n':
        Colour_BB[1] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[N] |= (sq(a8 + (j++ % 8) - (count * 8)));
        i++; break;

    case 'N':
        Colour_BB[0] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[N] |= (sq(a8 + (j++ % 8) - (count * 8)));
        i++; break;

    case 'r':
        Colour_BB[1] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[R] |= (sq(a8 + (j++ % 8) - (count * 8)));
        i++; break;

    case 'R':
        Colour_BB[0] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[R] |= (sq(a8 + (j++ % 8) - (count * 8)));
        i++; break;
        
    case 'q':
        Colour_BB[1] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[Q] |= (sq(a8 + (j++ % 8) - (count * 8)));
        i++; break;

    case 'Q':
        Colour_BB[0] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[Q] |= (sq(a8 + (j++ % 8) - (count * 8)));
        i++; break;

    case 'k':
        Colour_BB[1] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[K] |= (sq(a8 + (j++ % 8) - (count * 8)));
        i++; break; 

    case 'K':
        Colour_BB[0] |= (sq(a8 + (j % 8) - (count * 8)));
        Piece_BB[K] |= (sq(a8 + (j++ % 8) - (count * 8)));
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
        i++; break;

    case 'b':
        Rest |= (U64)(1);
        i++; break;

    case 'K':
        Rest |= (U64)(1) << 1;
        i++; break;

    case 'Q':
        Rest |= (U64)(1) << 2;
        i++; break;

    case 'k':
        Rest |= (U64)(1) << 3;
        i++; break;

    case 'q':
        Rest |= (U64)(1) << 4;
        i++; break;

    case '-':
        i++; break;

    case ' ':
        count++;
        i++; break; 

    default:
        std::cout << "Error while reading FEN, unexpected character!\n";
        break;
    }
    }while(count < 2);

    // Reading En Passant Square
    if(FEN[i] != '-')
    {
        Rest |= (U64)((FEN[i] - 'a') + (FEN[i + 1] -'1')* 8)  << 5;
        i += 3;
    }
    else i += 2;
    
    // Reading 50 move rule
    std::cout << FEN[i] << FEN[i+1] << "\n";
    if(FEN[i + 1] != ' ')
    {
        Rest |= (U64)((FEN[i] -'0')*10 + (FEN[i + 1] -'0')) << 11;
        i += 3;
    }
    
    else
    {
        Rest |= (U64)(FEN[i] - '0') << 11;
        i += 2;
    }
    

    // Reading move number
    U64 num = 0;
    for(int j = FEN.length(); i < j; i++)
        num += (FEN[i] - '0')*(pow(10, j-i-1));
    
    
    Rest |= num << 17;
}