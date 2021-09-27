#include <iostream>
#include "types.h"


int main()
{
    Position BB;
    Bitboards BB_Misc;
    BB_Misc.Init();
    BB.Init("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    BB.PrintBB(BB_Misc.GetKingAttacks(e1));
    
    return 0;
}