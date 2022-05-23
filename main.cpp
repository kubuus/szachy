#include <iostream>
#include "types.h"

Bitboards BB_Misc;

int main()
{
    Position BB;
    Game gm;
    BB_Misc.Init();
    BB.Init("krb5/8/8/8/8/p7/P7/K7 w - - 0 1");
    gm.Init(BB);

    if (BB.state == 1)
        printf("It's a mate!");

    if (BB.state == 2)
        printf("It's a stalemate!");

    system("Pause");
    

    
    return 0;
}