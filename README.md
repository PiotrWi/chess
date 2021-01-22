# Chess
My own implementation of chess engine. It contains:
* UCI application capable to work with xboard.
* Console application to play with.
* small subset of libraties and algorigms implementation

Project is not in stable version. Work on it is still ongoing and it acts as my personal playgound, so it may be possible that some functionalities are brokes since time to time.

# Building
Engine is developed in Linux gcc environment. There are no objection to build it elsewhere other than it was never been tested outside and it would require to some CMAKE's adaptations.

git clone
mkdir build
cd build
cmake ../chess
make -j 4

Then, there shall appear:
 * chess console application to play with
 * applications/uci_application UCI capable engine to be loaded by any GUI supported it
