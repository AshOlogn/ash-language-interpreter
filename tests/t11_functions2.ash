//examples of how functions can be declared inside functions, etc.
fun foo(int x) -> string {

    string y = x + x + "abc"

    fun bar(string y) -> string {
        return 2*y
    }

    return bar(y) 
}
println foo(5)

/* Expected output:
10abc10abc
*/