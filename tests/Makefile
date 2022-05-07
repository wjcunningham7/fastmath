COVERAGE	:= coverage.info

all:
	g++ -o fastmath_test fastmath_test.cpp -I ../include/fastmath -O0 -fprofile-arcs -ftest-coverage -fno-inline -fno-inline-small-functions -fno-default-inline -Wno-narrowing -lgtest -lpthread

run: all
	./fastmath_test

coverage: run
	lcov --capture --directory . --output-file=$(COVERAGE)
	lcov --remove $(COVERAGE) '/usr/*' --output-file $(COVERAGE)
	lcov --remove $(COVERAGE) '*/boost/*' --output-file $(COVERAGE)
	lcov --remove $(COVERAGE) '*/tests/*' --output-file $(COVERAGE)
	lcov --list $(COVERAGE)

gcov: run
	gcov -abcfru *.cpp

clean:
	@rm -f fastmath_test *.gcov *.gcda *.gcno $(COVERAGE)
