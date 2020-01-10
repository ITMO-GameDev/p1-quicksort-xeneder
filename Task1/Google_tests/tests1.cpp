#include <array>
#include "lib/googletest/include/gtest/gtest.h"
#include "../sort.h"

template<typename T>
void prepareData(T origin, T result, int size) {
  using Q = typename std::remove_pointer<T>::type;

  for (int c = 0; c < size; ++c) {
    std::cout << *(origin + c) << ' ';
    *(result + c) = *(origin + c);
  }
  std::cout << '\n';

  qsort(origin, size, sizeof(Q), [](const void * a, const void * b) { return ( *(char*)a - *(char*)b ); });

  for (int c = 0; c < size; ++c) {
    std::cout << *(origin + c) << ' ';
  }
  std::cout << '\n';
}

//Test that sorted array is not being resorted
TEST(Tests, FirstTest) {
  std::array<int, 10> origin = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::array<int, 10> result = origin;
  mySort(result.data(), result.data() + 9, sizeof(int), [](int a, int b) { return a < b; });
  EXPECT_TRUE(0 == std::memcmp(result.data(), origin.data(), origin.size()));
}

//Test that 10 element array is being sorted
TEST(Tests, SecondTest) {
  std::array<int, 10> origin = {9, 2, 3, 7, 8, 6, 5, 0, 4, 1};
  std::array<int, origin.size()> result;
  prepareData(origin.data(), result.data(), origin.size());
  mySort(result.data(), result.data() + origin.size() - 1, sizeof(int), [](int a, int b) { return a < b; });

  EXPECT_TRUE(0 == std::memcmp(result.data(), origin.data(), origin.size()));
}

//Test that random 100 element array is being sorted
TEST(Tests, ThirdTest) {
  std::array<int, 100> origin;
  for (int i = 0; i < origin.size(); ++i) {
    origin[i] = std::rand() % origin.size();
  }

  std::array<int, origin.size()> result;

  prepareData(origin.data(), result.data(), origin.size());

  mySort(result.data(), result.data() + result.size() - 1, sizeof(int), [](int a, int b) { return a < b; });

  for (int c = 0; c < result.size(); ++c) {
    std::cout << result[c] << ' ';
  }
  std::cout << '\n';

  EXPECT_TRUE(0 == std::memcmp(result.data(), origin.data(), origin.size()));
}

//Test that random 10 element char array is being sorted
TEST(Tests, Fourth) {
  std::array<char, 11> result = {'a', 'c', 'd', 'a', 'f', 'k', 'h', 'd', 'q', 'm', 'f'};
  std::array<char, 11> origin = result;

  prepareData(origin.data(), result.data(), origin.size());

  mySort(result.data(), result.data() + result.size() - 1, sizeof(char), [](char a, char b) { return a < b; });

  for (int c = 0; c < result.size(); ++c) {
    std::cout << result[c] << ' ';
  }

  EXPECT_TRUE(0 == std::memcmp(result.data(), origin.data(), origin.size()));
}

