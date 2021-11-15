#include <iostream>
#include "types.h"

Bitboards BB_Misc;

int main()
{
    Position BB;
    Game gm;
    BB_Misc.Init();
    BB.Init("rnbqkbnr/pppppQpp/8/7B/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    gm.Init(BB);
    gm.MoveGen(BB);
    gm.PrintGen(0);

    system("Pause");
    

    
    return 0;
}