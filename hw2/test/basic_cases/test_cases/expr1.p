/**
 * expr1.p: expression
 */
//&T-
expr1;

foo( a, b: integer ): integer
begin
	return a*b;
end
end

begin

        var a, b: integer;
        var L1darray: array 4 of integer;
       
        a := 2;
        b:= 3;
        L1darray[0] := 123;
	  
        print 1+2*(3+4);

        if a*3+1 <> b*4 mod 123 then
        begin
                print "test";
        end
        end if
	
	print foo(2*4,3) * a * L1darray[0];

end
end
