#include <iostream>
#include "sort.h"

int a[10] = {9, 1, 5, 4, 6, 7, 8, 2, 3, 0};

int main() {
  mySort(a, a + 9, sizeof(int), [](int a, int b) { return a < b; });

  for (int c = 0; c < 10; ++c) {
    std::cout << a[c] << ' ';
  }

  return 0;
}
