CC = g++ -std=c++17 -Wall -Werror -Wextra -Wuninitialized -Wsign-conversion -Wshadow -Weffc++ -pedantic -g
DELETE = rm -rf
TEST_FLAGS =  -lgtest -pthread

FIND_VALGRIND_ERRORS = grep --color=always -n "ERROR SUMMARY" $(VALGRIND_OUTPUT) | grep -v "0 errors from 0 contexts" || true
FIND_VALGRIND_LEAKS = grep --color=always -n "definitely lost:" $(VALGRIND_OUTPUT) | grep -v "0 bytes in 0 blocks" || true
VALGRIND = valgrind --log-file="$(VALGRIND_OUTPUT)" --track-origins=yes --trace-children=yes --leak-check=full --leak-resolution=med

VALGRIND_OUTPUT = valgrind.txt
LIB = s21_containers.a

TEST_TARGET  = s21_tests

SRC_LIB = source
BUILD_PATH = build
TEST_PATH = tests

SRC_LIB_FILES = $(wildcard $(SRC_LIB)/*.h)
TEST_FILES = $(wildcard $(TEST_PATH)/*.cpp)

SRC_LIB_OBJ = $(patsubst $(SRC_LIB)/%.h, $(BUILD_PATH)/%.o, $(SRC_LIB_FILES))
TEST_OBJ = $(patsubst $(TEST_PATH)/%.cpp, $(BUILD_PATH)/%.o, $(TEST_FILES))

CLANG_FORMAT = clang-format

.PHONY: all test clean style $(CLANG_FORMAT) $(VALGRIND)

all: test

$(BUILD_PATH):
	mkdir $(BUILD_PATH)

$(TEST_TARGET): style $(BUILD_PATH) $(TEST_OBJ)
	$(CC) $(TEST_OBJ) -o $(TEST_TARGET) $(TEST_FLAGS)

$(BUILD_PATH)/%.o: $(SRC_LIB)/%.h
	$(CC) -c $< -o $@

$(BUILD_PATH)/%.o: $(TEST_PATH)/%.cpp $(SRC_LIB_FILES)
	$(CC) -c $< -o $@

$(LIB): style $(BUILD_PATH) $(SRC_LIB_OBJ)
	ar rcv $(LIB) $(SRC_LIB_OBJ)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

debug:
	$(CC) $(SRC_LIB)/*.h $(SRC_LIB)/debug.cpp -o $(SRC_LIB)/debug
	./$(SRC_LIB)/debug

gcov_report:
	$(CC) --coverage $(TEST_FILES) $(TEST_FLAGS) -o report.out
	./report.out
	lcov -t "report" -o report.info --no-external -c -d .
	genhtml -o ./report report.info
	rm -rf *.gcda *.gcno *.info *.out
	open report/index.html

clean:
	$(DELETE) report $(TEST_TARGET) $(BUILD_PATH) $(VALGRIND_OUTPUT) $(LIB) *.gcda *.gcno *.info *.out report.out.dSYM *.o test s21_tests *.txt debug

format:
	cp ../../materials/linters/.clang-format .
	clang-format -i source/*.cpp
	clang-format -i source/*.h
	rm .clang-format

style:
	cp ../../materials/linters/.clang-format .
	clang-format -n source/*.cpp
	clang-format -n source/*.h
	rm .clang-format

valgrind: $(TEST_TARGET)
	$(VALGRIND) ./$(TEST_TARGET) || true
	$(FIND_VALGRIND_ERRORS)
	$(FIND_VALGRIND_LEAKS)

test_build :
	$(CC) source/test.cpp -o test && ./test