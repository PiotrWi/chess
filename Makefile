GTEST_DIR = externals/gtest
GTEST_LIB = googletest/build/libgtest.a

all: backend

ut: $(GTEST_DIR)/$(GTEST_LIB)
	gcc test/ut/main.cpp

.ONESHELL:
$(GTEST_DIR)/$(GTEST_LIB):
	cd $(GTEST_DIR)
	make
	cd -

backend: pieces
	gcc main.cpp -o chess-backend -I./Board

pieces:
	gcc Board/Pieces.cpp -I./Board

clean:
	rm chess-backend
