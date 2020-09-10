#pragma once

#include "store.hpp"
#include "visitor.hpp"

class ToyStore : public Store {
  public:
    ToyStore(const char *name) : Store{name} {}
    ~ToyStore() = default;

    void accept(StoreVisitorBase &p_visitor) override { p_visitor.visit(*this); }

  private:
    // some data structure for stock management
};
