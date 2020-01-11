#include <array>
#include "lib/googletest/include/gtest/gtest.h"
#include "../array.h"

//Test array constructor
TEST(Tests, ArrayConstructorTest) {
  EXPECT_NO_THROW(new Array<int>);
  EXPECT_NO_THROW(new Array<int>(0));
  EXPECT_THROW(new Array<int>(-1), std::runtime_error);
}

//Test that index being out of bounds
TEST(Tests, ArrayIndexOutOfBoundsTest) {
  Array<int> a;
  for (int i = 0; i < 10; ++i)
    a.insert(i);
  EXPECT_THROW(a[-1], std::range_error);
  EXPECT_THROW(a[10], std::range_error);
}

//Test removing element from array
TEST(Tests, ArrayElementRemovalTest) {
  Array<int> a;
  for (int i = 0; i < 6; ++i)
    a.insert(i);
  a.remove(3);
  EXPECT_TRUE( a[2] == 2);
  EXPECT_TRUE( a[3] == 4);
  EXPECT_TRUE( a[4] == 5);
  EXPECT_TRUE( a.getSize() == 5);
}

//Test assignment constructor
TEST(Tests, AssignmentConstuctorTest) {
  Array<int> a;
  for (int i = 0; i < 10; ++i)
    a.insert(i);
  Array<int> b = a;
  a.insert(0, 1);
  EXPECT_TRUE( a[0] == 1);
  EXPECT_TRUE( b[0] == 0);
}

//Test assignment operator
TEST(Tests, AssignmentOperatorTest) {
  Array<int> a;
  for (int i = 0; i < 10; ++i)
    a.insert(i);
  Array<int> b;
  b = a;
  a.insert(0, 1);
  EXPECT_TRUE( a[0] == 1);
  EXPECT_TRUE( b[0] == 0);
}

//Test movement constructor
TEST(Tests, MovementConstructorTest) {
  Array<int> a;
  for (int i = 0; i < 10; ++i)
    a.insert(i);
  Array<int> b(std::move(a));

  EXPECT_TRUE( b[0] == 0);
  EXPECT_TRUE( a.pointer == nullptr);
}

//Test movement operator
TEST(Tests, MovementOperatorTest) {
  Array<int> a;
  for (int i = 0; i < 10; ++i)
    a.insert(i);
  Array<int> b;
  b = std::move(a);

  EXPECT_TRUE( b[0] == 0);
  EXPECT_TRUE( a.pointer == nullptr);
}

//Test iterator
TEST(Tests, IteratorTest) {
  Array<int> a;
  for (int i = 0; i < 10; ++i)
    a.insert(i);
  Array<int>::Iterator it = a.iterator();
  EXPECT_TRUE( it.get() == 0);
  it.set(1);
  EXPECT_TRUE( it.get() == 1);
  do {
    it.next();
  } while (it.hasNext());
  EXPECT_FALSE( it.hasNext());
  EXPECT_TRUE( it.hasPrev());
  do {
    it.prev();
  } while (it.hasPrev());
  EXPECT_TRUE( it.hasNext());
  EXPECT_FALSE( it.hasPrev());
  it.remove();
  EXPECT_TRUE(a.getSize() == 9);
  it.toIndex(3);
  EXPECT_TRUE(it.get() == 4);
  it.insert(5);
  EXPECT_TRUE(it.get() == 5);
}
