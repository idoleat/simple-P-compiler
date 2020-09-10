//&S-
//&T-
//&D-

specExample;

var a: integer;
var d: 4;

sum(a,b: integer): integer
begin
	var c: integer;
	c := a + b;
	return c;
end
end

begin

var b, c: integer;
b := 4;
c := 6;

read a;
print a;

a := sum(b, d);
print a;

a := (b + c) * d;
print a;

if ( a <= 40 ) then  
begin
    print a;
end
else
begin
    print b;
end
end if

while b < 8 do
begin
    print b;
    b := b + 1;
end
end do

end
end
