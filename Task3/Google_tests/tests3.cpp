#include <array>
#include "lib/googletest/include/gtest/gtest.h"
#include "../dictionary.h"

void prepareDictionary(Dictionary<std::string, int>& dictionary) {
  dictionary.put("e", 1);
  dictionary.put("c", 2);
  dictionary.put("d", 3);
  dictionary.put("b", 4);
  dictionary.put("a", 5);
}

//Test array constructor
TEST(Tests, TestConstructor) {
  Dictionary<std::string, int> dictionary;
  EXPECT_TRUE(dictionary.size == 0);
}

//Test putting and balancing elements
TEST(Tests, TestElementPut) {
  Dictionary<std::string, int> dictionary;
  prepareDictionary(dictionary);
  EXPECT_TRUE(dictionary.rootNode->value == 3);
  EXPECT_TRUE(dictionary.rootNode->left->value == 4);
  EXPECT_TRUE(dictionary.rootNode->left->left->value == 5);
  EXPECT_TRUE(dictionary.rootNode->left->right->value == 2);
  EXPECT_TRUE(dictionary.rootNode->right->value == 1);
}

//Test removing elements
TEST(Tests, TestElemenRemoval) {
  Dictionary<std::string, int> dictionary;
  prepareDictionary(dictionary);
  dictionary.remove("d");
  EXPECT_TRUE(dictionary.rootNode->value == 2);
  EXPECT_TRUE(dictionary.rootNode->left->value == 4);
  EXPECT_TRUE(dictionary.rootNode->left->left->value == 5);
  EXPECT_TRUE(dictionary.rootNode->right->value == 1);
}

//Test contains and search
TEST(Tests, TestElementContainsSearch) {
  Dictionary<std::string, int> dictionary;
  prepareDictionary(dictionary);
  EXPECT_TRUE(dictionary.contains("e"));
  std::string f = "f";
  dictionary[f];
  EXPECT_TRUE(dictionary.contains(f));
}

//Test Iterator
TEST(Tests, TestIterator) {
  Dictionary<std::string, int> dictionary;
  prepareDictionary(dictionary);
  auto iterator = dictionary.iterator();
  EXPECT_TRUE(iterator.get() == 3);
  iterator.next();
  EXPECT_TRUE(iterator.get() == 4);
  iterator.next();
  EXPECT_TRUE(iterator.key() == "a");
  iterator.next();
  EXPECT_TRUE(iterator.key() == "c");
  iterator.next();
  EXPECT_FALSE(iterator.hasNext());
  iterator.prev();
  iterator.prev();
  iterator.prev();
  iterator.prev();
  EXPECT_FALSE(iterator.hasPrev());
  EXPECT_THROW(iterator.prev(), std::range_error);
}
