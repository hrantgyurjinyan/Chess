#include <iostream>
#include <vector>
#include <algorithm>

const int BOARD_SIZE = 8;

enum PieceType { EMPTY, KING, QUEEN, ROOK, BISHOP, KNIGHT };

struct Position {
    int x, y;

    bool isValid() const 
    {
        return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
    }

    bool operator==(const Position& other) const 
    {
        return x == other.x && y == other.y;
    }
};

class Piece 
{
public:
    virtual ~Piece() = default;
    virtual PieceType getType() const = 0;
    virtual std::vector<Position> getMoves(Position pos) const = 0;
    virtual char getSymbol() const = 0;
};

class King : public Piece 
{
public:
    PieceType getType() const override { return KING; }
    char getSymbol() const override { return 'K'; }

    std::vector<Position> getMoves(Position pos) const override 
    {
        std::vector<Position> moves = 
        {
            {pos.x - 1, pos.y - 1}, {pos.x - 1, pos.y}, {pos.x - 1, pos.y + 1},
            {pos.x, pos.y - 1},                             {pos.x, pos.y + 1},
            {pos.x + 1, pos.y - 1}, {pos.x + 1, pos.y}, {pos.x + 1, pos.y + 1}
        };
        moves.erase(std::remove_if(moves.begin(), moves.end(), [](Position pos) { return !pos.isValid(); }), moves.end());
        return moves;
    }
};

class Queen : public Piece 
{
public:
    PieceType getType() const override { return QUEEN; }
    char getSymbol() const override { return 'Q'; }

    std::vector<Position> getMoves(Position pos) const override {
        std::vector<Position> moves;
        for (int i = 1; i < BOARD_SIZE; ++i) {
            moves.push_back({pos.x + i, pos.y});
            moves.push_back({pos.x - i, pos.y});
            moves.push_back({pos.x, pos.y + i});
            moves.push_back({pos.x, pos.y - i});
            moves.push_back({pos.x + i, pos.y + i});
            moves.push_back({pos.x - i, pos.y - i});
            moves.push_back({pos.x + i, pos.y - i});
            moves.push_back({pos.x - i, pos.y + i});
        }
        moves.erase(std::remove_if(moves.begin(), moves.end(), [](Position pos) { return !pos.isValid(); }), moves.end());
        return moves;
    }
};

class Rook : public Piece 
{
public:
    PieceType getType() const override { return ROOK; }
    char getSymbol() const override { return 'R'; }

    std::vector<Position> getMoves(Position pos) const override {
        std::vector<Position> moves;
        for (int i = 1; i < BOARD_SIZE; ++i) {
            moves.push_back({pos.x + i, pos.y});
            moves.push_back({pos.x - i, pos.y});
            moves.push_back({pos.x, pos.y + i});
            moves.push_back({pos.x, pos.y - i});
        }
        moves.erase(std::remove_if(moves.begin(), moves.end(), [](Position pos) { return !pos.isValid(); }), moves.end());
        return moves;
    }
};

class Bishop : public Piece 
{
public:
    PieceType getType() const override { return BISHOP; }
    char getSymbol() const override { return 'B'; }

    std::vector<Position> getMoves(Position pos) const override {
        std::vector<Position> moves;
        for (int i = 1; i < BOARD_SIZE; ++i) {
            moves.push_back({pos.x + i, pos.y + i});
            moves.push_back({pos.x - i, pos.y - i});
            moves.push_back({pos.x + i, pos.y - i});
            moves.push_back({pos.x - i, pos.y + i});
        }
        moves.erase(std::remove_if(moves.begin(), moves.end(), [](Position pos) { return !pos.isValid(); }), moves.end());
        return moves;
    }
};

class Knight : public Piece 
{
public:
    PieceType getType() const override { return KNIGHT; }
    char getSymbol() const override { return 'N'; }

    std::vector<Position> getMoves(Position pos) const override 
    {
        std::vector<Position> moves = {
            {pos.x + 2, pos.y + 1}, {pos.x + 2, pos.y - 1},
            {pos.x - 2, pos.y + 1}, {pos.x - 2, pos.y - 1},
            {pos.x + 1, pos.y + 2}, {pos.x + 1, pos.y - 2},
            {pos.x - 1, pos.y + 2}, {pos.x - 1, pos.y - 2}
        };
        moves.erase(std::remove_if(moves.begin(), moves.end(), [](Position pos) { return !pos.isValid(); }), moves.end());
        return moves;
    }
};

class Board 
{
public:
    Board() 
    {
        board.resize(BOARD_SIZE, std::vector<Piece*>(BOARD_SIZE, nullptr));
    }

    ~Board() 
    {
        for (int i = 0; i < BOARD_SIZE; ++i) 
        {
            for (int j = 0; j < BOARD_SIZE; ++j) 
            {
                delete board[i][j];
            }
        }
    }

    bool placePiece(Position pos, Piece* piece) 
    {
        if (isOccupied(pos)) 
        {
            delete piece;
            return false;
        }
        board[pos.x][pos.y] = piece;
        return true;
    }

    bool isOccupied(Position pos) const {
        return board[pos.x][pos.y] != nullptr;
    }

    Piece* getPiece(Position pos) const {
        return board[pos.x][pos.y];
    }

    void removePiece(Position pos) {
        delete board[pos.x][pos.y];
        board[pos.x][pos.y] = nullptr;
    }

    void display() const 
    {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                if (board[x][y]) {
                    std::cout << board[x][y]->getSymbol() << " ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<std::vector<Piece*>> board;
};

class Game 
{
public:
    Game() : blackKingPos{-1, -1} {}

    bool addWhitePiece(Position pos, Piece* piece) {
        if (board.placePiece(pos, piece)) {
            whitePieces.push_back(pos);
            return true;
        }
        return false;
    }

    bool setBlackKing(Position pos) {
        if (board.placePiece(pos, new King())) {
            blackKingPos = pos;
            return true;
        }
        return false;
    }

    bool isCheck(Position kingPos) {
        for (Position piecePos : whitePieces) {
            Piece* piece = board.getPiece(piecePos);
            std::vector<Position> moves = piece->getMoves(piecePos);
            for (Position move : moves) {
                if (move == kingPos) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isThreatened(Position pos) 
    {
        for (Position piecePos : whitePieces) 
        {
            Piece* piece = board.getPiece(piecePos);
            std::vector<Position> moves = piece->getMoves(piecePos);
            for (Position move : moves) 
            {
                if (move == pos) {
                    return true;
                }
            }
        }
        return false;
    }

    bool hasLegalMoves(Position kingPos) 
    {
        std::vector<Position> kingMoves = board.getPiece(kingPos)->getMoves(kingPos);
        for (Position move : kingMoves) 
        {
            if (!board.isOccupied(move) || (board.isOccupied(move) && board.getPiece(move)->getType() != KING)) {
                if (!isThreatened(move)) 
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool isCheckmate() 
    {
        return isCheck(blackKingPos) && !hasLegalMoves(blackKingPos);
    }

    void displayBoard() const 
    {
        board.display();
    }

private:
    Board board;
    Position blackKingPos;
    std::vector<Position> whitePieces;
};

int main() {
    Game game;
    if (!game.setBlackKing({1, 1})) {
        std::cerr << "Failed to place Black King" << std::endl;
        return 1;
    }

    if (!game.addWhitePiece({1, 2}, new Queen()) ||
        !game.addWhitePiece({1, 0}, new Rook()) ||
        !game.addWhitePiece({5, 5}, new Bishop()) ||
        !game.addWhitePiece({4, 4}, new Knight())) {
        std::cerr << "Failed to place one or more white pieces" << std::endl;
        return 1;
    }

    game.displayBoard();

    if (game.isCheckmate()) {
        std::cout << "Checkmate!" << std::endl;
    } else {
        std::cout << "Not checkmate." << std::endl;
    }

    return 0;
}
