//&T-
adv;

var redecl : integer;

redecl(p1 : integer)
begin
    // [ERROR] redeclaration
    var p1 : real;
    var aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,
        aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa : integer;
end
end

hideDecl(p1 : integer) : string
begin
    begin
        // hide p1 of integer version
        var p1 : string;

        begin
            // [PASS] p1 of integer version should be hidden
            return p1;
        end

    end
    
    // [ERROR] return type is string
    return p1;
end
end

skipCheckOfRedecl(p1 : string) : integer
begin
    // [ERROR] redeclaration
    var p1 : integer;

    // [ERROR] p1 is string
    return p1;
end
end

begin
end
end
