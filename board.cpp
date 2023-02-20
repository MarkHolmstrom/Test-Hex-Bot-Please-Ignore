
#include "constants.h"
#include "board.h"
#include "unionfind.h"
#include <iostream>
#include <vector>

using namespace std;

HexBoard::HexBoard(int color, int board_size) {
    this->color = color;
    this->opp = -1 * color;
    this->current = BLACK;
    this->move_count = 0;
    this->history = stack<int>();
    this->init_board(board_size);
}


void HexBoard::init_board(int board_size) {
    this->board_size = board_size;
    this->board_size_2 = board_size * board_size;
    this->board = vector<int>(this->board_size_2);
    this->empties = unordered_set<int>();
    this->uf = UnionFind();
    for (int i = 0; i < this->board_size_2; i++) {
        this->board[i] = EMPTY;
        this->empties.insert(i);
        this->uf.insert(i);
    }
    this->uf.insert(LEFT);
    this->uf.insert(RIGHT);
    this->uf.insert(TOP);
    this->uf.insert(BOTTOM);
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

void HexBoard::seto(const string& move) {
    // Tells the bot about a move for the other bot
    //
    // Args:
    //      string move: A human-readable position on which the opponent has just
    // played
    int coord = this->move_to_coord(move);
    this->play_a(coord, this->opp);
}

void HexBoard::sety(const string& move) {
    // Set Your [tile]. Tells the bot to play a move for itself
    // Args:
    //      string move: A human-readable position on the board
    int coord = this->move_to_coord(move);
    this->play_a(coord, this->color);
}

void HexBoard::play_a(int coord, int color) {
    this->board[coord] = color;
    this->empties.erase(coord);
    for (int neighbour : this->neighbours[coord]) {
        if (neighbour < 0 || this->board[neighbour] == color) {
            this->uf.unionize(coord, neighbour);
        }
    }
    this->current = -1 * color;
    this->move_count++;
}

void HexBoard::unset(const string& move) {
    // Tells the bot to set a tile as unused
    // Args:
    //      string move: A human-readable position on the board
    int coord = this->move_to_coord(move);
    this->board[coord] = EMPTY;
}

void HexBoard::print_win() {
    // Checks whether or not the game has come to a close.
    // Prints:
    // int: 1 if this bot has won, -1 if the opponent has won, and 0 otherwise.
    // Note that draws are mathematically impossible in Hex.
    int winner = this->check_win();

    if (winner == 0) {
        cout << 0 << endl;
    } else {
        cout << (this->color == winner ? 1 : -1) << endl;
    }
}

int HexBoard::check_win() {
    // Checks whether or not the game has come to a close.
    // Returns:
    // int: EMPTY if the game is not over, else the winner, BLACK or WHITE.
    if (this->move_count < this->board_size) {
        return EMPTY;
    }
    if (this->uf.find(TOP) == this->uf.find(BOTTOM)) {
        return BLACK;
    }
    if (this->uf.find(LEFT) == this->uf.find(RIGHT)) {
        return WHITE;
    }
    return EMPTY;
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
            int neighbour = cell + offset;
            if (0 <= neighbour && neighbour < this->board_size_2) {
                neighbours.push_back(neighbour);
            }
        }
        if (cell < this->board_size) {
            neighbours.push_back(TOP);
        } else if (cell >= this->board_size_2 - this->board_size) {
            neighbours.push_back(BOTTOM);
        }
        if (cell % this->board_size == 0) {
            neighbours.push_back(LEFT);
        } else if ((cell + 1) % this->board_size == 0) {
            neighbours.push_back(RIGHT);
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

int HexBoard::move_to_coord(const string& move) {
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

bool HexBoard::is_legal(int coord) const {
    // Checks whether or not a move is legal
    // Args:
    //     int coord: A coordinate within this->board
    // Returns:
    //     bool: True if the move is legal, false otherwise
    return this->board[coord] == EMPTY;
}


void HexBoard::empty_history() {
    while (!this->history.empty()) {
        this->history.pop();
    }
    this->uf.empty_history();
}

void HexBoard::undo_history() {
    while (!this->history.empty()) {
        int move = this->history.top();
        this->history.pop();
        this->empties.insert(move);
        this->board[move] = EMPTY;
        this->move_count--;
        this->current = -1 * color;
    }
    this->uf.undo_history();
}
