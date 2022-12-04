#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <utility>
#include <string>
#include <vector>

//TODO pieces moving (probably cycle), showing move ability (where it can move)
bool is_in_board(int x, int y)
{
    return (x > 0 and x < 9 and y > 0 and y < 9);
}

class Piece
{
protected:
    int x = 0, y = 0;
    bool alive = false, defended = false;
    bool color = true; //true = white, false = black
    char type = 'Z';

public: //TODO ASK does it stay public and the upper code protected?
    Piece() : Piece(true) {}; //TODO ASK explicit

    explicit Piece(bool color) : color(color) {};

    [[nodiscard]] int get_x() const { return x; }

    [[nodiscard]] int get_y() const { return y; }

    [[nodiscard]] char get_type() const { return type; }

    [[nodiscard]] bool get_color() const { return color; }

    [[nodiscard]] bool is_alive() const { return alive; }

    [[nodiscard]] bool is_defended() const { return defended; }

    void move(int n_x, int n_y)
    {
        x = n_x;
        y = n_y;
    }

    //TODO write destructor
    //TODO ASK if can simplify
    ~Piece() = default;

    Piece(Piece const &src)
    {
        x = src.get_x();
        y = src.get_y();
        type = src.get_type();
        alive = src.is_alive();
        color = src.get_color();
        defended = src.is_defended();
    }

    Piece(Piece &&src) noexcept
    {
        x = src.get_x();
        y = src.get_y();
        type = src.get_type();
        alive = src.is_alive();
        color = src.get_color();
        defended = src.is_defended();
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
        std::swap(this->defended, tmp.defended);

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
        std::swap(this->defended, tmp.defended);

        return *this;
    }

    virtual void move_routine() {};

    [[nodiscard]] virtual bool has_moved() const { return false; }

    void die() { alive = false; }

    void respawn() { alive = true; }

};

class King final : public Piece
{
private:
    bool moved;

public:
    explicit King(bool color) : Piece(color), moved(false) { type = 'K'; }

    [[nodiscard]] bool has_moved() const override { return moved; }
};

class Queen final : public Piece
{
public:
    explicit Queen(bool color) : Piece(color) { type = 'Q'; }

    void move_routine() override
    {
        defended = false;
    }
};

class Rook final : public Piece
{//TODO ASK why not protected?
private:
    bool moved;
public:
    explicit Rook(bool color) : Piece(color), moved(false) { type = 'R'; }

    [[nodiscard]] bool has_moved() const override { return moved; }

    void move_routine() override
    {
        defended = false;
    }
};

class Bishop final : public Piece
{
public:
    explicit Bishop(bool color) : Piece(color) { type = 'B'; }

    void move_routine() override
    {
        defended = false;
    }
};

class Knight final : public Piece
{
public:
    explicit Knight(bool color) : Piece(color) { type = 'N'; }

    void move_routine() override
    {
        defended = false;
    }
};

class Pawn final : public Piece
{
private:
    bool moved;
public:
    explicit Pawn(bool color) : Piece(color), moved(false) { type = 'p'; } //TODO ASK why cannot write x(X)

    [[nodiscard]] bool has_moved() const override { return moved; }

    void move_routine() override
    {
        defended = false;
    }
};

class Pieces_Manager final
{
private:
    std::vector<std::vector<Piece>> pieces{{},
                                           {}};
    unsigned long long size = 0;
public:
    Pieces_Manager() : Pieces_Manager("default") {};

    explicit Pieces_Manager(std::string const &mode)
    {
        if (mode == "default")
        {
            for (int color = 0; color < 2; color++)
            {
                pieces[color].push_back(King(color));
                pieces[color].push_back(Queen(color));
                pieces[color].push_back(Rook(color));
                pieces[color].push_back(Rook(color));
                pieces[color].push_back(Bishop(color));
                pieces[color].push_back(Bishop(color));
                pieces[color].push_back(Knight(color));
                pieces[color].push_back(Knight(color));
                for (int i = 0; i < 8; i++)
                {
                    pieces[color].push_back(Pawn(color));
                }
            }
        }
        else if (mode == "extended")
        {
            for (int color = 0; color < 2; color++)
            {
                pieces[color].push_back(King(color));
                pieces[color].push_back(Queen(color));
                pieces[color].push_back(Rook(color));
                pieces[color].push_back(Rook(color));
                pieces[color].push_back(Bishop(color));
                pieces[color].push_back(Bishop(color));
                pieces[color].push_back(Knight(color));
                pieces[color].push_back(Knight(color));
                for (int i = 0; i < 8; i++)
                {
                    pieces[color].push_back(Pawn(color));
                }
                for (int i = 0; i < 8; i++)
                {
                    pieces[color].push_back(Queen(color));
                    pieces[color].push_back(Rook(color));
                    pieces[color].push_back(Bishop(color));
                    pieces[color].push_back(Knight(color));
                }
            }
        }
        else
            std::cout << "no such mode" << std::endl;

        size = pieces[0].size();
    }

    //TODO destructor
    ~Pieces_Manager() = default;

    Pieces_Manager(Pieces_Manager const &src)
    {
        if (pieces[0].empty())
            for (int i = 0; i < 32; i++)
            {
                if (i < 16)
                    pieces[0].push_back(src.no_ptr_piece_number(i));
                else
                    pieces[1].push_back(src.no_ptr_piece_number(i));
            }
        else
            for (int i = 0; i < 32; i++)
                pieces[i / 16][i % 16] = src.no_ptr_piece_number(i);

        size = pieces[0].size();
    }

    Pieces_Manager(Pieces_Manager &&src) noexcept
    {
        *this = src;
    }

    Pieces_Manager &operator=(Pieces_Manager const &src)
    {
        if (this == &src) return *this;

        Pieces_Manager tmp(src);
        std::swap(this->pieces, tmp.pieces);
        std::swap(this->size, tmp.size);

        return *this;
    }

    Pieces_Manager &operator=(Pieces_Manager &&src) noexcept
    {
        if (this == &src) return *this;

        Pieces_Manager tmp(src);
        *this = tmp;

        return *this;
    }

    [[nodiscard]] Piece *king(bool color) { return &pieces[color][0]; }

    [[nodiscard]] Piece *queen(bool color) { return &pieces[color][1]; }

    [[nodiscard]] Piece *rook(bool color, int which) { return &pieces[color][1 + which]; }

    [[nodiscard]] Piece *bishop(bool color, int which) { return &pieces[color][3 + which]; }

    [[nodiscard]] Piece *knight(bool color, int which) { return &pieces[color][5 + which]; }

    [[nodiscard]] Piece *pawn(bool color, int which) { return (&pieces[color][7 + which]); }

    [[nodiscard]] Piece *extra_piece(bool color, char type, int which)
    {
        if (type == 'Q') return &pieces[color][16 + (which - 1) * 4];
        else if (type == 'R') return &pieces[color][17 + (which - 1) * 4];
        else if (type == 'B') return &pieces[color][18 + (which - 1) * 4];
        else if (type == 'N') return &pieces[color][19 + (which - 1) * 4];
    }

    [[nodiscard]] Piece *piece_number(int which) { return &pieces[which / size][which % size]; }

    [[nodiscard]] Piece no_ptr_piece_number(int which) const { return pieces[which / size][which % size]; }
};

class Cell final
{
private:
    int x, y;
    bool empty = true, black_attacked = false, white_attacked = false;
    bool color;
    Piece *piece;
public:
    Cell() : x(1), y(1), color(false), piece(nullptr) {};

    Cell(int x, int y) : x(x), y(y), color((x + y) % 2), piece(nullptr) {};

    //TODO write destructor
    ~Cell() = default;

    Cell(Cell const &src)
    {
        x = src.get_x();
        y = src.get_y();
        empty = src.is_empty();
        piece = src.get_piece();
        color = src.get_color();
        white_attacked = src.is_attacked_by_white();
        black_attacked = src.is_attacked_by_black();
    }

    Cell(Cell &&src) noexcept
    {
        x = src.get_x();
        y = src.get_y();
        empty = src.is_empty();
        piece = src.get_piece();
        color = src.get_color();
        white_attacked = src.is_attacked_by_white();
        black_attacked = src.is_attacked_by_black();
    }

    Cell &operator=(Cell const &src)
    {
        if (this == &src) return *this;

        Cell tmp(src);
        std::swap(this->x, tmp.x);
        std::swap(this->y, tmp.y);
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

    [[nodiscard]] Piece *get_piece() const { return piece; }

    [[nodiscard]] bool is_empty() const { return empty; }

    [[nodiscard]] bool is_attacked_by_white() const { return white_attacked; }

    [[nodiscard]] bool is_attacked_by_black() const { return black_attacked; }

    [[nodiscard]] bool can_be_taken(bool piece_color) const { return piece->get_color() != piece_color; }

    [[nodiscard]] bool is_attacked_by(bool piece_color) const
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
        piece->respawn();
    }

    void unemploy()
    {
        empty = true;
        piece->die();
    }
};

class Position final
{
private:
    Cell board[8][8];
    Pieces_Manager pieces = Pieces_Manager("default");
    bool turn = true; // true = white, false = black
public:

    explicit Position(const std::string &mode)
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
            {
                board[i][j] = Cell(j + 1, i + 1);

                if (mode == "default")
                {
                    if (i == 1)
                        board[i][j].employ(pieces.pawn(true, j + 1));
                    else if (i == 6)
                        board[i][j].employ(pieces.pawn(false, j + 1));
                    else if (i == 0 or i == 7)
                    {
                        if (j == 0 or j == 7)
                            board[i][j].employ(pieces.rook(i == 0, j == 0 ? 1 : 2));
                        else if (j == 1 or j == 6)
                            board[i][j].employ(pieces.knight(i == 0, j == 1 ? 1 : 2));
                        else if (j == 2 or j == 5)
                            board[i][j].employ(pieces.bishop(i == 0, j == 2 ? 1 : 2));
                        else if (j == 3)
                            board[i][j].employ(pieces.queen(i == 0));
                        else if (j == 4)
                            board[i][j].employ(pieces.king(i == 0));
                    }
                }
                else if (mode == "Q_custom")
                {
                    //q_custom(pieces.rook(false, 1), i, j, 4, 4);
                    //q_custom(pieces.pawn(false, 1), i, j, 4, 7);
                    q_custom(pieces.king(false), i, j, 8, 7);
                    //q_custom(pieces.king(false), i, j, 2, 8);
                    //q_custom(pieces.queen(false), i, j, 4, 4);
                    //q_custom(pieces.knight(false, 1), i, j, 4, 4);
                    //q_custom(pieces.bishop(false, 1), i, j, 4, 4);
                }
                else
                {
                    std::cout << "no such mode" << std::endl;
                }

            }
    }

    //TODO write destructor
    ~Position() = default;

    Position(Position const &src)
    {
        /*TODO
         * Maybe here wil be a problem, because when copying seems, that
         * board of the new Position object will have pointers on the
         * pieces of the old position objects, as the Pieces object is
         * being copied separately. When changed, look at check_move_legality.*/
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                board[i][j] = src.get_cell(j + 1, i + 1);
        pieces = src.get_pieces();
    }

    Position(Position &&src) noexcept
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                board[i][j] = src.get_cell(j + 1, i + 1);
        pieces = src.get_pieces();
    }

    Position &operator=(Position const &src)
    {
        if (this == &src) return *this;

        Position tmp(src);
        std::swap(this->board, tmp.board);
        std::swap(this->pieces, tmp.pieces);

        return *this;
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

    [[nodiscard]] Pieces_Manager get_pieces() const { return pieces; }

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

    bool check_check(bool color)
    {
        int x = pieces.king(color)->get_x();
        int y = pieces.king(color)->get_y();

        for (int k = 0; k < 32; k++)
            if (pieces.piece_number(k)->is_alive())
                check_attack(*pieces.piece_number(k), board[y - 1][x - 1]);

        if (board[y - 1][x - 1].is_attacked_by(not color))
            return true;
        else
            return false;
    }

    bool check_move_legality(int x0, int y0, int x1, int y1)
    {
        //TODO in case of problems check Position(Position const &src)
        bool result;
        bool color = this->get_cell(x0, y0).get_piece()->get_color();

        this->move_piece(board[y0 - 1][x0 - 1], board[y1 - 1][x1 - 1]);
        result = (this->check_check(color));
        this->move_piece(board[y1 - 1][x1 - 1], board[y0 - 1][x0 - 1]);

        return result;
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
        for (auto &i: board)
            for (auto &j: i)
            {
                j.set_unattacked();
                for (int k = 0; k < 32; k++)
                    if (pieces.piece_number(k)->is_alive())
                        check_attack(*pieces.piece_number(k), j);
            }
    }

    void check_moves(Piece const &piece, std::vector<std::vector<int>> &result)
    {//TODO add checks, bounds, move en passant, taking other pieces, etc. ...
        int p_x = piece.get_x();
        int p_y = piece.get_y();
        int color = piece.get_color();
        char type = piece.get_type();
        std::vector<std::vector<int>> vars;
        //TODO add castling
        if (not piece.is_alive())
            return;
        if (type == 'p')
        {
            if (color)
            {
                if (not piece.has_moved())
                {
                    int x1 = p_x, y1 = p_y + 2;
                    if (board[y1 - 1][x1 - 1].is_empty())

                        add_moves(result, p_x, p_y, x1, y1);
                }
                {
                    int x1 = p_x, y1 = p_y + 1;
                    if (board[y1 - 1][x1 - 1].is_empty())
                        add_moves(result, p_x, p_y, x1, y1);
                }
                for (int i = -1; i < 2; i += 2)
                {
                    int x1 = p_x + 1, y1 = p_y + 1;
                    if ((not board[y1 - 1][x1 - 1].is_empty()) &&
                        board[y1 - 1][x1 - 1].can_be_taken(color))
                        add_moves(result, p_x, p_y, x1, y1);
                }
            }
            else
            {
                if (not piece.has_moved())
                {
                    int x1 = p_x, y1 = p_y - 2;
                    if (board[y1 - 1][x1 - 1].is_empty())
                        add_moves(result, p_x, p_y, x1, y1);
                }
                {
                    int x1 = p_x, y1 = p_y - 1;
                    if (board[y1 - 1][x1 - 1].is_empty())
                        add_moves(result, p_x, p_y, x1, y1);
                }
                for (int i = -1; i < 2; i += 2)
                {
                    int x1 = p_x + i, y1 = p_y - 1;
                    if ((not board[y1 - 1][x1 - 1].is_empty()) &&
                        board[y1 - 1][x1 - 1].can_be_taken(color))
                        add_moves(result, p_x, p_y, x1, y1);
                }
            }
        }
        else
        {
            for (int x = 1; x < 9; x++)
                for (int y = 1; y < 9; y++)
                {//TODO rewrite both this and check_attack
                    bool black = board[y - 1][x - 1].is_attacked_by_black();
                    bool white = board[y - 1][x - 1].is_attacked_by_white();
                    board[y - 1][x - 1].set_unattacked();
                    check_attack(piece, board[y - 1][x - 1]);
                    if (board[y - 1][x - 1].is_attacked_by(color))
                    {
                        std::vector tmp{x, y};
                        vars.push_back(tmp);
                    }
                    board[y - 1][x - 1].set_unattacked();
                    if (black)
                        board[y - 1][x - 1].set_attacked(false);
                    if (white)
                        board[y - 1][x - 1].set_attacked(true);
                }

            for (auto &var: vars)
            {
                add_moves(result, p_x, p_y, var[0], var[1]);
            }
        }
        //Fixme try set for different pieces different vars and then for all try it in cycle
    }

    void add_moves(std::vector<std::vector<int>> &vector, int p_x, int p_y, int x, int y)
    {
        if (is_in_board(x, y) &&
            (board[y - 1][x - 1].is_empty() ||
             board[y - 1][x - 1].can_be_taken(board[p_y - 1][p_x - 1].get_piece()->get_color())) &&
            not(check_move_legality(p_x, p_y, x, y)))
        {
            std::vector<int> tmp(2);
            tmp[0] = x;
            tmp[1] = y;
            vector.push_back(tmp);
        }
    }

    void move_piece(Cell &start, Cell &finish)
    {
        Piece *piece = start.get_piece();
        start.unemploy();
        finish.employ(piece);
    }

    void q_custom(Piece *piece, int &i, int &j, int p_x, int p_y)
    {
        if (i == p_y - 1 and j == p_x - 1)
            board[i][j].employ(piece);
    }
};

class GUI final
{
private:
    const float cell_side = 80, left_top_x = 40, left_top_y = 40;
    std::string pieces_style = "merida";
    float sc_width = 1080, sc_height = 720;
    sf::Color table_c = sf::Color(0xC3, 0x58, 0x31),
            white = sf::Color(0xE6, 0xD6, 0x90),
            black = sf::Color(0xA5, 0x20, 0x19);

    Position *position = nullptr;
    std::vector<std::vector<sf::Texture>> textures = std::vector<std::vector<sf::Texture>>(2, std::vector<sf::Texture>(16, sf::Texture()));
    std::vector<std::vector<sf::Sprite>> pieces{{},
                                                {}};
    std::vector<std::vector<sf::RectangleShape>> board;
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(sc_width, sc_height), "Chess");
    sf::RectangleShape screen;
public:
    GUI(Position *position) : position(position)
    {
        init_start();

        update();

    }

    void init_start()
    {
        screen.setSize({sc_width, sc_height});
        screen.setPosition(0, 0);
        screen.setFillColor(table_c);
        for (int i = 0; i < 8; i++)
        {
            std::vector<sf::RectangleShape> tmp;
            for (int j = 0; j < 8; j++)
            {
                sf::RectangleShape shape;
                shape.setSize({cell_side, cell_side});
                shape.setPosition(left_top_x + cell_side * i, left_top_y + cell_side * j);
                shape.setFillColor((i + j) % 2 == 0 ? white : black);
                tmp.push_back(shape);
            }
            board.push_back(tmp);
        }

        init_sprites();
    }

    void init_sprites()
    {//FIXME path
        std::string path = "C:/Users/Viktor/Documents/Chess/pics/Pieces/" + pieces_style + "/";
        std::string letters[16] = {"K", "Q", "R", "R", "B", "B", "N", "N",
                                   "P", "P", "P", "P", "P", "P", "P", "P"};

        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 16; j++)
            {
                std::string color = (i == 0 ? "b" : "w");
                sf::Texture txr;
                txr.loadFromFile(path + color + letters[j] + ".png");
                textures[i][j] = txr;
                sf::Sprite spr;
                spr.setTexture(textures[i][j]);

                pieces[i].push_back(spr);
            }

        for (int i = 0; i < 32; i++)
            if (position->get_pieces().piece_number(i)->is_alive())
            {
                int x = position->get_pieces().piece_number(i)->get_x();
                int y = position->get_pieces().piece_number(i)->get_y();
                float X = left_top_x + (x - 1) * cell_side;
                float Y = left_top_y + (8 - y) * cell_side;
                pieces[i / 16][i % 16].setPosition(X, Y);
            }
    }

    void update()
    {
        while (window.isOpen())
        {
            check_events();

            window.clear();
            draw();
            window.display();
        }
    }

    void check_events()
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    void draw()
    {
        window.draw(screen);
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                window.draw(board[i][j]);
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 16; j++)
                window.draw(pieces[i][j]);
    }

    void print_board(Position &position)
    {
        for (int y = 8; y > 0; y--)
        {
            for (int x = 1; x < 9; x++)
            {
                Cell cell(position.get_cell(x, y));
                if (cell.is_empty())
                    std::cout << '*';
                else
                    std::cout << cell.get_piece()->get_type();
            }

            std::cout << std::endl;
        }
    }

    void print_attacks(Position &position)
    {
        for (int y = 8; y > 0; y--)
        {
            for (int x = 1; x < 9; x++)
            {
                Cell cell(position.get_cell(x, y));
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
    }

    void print_piece_moves(Position &position, Piece &piece)
    {
        std::vector<std::vector<int>> res{};

        position.check_moves(piece, res);

        for (auto &re: res)
        {
            std::cout << re[0] << ' ' << re[1] << std::endl;
        }

        if (res.empty())
            std::cout << "NULL";

        std::cout << std::endl;
    }
};

class Game_Manager final
{
private:
    Position position;
    GUI interface;
public:

    Game_Manager(std::string const position_mode) : position(Position(position_mode)), interface(&position) {};

    void print_board() { interface.print_board(position); }

    void print_attacks()
    {
        position.check_attack_all();
        interface.print_attacks(position);
    }

    void print_piece_moves(int number)
    {
        interface.print_piece_moves(position, *position.get_pieces().piece_number(number));
    }
};

int main()
{
    std::cout << "Hello, Chess World!" << std::endl;

    Game_Manager GM("default");

    GM.print_board();
    GM.print_attacks();
    GM.print_piece_moves(0); //Black -> White;KQRRBBNNpppppppp

    return 0;
}