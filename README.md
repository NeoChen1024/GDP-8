# Neo_Chen's 16 Bit Virtual Machine (WIP)

![DCC GDP-8 Logo](https://gitlab.com/NeoWorkgroup/GDP-8/raw/master/Logo/DCC_GDP-8_Logo.png "Yes, this is our logo")

## 簡短介紹：

DCC GDP-8 是一個模仿 DEC PDP-8 的虛擬機，使用 16 位元機器字，
16 位元記憶體定址，外加 8 位元的分段。
各指令格式為了配合 16 進位，皆有 4 Bit 對齊，方便判讀。
本專案的目標是創造現代的，類似於「迷你電腦」(Mini Computer)
，的虛擬機，就和 DEC 的 PDP-8/E 或 PDP-11/70 一樣。

為了實做簡單（因為我的 C 語言程度不夠），還有我暫時不
懂虛擬記憶體的運作， DCC GDP-8 就和 PDP-8/E 一樣，沒有虛擬
記憶體，不區分 Kernel Mode 和 User Mode 的記憶體映射。

## 參考資料：
[PDP-8/E Assembly](http://homepage.divms.uiowa.edu/~jones/pdp8/man/)
