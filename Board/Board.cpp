#include "Board.hpp"

#include <initializer_list>

#include <NotationConversions.hpp>
#include <CheckChecker.hpp>
#include <MoveValidator.hpp>

static unsigned char fieldStringToNum(const char* field)
{
	unsigned char collumn = field[0] - 'a';
	unsigned char row = field[0] - '1';
	return row*8u + collumn;
}

Move::Move()
	: source(0)
	, destination(0)
	, isPromoted(false)
	, promoteTo(0) {}

Move::Move(unsigned char s, unsigned char d, bool isPromoted, unsigned char promoteTo)
	: source(s)
	, destination(d)
	, isPromoted(isPromoted)
	, promoteTo(promoteTo) {}

Move::Move(const char* sourceStr, const char* destinationStr, bool isPromoted, unsigned char promoteTo)
	: source(fieldStringToNum(sourceStr))
	, destination(fieldStringToNum(destinationStr))
	, isPromoted(isPromoted)
	, promoteTo(promoteTo) {}

unsigned char& Board::operator[](const char* field)
{
	unsigned char collumn = field[0] - 'a';
	unsigned char row = field[0] - '1';

	return fields[fieldStringToNum(field)];
}

unsigned char& Board::operator[](const unsigned char field)
{
	return fields[field];
}

const unsigned char& Board::operator[](const char* field) const
{
	return const_cast<Board&>(*this)[field];
}

const unsigned char& Board::operator[](const unsigned char field) const
{
	return const_cast<Board&>(*this)[field];
}

bool validateMove(const Board& board, const Move& move)
{
	return MoveValidator::validateMove(board, move);
}

bool isCheckOn(const Board& board, const NOTATION::COLOR::color c)
{
	return CheckChecker::isCheckOn(board, c);
}

void applyMove(Board& board, const Move& move)
{
	board[move.destination] = board[move.source] | NOTATION::MOVED::MOVED_MASK;
	board[move.source] = 0;

	if (NotationConversions::getPieceType(board[move.source]) == NOTATION::PIECES::KING)
	{
		bool isCastle = NotationConversions::getColumnNum(move.source) == NOTATION::COORDINATES::COLUMN::E
				and (NotationConversions::getColumnNum(move.source) == NOTATION::COORDINATES::COLUMN::C
					or NotationConversions::getColumnNum(move.source) == NOTATION::COORDINATES::COLUMN::G);

		if (isCastle)
		{
			auto row = NotationConversions::getRow(move.source);
			bool isLongCastle = NotationConversions::getColumnNum(move.source) == NOTATION::COORDINATES::COLUMN::C;
			if (isLongCastle)
			{
				auto rockSource = NotationConversions::getFieldNum(row, NOTATION::COORDINATES::COLUMN::A);
				auto rockDestination = NotationConversions::getFieldNum(row, NOTATION::COORDINATES::COLUMN::D);
				board[rockDestination] = board[rockSource];
				board[rockSource] = 0u;
			}
			else
			{
	            auto rockSource = NotationConversions::getFieldNum(row, NOTATION::COORDINATES::COLUMN::A);
	            auto rockDestination = NotationConversions::getFieldNum(row, NOTATION::COORDINATES::COLUMN::D);
	            board[rockDestination] = board[rockSource];
	            board[rockSource] = 0u;

			}
		}
	}

	if (move.isPromoted)
	{
		board[move.destination] = static_cast<unsigned char>(board.playerOnMove) |
		    (move.promoteTo & NOTATION::PIECES::PIECES_MASK);
	}

	board.playerOnMove = NotationConversions::switchColor(board.playerOnMove);
}

void initDefault(Board& board)
{
        board["a1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK;
        board["b1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT;
        board["c1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP;
        board["d1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::QUEEN;
        board["e1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::KING;
        board["f1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP;
        board["g1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT;
        board["h1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK;

        char field[3] = "a2";
        for (auto&& c: {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'})
        {
                field[0] = c;
                board[field] = NOTATION::COLOR::WHITE | NOTATION::PIECES::PAWN;
        }

        board["a8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK;
        board["b8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT;
        board["c8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP;
        board["d8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::QUEEN;
        board["e8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::KING;
        board["f8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP;
        board["g8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT;
        board["h8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK;

        field[2] = 7;
        for (auto&& c: {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'})
        {
                field[0] = c;
                board[field] = NOTATION::COLOR::BLACK | NOTATION::PIECES::PAWN;
        }
}
