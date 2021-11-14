#include <iostream>
#include "types.h"

Bitboards BB_Misc;

int main()
{
    Position BB;
    Game gm;
    BB_Misc.Init();
    BB.Init("1k1r1r2/8/8/8/8/8/P6P/R3K2R w KQ - 0 1");
    gm.Init(BB);
    gm.MoveGen(BB);
    gm.PrintGen(0);

    system("Pause");
    

    
    return 0;
}