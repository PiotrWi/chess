.PHONY: all
all:
	cmake -H. -Bbuild $(CMAKE_ARGS)
	cmake --build build/

.PHONY: applications
applications:
	cmake -H. -Bbuild $(CMAKE_ARGS)
	cmake --build build/ --target applications

.PHONY: ut
ut:
	cmake -H. -Bbuild $(CMAKE_ARGS)
	cmake --build build/ --target ut

.PHONY: ut-run
ut-run:
	cmake -H. -Bbuild $(CMAKE_ARGS)
	cmake --build build/ --target ut-run

.PHONY: pt 
pt:
	cmake -H. -Bbuild $(CMAKE_ARGS)
	cmake --build build/ --target pt

.PHONY: pt-run
pt:
	cmake -H. -Bbuild $(CMAKE_ARGS)
	cmake --build build/ --target pt

.PHONY: clean
clean:
	rm -rf build
	rm -rf bin
