//&T-
ReadStatement;
begin
    var arr: array 10 of array 90 of integer;
    var constant: 010;

    read i;
    read arr[1][3][2];

    // [ERROR] must be scalar type
    read arr[1];

    // [ERROR] cannot be constant or loop
    read constant;
    for i := 10 to 40 do
    begin
        read i;
    end
    end do
end
end
