#ifndef TREE_HPP
#define TREE_HPP
#include "./lib/ecautomata/ecautomata.hpp"
#include <chrono>
#include <iostream>
#include <thread>

enum State {
  EMPTY,
  ABYSS,
  BARRIER,
  TREE,
  PURPLE,
  PINK,
  BLUE,
  RED,
  ORANGE
};

typedef std::map<unsigned int, std::string> CellStateASCIIMap;

static CellStateASCIIMap asciimap = {
  {State::ABYSS, "\e[0;30m" + std::string("#") + "\e[0;0m"},
  {State::EMPTY, "\e[38;5;58m" + std::string(".") + "\e[0;0m"},
  {State::BARRIER, "\e[38;5;198m" + std::string("@") + "\e[0;0m"},
  {State::TREE, "\e[38;5;106m" + std::string("$") + "\e[0;0m"},
  {State::RED, "\e[38;5;88m" + std::string("0") + "\e[0;0m"},
  {State::PURPLE, "\e[38;5;128m" + std::string("0") + "\e[0;0m"},
  {State::PINK, "\e[38;5;170m" + std::string("0") + "\e[0;0m"},
  {State::BLUE, "\e[38;5;80m" + std::string("0") + "\e[0;0m"},
  {State::ORANGE, "\e[38;5;166m" + std::string("0") + "\e[0;0m"},
};

class TreeRuleset : public CustomGridRuleset<void> {
    virtual void apply(int currentCell, int *cellToUpdate, NeighboringCellStateCount &stateCount) override {
      if (currentCell == State::EMPTY && stateCount[State::EMPTY] >= 2) {
        *cellToUpdate = State::TREE;
      } else if (currentCell == State::TREE && stateCount[State::TREE] == 8) {
        *cellToUpdate = State::RED;
      } else if (this->isOrnament(currentCell) &&
                 (stateCount[State::RED] > 3 || stateCount[State::PURPLE] > 3 || stateCount[State::PINK] > 3 ||
                  stateCount[State::ORANGE] > 3 || stateCount[State::BLUE] > 3)) {
        *cellToUpdate = State::TREE;

      } else if (currentCell == State::RED) {
        *cellToUpdate = State::PINK;
      } else if (currentCell == State::PINK) {
        *cellToUpdate = State::PURPLE;
      } else if (currentCell == State::PURPLE) {
        *cellToUpdate = State::ORANGE;
      } else if (currentCell == State::ORANGE) {
        *cellToUpdate = State::BLUE;
      } else if (currentCell == State::BLUE) {
        *cellToUpdate = State::RED;
      } else {
        *cellToUpdate = currentCell;
      };
    };

  private:
    bool isOrnament(unsigned int currentCell) {
      return currentCell == State::RED || currentCell == State::PURPLE || currentCell == State::PINK ||
             currentCell == State::ORANGE || currentCell == State::BLUE;
    }
};

class ChristmasTree : public ECAutomata {
  public:
    ChristmasTree() {
      this->setNeighborhoodPosition({{0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}});
      this->builder.setPopValue(ABYSS);

      // Letting the framework know we are working with 10 states from 0 - 8
      for (int i = 0; i < 9; ++i) {
        this->setState(i);
      }
    };

    virtual void execute() override {
      const int MAX_GEN = 255;
      this->initTreeBorder();

      while (true) {
        this->printGrid();
        TreeRuleset ruleset;
        this->pass(ruleset);
      }
    };

  private:
    void printGrid() {
      system("clear");
      for (int row = 0; row < this->builder.getRows(); ++row) {
        for (int col = 0; col < this->builder.getCols(); ++col) {
          int cell = this->grid.at(row).at(col);
          auto iterator = asciimap.find(cell);
          if (iterator == asciimap.end()) {
            std::cout << "Given cell state of \"" << cell << "\" does not exist in the provided map!" << std::endl;
            exit(1);
          };
          std::cout << asciimap[cell];
        };
        std::cout << std::endl;
      };

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };

    void handleTreeEdges(int row) {
      for (int col = 0; col < this->builder.getCols(); ++col) {

        if (col == 0 || col == this->builder.getCols() - 1) {
          this->grid.at(row).at(col) = State::BARRIER;
        }
      };
    };

    void initTreeBorder() {

      // We need this to scale per screen, so im going to go an grab a little over a fourth of the available columns to use
      // in order to subtract from the total rows.
      const int SCALE = this->builder.getCols() / 4;
      for (int r = this->builder.getRows() - 1; r >= 0; --r) {
        const int OFFSET = SCALE * 2 - r;
        for (int c = OFFSET; c < this->builder.getCols() - OFFSET; ++c) {
          if (r == this->builder.getRows() - 1) {
            this->grid.at(r).at(c) = State::BARRIER;
          } else if (c == OFFSET || c == this->builder.getCols() - OFFSET - 1) {
            this->grid.at(r).at(c) = State::BARRIER;
          } else {
            this->grid.at(r).at(c) = State::EMPTY;
          };
        };
      };
    };
};

#endif
