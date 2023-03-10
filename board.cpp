#include <iostream>
#include <limits>

#include "board.hpp"

using namespace std;

Board::Board() { minPossibleValues = numeric_limits<int>::max(); }

void Board::setGrid(int gridNum, Grid grid) { grids[gridNum] = grid; }

void Board::computePossibleValues() {
  // Setup rows and columns
  for (int i = 0; i < 9; i++) {
    vector<Cell *> row = getRow(i);
    vector<Cell *> col = getCol(i);
  }

  cout << "Initial board:" << endl;
  print();

  int numUnassignedCells = countUnassignedCells();
  int prevCount = numUnassignedCells;
  
  bool noChange = false;
  int c = 0;
  while (numUnassignedCells > 0) {
    print();
    bool startOver = false;
    for (int g = 0; g < 9; g++) {
      if (startOver) {
        clearPossibleValues();
        g = 0;
        startOver = false;
        minPossibleValueCell = nullptr;
        minPossibleValues = numeric_limits<int>::max();
        print();
      }
      Grid *grid = &grids[g];
      for (int c = 0; c < 9; c++) {
        Cell *cell = grid->getCell(c);
        if (!cell->isLocationSet()) {
          cell->setLocation(findLocation(cell));
        }
        int row = getRowNum(g, c);
        int col = getColNum(g, c);
        
        if (!cell->isAssigned()) {
          cout << "Processing cell (" << row << ", " << col << ")" << endl;
          set<int> unassignedValues = findCommonUnassignedValues(
              grid->getUnassigedValues(), getUnassignedRowValues(row), getUnassignedColValues(col));
          cout << "Unassigned values: ";
          printSet(unassignedValues);
          int singleUnassigedValue = 0;
          if (unassignedValues.size() == 0) {
            print();
            Cell *possibleValCell = possibleValueCellStack.top();
            gotoNextAssignedPossibleValueCell(possibleValCell);
            possibleValCell = possibleValueCellStack.top();

            /*
            If we've reached the last possible value for this cell,
            we want to remove it from the possibleValueCellStack 
            */

            bool incrementedPossibleVal = incrementCurrentPossibleCell(possibleValCell);
            while (!possibleValueCellStack.empty() && !incrementedPossibleVal) {
              possibleValueCellStack.pop();
              possibleValCell = possibleValueCellStack.top();
              gotoNextAssignedPossibleValueCell(possibleValCell);
              possibleValCell = possibleValueCellStack.top();
              incrementedPossibleVal = incrementCurrentPossibleCell(possibleValCell);
            }

            startOver = true;
            break;
          } else if (unassignedValues.size() == 1) {
            singleUnassigedValue = *unassignedValues.begin();
            cell->assign(singleUnassigedValue, false);
            possibleValueCellStack.push(cell);
            cout << "possibleValueCellStack size: " << possibleValueCellStack.size() << endl;
            cout << "(" << row << ", " << col << ") -> "<< singleUnassigedValue << endl;

            addCellValueToContainers(cell);

            startOver = true;
            break;
          } else {
            for (set<int>::iterator itr = unassignedValues.begin();
                 itr != unassignedValues.end(); itr++) {
              cell->addPossibleValue(*itr);
            }

            cout << "(" << row << ", " << col << ") possible values: ";
            cell->printPossibleValues();
            int numPossibleValues = cell->getPossibleValues().size();
            if (numPossibleValues < minPossibleValues) {
              minPossibleValues = numPossibleValues; 
              minPossibleValueCell = cell;
            }
          }
        }
      }
    }

    numUnassignedCells = countUnassignedCells();
    cout << "Num unassigned cells: " << numUnassignedCells << endl;
    if (numUnassignedCells == 0) {
      cout << "BOARD SOLVED!!!" << endl;
      print();
      exit(0);
    }
    if (prevCount == numUnassignedCells) {
      noChange = true;
      break;
    } else {
      prevCount = numUnassignedCells;
    }
  }

  if (noChange) {
    cout << "Unable to assign any more cells - assigning possible values..."
         << endl;
    assignPossibleValues();
  }
}

bool Board::incrementCurrentPossibleCell(Cell* possibleValCell) {
  removeCellValueFromContainers(possibleValCell);
  if (possibleValCell->incrementCurrentPossibleValue()) {
    addCellValueToContainers(possibleValCell);
    return true;
  }

  return false;
}

void Board::gotoNextAssignedPossibleValueCell(Cell* possibleValCell) {
  bool assignedPossibleVal = possibleValCell->isAssignedPossibleValue();
  while (!assignedPossibleVal) {
    possibleValueCellStack.pop();
    cout << "possibleValueCellStack size: " << possibleValueCellStack.size() << endl;
    removeCellValueFromContainers(possibleValCell);

    if (possibleValueCellStack.size() == 1) {
      // Were at the very bottom of the cell stack.
      // So we need to mark the current value as an impossible value.
      possibleValCell->addImpossibleValue(possibleValCell->getCurrentPossibleValue());
      assignedPossibleVal = true;
    } else if (!possibleValueCellStack.empty()) {
      possibleValCell = possibleValueCellStack.top();
      assignedPossibleVal = possibleValCell->isAssignedPossibleValue();
    } else {
      cout << "possibleValueCellStack is empty - WE SHOULD NEVER COME IN HERE!!!" << endl;
      exit(0);
      break;
    }
  }
}

void Board::clearPossibleValues() {
  for (int g = 0; g < 9; g++) {
    for (int c = 0; c < 9; c++) {
      Cell* cell = grids[g].getCell(c);
      if (!cell->isAssigned()) {
        cell->clearPossibleValues();
      }
    }
  }
}

void Board::addCellValueToContainers(Cell *cell) {
  int cellValue = cell->getValue();
  CellLocation loc = cell->getLocation();
  grids[loc.grid].addValue(cellValue);
  rowValues.find(loc.row)->second.insert(cellValue);
  colValues.find(loc.col)->second.insert(cellValue);
}

void Board::removeCellValueFromContainers(Cell *cell) {
  int cellValue = cell->getValue();
  CellLocation loc = cell->getLocation();
  grids[loc.grid].removeValue(cellValue);
  rowValues.find(loc.row)->second.erase(cellValue);
  colValues.find(loc.col)->second.erase(cellValue);
  cell->unassign();
  cout << "(" << loc.row << ", " << loc.col << ") unassigned" << endl;
}

void Board::assignPossibleValues() {
  cout << "Entering Board::assignPossibleValues()" << endl;
  int possibleVal = minPossibleValueCell->getCurrentPossibleValue();
  cout << "Line 193" << endl;
  CellLocation loc = minPossibleValueCell->getLocation();
  cout << "Line 195" << endl;
  grids[loc.grid].addValue(possibleVal);
  cout << "Line 196" << endl;
  rowValues.find(loc.row)->second.insert(possibleVal);
  cout << "Line 197" << endl;
  colValues.find(loc.col)->second.insert(possibleVal);
  cout << "Line 199" << endl;
  possibleValueCellStack.push(minPossibleValueCell);
  minPossibleValueCell->assignCurrentPossibleValue();
  cout << "Line 202" << endl;
  cout << "(" << loc.row << ", " << loc.col << ") assigned possible value " << possibleVal << endl;
  print();

  minPossibleValues = numeric_limits<int>::max();
  computePossibleValues();
}

void Board::printSet(set<int> s) {
  for (set<int>::iterator itr = s.begin(); itr != s.end(); itr++) {
    cout << *itr << " ";
  }
  cout << endl;
}

int Board::getRowNum(int gridNum, int cellNum) {
  if (gridNum == 0 || gridNum == 1 || gridNum == 2) {
    if (cellNum == 0 || cellNum == 1 || cellNum == 2) {
      return 0;
    } else if (cellNum == 3 || cellNum == 4 || cellNum == 5) {
      return 1;
    } else if (cellNum == 6 || cellNum == 7 || cellNum == 8) {
      return 2;
    }
  } else if (gridNum == 3 || gridNum == 4 || gridNum == 5) {
    if (cellNum == 0 || cellNum == 1 || cellNum == 2) {
      return 3;
    } else if (cellNum == 3 || cellNum == 4 || cellNum == 5) {
      return 4;
    } else if (cellNum == 6 || cellNum == 7 || cellNum == 8) {
      return 5;
    }
  } else if (gridNum == 6 || gridNum == 7 || gridNum == 8) {
    if (cellNum == 0 || cellNum == 1 || cellNum == 2) {
      return 6;
    } else if (cellNum == 3 || cellNum == 4 || cellNum == 5) {
      return 7;
    } else if (cellNum == 6 || cellNum == 7 || cellNum == 8) {
      return 8;
    }
  }

  return -1;
}

int Board::getColNum(int gridNum, int cellNum) {
  if (gridNum == 0 || gridNum == 3 || gridNum == 6) {
    if (cellNum == 0 || cellNum == 3 || cellNum == 6) {
      return 0;
    } else if (cellNum == 1 || cellNum == 4 || cellNum == 7) {
      return 1;
    } else if (cellNum == 2 || cellNum == 5 || cellNum == 8) {
      return 2;
    }
  } else if (gridNum == 1 || gridNum == 4 || gridNum == 7) {
    if (cellNum == 0 || cellNum == 3 || cellNum == 6) {
      return 3;
    } else if (cellNum == 1 || cellNum == 4 || cellNum == 7) {
      return 4;
    } else if (cellNum == 2 || cellNum == 5 || cellNum == 8) {
      return 5;
    }
  } else if (gridNum == 2 || gridNum == 5 || gridNum == 8) {
    if (cellNum == 0 || cellNum == 3 || cellNum == 6) {
      return 6;
    } else if (cellNum == 1 || cellNum == 4 || cellNum == 7) {
      return 7;
    } else if (cellNum == 2 || cellNum == 5 || cellNum == 8) {
      return 8;
    }
  }

  return -1;
}

set<int> Board::getUnassignedRowValues(int row) {
  set<int> unassignedValues;

  set<int> values = rowValues.find(row)->second;
  for (int i = 1; i < 10; i++) {
    if (values.find(i) == values.end()) {
      unassignedValues.insert(i);
    }
  }

  return unassignedValues;
}

set<int> Board::getUnassignedColValues(int col) {
  set<int> unassignedValues;

  set<int> values = colValues.find(col)->second;
  // cout << "  Col values: ";
  // printSet(values);
  for (int i = 1; i < 10; i++) {
    if (values.find(i) == values.end()) {
      unassignedValues.insert(i);
    }
  }

  return unassignedValues;
}

int Board::countUnassignedCells() {
  int numUnassigned = 0;
  for (int g = 0; g < 9; g++) {
    Grid grid = grids[g];
    numUnassigned += grid.getUnassigedValues().size();
  }

  return numUnassigned;
}

void Board::print() {
  if (doPrint) {
    // 1 2 3 | 4 5 6 | 7 8 9 |
    // 1 2 3 | 4 5 6 | 7 8 9 |
    // 1 2 3 | 4 5 6 | 7 8 9 |
    // -----------------------
    cout << "-------------------------" << endl;
    for (int i = 0; i < 9; i++) {
      vector<Cell *> cells = getRow(i);
      for (int c = 0; c < 9; c++) {
        if (c == 0)
          cout << "| ";
        if (cells[c]->isAssigned())
          cout << cells[c]->getValue() << " ";
        else
          cout << "X ";
  
        if (c % 3 == 2)
          cout << "| ";
      }
      cout << endl;
      if (i % 3 == 2)
        cout << "-------------------------" << endl;
    }
  }
}

vector<Cell *> Board::getRow(int row) {
  map<int, vector<Cell *>>::iterator itr = rows.find(row);
  if (itr != rows.end()) {
    return itr->second;
  }

  vector<Cell *> rowCells(9);

  Grid *grid1;
  Grid *grid2;
  Grid *grid3;

  switch (row) {
  case 0:
  case 1:
  case 2:
    grid1 = &grids[0];
    grid2 = &grids[1];
    grid3 = &grids[2];
    break;
  case 3:
  case 4:
  case 5:
    grid1 = &grids[3];
    grid2 = &grids[4];
    grid3 = &grids[5];
    break;
  case 6:
  case 7:
  case 8:
    grid1 = &grids[6];
    grid2 = &grids[7];
    grid3 = &grids[8];
    break;
  default:
    break;
  }

  switch (row) {
  case 0:
  case 3:
  case 6:
    rowCells[0] = grid1->getCell(0);
    rowCells[1] = grid1->getCell(1);
    rowCells[2] = grid1->getCell(2);
    rowCells[3] = grid2->getCell(0);
    rowCells[4] = grid2->getCell(1);
    rowCells[5] = grid2->getCell(2);
    rowCells[6] = grid3->getCell(0);
    rowCells[7] = grid3->getCell(1);
    rowCells[8] = grid3->getCell(2);
    break;
  case 1:
  case 4:
  case 7:
    rowCells[0] = grid1->getCell(3);
    rowCells[1] = grid1->getCell(4);
    rowCells[2] = grid1->getCell(5);
    rowCells[3] = grid2->getCell(3);
    rowCells[4] = grid2->getCell(4);
    rowCells[5] = grid2->getCell(5);
    rowCells[6] = grid3->getCell(3);
    rowCells[7] = grid3->getCell(4);
    rowCells[8] = grid3->getCell(5);
    break;
  case 2:
  case 5:
  case 8:
    rowCells[0] = grid1->getCell(6);
    rowCells[1] = grid1->getCell(7);
    rowCells[2] = grid1->getCell(8);
    rowCells[3] = grid2->getCell(6);
    rowCells[4] = grid2->getCell(7);
    rowCells[5] = grid2->getCell(8);
    rowCells[6] = grid3->getCell(6);
    rowCells[7] = grid3->getCell(7);
    rowCells[8] = grid3->getCell(8);
    break;
  }

  rows[row] = rowCells;

  set<int> vals;
  for (int i = 0; i < 9; i++) {
    Cell *cell = rowCells[i];
    if (cell->isAssigned()) {
      vals.insert(cell->getValue());
    }
  }

  rowValues.insert(pair<int, set<int>>(row, vals));

  return rowCells;
}

vector<Cell *> Board::getCol(int col) {
  map<int, vector<Cell *>>::iterator itr = cols.find(col);
  if (itr != cols.end()) {
    return itr->second;
  }

  vector<Cell *> colCells(9);

  Grid *grid1;
  Grid *grid2;
  Grid *grid3;

  switch (col) {
  case 0:
  case 1:
  case 2:
    grid1 = &grids[0];
    grid2 = &grids[3];
    grid3 = &grids[6];
    break;
  case 3:
  case 4:
  case 5:
    grid1 = &grids[1];
    grid2 = &grids[4];
    grid3 = &grids[7];
    break;
  case 6:
  case 7:
  case 8:
    grid1 = &grids[2];
    grid2 = &grids[5];
    grid3 = &grids[8];
    break;
  default:
    break;
  }

  switch (col) {
  case 0:
  case 3:
  case 6:
    colCells[0] = grid1->getCell(0);
    colCells[1] = grid1->getCell(3);
    colCells[2] = grid1->getCell(6);
    colCells[3] = grid2->getCell(0);
    colCells[4] = grid2->getCell(3);
    colCells[5] = grid2->getCell(6);
    colCells[6] = grid3->getCell(0);
    colCells[7] = grid3->getCell(3);
    colCells[8] = grid3->getCell(6);
    break;
  case 1:
  case 4:
  case 7:
    colCells[0] = grid1->getCell(1);
    colCells[1] = grid1->getCell(4);
    colCells[2] = grid1->getCell(7);
    colCells[3] = grid2->getCell(1);
    colCells[4] = grid2->getCell(4);
    colCells[5] = grid2->getCell(7);
    colCells[6] = grid3->getCell(1);
    colCells[7] = grid3->getCell(4);
    colCells[8] = grid3->getCell(7);
    break;
  case 2:
  case 5:
  case 8:
    colCells[0] = grid1->getCell(2);
    colCells[1] = grid1->getCell(5);
    colCells[2] = grid1->getCell(8);
    colCells[3] = grid2->getCell(2);
    colCells[4] = grid2->getCell(5);
    colCells[5] = grid2->getCell(8);
    colCells[6] = grid3->getCell(2);
    colCells[7] = grid3->getCell(5);
    colCells[8] = grid3->getCell(8);
    break;
  }

  cols[col] = colCells;

  set<int> vals;
  for (int i = 0; i < 9; i++) {
    Cell *cell = colCells[i];
    if (cell->isAssigned()) {
      vals.insert(cell->getValue());
    }
  }
  colValues.insert(pair<int, set<int>>(col, vals));

  return colCells;
}

Grid *Board::getGrid(int gridNum) { return &grids[gridNum]; }

void Board::printCellSet(set<Cell *> cells) {
  set<Cell *>::iterator itr = cells.begin();
  while (itr != cells.end()) {
    (*itr)->print();
    cout << " ";
    itr++;
  }
  cout << endl;
}

void Board::printCellVec(vector<Cell *> cells) {
  vector<Cell *>::iterator itr = cells.begin();
  while (itr != cells.end()) {
    (*itr)->print();
    cout << " ";
    itr++;
  }
  cout << endl;
}

void Board::printMap(map<int, vector<Cell *>> myMap) {
  map<int, vector<Cell *>>::iterator itr = myMap.begin();
  while (itr != myMap.end()) {
    int key = itr->first;
    vector<Cell *> vec = itr->second;
    cout << "key: " << key << endl;
    cout << "values: ";
    printCellVec(vec);
    itr++;
  }
}

bool Board::rowContains(int row, int value) {
  set<int>::iterator itr = rowValues[row].find(value);
  set<int>::iterator itrEnd = rowValues[row].end();
  return itr != rowValues[row].end();
}

bool Board::colContains(int col, int value) {
  set<int>::iterator itr = colValues[col].find(value);
  return itr != colValues[col].end();
}

bool Board::gridContains(int grid, int value) {
  set<int> gridValues = grids[grid].getValues();
  set<int>::iterator itr = gridValues.find(value);
  return itr != gridValues.end();
}

set<int> Board::findCommonUnassignedValues(set<int> set1, set<int> set2,
                                                set<int> set3) {
  // cout << "Grid unassigned values: ";
  // printSet(set1);
  // cout << "Row unassigned values: ";
  // printSet(set2);
  // cout << "Col unassigned values: ";
  // printSet(set3);
  set<int> unassignedValues;
  for (int i = 1; i < 10; i++) {
    bool foundInGridValues = set1.find(i) != set1.end();
    bool foundInRowValues = set2.find(i) != set2.end();
    bool foundInColValues = set3.find(i) != set3.end();
    if (foundInGridValues && foundInRowValues && foundInColValues) {
      unassignedValues.insert(i); 
    }
  }

  return unassignedValues;
}

CellLocation Board::findLocation(Cell* cell) {
  CellLocation loc;
  for (int g = 0; g < 9; g++) {
    for (int c = 0; c < 9; c++) {
      if (grids[g].getCell(c) == cell) {
        loc.grid = g;
        if (g == 0 || g == 1 || g == 2) {
          if (c == 0 || c == 1 || c == 2) {
            loc.row = 0;
          } else if (c == 3 || c == 4 || c == 5) {
            loc.row = 1;
          } else if (c == 6 || c == 7 || c == 8) {
            loc.row = 2;
          }
        } else if (g == 3 || g == 4 || g == 5) {
          if (c == 0 || c == 1 || c == 2) {
            loc.row = 3;
          } else if (c == 3 || c == 4 || c == 5) {
            loc.row = 4;
          } else if (c == 6 || c == 7 || c == 8) {
            loc.row = 5;
          }
        } else if (g ==6 || g ==7 || g ==8) {
          if (c == 0 || c == 1 || c == 2) {
            loc.row = 6;
          } else if (c == 3 || c == 4 || c == 5) {
            loc.row = 7;
          } else if (c == 6 || c == 7 || c == 8) {
            loc.row = 8;
          }
        }
      
        if (g ==0 || g ==3 || g ==6) {
          if (c == 0 || c == 3 || c == 6) {
            loc.col = 0;
          } else if (c == 1 || c == 4 || c == 7) {
            loc.col = 1;
          } else if (c == 2 || c == 5 || c == 8) {
            loc.col = 2;
          }
        } else if (g ==1 || g ==4 || g ==7) {
          if (c == 0 || c == 3 || c == 6) {
            loc.col = 3;
          } else if (c == 1 || c == 4 || c == 7) {
            loc.col = 4;
          } else if (c == 2 || c == 5 || c == 8) {
            loc.col = 5;
          }
        } else if (g ==2 || g ==5 || g ==8) {
          if (c == 0 || c == 3 || c == 6) {
            loc.col = 6;
          } else if (c == 1 || c == 4 || c == 7) {
            loc.col = 7;
          } else if (c == 2 || c == 5 || c == 8) {
            loc.col = 8;
          }
        }
      }
    }
  }
  
  return loc;
}