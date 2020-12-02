GTEST_DIR = externals/gtest
GTEST_LIB = googletest/build/lib/libgtest.a
OBJ_FILES = build/pieces.o build/board.o build/NotationConversions.o build/CheckChecker.o build/MoveValidator.o

ut-run: ut
	./ut

ut: $(GTEST_DIR)/$(GTEST_LIB) build/ut/CheckCheckerTest.o build/ut/MoveValidatorTest.o
	g++ test/ut/main.cpp build/ut/CheckCheckerTest.o build/ut/MoveValidatorTest.o ${OBJ_FILES} $(GTEST_DIR)/$(GTEST_LIB) -lpthread -I./$(GTEST_DIR)/googletest/googletest/include/ -o ut

build/ut/CheckCheckerTest.o: test/ut/CheckCheckerTest.cpp build/CheckChecker.o
	g++ -c -I./$(GTEST_DIR)/googletest/googletest/include/ -I./Board -I./test/ut/  test/ut/CheckCheckerTest.cpp -o build/ut/CheckCheckerTest.o

build/ut/MoveValidatorTest.o: test/ut/MoveValidatorTest.cpp build/MoveValidator.o
	g++ -c -I./$(GTEST_DIR)/googletest/googletest/include/ -I./Board -I./test/ut/ test/ut/MoveValidatorTest.cpp  -o build/ut/MoveValidatorTest.o

.ONESHELL:
$(GTEST_DIR)/$(GTEST_LIB):
	cd $(GTEST_DIR)
	make
	cd -

all: ${OBJ_FILES} Board/Board.hpp main.cpp
	g++ main.cpp ${OBJ_FILES} -o chess-backend -I./Board

build/pieces.o: Board/Board.hpp Board/Pieces.cpp Board/Notation.hpp
	g++ -c Board/Pieces.cpp -I./Board -o build/pieces.o

build/board.o: Board/Board.cpp Board/Board.hpp Board/Notation.hpp
	g++ -c Board/Board.cpp -I./Board -o build/board.o

build/NotationConversions.o: Board/NotationConversions.cpp Board/NotationConversions.hpp Board/Notation.hpp
	g++ -c Board/NotationConversions.cpp -I./Board -o build/NotationConversions.o

build/CheckChecker.o: Board/CheckChecker.cpp Board/CheckChecker.hpp
	g++ -c Board/CheckChecker.cpp -I./Board -o build/CheckChecker.o

build/MoveValidator.o: Board/MoveValidator.cpp Board/MoveValidator.hpp
	g++ -c Board/MoveValidator.cpp -I./Board -o build/MoveValidator.o


clean:
	rm chess-backend build/*.o
