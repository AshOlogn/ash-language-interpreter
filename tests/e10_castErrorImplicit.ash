fun foo(double x) -> double {
    return x*x
}

int y = foo(5.5)
println y

/* Expected output:
StaticCastError on line 5
        int y = foo(5.5)
Cannot implicitly cast from type double to type int32
*/