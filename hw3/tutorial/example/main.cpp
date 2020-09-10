#include <iostream>
#include <memory>
#include <vector>

#include "ToyStore.hpp"
#include "BookStore.hpp"
#include "ChildVisitor.hpp"
#include "AdultVisitor.hpp"

int main() {
  std::vector<std::shared_ptr<Store>> stores;
  stores.emplace_back(std::make_shared<ToyStore>("Troll Story"));
  stores.emplace_back(std::make_shared<BookStore>("Tan Loan"));

  ChildVisitor child("Giorno Giovanna");
  for (const auto &store : stores) {
      store->accept(child);
  }

  AdultVisitor adult("Dio");
  for (const auto &store : stores) {
      store->accept(adult);
  }

  return 0;
}
