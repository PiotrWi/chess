#include <evaluatorIf.hpp>

#include <memory>

#include <cppflow/cppflow.h>
#include <BoardEngine.hpp>

cppflow::model* model;

namespace
{

int getPlayer(BoardEngine& be)
{
    return (be.board.playerOnMove == NOTATION::COLOR::color::white)
        ? 1
        : -1;
}

template <unsigned char castlingMask>
int getCastlingRights(BoardEngine& be)
{
    return (be.board.castlingRights & castlingMask) ? 1 : 0;
}

std::vector<int> getEnPassantSquare(BoardEngine& be)
{
    std::vector ep(64, 0);
    if (be.board.validEnPassant >= 0)
    {
        ep[be.board.validEnPassant] = 1;
    }
    return ep;
}

int to01(int in)
{
    return in ? 1 : 0;
}

void insertPieces(std::vector<int>& data, uint64_t bitset)
{
    for (auto i = 0u; i < 64u; ++i)
    {
        data.push_back(to01(bitset & (1u << i) ));
    }
}

}  // namespace

void init(const char* )
{
    model = new cppflow::model("/home/pioter/proj/chess/ml-poc/chess500r100r1rDO.tf");
    // model = new cppflow::model(configurationFile);

    /*for (auto&& operation : model->get_operations())
    {
        std::cout << operation << std::endl;
    }*/
}

int evaluatePosition(BoardEngine& be, unsigned int)
{
    // tmp here
    constexpr unsigned char WHITE_LONG_BIT = 1;
    constexpr unsigned char WHITE_SHORT_BIT = 2;
    constexpr unsigned char BLACK_LONG_BIT = 4;
    constexpr unsigned char BLACK_SHORT_BIT = 8;

    std::vector<int> data;

    data.push_back(getPlayer(be));
    data.push_back(getCastlingRights<WHITE_SHORT_BIT>(be));
    data.push_back(getCastlingRights<BLACK_SHORT_BIT>(be));
    data.push_back(getCastlingRights<WHITE_LONG_BIT>(be));
    data.push_back(getCastlingRights<BLACK_LONG_BIT>(be));
    auto ep = getEnPassantSquare(be);
    data.insert(data.end(), ep.begin(), ep.end());

    insertPieces(data, be.board.piecesBitSets[0].pawnsMask);
    insertPieces(data, be.board.piecesBitSets[1].pawnsMask);
    insertPieces(data, be.board.piecesBitSets[0].rocksMask);
    insertPieces(data, be.board.piecesBitSets[1].rocksMask);
    insertPieces(data, be.board.piecesBitSets[0].knightsMask);
    insertPieces(data, be.board.piecesBitSets[1].knightsMask);
    insertPieces(data, be.board.piecesBitSets[0].bishopsMask);
    insertPieces(data, be.board.piecesBitSets[1].bishopsMask);
    insertPieces(data, be.board.piecesBitSets[0].queensMask);
    insertPieces(data, be.board.piecesBitSets[1].queensMask);
    insertPieces(data, be.board.piecesBitSets[0].kingsMask);
    insertPieces(data, be.board.piecesBitSets[1].kingsMask);

    auto tensor = cppflow::tensor(data, std::vector<int64_t>{1, static_cast<int64_t>(data.size())});
    auto out = (*model)({{"serving_default_dense_6_input:0", tensor}}, {"StatefulPartitionedCall:0"});
    return static_cast<int>(out[0].get_data<float>()[0]);
}

int evaluatePositionAvx(BoardEngine& be, unsigned int validMovesCount)
{
    return evaluatePosition(be, validMovesCount);
}