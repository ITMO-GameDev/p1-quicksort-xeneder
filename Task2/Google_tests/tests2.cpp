#include <array>
#include "lib/googletest/include/gtest/gtest.h"
#include "../list.h"

//Test array constructor
TEST(ArrayTests, ConstructorTest) {
  EXPECT_NO_THROW(new Array<int>);
  EXPECT_NO_THROW(new Array<int>(0));
  EXPECT_THROW(new Array<int>(-1), std::runtime_error);
}

//Test that index being out of bounds
TEST(ArrayTests, IndexOutOfBoundsTest) {
  Array<int> a;
  for (int i = 0; i < 10; ++i)
    a.insert(i);
  EXPECT_THROW(a[-1], std::range_error);
  EXPECT_THROW(a[10], std::range_error);
}

//Test removing element from array
TEST(ArrayTests, ElementRemovalTest) {
  Array<int> a;
  for (int i = 0; i < 6; ++i)
    a.insert(i);
  a.remove(3);
  EXPECT_TRUE(a[2] == 2);
  EXPECT_TRUE(a[3] == 4);
  EXPECT_TRUE(a[4] == 5);
  EXPECT_TRUE(a.getSize() == 5);
}

//Test assignment constructor
TEST(ArrayTests, AssignmentConstuctorTest) {
  Array<int> a;
  for (int i = 0; i < 10; ++i)
    a.insert(i);
  Array<int> b(a);
  a.insert(0, 1);
  EXPECT_TRUE(a[0] == 1);
  EXPECT_TRUE(b[0] == 0);
}

//Test assignment operator
TEST(ArrayTests, AssignmentOperatorTest) {
  Array<int> a;
  for (int i = 0; i < 10; ++i)
    a.insert(i);
  Array<int> b;
  b = a;
  a.insert(0, 1);
  EXPECT_TRUE(a[0] == 1);
  EXPECT_TRUE(b[0] == 0);
}

//Test movement constructor
TEST(ArrayTests, MovementConstructorTest) {
  Array<int> a;
  for (int i = 0; i < 10; ++i)
    a.insert(i);
  Array<int> b(std::move(a));

  EXPECT_TRUE(b[0] == 0);
  EXPECT_TRUE(a.pointer == nullptr);
}

//Test movement operator
TEST(ArrayTests, MovementOperatorTest) {
  Array<int> a;
  for (int i = 0; i < 10; ++i)
    a.insert(i);
  Array<int> b = std::move(a);

  EXPECT_TRUE(b[0] == 0);
  EXPECT_TRUE(a.pointer == nullptr);
}

//Test iterator
TEST(ArrayTests, IteratorTest) {
  Array<int> a;
  for (int i = 0; i < 10; ++i)
    a.insert(i);
  Array<int>::Iterator it = a.iterator();
  EXPECT_TRUE(it.get() == 0);
  it.set(1);
  EXPECT_TRUE(it.get() == 1);
  do {
    it.next();
  } while (it.hasNext());
  EXPECT_FALSE(it.hasNext());
  EXPECT_TRUE(it.hasPrev());
  do {
    it.prev();
  } while (it.hasPrev());
  EXPECT_TRUE(it.hasNext());
  EXPECT_FALSE(it.hasPrev());
  it.remove();
  EXPECT_TRUE(a.getSize() == 9);
  it.toIndex(3);
  EXPECT_TRUE(it.get() == 4);
  it.insert(5);
  EXPECT_TRUE(it.get() == 5);
}

//Test copy list constructor
TEST(ListTests, CopyConstructorTest) {
  List<int> l;
  for (int i = 0; i < 10; ++i)
    l.appendToTail(i);
  List<int> m(l);
  l.appendToTail(10);
  EXPECT_TRUE(l.getTail() == 10);
  EXPECT_TRUE(m.getTail() == 9);
}

//Test move list constructor
TEST(ListTests, MoveConstructorTest) {
  List<int> l;
  for (int i = 0; i < 10; ++i)
    l.appendToTail(i);
  List<int> m(std::move(l));
  EXPECT_TRUE(m.getTail() == 9);
  EXPECT_THROW(l.getHead(), std::range_error);
}

//Test copy list constructor
TEST(ListTests, CopyOperatorTest) {
  List<int> l;
  for (int i = 0; i < 10; ++i)
    l.appendToTail(i);
  List<int> m;
  m = l;
  l.appendToTail(10);
  EXPECT_TRUE(l.getTail() == 10);
  EXPECT_TRUE(m.getTail() == 9);
}

//Test move list constructor
TEST(ListTests, MoveOperatorTest) {
  List<int> l;
  for (int i = 0; i < 10; ++i)
    l.appendToTail(i);
  List<int> m;
  m = std::move(std::move(l));
  EXPECT_TRUE(m.getTail() == 9);
  EXPECT_THROW(l.getHead(), std::range_error);
}

//Test append/prepend
TEST(ListTests, AppendPrependTest) {
  List<int> l;
  for (int i = 0; i < 10; ++i)
    l.appendToTail(i);
  l.prependToHead(-1);
  EXPECT_TRUE(l.getHead() == -1);
  l.appendToTail(11);
  EXPECT_TRUE(l.getTail() == 11);
  EXPECT_TRUE(l.getSize() == 12);
}

//Test remove elements
TEST(ListTests, RemoveElements) {
  List<int> l;
  for (int i = 0; i < 10; ++i)
    l.appendToTail(i);
  l.removeFromHead();
  EXPECT_TRUE(l.getHead() == 1);
  l.removeFromTail();
  EXPECT_TRUE(l.getTail() == 8);
  EXPECT_TRUE(l.getSize() == 8);
}

//Test iterator
TEST(ListTests, IteratorTest) {
  List<int> l;
  for (int i = 0; i < 10; ++i)
    l.appendToTail(i);
  List<int>::Iterator it = l.iterator();
  EXPECT_TRUE(it.get() == 0);
  it.set(1);
  EXPECT_TRUE(it.get() == 1);
  do {
    it.next();
  } while (it.hasNext());
  EXPECT_FALSE(it.hasNext());
  EXPECT_TRUE(it.hasPrev());
  do {
    it.prev();
  } while (it.hasPrev());
  EXPECT_TRUE(it.hasNext());
  EXPECT_FALSE(it.hasPrev());
  it.remove();
  EXPECT_TRUE(l.getSize() == 9);
  it.insert(5);
  EXPECT_TRUE(it.get() == 5);
}
