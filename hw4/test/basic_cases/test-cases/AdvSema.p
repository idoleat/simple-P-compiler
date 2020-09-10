//&T-
//&D-

AdvSema;

var global : integer;

procedure1()
begin
end
end

procedure2(a: integer)
begin
end
end

// [ERROR] redeclaration
global()
begin
end
end

extern() : integer;

// [ERROR] each parameter has a wrong declaration
parameterErrors(a, b, c, d, e, f : array 30 of array 0 of integer);

begin
    var int : integer; 
    var arr : array 30 of real;
    var i: string;

    // [ERROR] must be scalar type
    print procedure1();

    // [ERROR] only errors of invocation
    print null();
    print int("don't judge me");
    print procedure1("overflow");
    print procedure2("I'm string");

    // [ERROR] only error of operation
    print arr[1.0 and 2.0];

    for i := 19 to 22 do
    begin
        var str : string;

        for str := 19 to 89 do
        begin
        end
        end do

        // [ERROR] inner scope but i is loop_var
        for i := 19 to 25 do
        begin
        end
        end do

        // [ERROR] wrong operands
        print i and str;
    end
    end do

    // [ERROR] not a function id
    global();

    procedure2(extern());
end
end
