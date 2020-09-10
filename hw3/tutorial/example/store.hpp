#pragma once

#include <string>

class StoreVisitorBase;

class Store {
  public:
    Store(const char *name) : name(name) {}
    virtual ~Store() {}

    virtual void accept(StoreVisitorBase &p_visitor) = 0;

    const char *getNameCString() const { return name.c_str(); }

  private:
    std::string name;
};
