//&T-
BinaryOperator;

decimal(): integer
begin
    return 10;
end
end

octal(): integer
begin
    return 03;
end
end

scientific(): real
begin
    return 1.3e+123;
end
end

begin
    var vDecimal: integer;
    var vReal: 1.0;
    var vScientific: 123e+2;
    var vDecimalArr: array 1 of array 1 of integer;
    var vRealArr: array 1 of array 1 of real;
    var vStringArr: array 1 of array 1 of string;
    var vBoolArr: array 1 of boolean;

    // ------------------------
    // safe area
    // ------------------------

    // arithmetic
    print 03 + 10;
    print decimal() - octal();
    print (1 * 0124) * 1.0;
    print (vDecimal + 1.3e+123) / 03;
    print (vDecimal + scientific()) - vDecimal;

    // mod
    print 10 mod (03 mod vDecimalArr[1][3]);
    print octal() mod vDecimalArr[2][1];

    // boolean
    print true and (false or true);
    print false and true;
    print vBoolArr[3] or vBoolArr[2];

    // relational
    print 03 < 10;
    print decimal() <= octal();
    print 0124 = 1.0;
    print 1.3e+123 >= 03;
    print vDecimal > scientific();
    print vDecimal <> scientific();
    print (vDecimal <> scientific()) and (0124 <= vDecimal);

    // string
    print "SSLAB" + "NO.1";
    print "SSLAB" + (vStringArr[1][2] + "AAAAAAA");

    // ------------------------
    // unsafe area
    // ------------------------

    // arithmetic
    print vDecimalArr[2] + vRealArr[4][2];
    print vRealArr[4][2] - vDecimalArr[2];
    print vBoolArr[4] * vRealArr[3][3];
    print vRealArr[4][2] / vRealArr[2];
    print ("SSLAB" + (vRealArr[4][2] - vDecimalArr[2])) + (vRealArr[4][2] + (true and false));

    // mod
    print vDecimal mod vReal;
    print vReal mod decimal();
    print vReal mod "SSLAB";
    print vScientific mod true;
    print vStringArr[3][3] mod (3 mod 2);

    // boolean
    print 10 and true;
    print false and "SSLAB";
    print vBoolArr or true;

    // relational
    print vDecimalArr[2] < vRealArr[4][2];
    print vRealArr[4][2] = vDecimalArr[2];
    print vBoolArr[4] >= vRealArr[3][3];
    print vRealArr[4][2] <> vRealArr[2];
    print (vDecimal <> vBoolArr[3]) and (0124 <= vStringArr[3]);

    // string
    print "SSLAB" + decimal();
    print "SSLAB" - "SS";
    print "SSLAB" * "SS";
    print "SSLAB" / "SS";
    print "SSLAB" mod "SS";
    print "SSLAB" and "SS";
    print "SSLAB" or "SS";
    print "SSLAB" < "SS";
    print "SSLAB" > "SS";

end
end
