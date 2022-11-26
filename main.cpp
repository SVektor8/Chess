#include <iostream>
#include <algorithm>
#include <utility>
#include <string>
#include <vector>

class Piece
{
protected:
    int x, y;
    bool alive;
    bool color; //true = white, false = black

public: //TODO ASK does it stay public and the upper code protected?
    Piece() : alive(true) {};

    Piece(int x, int y, bool color) : x(x), y(y), alive(true), color(color) {};

    [[nodiscard]] int get_x() const { return x; }

    [[nodiscard]] int get_y() const { return y; }

    void move(int n_x, int n_y)
    {
        x = n_x;
        y = n_y;
    }

    //TODO rewrite methods of the "the rule of 5"

    ~Piece() = default;

    Piece(Piece const &rha)
    {
        x = rha.get_x();
        y = rha.get_y();

    }

    Piece &operator=(Piece &rha)
    {
        x = rha.get_x();
        y = rha.get_y();

        return *this;
    }

    Piece(Piece &&rha)
    {
        x = rha.get_x();
        y = rha.get_y();

    }

    Piece &operator=(Piece &&rha)
    {
        x = rha.get_x();
        y = rha.get_y();

        return *this;
    }

};


class King final : Piece
{
private:
    bool moved;
    bool checked;

public:
    King(int x, int y, bool color) : Piece(x, y, color)
    {
        moved = false;
        checked = false;
    };

    [[nodiscard]] bool has_moved() const { return moved; }

    [[nodiscard]] bool was_checked() const { return checked; }
};

class Queen final : Piece
{
public:
    Queen(int x, int y, bool color) : Piece(x, y, color) {};
};

class Rook final : Piece
{
private:
    bool moved;
public:
    Rook(int x, int y, bool color) : Piece(x, y, color)
    {
        moved = false;
    };

    [[nodiscard]] bool has_moved() const { return moved; }
};

class Bishop final : Piece
{
public:
    Bishop(int x, int y, bool color) : Piece(x, y, color) {};
};

class Knight final : Piece
{
public:
    Knight(int x, int y, bool color) : Piece(x, y, color) {};
};

class Pawn final : Piece
{
private:
    bool moved;
public:
    Pawn(int x, int y, bool color) : Piece(x, y, color) //TODO ASK why cannot write x(X)
    {
        moved = false;
    };

    [[nodiscard]] bool has_moved() const { return moved; }
};

class Cell final
{
private:
    int x, y;
    char vertical;
    bool empty;
    Piece piece;
public:
    Cell() : x(1), y(1), vertical('a') {};

    Cell(int x, int y) : x(x), y(y), vertical("nabcdefgh"[x]), empty(true) {};

    Cell(int x, int y, char name, bool color) :
            x(x), y(y), vertical("nabcdefgh"[x]), empty(false)
    {
        //TODO finish constructor after adding child classes
        Piece piece();
    };

    //TODO write destructor

    ~Cell() = default;

    Cell(Cell const &src)
    {
        x = src.get_x();
        y = src.get_y();
        vertical = src.get_vertical();
    }

    Cell(Cell &&src)
    {
        x = src.get_x();
        y = src.get_y();
        vertical = src.get_vertical();
    }

    Cell &operator=(Cell &src)
    {
        if (this == &src) return *this;

        Cell tmp(src);
        std::swap(this->x, tmp.x);
        std::swap(this->y, tmp.y);
        std::swap(this->vertical, tmp.vertical);
        std::swap(this->empty, tmp.empty);
        std::swap(this->piece, tmp.piece);
    }

    Cell &operator=(Cell &&src) noexcept
    {
        if (this == &src) return *this;

        Cell tmp(std::move(src));
        std::swap(this->x, tmp.x);
        std::swap(this->y, tmp.y);
        std::swap(this->vertical, tmp.vertical);
        std::swap(this->empty, tmp.empty);
        std::swap(this->piece, tmp.piece);
    }

    [[nodiscard]] int get_x() const { return x; } //TODO ASK nodiscard

    [[nodiscard]] int get_y() const { return y; }

    [[nodiscard]] char get_vertical() const { return vertical; }

    [[nodiscard]] Piece get_piece() { return piece; }

    [[nodiscard]] bool is_empty() const { return empty; }

    void employ(Piece &p)
    {
        piece = p;
        empty = false;
        piece.move(x, y);
    }

    void unemploy()
    {
        empty = true;
    }
};

class Position final
{
private:
    Cell board[8][8];
    Piece pieces[32];
public:
    //TODO edit constructor when finish with different pieces
    Position()
    {
        int curr = 0;
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
            {
                if (i == 1)
                    board[i][j] =
                            Cell(j + 1, i + 1, 'p', true);
                else if (i == 6)
                    board[i][j] =
                            Cell(j + 1, i + 1, 'p', false);
                else if (j == 0 or j == 7)
                    board[i][j] =
                            Cell(j + 1, i + 1, 'R', i == 0 ? true : false);
                else if (j == 1 or j == 6)
                    board[i][j] =
                            Cell(j + 1, i + 1, 'N', i == 0 ? true : false);
                else if (j == 2 or j == 5)
                    board[i][j] =
                            Cell(j + 1, i + 1, 'B', i == 0 ? true : false);
                else if (j == 3)
                    board[i][j] =
                            Cell(j + 1, i + 1, 'Q', i == 0 ? true : false);
                else if (j == 4)
                    board[i][j] =
                            Cell(j + 1, i + 1, 'K', i == 0 ? true : false);
                else
                {
                    board[i][j] =
                            Cell(j + 1, i + 1);
                    continue;
                }
                //pieces[curr] = &board[i][j].get_piece(); TODO
            }
    }

    //TODO upgrade methods of the "the rule of 5"

    ~Position() = default;

    Position(Position const &) = delete;

    Position &operator=(Piece &) = delete;

    Position(Position &&) = delete;

    Position &operator=(Position &&) = delete;
};

int main()
{
    std::cout << "Hello, Chess World!" << std::endl;
    return 0;
}
