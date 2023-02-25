#include <fstream>

#include "board.hpp"

int main() {
  Board board;

  string grids[] = {
      "4 1 X 8 X X 9 X X", "X 8 6 1 9 3 X 2 4", "2 9 3 X X 4 1 X X",
      "X X X 5 X X X X X", "9 5 8 X 4 X X 6 X", "X 2 X X X X X X 5",
      "7 X X X X 4 1 X X", "4 X 9 8 X 5 6 7 2", "X X 2 X X 7 3 4 X"};

  // Cell cell;
  // cell.addPossibleValue(5);
  // cell.addPossibleValue(7);
  // cell.addPossibleValue(3);

  for (int i = 0; i < 9; i++) {
    board.setGrid(i, Grid(grids[i]));
  }

  // set<int> vals({5, 7});
  // cout << "******************************" << endl;
  // board.getGrid(0).getCell(2).setPossibleValues(vals);
  // board.getGrid(0).getCell(2).printPossibleValues();

  // board.print();

  board.computePossibleValues();

  // board.print();
}