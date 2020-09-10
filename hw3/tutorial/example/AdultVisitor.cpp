#include "AdultVisitor.hpp"
#include "ToyStore.hpp"
#include "BookStore.hpp"

#include <iostream>

AdultVisitor::AdultVisitor(const char *name) : name(name) {}

void AdultVisitor::visit(ToyStore &toy_store) {
    std::printf("%s buys some models at %s\n",
                name.c_str(), toy_store.getNameCString());
}

void AdultVisitor::visit(BookStore &book_store) {
    std::printf("%s buys \"C++ Programming through Memes\" at %s\n",
                name.c_str(), book_store.getNameCString());
}
