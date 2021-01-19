#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

static void PrintVec(std::vector<int> const& vec) {
    if (vec.empty()) {
        return;
    }
    
    auto cit = vec.cbegin();
    
    std::cout << (*cit++);
    
    while(cit != vec.cend()) {
        std::cout << " " << (*cit++);
    }
}

// Dramatically simplified version of next_permutation
// just as a reference
template <class BidirectionalIterator>
bool my_next_permutation (BidirectionalIterator first,
                          BidirectionalIterator last) {
    if (first == last) {
        // Empty
        return false;
    }

    BidirectionalIterator i = first;
    ++i;
    if (i == last) {
        // Single element
        return false;
    }

    
    i = last;
    --i;
    
    // 1. Starting from the last element find the first element
    //    not in descending order (i) and its previous one (j).
    //    if all elements are in descending order reverse them
    //    and return false
    //
    // 2. Starting from the last element find the first element
    //    greater than the one just found (k). It should be always
    //    be one, in the worst case it is j.
    //
    // 3. Swap i and k. Note that it does change the main property:
    //    elements from [j,end) are in descending order 
    //
    // 4. Reverse starting from j to the end, they will be in assendng
    //    order to get next lexicographical permutation
    
    for(;;) {
        BidirectionalIterator j = i;
        --i;

        if (*i < *j) {
            BidirectionalIterator k = last;
            
            while (!(*i < *(--k)));

            iter_swap(i, k);
            reverse(j, last);
            return true;
        }

        if (i == first) {
            reverse(first, last);
            return false;
        }
    }
}

// Same with compare function
template <class BidirectionalIterator, typename Compare>
bool my_next_permutation (BidirectionalIterator first,
                          BidirectionalIterator last,
                          Compare comp) {
    if (first == last) {
        // Empty
        return false;
    }

    BidirectionalIterator i = first;
    ++i;
    if (i == last) {
        // Single element
        return false;
    }

    
    i = last;
    --i;
    
    // 1. Starting from the last element find the first element
    //    not in descending order (i) and its previous one (j).
    //    if all elements are in descending order reverse them
    //    and return false
    //
    // 2. Starting from the last element find the first element
    //    greater than the one just found (k). It should be always
    //    be one, in the worst case it is j.
    //
    // 3. Swap i and k. Note that it does change the main property:
    //    elements from [j,end) are in descending order 
    //
    // 4. Reverse starting from j to the end, they will be in ascending
    //    order to get next lexicographical permutation
    
    for(;;) {
        BidirectionalIterator j = i;
        --i;

        if (comp(*i, *j)) {
            BidirectionalIterator k = last;
            
            while (!comp(*i,*(--k)));

            iter_swap(i, k);
            reverse(j, last);
            return true;
        }

        if (i == first) {
            reverse(first, last);
            return false;
        }
    }
}


int
main(int, char**) {
    std::vector<int> va{1,2,3,4,5};
    std::vector<int> vb{1,2,3,4,5};
    std::vector<int> vc{1,2,3,4,5};

    int count = 0;
    std::cout << count << ": ";
    PrintVec(va);
    std::cout << "\n";

    for(;;) {
        bool res = std::next_permutation(va.begin(), va.end());
        bool my_res1 = my_next_permutation(vb.begin(), vb.end());
        bool my_res2 = my_next_permutation(vc.begin(), vc.end(), std::less<int>());
        assert(res == my_res1);
        assert(va == vb);
        assert(res == my_res2);
        assert(va == vc);

        if (!res) {
            break;
        }

        count++;

        std::cout << count << ": ";
        PrintVec(va);
        std::cout << "\n";
    }

    std::cout << "done: ";
    PrintVec(va);
    std::cout << "\n";
    
    return 0;
}
