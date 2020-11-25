GTEST_DIR = externals/gtest
GTEST_LIB = googletest/build/libgtest.a

ut: $(GTEST_DIR)/$(GTEST_LIB)
	g++ test/ut/main.cpp

.ONESHELL:
$(GTEST_DIR)/$(GTEST_LIB):
	cd $(GTEST_DIR)
	make
	cd -

all: build/pieces.o build/board.o build/NotationConversions.o build/CheckChecker.o Board/Board.hpp main.cpp
	g++ main.cpp build/pieces.o build/board.o build/NotationConversions.o build/CheckChecker.o -o chess-backend -I./Board

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
