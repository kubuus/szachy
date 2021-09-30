#include "types.h"

// Method checks whether a square is attacked in a given position. The colour is the colour of the attacker!
bool Position::AttackedSquare(eSquares Sq, eColour AtBy)
{
    U64 occ = GetPos(NC, NPT);
    Bitboards BB;
    BB.Init();
    
    if(BB.GetPawnAttacks(Sq, ~AtBy) & GetPos(AtBy, P)) return true;

    if(BB.GetKnightAttacks(Sq) & GetPos(AtBy, N)) return true;
    if(BB.GetKingAttacks(Sq)   & GetPos(AtBy, N)) return true;
    
    if(BB.GetRookAttacks(Sq, occ)   & (GetPos(AtBy, R) | GetPos(AtBy, Q))) return true;
    if(BB.GetBishopAttacks(Sq, occ) & (GetPos(AtBy, B) | GetPos(AtBy, Q))) return true;

    return false;
}

// Imported from https://www.chessprogramming.org/Square_Attacked_By
U64 inBetween(eSquares sq1, eSquares sq2) {
   const U64 m1   = U64(-1);
   const U64 a2a7 = U64(0x0001010101010100);
   const U64 b2g7 = U64(0x0040201008040200);
   const U64 h1b7 = U64(0x0002040810204080); /* Thanks Dustin, g2b7 did not work for c1-a3 */
   U64 btwn, line, rank, file;

   btwn  = (m1 << sq1) ^ (m1 << sq2);
   file  =   (sq2 & 7) - (sq1   & 7);
   rank  =  ((sq2 | 7) -  sq1) >> 3 ;
   line  =      (   (file  &  7) - 1) & a2a7; /* a2a7 if same file */
   line += 2 * ((   (rank  &  7) - 1) >> 58); /* b1g1 if same rank */
   line += (((rank - file) & 15) - 1) & b2g7; /* b2g7 if same diagonal */
   line += (((rank + file) & 15) - 1) & h1b7; /* h1b7 if same antidiag */
   line *= btwn & -btwn; /* mul acts like shift by smaller square */
   return line & btwn;   /* return the bits on that line in-between */
}