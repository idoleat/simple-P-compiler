//&T-
assignment;

begin
    var const : 10;
    var arr : array 20 of array 777 of real;
    var float : real;

    ff := arr[12][2];

    // [ERROR] array assignment (variable reference)
    arr := 1.23;

    // [ERROR] const assignment
    const := "SSLAB";

    // [ERROR] loop variable assignment
    for i := 1 to 30 do
    begin
        i := 213;
    end
    end do

    const := "NO.1" + float;

    // [ERROR] array assignment (expression)
    const := arr;
    float := arr[10];

    // [ERROR] type mismatch
    arr[0][23] := false;
    float := "string";

    float := 4210;

end
end
