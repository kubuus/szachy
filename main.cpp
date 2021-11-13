#include <iostream>
#include "types.h"

Bitboards BB_Misc;

int main()
{
    Position BB;
    Game gm;
    BB_Misc.Init();
    BB.Init("r1bqk1nr/pppp1ppp/2n5/2b1p3/2BPP3/2P2N2/P4PPP/RNBQK2R b KQkq - 0 6");
    gm.Init(BB);
    gm.MoveGen(BB);
    gm.PrintGen(0);

    system("Pause");
    

    
    return 0;
}