//&S-
//&T-
//&D-

advFunc;

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
a := 10;
print recursive(a);

end
end