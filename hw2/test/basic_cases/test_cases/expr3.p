/**
 * expr3.p: mixed expression(s)
 */
//&T-
expr3;

begin
        var a, b: integer;
        var d: real;
        var e: boolean;

        e := 1+2*3 < a;
        e := a >= 48 and a <= 57;
        e := a+b < d;
        e := d*23.4 >= 100.0;
        e := (1+2)*(3+4) <> a;

end
end
