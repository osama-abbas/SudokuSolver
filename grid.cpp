#include <iomanip>

#include "grid.hpp"

Grid::Grid() {}

Grid::Grid(string values) {
  string::iterator itr = values.begin();
  string::iterator end = values.end();
  int index = 0;
  while (itr != end) {
    switch (*itr) {
    case ' ':
      break;
    case 'X':
      grid[index++] = Cell();
      break;
    default:
      int value = *itr - '0';
      grid[index++] = Cell(value);
      this->values.insert(value);
    }
    itr++;
  }
}

void Grid::assign(Cell cell, int pos) { grid[pos] = cell; }

Cell *Grid::getCell(int pos) { return &grid[pos]; }

set<int> Grid::getValues() { return values; }

set<int> Grid::getUnassigedValues() {
  set<int> unassignedVals;
  for (int i = 1; i < 10; i++) {
    if (values.find(i) == values.end()) {
      unassignedVals.insert(i);
    }
  }

  return unassignedVals;
}

void Grid::addValue(int val) { values.insert(val); }

void Grid::removeValue(int val) { values.erase(val); }

void Grid::print() {
  for (int i = 0; i < 9; i++) {
    if (i % 3 == 0) {
      cout << endl;
    }
    grid[i].print();
  }
  cout << endl;
}