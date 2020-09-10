# Visitor Pattern

**實際範例**

我們在 `example/` 資料夾內放了一個簡單的範例，同學可以直接使用 `make` 來進行編譯並執行。裡面有不少實踐細節供同學參考。

## 為什麼要使用 Visitor Pattern？

單以這份作業來說，我們只需要對 AST 做一件事，就是輸出節點資訊。我們期望同學會對 AST 的子節點建立一個基礎型別，再根據不同 language construct 繼承出不同的節點型別。完成之後，同學只需要走訪 AST 並 dump 出每個節點的內容就完成這次作業了。
所以看起來根本不需要什麼 Visitor Pattern 就能完成 hw3，但在設計程式時，需要考慮未來可能的修改。

因此回到整學期的目標 - **撰寫編譯器**來看，我們後面需要對整棵 AST 做好幾種事情，除了輸出節點資訊外，我們可能還會需要語意分析、最佳化、產生機器碼等等任務需要完成，若每個任務都像這份一樣利用多型設計一個介面 (e.g., `print`, `codegen`)
 來管理，會讓每個節點的類別模組越來越肥；同時，最重要的是「**每個任務之間的關聯只有唯一一個共通點 - 『作用在 AST』上**」，造成每次開發時都要自行理解這些函式彼此間的關係（實際上沒什麼關係），是個很大的管理成本，大大降低整個編譯器專案的可維護性。

在物件導向程式設計中，有五個原則縮寫稱為 [SOLID](https://en.wikipedia.org/wiki/SOLID) 原則，其中 S 代表的是 Single Responsibility Principle (SRP)，上述的情況就有點違反 SRP 的味道在，因此我們會希望讓 AST 節點的相關類別就**做好一件事**，也就是「保存程式架構」就好；其餘的行為用另外的模組去擴充，而不是直接擴充在各個節點類別中。

Visitor Pattern 相當適合目前的狀況，它的核心概念在於「將『行為』與『資料』分開處理」。十分符合上面說的理想情況：AST 的節點負責保存程式架構（資料），而各個不同的訪問者 (visitor) 分別負責做語意分析、最佳化、產生機器碼等等（行為）。

實際上這份作業單單使用多型來完成是比較合理的，畢竟 `print` 是在展現自身所儲存的資料模樣，還算是在 SRP 的原則內，但是考量到下份作業的時程通常同學比較忙，因此決定在這份就建議同學來使用，這樣下一份作業同學能夠更專注在功能上的實作，而不用去研究設計範式 (Design Pattern) 的使用。

## Visitor Pattern

Visitor Pattern 是一種 **設計範式** (Design Pattern)，但其實不用很懂設計範式也可以使用 Visitor Pattern。在這裡會進一步介紹這個範式。

> :raised_hand: 在這之後的文章會把 Visitor Pattern 簡寫為 `VP`

**Visitor Pattern 初探**

這裡給個使用情境：一條街上有好幾間商店，像是玩具店、書店，而光顧的人有小孩、大人。不同的人在不同的商店會有對應的行為：

- 小孩
  - 玩具店：買積木
  - 書店：買 [JavaScript for Kids](https://www.reddit.com/r/ProgrammerHumor/comments/8uuw4g/javascript_for_kids/)
- 大人
  - 玩具店：買模型
  - 書店：買 [C++ Programming through Memes](https://www.goodreads.com/book/show/49749850-c-programming-through-memes)

若照著多型的方式去實做，你可能會寫出這樣的程式碼：

```cpp
// In ToyStore.hpp
class ToyStore : public Store {
  public:
    void visitedByChild(Child &child) override {
        std::cout << child.name() << " buys some Lego\n";
        sell("Lego");
    }

    void visitedByAdult(Adult &adult) override {
        std::cout << adult.name() << " buys some models\n";
        sell("model");
    }
};

// In BookStore.hpp
class BookStore : public Store {
  public:
    void visitedByChild(Child &child) override {
        std::cout << child.name() << " buys \"JavaScript for Kids\"\n";
        sell("JavaScript for Kids");
    }

    void visitedByAdult(Adult &adult) override {
        std::cout << adult.name() << " buys \"C++ Programming through Memes\"\n";
        sell("C++ Programming through Memes");
    }
};
```

可以發現，這些成員函式不僅跟類別本身沒有太大的關聯性，彼此間也沒什麼關係，甚至行為本身不是由該類別主導（訪問者買東西）。這時候我們就可以利用 VP 來重構這塊程式碼，把小孩與大人對於商店拜訪的行為邏輯集中，讓商店就擔任單純的資料提供者：

```cpp
// In ChildVisitor.hpp
class ChildVisitor : public StoreVisitorBase {
  public:
    void visit(ToyStore &toy_store) override {
        std::cout << name << " buys some Lego\n";
        toy_store.sell("Lego");
    }

    void visit(BookStore &book_store) override {
        std::cout << name << " buys \"JavaScript for Kids\"\n";
        book_store.sell("JavaScript for Kids");
    }
};

// In AdultVisitor.hpp
class AdultVisitor : public StoreVisitorBase {
  public:
    void visit(ToyStore &toy_store) override {
        std::cout << name << " buys some models\n";
        toy_store.sell("model");
    }

    void visit(BookStore &book_store) override {
        std::cout << name << " buys \"C++ Programming through Memes\"\n";
        book_store.sell("C++ Programming through Memes");
    }
};
```

兩個 visitors 已經針對不同目標撰寫特定行為了，接下來的問題則會是怎麼成功呼叫到對應的 `visit` 成員函式？

使用函式多載 (function overloading) 時，最後呼叫到哪個函式是透過編譯器在編譯時期決定的，它會利用呼叫函式時的參數型別來決定產生的機器碼會呼叫到哪個函式，因此我們想要的呼叫情境可能如下：

```cpp
ChildVisitor cv;

ToyStore ts(); // arguments omitted for simplicity
BookStore bs(); // arguments omitted for simplicity

cv.visit(ts);
cv.visit(bs);
```

但是，實際上的情境可能是我們走到了一條商店街，但是每家店都沒有招牌，不知道是開什麼的：

```cpp
std::vector <Store *> store_list = { s1, s2, s3 };
```

這時候用 `visit` 去接每個元素（店）作為參數，編譯器也無法正確判別該使用哪個 `visit` 方法，因為對編譯器來說每個元素的型別都是 `Store *`。那麼該如何從 `Store *` (pointer to base class) 得出實際上的型別 (derived class) 呢？

回想作業文件提到的多型概念：「呼叫同一個成員函式時，根據物件本身的實際型別會呼叫到『不同』的函式實作，因而有不同的行為」，我們可以利用虛擬函式 (virtual function) 的機制來使用到不同的函式實作；同時，C++ 的成員函式都有一個隱含的參數 - `this` 指標，指向的對象是物件自己，如此一來我們就能夠在這個虛擬成員函式中獲得實際上的型別指標，進而傳給 visitor 的 `visit` 函式：

```cpp
// In Store.hpp
class Store {
  public:
    virtual void accept(StoreVisitorBase &v) = 0; // pure virtual function
};

// In BookStore.hpp
class BookStore : public Store {
  public:
    void accept(StoreVisitorBase &v) override {
        v->visit(*this); // typeof(this) == BookStore *, calling visit(BookStore *book_store)
    }
};

// In main.cpp
ChildVisitor cv;
std::vector <Store *> store_list = { s1, s2, s3 };

for (Store *store : store_list) {
  store->accept(cv);
}
```

假設 s1 實際上是書店，而上面程式碼中的迴圈中拿到的變數 `store` 雖然型別為 `Store *` ，但是透過虛擬函式在迴圈內呼叫 `store->accept()` 時，呼叫到的函式就是在 `BookStore.hpp` 裡的實作版本，也因此我們能夠拿到型別為 `BookStore *` 的 `this` 指標，編譯器也就能夠判斷要呼叫到哪個版本的 `visit` 函式。

最後重新整理一下使用情況（括號內容為對應到作業的 AST）：

1. 將「資料」與「行為」分開處理 - 同類型的行為抽離出來寫成某個 visitor：

	首先，我們有一堆店家（AST 節點），而每個店家實際上賣的東西可能不同（AST 節點可能分 `IfNode`, `AssignmentNode` 等等），我們希望店家就單純負責陳列商品就好（AST 節點保存程式架構），來訪者具體行為我們分開處理。

2. 讓資料提供者提供一個窗口，讓來訪者能夠辨識真實型別，進而獲取適當的資料：

	在店家 (AST 節點的基礎類別）的類別中宣告 pure virtual function - `accept` 供外人使用，實作細節則由各個實體商店（AST 的各種節點）提供，內容基本上就是提供店家身份（AST 節點實際型別） - `this` 指標給來訪者。

**Visitor Pattern 的缺點**

考慮這個情況：

> --- [簡答題] 一題 10 分 ---
>
> > `編義氣` 是個鑽研編譯器多年的勇者，最近十年來他潛心開發一個偉大的編譯器 **LCC** - Life Companion Compiler（人生伴侶編譯器）：只需要描述另一半的特質，`LCC` 就可以編譯出一個針對用戶要求的完美另一半。在寫好 5000 多個進行最佳化分析的 visitor class 之後 ... 完了，他發現他描述另一半的語法樹節點型別就獨獨缺了 `腦子`。
> >
> > `編義氣` 是個真實的人，絕不能容許另一半沒腦子，但也不能接受自己沒有另一半，只好認份的補上有關 `腦子` 的所有 visit member functions。
>
> ~試問：各位同學 hw3 能拿幾分？~

回到 `LCC` 的狀況，在寫好一堆 `visitor` class 之後如果想要新增新的節點，就會是噩夢的開始。使用者必須要在自己寫的所有 `visitor` class 中一一支援`腦子`型別。在一個有 5000 個 `visitor` class 的專案上支援一個新型別，就要再寫 5000 個函式。

但編譯器這門課不會有這個問題，原因很簡單：因為*~~各位同學都不需要 LCC 就能找到自己的另一半~~*我們的節點型別在作業三之後就不會再新增了。這種情況之下，使用 `Visitor Pattern` 基本上不會遇到缺點。

再告訴各位同學一點，現今的編譯器實作也大多採用 `Visitor Pattern`。也可以說這是在撰寫編譯器上一個非常經典的設計範式，學會這個之後你的專案就更接近現實世界了一些。 還有未來如果提供範例程式碼或是有考試的話，也都預設同學已經學會 `VP` 了哦 :heart:。
