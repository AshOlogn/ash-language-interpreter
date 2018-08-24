int x = 5
if(x > 6)
    println "first"
elif x == 5
    println "second"

string y = "abcd"
if(y == "abcd")
    println y+y
else
    println y*3

//none should execute in this case
string z = "zzzzz"
if(z < "ZZZZZZ")
    println "this should not print"
elif z > "zzzzzzzzzzzzzzzzz"
    println "neither should this"

/* Expected output:
second
abcdabcd
*/