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

.PHONY: clean
clean:
	rm -rf build
	rm -rf bin

