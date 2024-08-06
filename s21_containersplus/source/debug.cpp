#include <iostream>

#include "s21_array.h"

int main() {
  s21::array<int, 5> my_array = {1, 2, 3, 4, 5};

  std::cout << "Initial array: ";
  for (auto it = my_array.begin(); it != my_array.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  std::cout << "Front element: " << my_array.front() << std::endl;
  std::cout << "Back element: " << my_array.back() << std::endl;

  my_array.fill(10);
  std::cout << "After fill(10): ";
  for (auto it = my_array.begin(); it != my_array.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  s21::array<int, 5> another_array = {5, 4, 3, 2, 1};
  my_array.swap(another_array);
  std::cout << "After swap with another_array: ";
  for (auto it = my_array.begin(); it != my_array.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  return 0;
}
