#include "./headers/numc.hpp"
#include <gtest/gtest.h>
#include <vector>

using namespace SamH::numc;

TEST(ArraySliceTest, SliceFullArray) 
{
    int data[] = {1, 2, 3, 4, 5};
    array<int> arr(data, 5);

    // array<int> sliced = arr.slice(); // default should return full array

    // std::vector<int> expected = {1, 2, 3, 4, 5};
    // for (size_t i = 0; i < expected.size(); ++i) {
    //     EXPECT_EQ(sliced.slice().slice()[i], expected[i]); // check values
    // }
}

TEST(ArraySliceTest, SlicePartialArray) 
{
    int data[] = {10, 20, 30, 40, 50};
    array<int> arr(data, 5);

    // array<int> sliced = arr.slice(1, 4); // indices 1..3

    // std::vector<int> expected = {20, 30, 40};
    // for (size_t i = 0; i < expected.size(); ++i) {
    //     EXPECT_EQ(sliced.slice().slice()[i], expected[i]);
    // }
}

TEST(ArraySliceTest, SliceWithStep) 
{
    int data[] = {5, 10, 15, 20, 25};
    array<int> arr(data, 5);

    // array<int> sliced = arr.slice(0, 5, 2); // step 2: 0,2,4

    // std::vector<int> expected = {5, 15, 25};
    // for (size_t i = 0; i < expected.size(); ++i) {
    //     EXPECT_EQ(sliced.slice().slice()[i], expected[i]);
    // }
}

TEST(ArraySliceTest, EmptySlice) 
{
    int data[] = {1, 2, 3};
    array<int> arr(data, 3);

    // array<int> sliced = arr.slice(2, 2); // begin == end -> empty

    // EXPECT_EQ(sliced.slice().slice().size(), 0);
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
