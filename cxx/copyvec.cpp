#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>
#include <numeric>
 
int main()
{
    std::vector<int> from_vector(50);
    std::iota(from_vector.begin(), from_vector.end(), 0);
 
    std::vector<int> to_vector_0;
    std::copy(from_vector.begin(), from_vector.end(), std::back_inserter(to_vector_0));
                                                    // or, alternatively,
                                                    //  std::vector<int> to_vector(from_vector.size());
                                                    //  std::copy(from_vector.begin(), from_vector.end(), to_vector.begin());
                                                    // either way is equivalent to
                                                    //  std::vector<int> to_vector = from_vector;
 
    std::cout << "\nto_vector_00 contains: ";
    std::copy(to_vector_0.begin(), to_vector_0.end(), std::ostream_iterator<int>(std::cout, " "));
    
    
    // std::vector<int> to_vector(20);
    std::vector<int> to_vector;
    to_vector.reserve(100);
    
    //  OK, all
    // std::copy(from_vector.begin(), from_vector.end(), std::back_inserter(to_vector));
    auto ptr_from = reinterpret_cast<int*> (from_vector.data());
    auto ptr_to = reinterpret_cast<char*> (to_vector.data());
    constexpr size_t LOTSZ = 3, LOTTIMES = 2;
    for (size_t itimes = 0; itimes < LOTTIMES; itimes++)
    {
        // std::copy(ptr_from, ptr_from + 2, ptr_to + (itimes++) * 2);
        std::cout << "\ncopying from " << (itimes * LOTSZ) << " to " << ((itimes+1) * LOTSZ);
        std::copy(ptr_from + itimes * LOTSZ, ptr_from + (itimes+1) * LOTSZ, std::back_inserter(to_vector));
    }

    std::cout << "\nto_vector contains: ";
    std::copy(to_vector.begin(), to_vector.end(), std::ostream_iterator<int>(std::cout, " "));
   
    std::cout << "\n\n";
}
