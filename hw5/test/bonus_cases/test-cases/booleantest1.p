//&S-
//&T-
//&D-

booleantest1;

larger(a, b: integer): boolean
begin
	var result: boolean;
	result := a > b;
	return result;
end
end

test(a: integer; b: boolean): integer
begin
    if( b ) then
    begin
        return a;
    end
    else
    begin
        return 0;
    end
    end if
end
end

begin

var a, b : integer;
a := 10;
b := 5;

if( larger(a, b) ) then
begin
    print a;
end
else
begin
    print b;
end
end if

a := test(a, false);
print a;

end
end
