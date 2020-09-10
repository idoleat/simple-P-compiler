/**
 * general4.p: general case 4
 */
//&T-
general4;

recursive( index: integer ): integer
begin
    if ( index = 1 ) then  
    begin
        return 1;
    end
    else
    begin
        return recursive( index-1 )+index;
    end
    end if
end 
end

begin
    var a : integer;
    read a;
    print recursive(a);
end 
end
