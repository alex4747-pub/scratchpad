#include <cassert>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

// Three variants of kadane's algorithm for
// max sub-array problem
//
// The kadane_b is not that good.

int kadane_a(vector<int> const& vec) {
    int max_so_far = numeric_limits<int>::min();
    int max_ending_here = 0;

    for (auto val: vec) {
        max_ending_here = max_ending_here + val;

        if (max_so_far < max_ending_here) {
            max_so_far = max_ending_here;
        }

        if (max_ending_here < 0) {
            max_ending_here = 0;
        }
    }
    return max_so_far;
}

int kadane_b(vector<int> const& vec) {
    // This one is from wikipedia

    // This one would not work
    // if we have only negative values!!!
    int best_sum  = 0;
    int current_sum = 0;

    for (auto val: vec) {
        if ((current_sum + val) < 0) {
            current_sum = 0;
        } else {
            current_sum += val;
        }

        if (current_sum > best_sum) {
            best_sum = current_sum;
        }
    }

    return best_sum;
}

int kadane_c(vector<int> const& vec) {
    int best_sum = numeric_limits<int>::min();
    int current_sum = 0;

    for (auto val: vec) {
        if (val > (val + current_sum)) {
            current_sum = val;
        } else {
            current_sum += val;
        }

        if (current_sum > best_sum) {
            best_sum = current_sum;
        }
    }

    return best_sum;
}

int
main(int, char**) {
    vector<int> vec0{-2, -3, 4, -1, -2, 1, 5, -3};

    int res0 = kadane_a(vec0);
    assert(res0 == 7);

    res0 = kadane_b(vec0);
    assert(res0 == 7);

    res0 = kadane_c(vec0);
    assert(res0 == 7);

    vector<int> vec1{-2, -3, 4, 5, -10, 2, -30, -1, -2, 1, 5, 7, -3};

    int res1 = kadane_a(vec1);
    assert(res1 == 13);

    res1 = kadane_b(vec1);
    assert(res1 == 13);

    res1 = kadane_c(vec1);
    assert(res1 == 13);

    vector<int> vec2{-10, -3, -10, -30, -1, -2, -3};

    int res2 = kadane_a(vec2);
    assert(res2 == -1);

    res2 = kadane_b(vec2);
    assert(res2 == 0); // This is incorrect result

    res2 = kadane_c(vec2);
    assert(res2 == -1);

    return 0;
}
