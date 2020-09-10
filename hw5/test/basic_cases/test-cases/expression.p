//&S-
//&T-
//&D-

expression;

var gv: integer;
var gc: 2;

begin

var lv: integer;
var lc: 2;

gv := 2;
lv := 2;

gv := lc + gv + gc + lv;
lv := lc * gv * gc * lv;

print gv;
print lv;

gv := lc + ((gv + gc) * lv);
lv := (lc + (gv + (gc + (lv + (lc + (gv + (gc + (lv + lc))))))));

print gv;
print lv;

end
end
