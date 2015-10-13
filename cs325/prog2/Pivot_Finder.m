function [ p ] = Pivot_Finder( array, a_size )

    %P = zeros(1,ceil(a_size/5));
        
    for i = 1:ceil(a_size/5)
        if (i - 1) * 5 + 5 > a_size
                        
            P(i) = (median(array((i-1)*5+1:a_size)));
        
        else 
            P(i) = (median(array((i-1)*5+1:((i-1)*5+5))));
        end
    end
        
    if size(P) <= 5
        p = (median(P));
    else
        p = Pivot_Finder(P, size(P));
    end
end
