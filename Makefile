GTEST_DIR = externals/gtest
GTEST_LIB = googletest/build/libgtest.a

ut: $(GTEST_DIR)/$(GTEST_LIB)
	g++ test/ut/main.cpp

.ONESHELL:
$(GTEST_DIR)/$(GTEST_LIB):
	cd $(GTEST_DIR)
	make
	cd -

all: pieces.o board.o
	g++ main.cpp pieces.o board.o -o chess-backend -I./Board

pieces.o:
	g++ -c Board/Pieces.cpp -I./Board -o pieces.o

board.o:
	g++ -c Board/Board.cpp -I./Board -o board.o
clean:
	rm chess-backend
