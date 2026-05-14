# Vector\<T\>

My own implementation of a dynamic array in C++ — a university OOP assignment.  
Basically a hand-rolled `std::vector`.

## Features

- works with any type
- grows automatically on `push_back` (doubles capacity)
- `Iterator` and `ConstIterator` — range-based for works fine
- bounds checking — `operator[]`, `insert`, `erase` throw `std::runtime_error`
- prints via `<<` as `[1, 2, 3]`

## Usage

```cpp
#include "vector.h"

Vector<int> v{1, 2, 3};
v.push_back(4);
v.pop_back();

for (auto& x : v) x *= 2;

std::cout << v; // [2, 4]

v.insert(v.begin(), 0);
v.erase(v.begin() + 1);

v.reserve(100);
v.shrink_to_fit();
```

## Requirements

- C++17
- header-only, just include `vector.h`

