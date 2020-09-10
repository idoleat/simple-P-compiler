//&S-
//&T-
//&D-

arraytest2;

sum(a: array 3 of array 3 of integer): integer
begin
	var result: integer;
	result := a[1][2] + a[2][1];
	return result;
end
end

begin

var a : array 3 of array 3 of integer;
a[1][2] := 5;
a[2][1] := 10;
print a[1][2];
print a[2][1];
print sum(a);
read a[0][0];
print a[0][0];

if( a[1][2] > a[2][1] ) then
begin
    print a[1][2];
end
else
begin
    print a[2][1];
end
end if

end
end
