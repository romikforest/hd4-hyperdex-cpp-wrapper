# hd4
hyperdex database c++ wrapper library

I think I will not use or support this code :(
It was working when I had written it.

It provides easy access to the hyperdex database (http://hyperdex.org/) from c++ code.
You can use standard vectors, arrays, tuples and so on as data containers.
Some extended type support was added.
You can serialize any object using boost serialization library.
Google macaroons was supported.

I have no time to continue this project and now I want it to be open source.
May be it will help to somebody.

The main code for hyperdex c++ wrapper is under the src/common/hyperdex_cpp 
It uses src/common/forest

Some third party code may be used:
(ltalloc, lockless allocator, boost experimental expected and functional,
google macaroons, sparsehash, btree and so on)

This project includes modified version of date.h by Howard Hinnant (MIT Licensed)
The license text is included in file src/common/forest/data.h

Other third party components must be downloaded separately.

The project is not issued for compilation as a library, it has empty main.cpp file, where you can call tests.
I'm sorry the code is not properly issued.

Some tests may not work as API has changed due the development.

I have not tested this code with actual hyperdex API.
