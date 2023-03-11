#include <fstream>

#include "board.hpp"

int main() {
  Board board;

  // string grids[] = {
  //     "4 1 X 8 X X 9 X X", "X 8 6 1 9 3 X 2 4", "2 9 3 X X 4 1 X X",
  //     "X X X 5 X X X X X", "9 5 8 X 4 X X 6 X", "X 2 X X X X X X 5",
  //     "7 X X X X 4 1 X X", "4 X 9 8 X 5 6 7 2", "X X 2 X X 7 3 4 X"};

  // for (int i = 0; i < 9; i++) {
  //   board.setGrid(i, Grid(grids[i]));
  // }

  // board.computePossibleValues();

  string grids2[] = {
      "X X 4 X X 5 X X X", "X 6 X 8 X X X X X", "9 X 3 X X X X 4 X",
      "X X 9 X X X X 6 X", "7 X X X X 3 X X X", "X X X X X X X 8 4",
      "X 2 X X X 6 X 8 X", "X X 4 X X X X 9 7", "3 X X 7 2 X X X X"};

  for (int i = 0; i < 9; i++) {
    board.setGrid(i, Grid(grids2[i]));
  }

  board.computePossibleValues();
}