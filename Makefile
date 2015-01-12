JASON_DEBUG ?=

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L.
CPPFLAGS = -Iinclude

ifeq ($(JSON_DEBUG),1)
CXXFLAGS += -g -O0
else
CXXFLAGS += -O2
endif

PREFIX ?= /usr/local

DEPS = cppunit

CXXFLAGS += $(shell pkg-config --cflags $(DEPS)) -fPIC
LIBS = -Wl,--as-needed $(shell pkg-config --libs $(DEPS))

TARGET = libjsoncc.so
TESTS = test_jsoncc

SRC = $(wildcard src/*.cc)
OBJ = $(SRC:%.cc=%.o)

TEST_SRC = $(wildcard tests/*.cc)
TEST_OBJ = $(TEST_SRC:%.cc=%.o)
TEST_LIB = libjsoncc.a

ALL_OBJ = $(OBJ) $(TEST_OBJ)
GCNO = $(ALL_OBJ:%.o=%.gcno)
GCDA = $(ALL_OBJ:%.o=%.gcda)

all: $(TARGET) $(TESTS)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS) -shared $(LIBS)

$(TEST_LIB): $(OBJ)
	ar rcs $@ $^

$(TESTS): $(TEST_LIB) $(TEST_OBJ)
	$(CXX) -o $@ $(TEST_OBJ) $(TEST_LIB) $(LDFLAGS) $(LIBS)

run_tests: $(TESTS)
	./$(TESTS)

run_valgrind: $(TESTS)
	LD_LIBRARY_PATH=. valgrind --leak-check=full ./$(TESTS)

run_gdb: $(TESTS)
	LD_LIBRARY_PATH=. gdb ./$(TESTS)

install: all
	install -d $(PREFIX)/lib
	install -m 755 $(TARGET) $(PREFIX)/lib/
	install -d $(PREFIX)/include/
	install -m 644 include/*.h $(PREFIX)/include/

clean:
	rm -rf $(OBJ) $(TARGET) $(TEST_OBJ) $(TESTS) $(TEST_LIB) $(GCNO) $(GCDA)

.PHONY: all clean
