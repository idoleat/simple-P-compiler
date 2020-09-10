#pragma once

#include "visitor.hpp"

#include <string>

class AdultVisitor : public StoreVisitorBase {
  public:
    AdultVisitor(const char *);
    ~AdultVisitor() = default;

    void visit(ToyStore &toy_store) override;
    void visit(BookStore &book_store) override;
  private:
    std::string name;
    // some data structure for something like how much money this adult has
};
