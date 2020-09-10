//&T-
PrintStatement;
begin
    var arr: array 10 of array 90 of integer;

    print arr[1][2][3];

    print arr[1][2];

    // [ERROR] must be scalar
    print arr[1];
    print arr;
end
end
