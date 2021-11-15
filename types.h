#include <cstdint>
#include <string>
#include <array>
#include <unordered_map>

using U64 = uint64_t;

#define sq(x)  (U64(1) << (x))

int static TrailingZeros(U64 n) {
#if defined (__GNUC__)
    return __builtin_ctzll(n);
#endif
#if defined(_MSC_VER)
    unsigned long index;
    _BitScanForward64(&index, n);
    return index;
#endif
}
int static PopCount(U64 bb) {
#if defined (__GNUC__)
    return __builtin_popcountll(bb);
#endif
#if defined(_MSC_VER)
    //#include <intrin.h>
    return __popcnt64(bb);
#endif
}
U64 static LS1B(U64 bb) {return bb & -bb;}


enum eColour {White, Black, NC}; 
inline eColour operator~(eColour Col) {return eColour(Col ^ Black);}; 

enum ePieceType {P, N, B, R, Q, K, NPT};
inline ePieceType operator++(ePieceType pt) { return ePieceType(pt + 1); };

enum ePiece {wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK, no_Piece};
enum eFiles {A_FILE, B_FILE, C_FILE, D_FILE, E_FILE, F_FILE, G_FILE, H_FILE};
enum eRanks {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8};
enum eMoveType {QUIET, CAPTURE, DOUBLE_PAWN_PUSH, EN_PASSANT_CAPTURE, K_CASTLE, Q_CASTLE, N_PROMOTION, B_PROMOTION, R_PROMOTION, Q_PROMOTION, NO_TYPE};
enum eSquares {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8,
    n_sq
};

enum eDir {
    West = -1, North = 8, East = -West, South = -North,
    NW = North + West, NE = North + East, SW = South + West, SE = South + East,
    NNW = 17, NNE = 15, WNW = 10, ENE = 6,
    SSW = -NNE, SSE = -NNW, WSW = -ENE, ESE = - WNW,
};

constexpr U64 RANK_1_BB = 0x00000000000000FF;
constexpr U64 RANK_2_BB = 0x000000000000FF00;
constexpr U64 RANK_3_BB = 0x0000000000FF0000;
constexpr U64 RANK_4_BB = 0x00000000FF000000;
constexpr U64 RANK_5_BB = 0x000000FF00000000;
constexpr U64 RANK_6_BB = 0x0000FF0000000000;
constexpr U64 RANK_7_BB = 0x00FF000000000000;
constexpr U64 RANK_8_BB = 0xFF00000000000000;

constexpr U64 A_FILE_BB = 0x0101010101010101;
constexpr U64 B_FILE_BB = 0x0202020202020202;
constexpr U64 C_FILE_BB = 0x0404040404040404;
constexpr U64 D_FILE_BB = 0x0808080808080808;
constexpr U64 E_FILE_BB = 0x1010101010101010;
constexpr U64 F_FILE_BB = 0x2020202020202020;
constexpr U64 G_FILE_BB = 0x4040404040404040;
constexpr U64 H_FILE_BB = 0x8080808080808080;

constexpr U64 QUEENSIDE_BB = 0xF0F0F0F0F0F0F0F0;
constexpr U64 KINGSIDE_BB = ~QUEENSIDE_BB;
constexpr U64 LIGHT_SQUARES_BB = 0x0102010201020102 | (0x0102010201020102 << 2) | (0x0102010201020102 << 4) | (0x0102010201020102 << 6);
constexpr U64 DARK_SQUARES_BB = ~LIGHT_SQUARES_BB;

constexpr U64 Q_CASTLE_MASK = U64(14);
constexpr U64 K_CASTLE_MASK = U64(96);


constexpr U64 ShiftNorth(U64 b) {return (b << 8);}
constexpr U64 ShiftSouth(U64 b) {return (b >> 8);}
constexpr U64 ShiftWest(U64 b) {return ((b & (~A_FILE_BB)) >> 1);}
constexpr U64 ShiftEast(U64 b) {return ((b & (~H_FILE_BB)) << 1);}
constexpr U64 ShiftNW(U64 b) {return ((b & (~A_FILE_BB)) << 7);}
constexpr U64 ShiftNE(U64 b) {return ((b & (~H_FILE_BB)) << 9);}
constexpr U64 ShiftSW(U64 b) {return ((b & (~A_FILE_BB)) >> 9);}
constexpr U64 ShiftSE(U64 b) {return ((b & (~H_FILE_BB)) >> 7);}

constexpr U64 ShiftSideways(U64 b) {return ShiftWest(b) | ShiftEast(b);};

class Move{
public:
    eSquares MoveFrom = n_sq;
    eSquares MoveTo = n_sq;
    eMoveType MoveType = NO_TYPE;
    ePieceType PieceType = NPT;
    eSquares EPsquare = n_sq;

    void Init(eSquares MF, eSquares MT, eMoveType MTP, ePieceType PT, eSquares EPS)
    {
        MoveFrom = MF; MoveTo = MT; MoveType = MTP; PieceType = PT; EPsquare = EPS;
    };

};

class Bitboards{
private:
    U64 PawnAttacks[2][64];
    U64 PawnMoves[2][64];
    U64 KingAttacks[64];
    U64 KnightAttacks[64];

public:
    void Init();
    U64 GetPawnAttacks(eSquares sq, eColour col);
    U64 GetPawnMoves(eSquares sq, eColour col, U64 occ);
    U64 GetKnightAttacks(eSquares sq);
    U64 GetRookAttacks(eSquares sq, U64 occ);
    U64 GetBishopAttacks(eSquares sq, U64 occ);
    U64 GetQueenAttacks(eSquares sq, U64 occ);
    U64 GetKingAttacks(eSquares sq);
    U64 GetAttacks(ePieceType piece, eSquares sq, U64 occ, eColour col);
};

// I have no idea why extern doesn't work here, even when I only declare it once.
extern Bitboards BB_Misc;


class Position{
private:
    U64 Colour_BB[2] = {0, 0};
    U64 Piece_BB[6] = {0, 0, 0, 0, 0, 0};
    U64 ZobPieces[12][64] {};
    U64 ZobCR[16] {};
    U64 ZobEPsq[8] {};
    U64 HashKey = 0;
    
public:    

    eColour Turn = NC;               
    int CastRights = 0;         // Castling rights for both sides
    eSquares EPsq = n_sq;       // En Passant squae
    int ply = 0;                // 50 move rule
    int move_no = 0;            // Move number
    int state = 0;              // State of the game, 0 is normal, 1 is mate, 2 is stalemate

    
    Position *Undo = NULL;

    void Init(std::string FEN);
    void InitHashKey();

    U64 GetPos(eColour Col, ePieceType PieceType);
    U64 GetHash() { return HashKey; };
    int GetTurn() { return Turn; };
    U64 InBetween(eSquares StartingSq, eSquares TargetSq);
    
    void MakeMove(Move Movedo);
    bool IsLegal(Move Move);

    void PrintBB(U64 bb);
    bool AttackedSquare(eSquares Square, eColour Col);
    U64 CastlingRights(int Col);

    std::array<ePiece, 64> PieceList {};
    void UpdatePieceList(eSquares StartingSq, eSquares TargetSq, ePiece Piece);
    void UpdateHashKey(eSquares StartingSq, eSquares TargetSq);
};

class Game
{
private:
    std::unordered_map<U64, Position> PositionMap;
    std::vector<std::vector<U64>> MoveList; // 2d vector, first dim is depth, second is positions in this depth
    int Depth = 0;
       
public:
    void Init(Position Pos);
    
    void PushPos(Position Pos) { PositionMap.insert({ Pos.GetHash(), Pos }); };
    Position FindPos(U64 Hash) { return PositionMap.find(Hash)->second; };

    void MoveGen(Position Pos);
    void UndoMove(Move MoveUndo);

    void PrintGen(int dep);
};


