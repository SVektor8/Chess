#include <iostream>
#include <algorithm>
#include <utility>
#include <string>
#include <vector>
   
//TODO pieces moving (probably cycle), showing move ability (where it can move)

class Piece
{
protected:
    int x, y;
    bool alive;
    bool color; //true = white, false = black
    char type = 'Z';

public: //TODO ASK does it stay public and the upper code protected?
    Piece() : alive(true) {};

    Piece(bool color) : alive(true), color(color) {};

    [[nodiscard]] int get_x() const { return x; }

    [[nodiscard]] int get_y() const { return y; }

    [[nodiscard]] char get_type() const { return type; }

    [[nodiscard]] bool get_color() const { return color; }

    [[nodiscard]] bool is_alive() const { return alive; }

    void move(int n_x, int n_y)
    {
        x = n_x;
        y = n_y;
    }

    //TODO write destructor
    ~Piece() = default;

    Piece(Piece const &src)
    {
        x = src.get_x();
        y = src.get_y();
        type = src.get_type();
        alive = src.is_alive();
        color = src.get_color();
    }

    Piece(Piece &&src)
    {
        x = src.get_x();
        y = src.get_y();
        type = src.get_type();
        alive = src.is_alive();
        color = src.get_color();
    }

    Piece &operator=(Piece &src)
    {
        if (this == &src) return *this;

        Piece tmp(src);
        std::swap(this->x, tmp.x);
        std::swap(this->y, tmp.y);
        std::swap(this->type, tmp.type);
        std::swap(this->alive, tmp.alive);
        std::swap(this->color, tmp.color);

        return *this;
    }

    Piece &operator=(Piece &&src)
    {
        if (this == &src) return *this;

        Piece tmp(std::move(src));
        std::swap(this->x, tmp.x);
        std::swap(this->y, tmp.y);
        std::swap(this->type, tmp.type);
        std::swap(this->alive, tmp.alive);
        std::swap(this->color, tmp.color);

        return *this;
    }

};

class King final : public Piece
{
private:
    bool moved;
    bool checked;

public:
    King(bool color) : Piece(color), moved(false), checked(false) { type = 'K'; }

    [[nodiscard]] bool has_moved() const { return moved; }

    [[nodiscard]] bool was_checked() const { return checked; }
};

class Queen final : public Piece
{
public:
    Queen(bool color) : Piece(color) { type = 'Q'; }
};

class Rook final : public Piece
{//TODO ASK why not protected?
private:
    bool moved;
public:
    Rook(bool color) : Piece(color), moved(false) { type = 'R'; }

    [[nodiscard]] bool has_moved() const { return moved; }
};

class Bishop final : public Piece
{
public:
    Bishop(bool color) : Piece(color) { type = 'B'; }
};

class Knight final : public Piece
{
public:
    Knight(bool color) : Piece(color) { type = 'N'; }
};

class Pawn final : public Piece
{
private:
    bool moved;
public:
    Pawn(bool color) : Piece(color), moved(false) { type = 'p'; } //TODO ASK why cannot write x(X)

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
    Cell() : x(1), y(1), vertical('a'), empty(true) {};

    Cell(int x, int y) : x(x), y(y), vertical("nabcdefgh"[x]), empty(true) {};

    //TODO write destructor
    ~Cell() = default;

    Cell(Cell const &src)
    {
        x = src.get_x();
        y = src.get_y();
        vertical = src.get_vertical();
        empty = src.is_empty();
        piece = src.get_piece();
    }

    Cell(Cell &&src)
    {
        x = src.get_x();
        y = src.get_y();
        vertical = src.get_vertical();
        empty = src.is_empty();
        piece = src.get_piece();
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

    [[nodiscard]] Piece get_piece() const { return piece; }

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
    std::vector<Piece> pieces;
public:
    //TODO edit constructor for custom positions
    Position(const std::string &&mode)
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
            {
                board[i][j] = Cell(j + 1, i + 1);

                if (i == 1)
                    pieces.push_back(Pawn(true));
                else if (i == 6)
                    pieces.push_back(Pawn(false));
                else if (i == 0 or i == 7)
                {
                    if (j == 0 or j == 7)
                        pieces.push_back(Rook(i == 0 ? true : false));
                    else if (j == 1 or j == 6)
                        pieces.push_back(Knight(i == 0 ? true : false));
                    else if (j == 2 or j == 5)
                        pieces.push_back(Bishop(i == 0 ? true : false));
                    else if (j == 3)
                        pieces.push_back(Queen(i == 0 ? true : false));
                    else if (j == 4)
                        pieces.push_back(King(i == 0 ? true : false));
                } else
                    continue;

                if (mode == "default")
                {
                    board[i][j].employ(pieces[pieces.size() - 1]);
                }
            }
    }

    //TODO write destructor
    ~Position() = default;

    Position(Position const &src)
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                board[i][j] = src.get_cell(j + 1, i + 1);
        for (int i = 0; i < 32; i++)
            pieces.push_back(src.get_piece(i));
    }

    Position(Position &&src)
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                board[i][j] = src.get_cell(j + 1, i + 1);
        for (int i = 0; i < 32; i++)
            pieces.push_back(src.get_piece(i));
    }

    Position &operator=(Position &src)
    {
        if (this == &src) return *this;

        Position tmp(src);
        std::swap(this->board, tmp.board);
        std::swap(this->pieces, tmp.pieces);
    }

    Position &operator=(Position &&src)
    {
        if (this == &src) return *this;

        Position tmp(std::move(src));
        std::swap(this->board, tmp.board);
        std::swap(this->pieces, tmp.pieces);
    }

    [[nodiscard]] Cell get_cell(int x, int y) const
    {
        return board[y - 1][x - 1];
    }

    [[nodiscard]] Piece get_piece(int i) const
    {
        return pieces[i];
    }
};

int main()
{
    std::cout << "Hello, Chess World!" << std::endl;

    Position posi("defaul:"), pos("default");
    posi = pos;

    for (int y = 8; y > 0; y--)
    {
        for (int x = 1; x < 9; x++)
        {
            Cell cell(posi.get_cell(x, y));
            if (cell.is_empty())
                std::cout << '*';
            else
                std::cout << cell.get_piece().get_type();
        }

        std::cout << std::endl;
    }

    return 0;
}
