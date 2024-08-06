#include <iostream>

#include "s21_map.h"

int main() {
  s21::map<int, std::string> my_map;
  my_map.insert({1, "one"});
  my_map.insert({2, "two"});
  my_map.insert({3, "three"});

  std::cout << my_map.at(1) << std::endl;
  my_map.print();

  my_map[4] = "four";
  std::cout << "my_map:" << std::endl;
  std::cout << my_map[1] << std::endl;
  std::cout << my_map[2] << std::endl;
  std::cout << my_map[3] << std::endl;
  std::cout << my_map[4] << std::endl;

  if (my_map.empty()) {
    std::cout << "my_map is empty" << std::endl;
  } else {
    std::cout << "Size of my_map: " << my_map.size() << std::endl;
  }
  my_map.insert(5, "five");
  my_map.print();

  std::cout << "merge" << std::endl;
  s21::map<int, std::string> my_map2 = {{2, "new_two"}, {3, "lfg"}, {6, "six"}};
  my_map.merge(my_map2);
  my_map.print();

  my_map.insert_or_assign(5, "six");
  my_map.print();

  auto pr1 = my_map.insert_or_assign(1, "b");

  std::cout << *pr1.first << " " << pr1.second << std::endl;

  std::cout << "Copied map:" << std::endl;
  s21::map<int, std::string> map2 = my_map;
  map2.print();

  return 0;
}