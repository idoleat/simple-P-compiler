//&S-
//&T-
//&D-

realtest2;

sum(a, b: real): real
begin
	var result: real;
	result := a + b;
	return result;
end
end

begin

var a, b : real;
a := 1.1;
b := 2.2;
print sum(a, b);

if( a > b ) then
begin
    print a;
end
else
begin
    print b;
end
end if

end
end
