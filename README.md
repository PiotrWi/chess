# Chess
My own implementation of chess engine. It contains:
* UCI application capable to work with xboard.
* Console application to play with.
* small subset of libraties and algorigms implementation

Project is not in stable version. Work on it is still ongoing and it acts as my personal playgound, so it may be possible that some functionalities are brokes since time to time.

# Building
Engine is developed in Linux gcc environment. Whole build can be done via makefile wrapper or by cmake.

## Applications build
make applications

## running uts
make ut-run

## running mts
make mt-run

## debug targets
make ${TARGET_NAME} CMAKE_ARGS='-DCMAKE_BUILD_TYPE=Debug'

Then, there shall appear:
 * chess console application to play with
 * applications/uci_application UCI capable engine to be loaded by any GUI supported it
