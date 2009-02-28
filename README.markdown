# Hilbert
#### a multi-threaded program to draw hilbert curves

Compile with
    make clean && make

You may then run the program as follows
    Usage:  ./a1 type order
            type must be A-D.
            order must be 1 through 7.

See [here](http://www.cs.gsu.edu/~skarmakar/cs4320/hilbert.htm) for original assignment.

See [here](http://en.wikipedia.org/wiki/Hilbert_curve) for more explanation of Hilbert Curves. Simply put, hilbert curves are a type of fractal space-filling curves devised by German mathematician David Hilbert in 1891.

This project was done as an assignment for [CSC4320 Operating Systems](http://www.cs.gsu.edu/~skarmakar/csc4320_teaching.html) at [Georgia State University](http://www.cs.gsu.edu/).
This particular implementation of of the Hilbert Curve drawing algorithm is not only recursive, but also multi-threaded. Be careful, every succsessive iteration of the fractal spawns extra threads polynomially. We took down our class server pretty quickly with this assignment :)