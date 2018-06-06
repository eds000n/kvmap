# KVMap

A simple implementation of a map with multiple access through a RESTful API with cache eviction and swap.

## Prerequisites
* [restbed](https://github.com/Corvusoft/restbed)
* dialog

Restbed can be installed with cmake and you can install dialog with yum, apt-get or brew.

## Build
```
mkdir build
cd build
cmake ..
make
```

## Configuration
The file .config contains the information regarding the server and the port

## Running
Start the server with
```
./KVMap
```
Then use the client for connecting to KVMap
