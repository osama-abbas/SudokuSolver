#include <iostream>
#include <set>

using namespace std;

struct CellLocation {
  int grid;
  int row;
  int col;
};

class Cell {
    
    private:
        bool assigned;
        bool assignedPossibleValue = false;
        int value;
        set<int> possibleValues;
        set<int> impossibleValues;
        set<int>::iterator currentPossibleValue;
        CellLocation location;

    public:
        Cell();
        Cell(int val);
        void assign(int val, bool clearPossibleValues);
        void setLocation(CellLocation loc) { location = loc; }
        void unassign();
        CellLocation getLocation() { return location; }
        int getValue() { return value; }
        bool isAssigned() { return assigned; }
        bool isAssignedPossibleValue() { return assignedPossibleValue; }
        set<int>& getPossibleValues() { return possibleValues; }
        void addPossibleValue(int val);
        void addImpossibleValue(int val) { impossibleValues.insert(val); }
        void printPossibleValues();
        int getCurrentPossibleValue() { return *currentPossibleValue; }
        bool incrementCurrentPossibleValue();
        int assignCurrentPossibleValue();
        void clearPossibleValues() { possibleValues.clear(); }
        void removeCurrentPossibleValue();
        void print() const;
        static CellLocation findLocation(int grid, int cellIdx) {
          CellLocation loc;
          loc.grid = grid;
          if (grid == 0 || grid == 1 || grid == 2) {
            if (cellIdx == 0 || cellIdx == 1 || cellIdx == 2) {
              loc.row = 0;
            } else if (cellIdx == 3 || cellIdx == 4 || cellIdx == 5) {
              loc.row = 1;
            } else if (cellIdx == 6 || cellIdx == 7 || cellIdx == 8) {
              loc.row = 2;
            }
          } else if (grid == 3 || grid == 4 || grid == 5) {
            if (cellIdx == 0 || cellIdx == 1 || cellIdx == 2) {
              loc.row = 3;
            } else if (cellIdx == 3 || cellIdx == 4 || cellIdx == 5) {
              loc.row = 4;
            } else if (cellIdx == 6 || cellIdx == 7 || cellIdx == 8) {
              loc.row = 5;
            }
          } else if (grid ==6 || grid ==7 || grid ==8) {
            if (cellIdx == 0 || cellIdx == 1 || cellIdx == 2) {
              loc.row = 6;
            } else if (cellIdx == 3 || cellIdx == 4 || cellIdx == 5) {
              loc.row = 7;
            } else if (cellIdx == 6 || cellIdx == 7 || cellIdx == 8) {
              loc.row = 8;
            }
          }
        
          if (grid ==0 || grid ==3 || grid ==6) {
            if (cellIdx == 0 || cellIdx == 3 || cellIdx == 6) {
              loc.col = 0;
            } else if (cellIdx == 1 || cellIdx == 4 || cellIdx == 7) {
              loc.col = 1;
            } else if (cellIdx == 2 || cellIdx == 5 || cellIdx == 8) {
              loc.col = 2;
            }
          } else if (grid ==1 || grid ==4 || grid ==7) {
            if (cellIdx == 0 || cellIdx == 3 || cellIdx == 6) {
              loc.col = 3;
            } else if (cellIdx == 1 || cellIdx == 4 || cellIdx == 7) {
              loc.col = 4;
            } else if (cellIdx == 2 || cellIdx == 5 || cellIdx == 8) {
              loc.col = 5;
            }
          } else if (grid ==2 || grid ==5 || grid ==8) {
            if (cellIdx == 0 || cellIdx == 3 || cellIdx == 6) {
              loc.col = 6;
            } else if (cellIdx == 1 || cellIdx == 4 || cellIdx == 7) {
              loc.col = 7;
            } else if (cellIdx == 2 || cellIdx == 5 || cellIdx == 8) {
              loc.col = 8;
            }
          }
        
          return loc;
        }
};