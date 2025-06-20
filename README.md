# NContainer

**NContainer** is a lightweight, custom dynamic container class for C++ that mimics some functionality of `std::vector`, with added flexibility and full manual memory control. It supports standard operations like append, remove, assign, sort, and more.

## 🚀 Features

- Manual memory management (no STL containers used)
- Dynamic resizing with safe reallocation
- Element insertion/removal at arbitrary positions
- Assignment from raw arrays or other NContainers
- Basic sorting algorithms: Bubble Sort and Insertion Sort
- Works with any trivially copyable type

## 📦 Example Usage

```cpp
#include "n_container.hpp"

int main() {
    NE::NContainer<int> c;
    int data[] = {1, 2, 3};
    c.Assign(data);
    c.Append(4, c.GetLength());  // Append at end
    c.Replace(5, 0);             // Replace index 0
    std::cout << c << std::endl; // Print: { 5 2 3 4 }
}
```

## 🗃️ File Structure

```
NContainer/
├── include/
│   └── n_container.hpp     # Container interface
│   └── n_container.inl     # Templated implementation
├── tests/
│   └── main.cpp            # Usage example / basic test
├── README.md
├── LICENSE
└── .gitignore
```
