MAKEFILE_DIR = $(CURDIR)

.PHONY: all
all:
	cmake -H. -Bbuild $(CMAKE_ARGS)
	cmake --build build/ --parallel

.PHONY: applications
applications:
	cmake -H. -Bbuild $(CMAKE_ARGS)
	cmake --build build/ --target applications --parallel

.PHONY: ut
ut:
	cmake -H. -Bbuild $(CMAKE_ARGS)
	cmake --build build/ --target ut --parallel

.PHONY: ut-run
ut-run: ut
	ctest -R ut-run --test-dir build --output-on-failure

.PHONY: pt 
pt:
	cmake -H. -Bbuild $(CMAKE_ARGS)
	cmake --build build/ --target pt --parallel

.PHONY: pt-run
pt-run: pt
	ctest -R pt-run --test-dir build --output-on-failure

.PHONY: strenght_comparator-run
strenght_comparator-run: applications
	CHESS_BIN_DIR=${MAKEFILE_DIR}/bin python3 bin/python/StrenghtComparator.py

.PHONY: coefficient_adjuster-run
coefficient_adjuster-run: applications
	CHESS_BIN_DIR=${MAKEFILE_DIR}/bin python3 bin/python/CoefficientAdjuster.py

.PHONY: clean
clean:
	rm -rf build
	rm -rf bin

