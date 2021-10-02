#include <iostream>
#include "types.h"

Bitboards BB_Misc;

int main()
{
    Position BB;
    BB_Misc.Init();
    BB.Init("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    BB.PrintBB(BB.GetPos(White, R));
    for (int i = 0; i < 64; i++)
    {
        printf("%d - %d\n", i, BB.PieceList[i]);
    }
    

    
    return 0;
}