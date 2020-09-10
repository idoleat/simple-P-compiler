//&S-
//&T-

AssignRead;

// variable reference, assignment, read

begin
    var a: integer;
    var b: boolean;
    var c: string;
    var d: array 1 of array 9 of real;

    a := 10;
    b := false;
    c := "Compiler Design";
    d[1][5] := 10.09;

    a := a;

    a := a + 231;
    b := not b;
    c := "Compiler Design" + "No.1" + c;
    d[a + 1][-1 + 10] := 12.28;

    read a;
    read d[a * a][2];
end
end
