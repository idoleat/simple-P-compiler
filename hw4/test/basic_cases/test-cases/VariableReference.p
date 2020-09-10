//&T-
VariableReference;

// 5 semantic errors defined in Variable Reference

func(a: integer)
begin
    var arr: array 30 of integer;
    var err: array 0 of integer;

    // [ERROR] symbol not found
    print arrr[30];

    // [ERROR] symbol not variable
    print func[30];

    // [ERROR] error in declaration
    print err[1.0];

    // [ERROR] index not integer type
    print arr[1.0];

    // [ERROR] over subscript
    print a[40][50][60];
end
end

begin
    func(10);
end
end
