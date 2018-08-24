//showcases the arithmetic evaluation functionality of Ash
//note that order of operations is respected

println 1
println 1 + 2 + 3
println 1 - 2 + 3.5
println 5*3/2
println 5.0*3/2
println 3 ** 5
println 1 + 2*3 - 6
println 5 % 2
println 5.0 % 2.4
println ((5-2) ** (3+2))/2

//now some bitwise operations
println 5 & 3
println 5 | 3
println 5 ^ 3
println 7 & 5 ^ 7 | 1


/* Expected output (double outputs may be a close approximation to expected):
1
6
2.5
7
7.5
243.0   -- note that exponentiation always produces a double
1
1
0.2
121.5
1
7
6
3
*/