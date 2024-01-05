    #include <iostream>
    
    // g++ -std=c++14 
    
    template<typename T>
    T incr(T v)
    {
    	return v + 1;
    }
    
    
    int main()
    {
    	float f = 2.0;
    	int i = 3;
    
    	auto selfincr = [] (auto & value)
    		{
    			//value = incr<std::remove_reference<decltype(value)>>(value);	// A
    			//value = incr<decltype(value)>(value);							// B
    			value = incr<std::remove_reference_t<decltype(value)>>(value);	// C
    			value = incr(value); 											// D
    		};
    
    
    	selfincr(f);
    	selfincr(i);
    	
    	std::cout << "f " << f << ", i " << i << std::endl;
    	return 0;
    }
