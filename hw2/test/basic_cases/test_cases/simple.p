//&T-
//test variable_reference := expression
simple;

begin
    var a : integer;
    var b : integer;
    var c : integer;
    a := 1;
    b := a + 2;
    c := a * b - 1;
    a := c * 3 / 3 + 4;
    b := a;
    c := -5;
end
end
