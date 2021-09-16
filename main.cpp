#include <iostream>
#include "types.h"


int main()
{
    Position BB;
    BB.Init("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    BB.PrintBB(BB.GetPos(NC, NPT));
    BB.PrintBB(sq(e2));
    BB.Move(e2, e7, P);
    BB.PrintBB(BB.GetPos(NC, NPT));
    BB.PrintBB(BB.GetPos(White, NPT));
    BB.PrintBB(BB.GetPos(White, P));
    BB.PrintBB(BB.GetPos(NC, P));
    return 0;
}