#include "cell.hpp"
#include <algorithm>

Cell::Cell() {
  assigned = false;
  value = 0;
  currentPossibleValueIdx = -1;
}

Cell::Cell(int val) {
  assigned = true;
  value = val;
  currentPossibleValueIdx = -1;
}

void Cell::assign(int val, bool clearPossibleValues) {
  if (val == 0) {
    cout << "I'm 0!" << endl;
  }
  assigned = true;
  value = val;
  if (clearPossibleValues) {
    possibleValues.clear();
  }
}

void Cell::unassign() {
  assigned = false;
  assignedPossibleValue = false;
  value = 0;
}

void Cell::printPossibleValues() {
  for (vector<int>::iterator itr = possibleValues.begin();
       itr != possibleValues.end(); itr++) {
    cout << *itr << " ";
  }
  cout << endl;
}

bool Cell::incrementCurrentPossibleValue() {
  cout << "Possible values: ";
  printPossibleValues();
  if (currentPossibleValueIdx != -1 &&
      currentPossibleValueIdx < possibleValues.size() - 1) {
    cout << "Cell (" << location.row << ", " << location.col << ") ";
    cout << "incrementing current possible value from "
         << possibleValues[currentPossibleValueIdx];
    currentPossibleValueIdx++;
    cout << " to " << possibleValues[currentPossibleValueIdx] << endl;
    assignCurrentPossibleValue();
    return true;
  } else {
    cout << "Unable to increment current possible value" << endl;
    return false;
  }
}

int Cell::assignCurrentPossibleValue() {
  cout << "Possible values: ";
  printPossibleValues();
  cout << "Assigning "<< possibleValues[currentPossibleValueIdx] << endl;
  assign(possibleValues[currentPossibleValueIdx], false);
  assignedPossibleValue = true;
  return possibleValues[currentPossibleValueIdx];
}

void Cell::addPossibleValue(int val) {
  if (val == 0) {
    cout << "I'm 0!" << endl;
  }
  bool isImpossibleValue = impossibleValues.find(val) != impossibleValues.end();
  bool alreadyInPossibleValues = find(possibleValues.begin(), possibleValues.end(), val) != possibleValues.end();
  if (!isImpossibleValue && !alreadyInPossibleValues) {
    possibleValues.push_back(val);
    if (currentPossibleValueIdx == -1) {
      currentPossibleValueIdx = 0;
    }
  }
}

void Cell::clearPossibleValues() {
  possibleValues.clear();
}

void Cell::removeCurrentPossibleValue() {
  possibleValues.erase(possibleValues.begin() + currentPossibleValueIdx);
  if (currentPossibleValueIdx < possibleValues.size() - 1) {
    currentPossibleValueIdx++;
  }
}

void Cell::print() const {
  if (assigned) {
    cout << value;
  } else {
    cout << "X";
  }
}