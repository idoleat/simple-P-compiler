/**
 * general3.p: general case 3
 */
//&T-
general3;

begin
        var a: array 9 of string;
        var list: string;

        print 1+2+3*(4+5)+((6*7)-8);

        for i := 1 to 10 do 
        begin
            read a[i];
        end
        end do

        list := "";
        for i := 3 to 12 do
        begin
            list := list+" "+a[i-2];
        end
        end do

        print list;

end 
end
