#pragma once

class ToyStore;
class BookStore;

class StoreVisitorBase {
  public:
    virtual void visit(ToyStore &p_toy_store) = 0;
    virtual void visit(BookStore &p_book_store) = 0;
};
