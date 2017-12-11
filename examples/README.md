## Using a plugin

A onemax function has been implemented in C in the file `onemax.c`. To
compile it, from the build direcory, cd to `examples/` and

    make plugin
    
Then you can dynamically load the onemax function from the library
into hnco with

    hnco -F 1000 --path ./libfoo.so --fn-name onemax

This should work with any C function with the following prototype:

    double fn(const char[], size_t);
