function [ kth ] = RandomizedSelect( array, a_size, k )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    
    %size_array = size(array);
    %a_size = size_array(2); % rawr >_<
    
    array_left = zeros(size(array));
    array_right = zeros(size(array));
    num_left = 0;
    num_right = 0;
    pivot_index = ceil(rand(1)*a_size);
    pivot_val = array(pivot_index);
    
    
    
    for i = 1:a_size
        i = i;
        if array(i) < pivot_val
            num_left = num_left+1;
            array_left(num_left) = array(i);
        end
		
		if array(i) > pivot_val
            num_right = num_right+1;
            array_right(num_right) = array(i);
        end
    end
    
    if (num_left < (k - 1)) 
		kth = RandomizedSelect(array_right, num_right, k - num_left - 1);
   	end
		
	if (num_left > (k - 1))
	    	kth = RandomizedSelect(array_left, num_left, k);
    end
	
	if num_left == (k - 1)
    	kth = pivot_val;
	end
	

end

