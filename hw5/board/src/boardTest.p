//&S-
//&T-
//&D-

boardTest;

var globalVar: integer;
var globalConst: 3;

init();
testLocal(passIn: integer);
testGlobal(passIn: integer);
testGlobalConst(passIn: integer);
testExpression(passIn: integer);
testFunction(passIn: integer);
testExprFunc(passIn: integer);
testCondition(passIn: integer);
testFor(passIn: integer);
testWhile(passIn: integer);
showResult();

sum(x1, x2: integer): integer
begin
	var result: integer;
	result := x1 + x2;
	return result;
end
end

begin

var localVar: integer;
localVar := 1;
globalVar := 2;

init();

testLocal(localVar);
testGlobal(globalVar);
testGlobalConst(globalConst);
testExpression(localVar + globalConst);
testFunction(sum(globalVar, globalConst));
testExprFunc(localVar + sum(globalVar, globalConst));

if ( localVar >= 1 ) then  
begin
    testCondition(7);
end
else
begin
    testCondition(8);
end
end if

for i := 0 to 7 do
begin
    localVar := localVar + 1;
end
end do

testFor(localVar);

while globalVar < 10 do
begin
    globalVar := globalVar + 1;
end
end do

testWhile(globalVar);

showResult();

end
end
