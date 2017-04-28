# Cape of Helius

This project is started solely for learning c++, gamedev, cocos2d-x and for creating the most awesome present ever. 
My initial idea was to create a log each day, telling what my though were and why a chose the particular implementation, but unfortunately I forgot to do that many times.
So there are some rants under `/logs` and some random tags in the git history.  
Check the `/gif` directory to see some ingame play.

# Dependencies

At least `cmake make g++ gcc git`.

# Install
`git clone https://username@bitbucket.org/pnikray/capeofhelius`

Add the submodule cocos2d-x, the history of the repo is large (200+ mb), so we only fetch a small amount of it.  
`git submodule update --init --depth 5 src/cocos2d`

Install all other submodules.  
`git submodule update --init --recursive`

Install all the dependencies.  
`python download-deps.py` in the direcory `src/cocos2d`

# Build
1. `mkdir src/build`
2. `cd src/build`
3. `cmake ..`
4. `make`

After you build the project you can find the binary (aka the game) in `src/build/bin`.

# 3rd party stuff

All the tiles are from [kenny](http://kenney.nl/assets) and [the player animations](http://gaurav.munjal.us/Universal-LPC-Spritesheet-Character-Generator) are from Guarav0.

Furthermore [cocos2d-x](http://www.cocos2d-x.org) is used as the game engine.

# Doxygen
There is a doxygen config file which, `doxygen config.doxy` to get the documentation.
