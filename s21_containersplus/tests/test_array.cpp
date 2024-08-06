#include <gtest/gtest.h>
#include <array>
#include "../s21_containersplus.h"

TEST(ArrayMemberFunctions, TestDefault) {
  s21::array<int, 5> s21_array;
  std::array<int, 5> std_array = {0};
  EXPECT_EQ(s21_array.size(), std_array.size());
  EXPECT_EQ(s21_array.empty(), std_array.empty());
}

TEST(ArrayMemberFunctions, TestInitializer) {
  s21::array<int, 4> s21_array = {1, 2, 3, 4};
  std::array<int, 4> std_array = {1, 2, 3, 4};
  EXPECT_EQ(s21_array.front(), std_array.front());
  EXPECT_EQ(s21_array.back(), std_array.back());
  EXPECT_EQ(s21_array.size(), std_array.size());
  EXPECT_EQ(s21_array.empty(), std_array.empty());
}

TEST(ArrayMemberFunctions, TestCopy) {
  s21::array<char, 4> s21_array_1 = {'a', 'b', 'c', 'd'};
  std::array<char, 4> std_array_1 = {'a', 'b', 'c', 'd'};
  s21::array<char, 4> s21_array_2 = s21_array_1;
  std::array<char, 4> std_array_2 = std_array_1;

  EXPECT_EQ(s21_array_1.size(), s21_array_2.size());
  EXPECT_EQ(std_array_1.size(), std_array_2.size());
  auto it1 = s21_array_1.begin();
  auto it2 = s21_array_2.begin();
  while (it1 != s21_array_1.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
}

TEST(ArrayMemberFunctions, TestMove) {
  s21::array<char, 4> s21_array_1 = {'a', 'b', 'c', 'd'};
  std::array<char, 4> std_array_1 = {'a', 'b', 'c', 'd'};
  s21::array<char, 4> s21_array_2 = std::move(s21_array_1);
  std::array<char, 4> std_array_2 = std::move(std_array_1);

  EXPECT_EQ(s21_array_2.size(), std_array_2.size());
  EXPECT_EQ(s21_array_1.size(), std_array_1.size());
  EXPECT_EQ(s21_array_1.empty(), std_array_1.empty());
  auto it1 = s21_array_2.begin();
  auto it2 = std_array_2.begin();
  while (it1 != s21_array_2.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
}

TEST(ArrayElementAccess, TestAt) {
  s21::array<int, 5> s21_array = {1, 2, 3, 4, 5};
  std::array<int, 5> std_array = {1, 2, 3, 4, 5};

  EXPECT_EQ(s21_array.at(0), std_array.at(0));
  EXPECT_EQ(s21_array.at(1), std_array.at(1));
  EXPECT_EQ(s21_array.at(2), std_array.at(2));
  EXPECT_EQ(s21_array.at(3), std_array.at(3));
  EXPECT_EQ(s21_array.at(4), std_array.at(4));
}

TEST(ArrayElementAccess, TestBrackets) {
  s21::array<int, 3> s21_array = {1, 2, 3};
  std::array<int, 3> std_array = {1, 2, 3};
  EXPECT_EQ(s21_array[0], std_array[0]);
  EXPECT_EQ(s21_array[1], std_array[1]);
  EXPECT_EQ(s21_array[2], std_array[2]);
}

TEST(ArrayElementAccess, TestFront) {
  s21::array<int, 3> s21_array = {1, 2, 3};
  std::array<int, 3> std_array = {1, 2, 3};
  EXPECT_EQ(s21_array.front(), std_array.front());
}

TEST(ArrayElementAccess, TestBack) {
  s21::array<int, 3> s21_array = {1, 2, 3};
  std::array<int, 3> std_array = {1, 2, 3};
  EXPECT_EQ(s21_array.back(), std_array.back());
}

TEST(ArrayElementAccess, TestData) {
  s21::array<int, 3> s21_array = {1, 2, 3};
  std::array<int, 3> std_array = {1, 2, 3};

  EXPECT_EQ(*s21_array.data(), *std_array.data());
}

TEST(ArrayIterators, TestBegin) {
  s21::array<int, 3> s21_array = {1, 2, 3};
  std::array<int, 3> std_array = {1, 2, 3};

  EXPECT_EQ(*s21_array.begin(), *std_array.begin());
}

TEST(ArrayIterators, TestEnd) {
  s21::array<int, 3> s21_array = {1, 2, 3};
  std::array<int, 3> std_array = {1, 2, 3};

  EXPECT_EQ(*(s21_array.end() - 1), *(std_array.end() - 1));
}

TEST(ArrayCapacity, TestEmpty) {
  s21::array<int, 3> s21_array = {1, 2, 3};
  std::array<int, 3> std_array = {1, 2, 3};

  EXPECT_EQ(s21_array.empty(), std_array.empty());
}

TEST(ArrayCapacity, TestSize) {
  s21::array<int, 3> s21_array = {1, 2, 3};
  std::array<int, 3> std_array = {1, 2, 3};

  EXPECT_EQ(s21_array.size(), std_array.size());
}

TEST(ArrayCapacity, TestMaxSize) {
  s21::array<int, 3> s21_array = {1, 2, 3};
  std::array<int, 3> std_array = {1, 2, 3};

  EXPECT_EQ(s21_array.max_size(), std_array.max_size());
}

TEST(ArrayModifiers, TestSwap) {
  s21::array<char, 5> s21_array_1 = {'a', 'b', 'c', 'd', 'e'};
  s21::array<char, 5> s21_array_2 = {'f', 'g', 'h', 'i', 'j'};
  std::array<char, 5> std_array_1 = {'a', 'b', 'c', 'd', 'e'};
  std::array<char, 5> std_array_2 = {'f', 'g', 'h', 'i', 'j'};
  s21_array_1.swap(s21_array_2);
  std_array_1.swap(std_array_2);
  auto it1 = s21_array_1.begin();
  auto it2 = s21_array_2.begin();
  auto it3 = std_array_1.begin();
  auto it4 = std_array_2.begin();
  while (it1 != s21_array_1.end()) {
    EXPECT_EQ(*it1, *it3);
    ++it1, ++it3;
  }
  while (it2 != s21_array_2.end()) {
    EXPECT_EQ(*it2, *it4);
    ++it2, ++it4;
  }
  EXPECT_EQ(s21_array_1.size(), std_array_2.size());
  EXPECT_EQ(s21_array_2.size(), std_array_1.size());
}

TEST(ArrayModifiers, TestFill) {
  s21::array<int, 5> s21_array;
  std::array<int, 5> std_array;
  s21_array.fill(7);
  std_array.fill(7);
  for (size_t i = 0; i < s21_array.size(); ++i) {
    EXPECT_EQ(s21_array[i], std_array[i]);
  }
}
