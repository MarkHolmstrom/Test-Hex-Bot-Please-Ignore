
#include "constants.h"
#include "board.h"
#include <iostream>
#include <set>
#include <vector>

using namespace std;

HexBoard::HexBoard(int color, int board_size) {
    this->color = color;
    this->opp = -1 * color;
    this->move_count = 0;
    this->init_board(board_size);
}


void HexBoard::init_board(int board_size) {
    this->board_size = board_size;
    this->board_size_2 = board_size * board_size;
    this->board = new int[this->board_size_2];
    for (int i = 0; i < this->board_size_2; i++) {
        this->board[i] = EMPTY;
    }
    this->init_neighbours();
}


void HexBoard::show_board() {
    // Prints the board to stdout. This is primarily used for
    // testing purposes & when playing against a human opponent
    for (int i = 1; i < this->board_size_2 + 1; i++) {
        cout << this->tile_chars[this->board[i - 1]];
        if (i % this->board_size == 0) {
            cout << "|";
        }
    }
    cout << endl;
}


void HexBoard::make_move() {
    // Generates the move. For this bot, the move is randomly selected from all
    // empty positions.

    vector<int> empties;
    for (int i = 0; i < this->board_size_2; i++) {
        if (this->board[i] == EMPTY) {
            empties.push_back(i);
        }
    }

    int coord = empties[rand() % empties.size()];
    this->sety(coord);

    string move = this->coord_to_move(coord);
    cout << move << endl;
}

void HexBoard::seto(string move) {
    // Tells the bot about a move for the other bot
    //
    // Args:
    //      string move: A human-readable position on which the opponent has just
    // played

    int coord = this->move_to_coord(move);
    this->board[coord] = this->opp;
    this->move_count += 1;
}

void HexBoard::sety(string move) {
    // Set Your [tile]. Tells the bot to play a move for itself
    // Args:
    //      string move: A human-readable position on the board
    int coord = this->move_to_coord(move);
    this->board[coord] = this->color;
    this->move_count += 1;
}

void HexBoard::sety(int coord) {
    // Set Your [tile]. Tells the bot to play a move for itself
    // Args:
    //      int coord: A position on the board
    this->board[coord] = this->color;
    this->move_count += 1;
}

void HexBoard::unset(string move) {
    // Tells the bot to set a tile as unused
    // Args:
    //      string move: A human-readable position on the board
    int coord = this->move_to_coord(move);
    this->board[coord] = EMPTY;
}

bool HexBoard::dfs(int i, int color, set<int>& seen) {
        // Args:
        //     int i: The current location of the depth-first search
        //     int color: The current color of the dfs.
        bool is_right_column = (i + 1) % this->board_size == 0;

        if (color == WHITE && is_right_column) {
            return true;
        }

        bool is_bottom_row = i >= this->board_size * (this->board_size - 1);
        if (color == BLACK && is_bottom_row) {
            return true;
        }

        // Label hexagon as 'visited' so we don't get infinite recursion
        seen.insert(i);
        for (int neighbour : this->neighbours[i]) {
            if (
                seen.find(neighbour) == seen.end() &&
                this->board[neighbour] == color &&
                this->dfs(neighbour, color, seen)
            ) {
                return true;
            }
        }
        seen.erase(i);
        return false;
}

void HexBoard::check_win() {
    // Checks whether or not the game has come to a close.
    // Prints:
    // int: 1 if this bot has won, -1 if the opponent has won, and 0 otherwise. Note that draws
    // are mathematically impossible in Hex.
    set<int> seen;

    // Iterate over all starting spaces for black & white, performing dfs on non-empty spaces
    for (int i = 0; i < this->board_size; i++) {
        if (this->board[i] == BLACK && dfs(i, BLACK, seen)) {
            cout << (this->color == BLACK ? 1 : -1) << endl;
            return;
        }
    }

    for (int i = 0; i < this->board_size_2; i += this->board_size) {
        if (this->board[i] == WHITE && dfs(i, WHITE, seen)) {
            cout << (this->color == WHITE ? 1 : -1) << endl;
            return;
        }
    }

    cout << 0 << endl;
}

void HexBoard::init_neighbours() {
    this->neighbours = vector<vector<int>>();
    // Precalculates all neighbours for each cell
    vector<int> offsets_normal = {-1, 1, -this->board_size, this->board_size, -this->board_size + 1, this->board_size - 1};
    vector<int> offsets_left = {1, -this->board_size, this->board_size, -this->board_size + 1};
    vector<int> offsets_right = {-1, -this->board_size, this->board_size, this->board_size - 1};

    for (int cell = 0; cell < this->board_size_2; cell++) {
        vector<int>* offsets;
        if ((cell + 1) % this->board_size == 0) {
            offsets = &offsets_right;
        } else if (cell % this->board_size == 0) {
            offsets = &offsets_left;
        } else {
            offsets = &offsets_normal;
        }

        vector<int> neighbours;
        for (int offset : *offsets) {
            if (0 <= cell + offset && cell + offset < this->board_size_2) {
                neighbours.push_back(cell + offset);
            }
        }
        this->neighbours.push_back(neighbours);
    }
}


string HexBoard::coord_to_move(int coord) {
    // Converts an integer coordinate to a human-readable move
    // Args:
    //    int coord: A coordinate within this->board
    // Returns:
    //    string: A human-readable version of coord
    char letter = coord / this->board_size + 'a';
    int number = coord % this->board_size + 1;
    string move = letter + to_string(number);
    return move;
}

int HexBoard::move_to_coord(string move) {
    // Converts a human-readable move to a coordinate within this->board
    // Args:
    //    string move: A human-readable position on the board

    // Returns:
    //    int: The integer coordinate of 'move', used to interact with the board

    int column = stoi(move.substr(1)) - 1;
    int row = move[0] - 'a';
    return row * this->board_size + column;
}

void HexBoard::swap() {
    // Performs the 'swap' move
    std::swap(this->opp, this->color);
    this->move_count++;
}


void HexBoard::flip() {
    // Reflects the board (on long axis) and swaps tiles to take advantage of hex's symmetry.

    //         board_2d = np.reshape(self.board, (self.this->board_size, self.this->board_size))
    //         self.board = np.reshape(np.transpose(board_2d), self.this->board_size ** 2) * -1
    // TODO
}
