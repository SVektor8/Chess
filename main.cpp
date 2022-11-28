#include <iostream>
#include <algorithm>
#include <utility>
#include <string>
#include <vector>

//TODO pieces moving (probably cycle), showing move ability (where it can move)

void add_moves(std::vector<std::vector<int>> &vector, int x, int y)
{
    std::vector<int> tmp(2);
    tmp[0] = x;
    tmp[1] = y;
    vector.push_back(tmp);
}

class Piece
{
protected:
    int x, y;
    bool alive;
    bool color; //true = white, false = black
    char type = 'Z';

public: //TODO ASK does it stay public and the upper code protected?
    Piece() : alive(true), color(true) {};

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

    Piece(Piece &&src) noexcept
    {
        x = src.get_x();
        y = src.get_y();
        type = src.get_type();
        alive = src.is_alive();
        color = src.get_color();
    }

    Piece &operator=(Piece const &src)
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

    Piece &operator=(Piece &&src) noexcept
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
    bool empty, black_attacked = false, white_attacked = false;
    bool color;
    Piece *piece;
public:
    Cell() : x(1), y(1), vertical('a'), empty(true), color(false), piece(nullptr) {};

    Cell(int x, int y) : x(x), y(y), vertical("nabcdefgh"[x]), empty(true), color((x + y) % 2), piece(nullptr) {};

    //TODO write destructor
    ~Cell() = default;

    Cell(Cell const &src)
    {
        x = src.get_x();
        y = src.get_y();
        vertical = src.get_vertical();
        empty = src.is_empty();
        piece = src.get_piece();
        color = src.get_color();
        white_attacked = src.is_attacked_by_white();
        black_attacked = src.is_attacked_by_black();
    }

    Cell(Cell &&src) noexcept
    {
        Cell tmp(src);
        *this = tmp;
    }

    Cell &operator=(Cell const &src)
    {
        if (this == &src) return *this;

        Cell tmp(src);
        std::swap(this->x, tmp.x);
        std::swap(this->y, tmp.y);
        std::swap(this->vertical, tmp.vertical);
        std::swap(this->empty, tmp.empty);
        std::swap(this->piece, tmp.piece);
        std::swap(this->color, tmp.color);
        std::swap(this->white_attacked, tmp.white_attacked);
        std::swap(this->black_attacked, tmp.black_attacked);

        return *this;
    }

    Cell &operator=(Cell &&src) noexcept
    {
        if (this == &src) return *this;

        Cell tmp(std::move(src));
        *this = tmp;

        return *this;
    }

    [[nodiscard]] int get_x() const { return x; } //TODO ASK nodiscard

    [[nodiscard]] int get_y() const { return y; }

    [[nodiscard]] int get_color() const { return color; }

    [[nodiscard]] char get_vertical() const { return vertical; }

    [[nodiscard]] Piece *get_piece() const { return piece; }

    [[nodiscard]] bool is_empty() const { return empty; }

    [[nodiscard]] bool is_attacked_by_white() const { return white_attacked; }

    [[nodiscard]] bool is_attacked_by_black() const { return black_attacked; }

    bool is_attacked_by(bool piece_color) const
    {
        if (piece_color)
            return white_attacked;
        else
            return black_attacked;
    }

    void set_attacked(bool p_color)
    {
        if (p_color)
            white_attacked = true;
        else
            black_attacked = true;
    }

    void set_unattacked()
    {
        white_attacked = false;
        black_attacked = false;
    }

    void employ(Piece *p)
    {
        piece = p;
        empty = false;
        piece->move(x, y);
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
    std::vector<Piece> pieces; // if default: RNBQKBNRpppppppp|ppppppppRNBQKBNR
    bool move = true, checked = false; // true = white, false = black
public:
    //TODO edit constructor for custom positions
    Position(const std::string &mode)
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
                }
                else
                    continue;

                if (mode == "default")
                {
                    board[i][j].employ(&pieces[pieces.size() - 1]);
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

    Position(Position &&src) noexcept
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                board[i][j] = src.get_cell(j + 1, i + 1);
        for (int i = 0; i < 32; i++)
            pieces.push_back(src.get_piece(i));
    }

    Position &operator=(Position const &src)
    {
        if (this == &src) return *this;

        Position tmp(src);
        std::swap(this->board, tmp.board);
        std::swap(this->pieces, tmp.pieces);
    }

    Position &operator=(Position &&src) noexcept
    {
        if (this == &src) return *this;

        Position tmp(std::move(src));
        std::swap(this->board, tmp.board);
        std::swap(this->pieces, tmp.pieces);

        return *this;
    }

    [[nodiscard]] Cell get_cell(int x, int y) const
    {
        return board[y - 1][x - 1];
    }

    [[nodiscard]] Piece get_piece(int i) const
    {
        return pieces[i];
    }

    bool vertical_is_free(const int x, int y1, int y2)
    {
        if (y1 > y2)
            std::swap(y1, y2);

        for (int y = y1 + 1; y < y2; y++)
            if (not board[y - 1][x - 1].is_empty())
                return false;
        return true;
    }

    bool horizontal_is_free(const int y, int x1, int x2)
    {
        if (x1 > x2)
            std::swap(x1, x2);

        for (int x = x1 + 1; x < x2; x++)
            if (not board[y - 1][x - 1].is_empty())
                return false;
        return true;
    }

    bool diagonal_is_free(int x1, int y1, int x2, int y2)
    {
        if (x1 > x2)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        if (y1 < y2)
        {
            for (int x = x1 + 1, y = y1 + 1; x < x2; x++, y++)
                if (not board[y - 1][x - 1].is_empty())
                    return false;
            return true;
        }
        else
        {
            for (int x = x1 + 1, y = y1 - 1; x < x2; x++, y--)
                if (not board[y - 1][x - 1].is_empty())
                    return false;
            return true;
        }
    }

    void check_attack(Piece const &piece, Cell &cell)
    {
        int c_x = cell.get_x(), c_y = cell.get_y();
        int p_x = piece.get_x(), p_y = piece.get_y();
        bool color = piece.get_color();

        if (not(c_x == p_x and (c_y == p_y)))
        {
            if (piece.get_type() == 'p')
            {
                if (color)
                {
                    if (c_y - p_y == 1 and abs(c_x - p_x) == 1) // white pawn
                        cell.set_attacked(color);
                }
                else
                {
                    if (c_y - p_y == -1 and abs(c_x - p_x) == 1) // black pawn
                        cell.set_attacked(color);
                }
            }
            else if (piece.get_type() == 'K')
            {
                if (abs(c_y - p_y) <= 1 and abs(c_x - p_x) <= 1)
                    cell.set_attacked(color);
            }
            else if (piece.get_type() == 'R')
            {
                if (c_x == p_x and vertical_is_free(c_x, c_y, p_y)
                                   or c_y == p_y and horizontal_is_free(c_y, c_x, p_x))
                    cell.set_attacked(color);

            }
            else if (piece.get_type() == 'N')
            {
                if (c_y != p_y and c_x != p_x
                    and (abs(c_y - p_y) == 2 and abs(c_x - p_x) == 1 or
                                                 abs(c_x - p_x) == 2 and abs(c_y - p_y) == 1))
                    cell.set_attacked(color);
            }
            else if (piece.get_type() == 'B')
            {
                if (abs(c_y - p_y) == abs(c_x - p_x) and
                    diagonal_is_free(c_x, c_y, p_x, p_y))
                    cell.set_attacked(color);
            }
            else if (piece.get_type() == 'Q')
            {
                if (c_x == p_x and vertical_is_free(c_x, c_y, p_y)
                                   or c_y == p_y and horizontal_is_free(c_y, c_x, p_x))
                    cell.set_attacked(color);

                if (abs(c_y - p_y) == abs(c_x - p_x) and
                    diagonal_is_free(c_x, c_y, p_x, p_y))
                    cell.set_attacked(color);
            }
        }
    }

    void check_attack_all()
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                for (int k = 0; k < 32; k++)
                    if (pieces[k].is_alive())
                        check_attack(pieces[k], board[i][j]);
    }

    void check_moves(Piece const &piece, std::vector<std::vector<int>> &result)
    {
        int p_x = piece.get_x();
        int p_y = piece.get_y();
        int color = piece.get_color();

        if (piece.get_type() == 'K')
        {
            for (int i = -1; i < 2; i++)
                for (int j = -1; j < 2; j++)
                    if (i != 0 or j != 0)
                    {
                        int x1 = p_x + i, y1 = p_y + j;
                        if (not board[y1 -1][x1 - 1].is_attacked_by(not color))
                        {
                            add_moves(result, x1, y1);
                        }
                    }
        }
        //else if
    }
};

int main()
{
    std::cout << "Hello, Chess World!" << std::endl;

    //Position posi("default");
    Position posi = Position("default");
    //posi = pos;

    for (int y = 8; y > 0; y--)
    {
        for (int x = 1; x < 9; x++)
        {
            Cell cell(posi.get_cell(x, y));
            if (cell.is_empty())
                std::cout << '*';
            else
                std::cout << cell.get_piece()->get_type();
        }

        std::cout << std::endl;
    }

    for (int y = 8; y > 0; y--)
    {
        for (int x = 1; x < 9; x++)
        {
            Cell cell(posi.get_cell(x, y));
            std::cout << cell.get_color();
        }

        std::cout << std::endl;
    }
    posi.check_attack_all();
    std::cout << std::endl;
    for (int y = 8; y > 0; y--)
    {
        for (int x = 1; x < 9; x++)
        {
            Cell cell(posi.get_cell(x, y));
            if (cell.is_attacked_by_white())
            {
                if (cell.is_attacked_by_black())
                    std::cout << 'b';
                else
                    std::cout << 'W';
            }
            else if (cell.is_attacked_by_black())
                std::cout << 'B';
            else
                std::cout << 'n';
        }

        std::cout << std::endl;
    }
    return 0;
}
