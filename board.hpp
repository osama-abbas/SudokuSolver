#include <map>
#include <set>
#include <stack>
#include <vector>

#include "grid.hpp"

struct CellLocation;

class Board {
    private:
        Grid grids[9];
        bool assignedPossibleValue = false;
        int minPossibleValues;
        int minPossibleGridNum;
        int minPossibleCellNum;
        Cell* mostRecentlyAssignedCell;
        map<int, vector<Cell*>> rows;
        map<int, vector<Cell*>> cols;
        map<int, set<int>> rowValues;
        map<int, set<int>> colValues;
        vector<Cell*> getRow(int row);
        vector<Cell*> getCol(int col);
        stack<Cell*> possibleValueCellStack;
        int getRowNum(int gridNum, int cellNum);
        int getColNum(int gridNum, int cellNum);
        set<int> getUnassignedRowValues(int row);
        set<int> getUnassignedColValues(int col);
        int countUnassignedCells();
        void assignPossibleValues();
        void printSet(set<int> s);
        void printMap(map<int, vector<Cell*>> myMap);
        void printCellSet(set<Cell*> cells);
        void printCellVec(vector<Cell*> cells);
        bool rowContains(int row, int value);
        bool colContains(int col, int value);
        bool gridContains(int grid, int value);
        // void clearPossibleValues();
        set<int> findCommonUnassignedValues(set<int> set1, set<int> set2, set<int> set3);
        CellLocation findCell(Cell* cell);
        void addCellValueToContainers(Cell* cell);
        void removeCellValueFromContainers(Cell* cell);
        // void clearAllPossibleValues();
        void removeUnassignedPossibleCellsFromStack();

    public:
        Board();
        void setGrid(int gridNum, Grid grid);
        void print();
        void computePossibleValues();
        Grid* getGrid(int gridNum);
        CellLocation findLocation(Cell* cell);
};