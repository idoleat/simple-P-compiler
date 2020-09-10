#pragma once

#include "store.hpp"
#include "visitor.hpp"

class BookStore : public Store {
  public:
    BookStore(const char *name) : Store{name} {}
    ~BookStore() = default;

    void accept(StoreVisitorBase &p_visitor) override { p_visitor.visit(*this); }

  private:
    // some data structure for stock management
};
