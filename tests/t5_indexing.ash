println "abcd"[1]
println "abcdef"[5]
println "abcde"[1:3]
println "abcdef"[2:-2]

println [1,2,3,4,5][2]
println ["abc", "def", "ghi", "jkl"][1:3]
println [3.5, 4.6, 8.9, 9.5, 10.0][-3]

/* expected output:
a
f
bc
cde
3
[def, ghi]
8.9
*/