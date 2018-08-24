int x = 5
println x + 3
println x*x*x

int y = (int) -5.5
println y + "abc"

string z = "abcdef" * 3
println z[2:10]

int a = 5
a *= 10
println a

a /= 2
println a

/* Expected output:
8
125
-5abc
cdefabcd
50
25
*/