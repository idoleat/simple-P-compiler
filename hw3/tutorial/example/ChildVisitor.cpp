#include "ChildVisitor.hpp"
#include "ToyStore.hpp"
#include "BookStore.hpp"

#include <iostream>

ChildVisitor::ChildVisitor(const char *name) : name(name) {}

void ChildVisitor::visit(ToyStore &toy_store) {
    std::printf("%s buys some Lego at %s\n",
                name.c_str(), toy_store.getNameCString());
}

void ChildVisitor::visit(BookStore &book_store) {
    std::printf("%s buys \"JavaScript for Kid\" at %s\n",
                name.c_str(), book_store.getNameCString());
}
