fun foo(char x) {
    return x
}

println foo('B')

/* Expected output:
ParseSyntaxError error on line 1:
                fun foo(char x) {
with element {
Expected -> followed by return type
*/