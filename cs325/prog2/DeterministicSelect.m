function [ kth ] = DeterministicSelect( array, a_size, k )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

    array_left = zeros(size(array));
    array_right = zeros(size(array));
    num_left = 0;
    num_right = 0;
    toggle = 0;
    pivot_val = Pivot_Finder(array, a_size);
    
    for i = 1:a_size
        if array(i) < pivot_val
           num_left = num_left+1;
           array_left(num_left) = array(i);
 
        end
		
        if array(i) > pivot_val
            num_right = num_right+1;
            array_right(num_right) = array(i);
        end
        if array(i) == pivot_val
            toggle = toggle + 1;
        end
    end
    
    if (num_left < (k - 1)) 
		kth = DeterministicSelect(array_right, num_right, k - num_left - toggle);
    end
		
    if (num_left > (k - 1))
	    kth = DeterministicSelect(array_left, num_left, k);
    end
	
	if num_left == (k - 1)
    	kth = min(array_right(1:num_right));
        if toggle > 0
            kth = pivot_val;
        end
	end
	
    
    

end

