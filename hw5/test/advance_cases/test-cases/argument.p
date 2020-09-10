//&S-
//&T-
//&D-

argument;

var gv: integer;
var gc: 2;

sum(a,b,c,d,e,f,g,h,i: integer): integer
begin
	var result: integer;
	result := a + b + c + d + e + f + g + h + i;
	return result;
end
end

begin

var lv: integer;
var lc: 2;

gv := 2;
lv := 2;

gv := sum(gv, gc, lv, lc, gv, gc, lv, lc, gv);

print gv;

end
end