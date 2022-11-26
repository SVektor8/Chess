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

public:
    Piece() : alive(true) {};

    Piece(int x, int y) : x(x), y(y), alive(true) {};

    [[nodiscard]] int get_x() const { return x; }

    [[nodiscard]] int get_y() const { return y; }

    void move(int n_x, int n_y)
    {
        x = n_x;
        y = n_y;
    }

    //TODO upgrade methods of the "the rule of 5"

    ~Piece() = default;

    Piece(Piece const &) = delete;

    Piece &operator=(Piece &rha)
    {
        x = rha.get_x();
        y = rha.get_y();

        return *this;
    }

    Piece(Piece &&other) = delete;

    Piece &operator=(Piece &&other) = delete;

};

class Cell
{
private:
    int x, y;
    char vertical;
    bool empty;
    Piece piece;
public:
    Cell(int x, int y) : x(x), y(y), vertical("nabcdefgh"[x]), empty(true) {};

    Cell(int x, int y, std::string name, bool color) :
            x(x), y(y), vertical("nabcdefgh"[x]), empty(false)
    {
        //TODO finish constructor after adding child classes
        Piece piece();
    };

    [[nodiscard]] int get_x() const { return x; }

    [[nodiscard]] int get_y() const { return y; }

    [[nodiscard]] char get_vert() const { return vertical; }

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

class Position
{
private:
    Cell board[8][8];
    Piece pieces[32];
public:
    Position()
};

int main()
{
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
