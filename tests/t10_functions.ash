fun foo(int x) -> int {
    return x*x
}

println foo(-2)
println foo(-1)
println foo(0)
println foo(1)
println foo(2)
println foo(3)
println foo(4)

//example of recursion
fun fibonacci(int x) -> int {
    if(x == 0 || x == 1)
        return x
    return fibonacci(x-1) + fibonacci(x-2)
}

string sequence = ""
for(int x = 0; x < 10; x++) {
    sequence += fibonacci(x) + " "
}

println(sequence)

/* Expected output:
4
1
0
1
4
9
16
0 1 1 2 3 5 8 13 21 34
*/


