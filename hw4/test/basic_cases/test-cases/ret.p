//&T-
ret;

func() : boolean
begin
    var bool : array 10 of array 100 of boolean;

    // [ERROR] incompatible type
    return bool[1];

    // [CORRECT]
    return bool[1][1];

    // [ERROR] incompatible type
    return bool[1][1][1];
end
end

begin
    // [ERROR] program doesn't return a value
    return 10;
end
end
