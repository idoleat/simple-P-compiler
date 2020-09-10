//&T-
FunctionInvocation;

func() : boolean
begin
    return true;
end
end

func2(a: integer; b: string)
begin
end
end

func3(a: integer; b: string; c: array 4 of integer)
begin
end
end

begin
    var arr: array 7 of integer;

    // [ERROR] identifier has to be in symbol tables
    null(123, 456);

    // [ERROR] kind of symbol has to be function
    arr(123, 456);

    // [ERROR] # of arguments isn't the same as parameters
    func("overflow");

    // [CORRECT]
    func2(123, "456");

    // [ERROR] incompatible argument and parameter types
    func2("123", "456");

    // [ERROR] incompatible argument and parameter types
    func2(123, 456);

    // [ERROR] incompatible argument and parameter types
    func3(123, "456", arr);
end
end
