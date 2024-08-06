#include <iostream>

#include "s21_list.h"
#include "s21_queue.h"
#include "s21_stack.h"

int main() {
  s21::stack<int> my_stack;

  std::cout << "Initial stack (empty): ";
  while (!my_stack.empty()) {
    std::cout << my_stack.top() << " ";
    my_stack.pop();
  }
  std::cout << std::endl;

  my_stack.insert_many_front(1, 2, 3);
  std::cout << "After insert_many_front(1, 2, 3): ";
  s21::stack<int> temp_stack =
      my_stack;  // Для вывода элементов без изменения оригинального стека
  while (!temp_stack.empty()) {
    std::cout << temp_stack.top() << " ";
    temp_stack.pop();
  }
  std::cout << std::endl;

  my_stack.insert_many_front(4, 5, 6);
  std::cout << "After insert_many_front(4, 5, 6): ";
  temp_stack =
      my_stack;  // Для вывода элементов без изменения оригинального стека
  while (!temp_stack.empty()) {
    std::cout << temp_stack.top() << " ";
    temp_stack.pop();
  }
  std::cout << std::endl;

  my_stack.insert_many_front(7, 8, 9);
  std::cout << "After insert_many_front(7, 8, 9): ";
  temp_stack =
      my_stack;  // Для вывода элементов без изменения оригинального стека
  while (!temp_stack.empty()) {
    std::cout << temp_stack.top() << " ";
    temp_stack.pop();
  }
  std::cout << std::endl;

  std::cout << "head: " << my_stack.top() << std::endl;

  return 0;
}
