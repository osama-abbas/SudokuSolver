#include "cell.hpp"

class Grid {
    private:
        Cell grid[9];
        set<int> values;

    public:
        Grid();
        Grid(string values);
        void assign(Cell cell, int pos);
        Cell* getCell(int pos);
        set<int> getValues();
        set<int> getUnassigedValues();
        void addValue(int val);
        void removeValue(int val);
        void print();
};