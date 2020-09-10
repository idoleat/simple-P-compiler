/**
 * relation.p: < <= = >= > <>       simple case(s)
 */
//&T-
relation;

begin

        var a: boolean;
        var b, c: integer;

        a := 1 <> 2;
        a := 101 < 23;
        a := b = 0;
        a := c >= b;
        a := 100 > b;
        a := 56 <= c;
end
end
