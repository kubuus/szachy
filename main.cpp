#include <iostream>
#include "types.h"

Bitboards BB_Misc;

int main()
{
    Position BB;
    Vertex v;
    BB_Misc.Init();
    BB.Init("krb5/8/8/8/8/p7/PPP5/K7 w - - 0 1");
    v.Pos = BB;
    v.MoveGen();

    v.Pos.Print();
    for (int i = 0; i < v.children.size(); i++)
    {
        v.children[i]->Pos.Print();
    }

    system("Pause");
    

    
    return 0;
}