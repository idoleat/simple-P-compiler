/**
 * declErr.p: local variable declaration after statement(s)
 */
//&T-
declErr;

var globalA: integer;

begin

        globalA := 123;

        var b: integer;

        b := globalA*globalA;

end
end
