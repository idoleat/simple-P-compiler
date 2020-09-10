#pragma once

#include "visitor.hpp"

#include <string>

class ChildVisitor : public StoreVisitorBase {
  public:
    ChildVisitor(const char *name);
    ~ChildVisitor() = default;

    void visit(ToyStore &toy_store) override;
    void visit(BookStore &book_store) override;
  private:
    std::string name;
    // some data structure for something like how much money this kid has
};
