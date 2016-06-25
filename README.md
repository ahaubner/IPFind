# IPFind

Simple library for discovering a network application by broadcasting.

Caution: Mostly untested.

# Dependencies
## Build tools:
- C++14 compiler, tested with...
  - clang 3.8
  - g++ 6.1.1
- QMake

## Libraries
- boost asio
  - boost system

# Build instructions
## Setup
### GNU/Linux
[Ubuntu] Install packages:
```sh
sudo apt-get -y install git build-essential libboost-system-dev qt5-default
```
(qt5-default is needed for qmake. TODO: CMake alternative)

Clone project:
```sh
git clone https://github.com/ahaubner/IPFind.git
```

## Building
### GNU/Linux
```sh
cd IPFind
qmake
make
```

TODO: Windows

# License
Its a zlib like license. See LICENSE file.
