#include <iostream>
#include "types.h"

int main()
{
    Position BB;
    BB_Misc.Init();
    BB.Init("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    U64 bb = BB.GetPos(White, P);
    do {
        BB.PrintBB(bb & -bb);
    } while (bb &= ~(-bb));

    system("Pause");
    

    
    return 0;
}