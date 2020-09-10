//&S-
//&T-
//&D-

arraytest1;

sum(a: array 10 of integer): integer
begin
	var result: integer;
	result := a[1] + a[2];
	return result;
end
end

begin

var a : array 10 of integer;
a[1] := 10;
a[2] := 5;
print a[1];
print a[2];
print sum(a);
read a[3];
print a[3];

if( a[1] > a[2] ) then
begin
    print a[1];
end
else
begin
    print a[2];
end
end if

end
end
