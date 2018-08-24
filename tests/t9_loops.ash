int x
for(x = 0; x < 10; x++) {
    println x
}

for(double y = 0; y < 10.5; y += 2.5) {
    println y
}

int index = 0
while(index < 5) {
    println index*index
    index++
}

//this should not execute
for(int i = 0; i < -1; i+=2) {
    println "should not print"
}

//neither should this
int a = -1
while(a > 5) {
    println "neither should this"
}

/* Expected output:
0
1
2
3
4
5
6
7
8
9
0.0
2.5
5.0
7.5
10.0
0
1
4
9
16
*/