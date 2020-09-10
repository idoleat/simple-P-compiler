/**
 * general5.p: general case 5
 */
//&T-
general5;

var sptr: integer;
var data: array 99 of string;
var sizeStack: 100;

init()
begin
    sptr := -1;
end
end

push( item: string )
begin
    sptr := sptr+1;
    data[sptr] := item;
end
end

top():string
begin
    return data[sptr];
end
end

pop():string
begin
    sptr := sptr-1;
    return data[sptr+1];
end
end

isEmpty(): boolean
begin
    return sptr = -1;
end
end

isFull(): boolean
begin
    return (sptr = (sizeStack-1));
end
end

begin
    init();
    push("hello");
    push(" P language");
    push("\n");

    while not isEmpty() do
    begin
        print pop();
    end
    end do
end 
end
