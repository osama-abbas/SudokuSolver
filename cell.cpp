#include "cell.hpp"

Cell::Cell() {
  assigned = false;
  value = 0;
  currentPossibleValue = possibleValues.begin();
}

Cell::Cell(int val) {
  assigned = true;
  value = val;
}

void Cell::assign(int val, bool clearPossibleValues) {
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
  for (set<int>::iterator itr = possibleValues.begin();
       itr != possibleValues.end(); itr++) {
    cout << *itr << " ";
  }
  cout << endl;
}

bool Cell::incrementCurrentPossibleValue() {
  cout << "Possible values: ";
  printPossibleValues();
  if (currentPossibleValue != possibleValues.end()) {
    cout << "Incrementing current possible value from " << *currentPossibleValue;
    currentPossibleValue++;
    cout << " to " << *currentPossibleValue << endl;
    assignCurrentPossibleValue();
    return true;
  } else {
    cout << "Unable to increment current possible value" << endl;
    return false;
  }
}

int Cell::assignCurrentPossibleValue() {
  assign(*currentPossibleValue, false);
  assignedPossibleValue = true;
  return *currentPossibleValue;
}

void Cell::addPossibleValue(int val) {
  if (impossibleValues.find(val) == impossibleValues.end()) {
    possibleValues.insert(val);
    currentPossibleValue = possibleValues.begin();
  }
}

void Cell::removeCurrentPossibleValue() {
  set<int>::iterator nextIt = next(currentPossibleValue, 1);
  possibleValues.erase(currentPossibleValue);
  currentPossibleValue = nextIt;
}

void Cell::print() const {
  if (assigned) {
    cout << value;
  } else {
    cout << "X";
  }
}