#include <gtest/gtest.h>
#include <vector>
#include "lib/vector.hpp"

TEST(VectorTest, PushBackTest) {
    Vector<int> my_vec;
    std::vector<int> std_vec;
    my_vec.push_back(1);
    std_vec.push_back(1);
    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));
}

TEST(VectorTest, PopBackTest) {
    Vector<int> my_vec;
    std::vector<int> std_vec;
    my_vec.push_back(1);
    std_vec.push_back(1);
    my_vec.pop_back();
    std_vec.pop_back();
    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));
}

TEST(VectorTest, MovePushBackTest) {
    Vector<int> my_vec;
    std::vector<int> std_vec;
    my_vec.push_back(std::move(1));
    std_vec.push_back(std::move(1));
    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));
}

TEST(VectorTest, InsertTest) {
    Vector<int> my_vec;
    std::vector<int> std_vec;
    my_vec.insert(my_vec.begin(), 1);
    std_vec.insert(std_vec.begin(), 1);
    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));
}

TEST(VectorTest, InsertMoveTest) {
    Vector<int> my_vec;
    std::vector<int> std_vec;
    my_vec.insert(my_vec.begin(), std::move(1));
    std_vec.insert(std_vec.begin(), std::move(1));
    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));
}

TEST(VectorTest, MoveConstructorTest) {
    Vector<int> my_vec(std::move(3));
    std::vector<int> std_vec(std::move(3));
    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));
}

TEST(VectorTest, CopyConstructorTest) {
    Vector<int> copy ({1, 2, 3});
    std::vector<int> std_copy ({1, 2, 3});
    Vector<int> my_vec(copy);
    std::vector<int> std_vec(std_copy);
    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));

    copy.pop_back();
    ASSERT_FALSE(std::equal(
        my_vec.begin(), my_vec.end(),
        copy.begin(), copy.end()
    ));
}

TEST(VectorTest, MoveConstructorTest2) {
    Vector<int> copy ({1, 2, 3});
    std::vector<int> std_copy ({1, 2, 3});
    Vector<int> my_vec(std::move(copy));
    std::vector<int> std_vec(std::move(std_copy));
    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));

    ASSERT_EQ(copy.size(), 0);
}

TEST(VectorTest, AllocatorConstructorTest) {
    Vector<int> copy ({1, 2, 3});
    std::vector<int> std_copy ({1, 2, 3});
    Vector<int> my_vec(copy, copy.get_allocator());
    std::vector<int> std_vec(std_copy, std_copy.get_allocator());
    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));
}

TEST(VectorTest, AllocatorMoveConstructorTest) {
    Vector<int> copy ({1, 2, 3});
    std::vector<int> std_copy ({1, 2, 3});
    Vector<int> my_vec(std::move(copy), copy.get_allocator());
    std::vector<int> std_vec(std::move(std_copy), std_copy.get_allocator());
    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));
}

TEST(VectorTest, AllocatorListConstructorTest) {
    Vector<int> my_vec({1, 2, 3}, std::allocator<int>());
    std::vector<int> std_vec({1, 2, 3}, std::allocator<int>());
    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));

    ASSERT_EQ(my_vec.get_allocator(), std_vec.get_allocator());
}

TEST(VectorTest, InsertElementsTest) {
    Vector<int> my_vec;
    std::vector<int> std_vec;

    my_vec.insert(my_vec.begin(), 4, 1);
    std_vec.insert(std_vec.begin(), 4, 1);

    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));
}

TEST(VectorTest, InsertVecTest) {
    Vector<int> copy ({1, 2, 3});
    std::vector<int> std_copy ({1, 2, 3});
    Vector<int> my_vec;
    std::vector<int> std_vec;

    my_vec.insert(my_vec.begin(), copy.begin(), copy.end());
    std_vec.insert(std_vec.begin(), std_copy.begin(), std_copy.end());

    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));
}

TEST(VectorTest, InsertListTest) {
    Vector<int> my_vec;
    std::vector<int> std_vec;

    my_vec.insert(my_vec.begin(), {1, 2, 3});
    std_vec.insert(std_vec.begin(), {1, 2, 3});

    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));
}

TEST(VectorTest, BoarderTest) {
    Vector<int> my_vec;
    std::vector<int> std_vec;

    my_vec.insert(my_vec.begin(), {1, 2, 3});
    std_vec.insert(std_vec.begin(), {1, 2, 3});

    ASSERT_EQ(my_vec[0], std_vec[0]);
}

TEST(VectorTest, AtTest) {
    Vector<int> my_vec;
    std::vector<int> std_vec;

    my_vec.insert(my_vec.begin(), {1, 2, 3});
    std_vec.insert(std_vec.begin(), {1, 2, 3});

    ASSERT_EQ(my_vec.at(0), std_vec.at(0));
}

TEST(VectorTest, FrontTest) {
    Vector<int> my_vec;
    std::vector<int> std_vec;

    my_vec.insert(my_vec.begin(), {1, 2, 3});
    std_vec.insert(std_vec.begin(), {1, 2, 3});

    ASSERT_EQ(my_vec.front(), std_vec.front());
}

TEST(VectorTest, BackTest) {
    Vector<int> my_vec;
    std::vector<int> std_vec;

    my_vec.insert(my_vec.begin(), {1, 2, 3});
    std_vec.insert(std_vec.begin(), {1, 2, 3});

    ASSERT_EQ(my_vec.back(), std_vec.back());
}

TEST(VectorTest, EmptyTest1) {
    Vector<int> my_vec;
    std::vector<int> std_vec;

    my_vec.insert(my_vec.begin(), {1, 2, 3});
    std_vec.insert(std_vec.begin(), {1, 2, 3});

    ASSERT_TRUE(!my_vec.empty());
}

TEST(VectorTest, EmptyTest2) {
    Vector<int> my_vec;
    std::vector<int> std_vec;

    my_vec.insert(my_vec.begin(), {1, 2, 3});
    std_vec.insert(std_vec.begin(), {1, 2, 3});

    ASSERT_FALSE(my_vec.empty());
}

TEST(VectorTest, EmptyTest3) {
    Vector<int> my_vec;
    std::vector<int> std_vec;

    ASSERT_TRUE(my_vec.empty());
}

TEST(VectorTest, AssignTest1) {
    Vector<int> my_vec;
    std::vector<int> std_vec;

    my_vec.assign(3, 1);
    std_vec.assign(3, 1);

    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));
}

TEST(VectorTest, AssignTest2) {
    Vector<int> my_vec;
    std::vector<int> std_vec;

    my_vec.push_back(1);
    std_vec.push_back(1);

    my_vec.assign(3, 1);
    std_vec.assign(3, 1);

    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));

    ASSERT_EQ(my_vec.size(), 3);
    ASSERT_EQ(std_vec.size(), 3);
}

TEST(VectorTest, ResizeTest) {
    Vector<int> my_vec;
    std::vector<int> std_vec;

    my_vec.resize(3);
    std_vec.resize(3);

    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));

    ASSERT_EQ(my_vec.size(), 3);
    ASSERT_EQ(std_vec.size(), 3);
}

TEST(VectorTest, AssignListTest) {
     Vector<int> my_vec;
    std::vector<int> std_vec;

    my_vec.assign({1, 2, 3});
    std_vec.assign({1, 2, 3});

    ASSERT_TRUE(std::equal(
        my_vec.begin(), my_vec.end(),
        std_vec.begin(), std_vec.end()
    ));
}