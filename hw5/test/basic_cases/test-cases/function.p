//&S-
//&T-
//&D-

function;

var gv: integer;
var gc: 2;

product(a,b: integer): integer
begin
	var result: integer;
	result := a * b;
	return result;
end
end

sum(a,b: integer): integer
begin
	var result: integer;
	result := a + b;
	return result;
end
end

dot(x1,y1,x2,y2: integer): integer
begin
	var result: integer;
	result := sum(product(x1, y1), product(x2, y2));
	return result;
end
end

begin

var lv: integer;
var lc: 2;

gv := 2;
lv := 2;

gv := product(gv, gc);
lv := gv + product(lv, lc);

print gv;
print lv;

gv := dot(gv, gc, lv, lc);

print gv;

end
end
