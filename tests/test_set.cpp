#include <gtest/gtest.h>

#include <set>

#include "../s21_containers.h"

TEST(set, ConstructorDefaultSet) {
  s21::set<char> s21_empty_set;
  std::set<char> std_empty_set;
  EXPECT_EQ(s21_empty_set.empty(), std_empty_set.empty());
}

TEST(set, ConstructorInitializerSet) {
  s21::set<char> s21_set = {'a', 'b', 'c', 'd'};
  std::set<char> std_set = {'a', 'b', 'c', 'd'};
  EXPECT_EQ(s21_set.size(), std_set.size());
  auto s21_it = s21_set.begin();
  auto std_it = std_set.begin();
  while (s21_it != s21_set.end()) {
    EXPECT_EQ(*s21_it, *std_it);
    ++s21_it, ++std_it;
  }
}

TEST(set, ConstructorInitializer2Set) {
  s21::set<char> s21_set = {};
  std::set<char> std_set = {};
  EXPECT_EQ(s21_set.size(), std_set.size());
  auto s21_it = s21_set.begin();
  auto std_it = std_set.begin();
  while (s21_it != s21_set.end()) {
    EXPECT_EQ(*s21_it, *std_it);
    ++s21_it, ++std_it;
  }
}

TEST(set, AssignmentCopySet) {
  s21::set<int> s21_set = {1, 2, 3, 4, 5};
  std::set<int> std_set = {1, 2, 3, 4, 5};
  s21::set<int> s21_set_copy = s21_set;
  std::set<int> std_set_copy = std_set;
  EXPECT_EQ(s21_set_copy.size(), std_set_copy.size());
  auto s21_it = s21_set_copy.begin();
  auto std_it = std_set_copy.begin();
  while (s21_it != s21_set.end()) {
    EXPECT_EQ(*s21_it, *std_it);
    ++s21_it, ++std_it;
  }
}

TEST(set, ConstructorMoveSet) {
  s21::set<int> s21_set = {1, 2, 3, 4, 5};
  std::set<int> std_set = {1, 2, 3, 4, 5};
  s21::set<int> s21_set_copy = std::move(s21_set);
  std::set<int> std_set_copy = std::move(std_set);

  EXPECT_EQ(s21_set_copy.size(), std_set_copy.size());
  auto s21_it = s21_set_copy.begin();
  auto std_it = std_set_copy.begin();
  while (s21_it != s21_set.end()) {
    EXPECT_EQ(*s21_it, *std_it);
    ++s21_it, ++std_it;
  }
}

TEST(set, SetIteratorsSet) {
  s21::set<std::string> s21_set = {"This", "is", "the", "way"};
  std::set<std::string> std_set = {"This", "is", "the", "way"};
  auto s21_it = s21_set.begin();
  auto std_it = std_set.begin();
  EXPECT_EQ(*std_it, *s21_it);

  if (s21_it != s21_set.begin() && std_it != std_set.begin()) {
    --s21_it;
    --std_it;
    EXPECT_EQ(*std_it, *s21_it);
  }
}

TEST(set, CapacitySet) {
  s21::set<char> s21_empty_set;
  std::set<char> std_empty_set;
  EXPECT_EQ(s21_empty_set.empty(), std_empty_set.empty());
  EXPECT_EQ(s21_empty_set.size(), std_empty_set.size());
  s21_empty_set.insert('b');
  std_empty_set.insert('c');
  EXPECT_EQ(s21_empty_set.empty(), std_empty_set.empty());
  EXPECT_EQ(s21_empty_set.size(), std_empty_set.size());
}

TEST(set, ClearSet) {
  s21::set<char> s21_empty_set;
  std::set<char> std_empty_set;
  s21_empty_set.clear();
  std_empty_set.clear();
  EXPECT_EQ(s21_empty_set.empty(), std_empty_set.empty());
  EXPECT_EQ(s21_empty_set.size(), std_empty_set.size());
  s21_empty_set.insert('a');
  std_empty_set.insert('b');
  EXPECT_EQ(s21_empty_set.empty(), std_empty_set.empty());
  EXPECT_EQ(s21_empty_set.size(), std_empty_set.size());
  s21_empty_set.clear();
  std_empty_set.clear();
  EXPECT_EQ(s21_empty_set.empty(), std_empty_set.empty());
  EXPECT_EQ(s21_empty_set.size(), std_empty_set.size());
}

TEST(set, InsertSet) {
  s21::set<std::string> s21_set = {"This", "is", "the", "way"};
  std::set<std::string> std_set = {"This", "is", "the", "way"};
  auto s21_pr = s21_set.insert("best");
  auto orig_pr = std_set.insert("best");
  EXPECT_TRUE(s21_pr.second == orig_pr.second);
  EXPECT_EQ(*s21_pr.first, *orig_pr.first);
  s21_pr = s21_set.insert("is");
  orig_pr = std_set.insert("is");
  EXPECT_TRUE(s21_pr.second == orig_pr.second);
  EXPECT_EQ(*s21_pr.first, *orig_pr.first);
}

TEST(set, EraseSet) {
  s21::set<int> s21_set = {5, 4, 3, 2, 7, 8, 9};
  std::set<int> std_set = {5, 4, 3, 2, 7, 8, 9};
  auto size = s21_set.size();
  s21_set.erase(s21_set.end());
  auto new_size = s21_set.size();
  EXPECT_EQ(size, new_size);
  s21_set.erase(s21_set.begin());
  std_set.erase(std_set.begin());
  auto s21_it = s21_set.begin();
  auto std_it = std_set.begin();
  while (s21_it != s21_set.end()) {
    EXPECT_EQ(*s21_it, *std_it);
    ++s21_it, ++std_it;
  }
}

TEST(set, swap) {
  s21::set<char> set1 = {'a', '2', 'c', '4'};
  std::set<char> set2 = {'a', '2', 'c', '4'};
  s21::set<char> a;
  std::set<char> b;
  set1.swap(a);
  set2.swap(b);
  EXPECT_EQ(set1.size(), set2.size());
  EXPECT_EQ(a.size(), b.size());
  auto s21_it = a.begin();
  auto std_it = b.begin();
  while (std_it != b.end()) {
    EXPECT_EQ(*s21_it, *std_it);
    ++s21_it, ++std_it;
  }
}

TEST(set, MergeSet) {
  s21::set<int> s21_set = {1};
  s21::set<int> s21_merge_set = {3, 4, 5};
  s21_set.merge(s21_merge_set);
  std::set<int> std_set = {1};
  std::set<int> std_merge_set = {3, 4, 5};
  std_set.merge(std_merge_set);
  auto s21_it = s21_set.begin();
  auto std_it = std_set.begin();
  while (s21_it != s21_set.end()) {
    EXPECT_EQ(*s21_it, *std_it);
    ++s21_it, ++std_it;
  }
  EXPECT_EQ(std_set.size(), s21_set.size());
  EXPECT_EQ(s21_merge_set.size(), std_merge_set.size());
}

TEST(set, FindSet) {
  s21::set<double> s21_set = {2.1, 2.2, 2.3, 2.4, 2.5, 2.6};
  std::set<double> std_set = {2.1, 2.2, 2.3, 2.4, 2.5, 2.6};
  auto s21_it = s21_set.find(2.4);
  auto std_it = std_set.find(2.4);
  EXPECT_TRUE(*std_it == *s21_it);
}

TEST(set, ContainsSet) {
  s21::set<double> s21_set = {2.1, 2.2, 2.3, 2.4, 2.5, 2.6};
  std::set<double> std_set = {2.1, 2.2, 2.3, 2.4, 2.5, 2.6};
  EXPECT_EQ(s21_set.contains(2), std_set.find(2) != std_set.end());
  EXPECT_EQ(s21_set.contains(2.1), std_set.find(2.1) != std_set.end());
}

TEST(set, InsertMany) {
  s21::set<int> s21_set;
  s21::vector<std::pair<s21::set<int>::iterator, bool>> results =
      s21_set.insert_many(1, 2, 3, 4, 5);
  EXPECT_EQ(results.size(), 5);
  EXPECT_TRUE(results[0].second);
  EXPECT_TRUE(results[1].second);
  EXPECT_TRUE(results[2].second);
  EXPECT_TRUE(results[3].second);
  EXPECT_TRUE(results[4].second);
  EXPECT_EQ(*results[0].first, 1);
  EXPECT_EQ(*results[1].first, 2);
  EXPECT_EQ(*results[2].first, 3);
  EXPECT_EQ(*results[3].first, 4);
  EXPECT_EQ(*results[4].first, 5);
}