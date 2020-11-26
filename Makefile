GTEST_DIR = externals/gtest
GTEST_LIB = googletest/build/lib/libgtest.a
OBJ_FILES = build/pieces.o build/board.o build/NotationConversions.o build/CheckChecker.o

ut-run: ut
	./ut

ut: $(GTEST_DIR)/$(GTEST_LIB) build/ut/CheckCheckerTest.o
	g++ test/ut/main.cpp build/ut/CheckCheckerTest.o ${OBJ_FILES} $(GTEST_DIR)/$(GTEST_LIB) -lpthread -I./$(GTEST_DIR)/googletest/googletest/include/ -o ut

build/ut/CheckCheckerTest.o: test/ut/CheckCheckerTest.cpp build/CheckChecker.o
	g++ -c -I./$(GTEST_DIR)/googletest/googletest/include/ -I./Board test/ut/CheckCheckerTest.cpp -o build/ut/CheckCheckerTest.o

.ONESHELL:
$(GTEST_DIR)/$(GTEST_LIB):
	cd $(GTEST_DIR)
	make
	cd -

all: build/pieces.o build/board.o build/NotationConversions.o build/CheckChecker.o Board/Board.hpp main.cpp
	g++ main.cpp ${OBJ_FILES} -o chess-backend -I./Board

build/pieces.o: Board/Board.hpp Board/Pieces.cpp Board/Notation.hpp
	g++ -c Board/Pieces.cpp -I./Board -o build/pieces.o

build/board.o: Board/Board.cpp Board/Board.hpp Board/Notation.hpp
	g++ -c Board/Board.cpp -I./Board -o build/board.o

build/NotationConversions.o: Board/NotationConversions.cpp Board/NotationConversions.hpp Board/Notation.hpp
	g++ -c Board/NotationConversions.cpp -I./Board -o build/NotationConversions.o

build/CheckChecker.o: Board/CheckChecker.cpp Board/CheckChecker.hpp
	g++ -c Board/CheckChecker.cpp -I./Board -o build/CheckChecker.o

clean:
	rm chess-backend build/*.o
