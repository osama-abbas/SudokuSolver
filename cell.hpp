#include <iostream>
#include <set>
#include <vector>

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
        vector<int> possibleValues;
        set<int> impossibleValues;
        int currentPossibleValueIdx;
        CellLocation location;
        bool locationSet;

    public:
        Cell();
        Cell(int val);
        void assign(int val, bool clearPossibleValues);
        bool isLocationSet() { return locationSet; }
        void setLocation(CellLocation loc) { 
          location = loc; 
          locationSet = true; 
        }
        void unassign();
        CellLocation getLocation() { return location; }
        int getValue() { return value; }
        bool isAssigned() { return assigned; }
        bool isAssignedPossibleValue() { return assignedPossibleValue; }
        vector<int>& getPossibleValues() { return possibleValues; }
        void addPossibleValue(int val);
        void clearPossibleValues();
        void addImpossibleValue(int val) { impossibleValues.insert(val); }
        void printPossibleValues();
        int getCurrentPossibleValue() { return possibleValues[currentPossibleValueIdx]; }
        bool incrementCurrentPossibleValue();
        bool canIncrementCurrentPossibleValue();
        int assignCurrentPossibleValue();
        void print() const;
};