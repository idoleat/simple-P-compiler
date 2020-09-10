//&T-
redecl;

var funcSymbolRedecl : integer;

// [ERROR] redeclaration
funcSymbolRedecl()
begin
end
end


funcShareScopeWithCompound(p1 : integer)
begin
    // [ERROR] redeclaration
    var p1 : string;
end
end

hideDecl(p1 : integer)
begin
    begin
        // [PASS] p1 in inner scope
        var p1 : string;
    end
end
end

forNode()
begin
    for i := 19 to 22 do
    begin
        // [ERROR] inner scope but i is loop_var
        var i : real;

        // [ERROR] inner scope but i is loop_var
        for i := 19 to 25 do
        begin
        end
        end do
    end
    end do
end
end

begin
end
end
