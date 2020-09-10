# 作業 4 報告


> 學生： 簡志瑋
>
> 學號： 0411275


## 作業架構
期末將近，不是很確定有沒有時間完成 multi-pass 的實做方法(可能其實沒有很難但是我對one-pass比較有把握)，很顯然的 one-pass 是比較簡單的作法，所以我決定還是先求有再求好

### Symbol Table 及其 Manager
這部份比較簡單所以先行完成
* `SymbolEntry`
    * 儲存 Symbol 的資訊，透過基本的 `Get` `Set` 界面存取
* `SymbolTable`
    * 用 `unordered_map` 來存 symbol，平均 lookup 速度為 constant time.
    * 另外用一個 `list` 來存要輸出的順序
    * Invalid declration 會有另外一個 `unordered_set` 存起來放在 `ConetextManager`，讓 variable reference 查詢是否合法
* `SymbolManager`
    * 用 `list` 存放 symbol table
    * `list` 可以被 lock 起來(不能新增/刪除)，像是要讓 parameters 和 compound statement 共用同一張表的時候，就在新增 parameters 的時候把 `list` 鎖起來，compound statement 內的東西就會與 parameters 同一張 table，並另外用一個 `bool` 來開關這種行為。Loop var 也是一樣的處理方式
    * 這邊的 `lookup` 會額外查詢 outer scope 的table，如果當前 table 沒有找到的話

* `ContextManager`
    * 存一些雜七雜八的 symbol 相關額外訊息
        * 在這邊開一張 table 專門存 loop_var ，擁有所有 table 的操作方便好用
        * 非法 variable 也存在這邊，不過就只是一個 `unordered_set` 方便查而已
        * 紀錄上一個被存進 table 的 entry 是誰，由於 variable 後馬上接著 constant，所以constant就直接存取上一個 entry 來新增 attribute 並標記 kind 為 constant
        * 一個修掉 variable reference 中 `[ digit ]` 的函式

### 資訊傳遞
* 建了一顆跟 AST 一樣結構的樹儲存資訊
    * 在 visit 每個 node 時，visit child之前，`new` 一個 `iNode` 並初始化(連接parent，傳遞error message...)
    * 因此在 visit child 之後也會有 child nodes 的資訊了
    * 功能有傳遞型別、child 或 parent 是否有錯誤、node的location，以及額外任意自訂資訊

### Error Message
根據助教提示印出在 error message 中印出 source code 的方法
* 複製整個 code 傳給 Analyzer
    * 用空間換時間，build 的時候 RAM 吃好吃滿也蠻正常的吧 (x
* 只紀錄位置，用 `fseek()`, `fgets()` 或其他讀檔函式查詢
    * 用時間換空間，我覺得一直讀檔實在太花時間了，所以用了上面的方法
印出Source code

實做方法是 `scanner.l` 每讀完一行就會把 code 存進一個 `vector` ，再用 `location.line` Error Message 統一由 `ErrorHandler` 儲存病並輸出。handler 有個 `list` 存放 error message，在離開 program node 之前 dump 出來

### 錯誤偵測
依據助教提供的文件檢查錯誤，並利用以上已經做好的工具把錯誤資訊傳遞出來。

## 遇到的困難
* node之間的資訊傳遞複雜
* 研究助教寫的東西要怎麼用也花一些時間，畢竟我之前沒什麼接觸 `c++11` 以後的東西，不過的確實是個學習的好機會
* 某些發現情況下處理過得字串輸出都會變成亂碼(non-unicode character)，例如字串concatinate之後就會印出 UTF-8 無法 decode 的字元 `0x某某` 。或是在輸出錯誤訊息需要先處理字串，使用 `strcat` `sprintf` `std::string operator+` 都會造成一樣結果，所以只好用一個list存起來一個一個印了。
    * 首先猜測是有危險的寫入，覆蓋到別人的記憶體了，之前也範過這個錯，不過那時候只是字串後面忘記加`'\0'`，容易偵錯。目前還不太會用debbuger，得不到太多其他資訊
    * 最後發現其實是把會被釋放的記憶體位置存到指標身上，然後又在釋放後去存取，像是把區域變數的位置傳給外面(C#寫太多= =)，這時應該要傳值而不是位置。目前還有幾個Segment fault不知道是不是也是這樣的原因。
    * 感謝助教提供測試腳本的 patch 讓我更易於偵錯
    * 目前的補救辦法是把有問題的字串用 `strdup` 複製內容後再傳遞
    * 現階段的解決辦法是對我這個c++新手而言處理字串全部都用 `std::string` 以物件形式處理，或是全部以 `C String` 處理，混著用會讓我不小心寫出危險的東西。也許等經驗累積夠了才能比較自在的兼用。
* 糟糕的記憶體管理，沒在寫 destructor 也沒在釋放沒用到的記憶體
* 助教說不要都用字串儲存，但是助教已經寫好的AST node的資訊回傳都字串，所以我覺得我還是處理了很多字串
* 大部分的時間都在踩c++的坑，和自己挖的坑，些新功能的時間反而不多
* 哪些該開 private，哪些該開 public 不太確定。以往的習慣是基本上大部分 member 都先 private，再用 public function 當界面存取，但是這次有點覺得界面會開的太繁雜，所以有些memeber索性就直接 public了。
* 我還是有一個 issue 沒有解決，查詢一個 entry 出來之後有機會在執行 SymbolEntry 上的 `GetName()`、`GetKind()` 等 member function 時會segment fault，用 `gdb` 查看結果是儲存資料的 private member 都是 `0x0`，我也不知道為什麼，最後在印 symbol 出來的時候也是一樣的查詢方法卻沒有錯誤。目前都是用其他方法繞過去了。

<!--
* 善用const

```c++
DeclNode::VarNodes vars = p_decl.variables(); // why this cause compiler error?
```
-->
