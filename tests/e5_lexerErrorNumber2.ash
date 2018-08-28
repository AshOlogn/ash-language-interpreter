int x = 123_a456
println x

/* Expected output:
LexerError on line 1:
        int x = 123_a456
with token 123_a
Number literal can only contain digits and <= 1 decimal point
*/