#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "lib/vector.hpp"

TEST(IteratorTest, StressTest) {
    const int N = 10000;
    Vector<int> my_vec(N);
    std::vector<int> std_vec(N);

    for (int i = 0; i < N; ++i) {
        my_vec[i] = std_vec[i] = i;
    }

    auto my_it = my_vec.begin();
    auto std_it = std_vec.begin();
    
    for (int i = 0; i < N; ++i) {
        ASSERT_EQ(my_it[i], std_it[i]);
        ASSERT_EQ(*(my_it + i), *(std_it + i));
    }
}

TEST(IteratorTest, STLAlgorithms) {
    Vector<int> my_vec = {5, 3, 1, 4, 2};
    std::vector<int> std_vec = {5, 3, 1, 4, 2};

    std::sort(my_vec.begin(), my_vec.end());
    std::sort(std_vec.begin(), std_vec.end());

    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));
}

TEST(IteratorTest, ReverseIterators) {
    Vector<int> my_vec = {1, 2, 3};
    std::vector<int> std_vec = {1, 2, 3};

    auto my_rit = my_vec.rbegin();
    auto std_rit = std_vec.rbegin();

    ASSERT_EQ(*my_rit, *std_rit);
    ASSERT_EQ(*(my_rit + 1), *(std_rit + 1));
}

TEST(IteratorTest, ConstIterators) {
    const Vector<int> my_vec = {2, 4, 6};
    const std::vector<int> std_vec = {2, 4, 6};

    auto my_it = my_vec.cbegin();
    auto std_it = std_vec.cbegin();

    ASSERT_EQ(*my_it, *std_it);
    static_assert(!std::is_assignable_v<decltype(*my_it), int>);
}

TEST(IteratorTest, RandomAccessOperations) {
    Vector<int> my_vec = {1, 3, 5, 7, 9};
    std::vector<int> std_vec = {1, 3, 5, 7, 9};

    auto my_it = my_vec.begin();
    auto std_it = std_vec.begin();

    ASSERT_EQ(my_it[2], std_it[2]);         
    ASSERT_EQ(*(my_it += 2), *(std_it += 2)); 
    ASSERT_EQ(*(my_it -= 1), *(std_it -= 1)); 
    ASSERT_EQ(my_it - 1, my_vec.begin());    
    ASSERT_EQ(my_it - 1 >= my_vec.begin(), true); 
    ASSERT_EQ(my_it <= my_vec.end(), true);  
    ASSERT_EQ(my_it > my_vec.begin(), true); 
    ASSERT_EQ(my_it < my_vec.end(), true);  
}