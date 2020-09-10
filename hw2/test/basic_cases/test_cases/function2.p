/**
 * function2.p: function 2
 */
//&T-
function2;

// function declaration
sum(a, b: integer): integer;
product(a, b: integer): integer;

// function definition
dot(x1, y1, x2, y2: integer): integer
begin
	var result: integer;
	result := sum(product(x1, y1), product(x2, y2));
	return result;
end
end

begin
        var a : integer;
        var b : integer;

        a := 1;
        b := dot(a, a);
end
end
