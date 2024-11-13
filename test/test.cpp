#include <catch2/catch_test_macros.hpp>
#include "rapidcheck.h"
#include "rapidcheck/catch.h"
#include <limits>
#include <iostream>
#include <numeric>
#include <random>
#include "program1.cpp"
#include "program2.cpp"
#include "program3.cpp"
#include "program4.cpp"
#include "program5A.cpp"
#include "program5B.cpp"
#include <sstream>
using namespace std;

std::string vectorToString(const std::vector<int>& vec) {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        ss << vec[i];
        if (i != vec.size() - 1) {
            ss << ",";  // Add comma between elements, but not after the last element
        }
    }
    ss << "]";
    return ss.str();
}


std::string print2DVector(const std::vector<std::vector<int>>& vec) {
    std::stringstream ss;
    ss << "[\n";
    for (const auto& row : vec) {
        ss << "  [";
        for (size_t i = 0; i < row.size(); ++i) {
            ss << row[i];
            if (i != row.size() - 1) ss << ", ";
        }
        ss << "],\n";
    }
    ss << "]";
    return ss.str();
}

TEST_CASE("Example Test", "[flag]"){

    int one =1;


    REQUIRE(one == 1);


    REQUIRE(true);
}


TEST_CASE("Example  Test", "[property]"){


    rc::prop("Associative property",
             [](int a, int b)
             {
                 // rapidcheck will produce the `a` and `b`
                 return (a *b) == (b *a);
                 // failed test
             });
}

// Property-Based Test for generateCumulativeWidths
TEST_CASE("Property-Based Test for generateCumulativeWidths", "[generateCumulativeWidths][property]") {
    rc::prop("Cumulative widths are correctly computed for all start-end pairs", [](const std::vector<int>& widths) {
        // Generate the cumulative widths using the function
        std::vector<std::vector<long>> cumulativeWidths = generateCumulativeWidths(widths);
        int n = widths.size();

        // Property 1: Row Count
        RC_ASSERT(cumulativeWidths.size() == n);

        // Iterate over each start index
        for (int start = 0; start < n; ++start) {
            // Property 2: Row Length
            RC_ASSERT(cumulativeWidths[start].size() == (n - start));

            int sum = 0;

            // Iterate over each end index in the current row
            for (int offset = 0; offset < (n - start); ++offset) {
                int end = start + offset;
                sum += widths[end];

                // Property 3: Cumulative Sum
                RC_ASSERT(cumulativeWidths[start][offset] == sum);
            }
        }

        return true;
    });
}

// Specific Test Case to Confirm Functionality
TEST_CASE("Program 5: Test generateCumulativeWidths Function", "[generateCumulativeWidths][specific]") {
    // Example 1
    std::vector<int> widths1 = {1, 2, 3};
    std::vector<std::vector<long>> result1 = generateCumulativeWidths(widths1);
    //std::cout << "Example 1:\n" << print2DVector(result1) << "\n\n";

    // Expected Output: [[1, 3, 6], [2, 5], [3]]
    std::vector<std::vector<long>> expected1 = {
            {1, 3, 6},
            {2, 5},
            {3}
    };
    REQUIRE(result1 == expected1);

    // Example 2 (User's Specific Case)
    std::vector<int> widths2 = {23278, 24267, 26282, 11580, 25721};
    std::vector<std::vector<long>> result2 = generateCumulativeWidths(widths2);
    //std::cout << "Example 2 (User's Specific Case):\n" << print2DVector(result2) << "\n\n";

    // Expected Output:
    // [
    //   [23278, 47545, 73827, 85307, 111128],
    //   [24267, 50549, 62129, 87850],
    //   [26282, 37862, 63583],
    //   [11580, 37301],
    //   [25721]
    // ]
    std::vector<std::vector<long>> expected2 = {
            {23278, 47545, 73827, 85407, 111128},
            {24267, 50549, 62129, 87850},
            {26282, 37862, 63583},
            {11580, 37301},
            {25721}
    };
    REQUIRE(result2 == expected2);

    // Additional Example 3
    std::vector<int> widths3 = {7, 1, 2, 3, 5, 8, 1};
    std::vector<std::vector<long>> result3 = generateCumulativeWidths(widths3);
    //std::cout << "Example 3:\n" << print2DVector(result3) << "\n\n";

    // Expected Output:
    // [
    //   [7, 8, 10, 13, 18, 26, 27],
    //   [1, 3, 6, 11, 19, 20],
    //   [2, 5, 10, 18, 19],
    //   [3, 8, 16, 17],
    //   [5, 13, 14],
    //   [8, 9],
    //   [1]
    // ]
    std::vector<std::vector<long>> expected3 = {
            {7, 8, 10, 13, 18, 26, 27},
            {1, 3, 6, 11, 19, 20},
            {2, 5, 10, 18, 19},
            {3, 8, 16, 17},
            {5, 13, 14},
            {8, 9},
            {1}
    };
    REQUIRE(result3 == expected3);
}

TEST_CASE("testing if sequences with only one local minimum give the same results for programs 3,4,5A,5B", "[property]"){

    // Property 1: Descending
    rc::prop("Descending: Check if all the paintings and total height are consistent across programs 3,4,5A,5B",
             []()
             {
                 int n = *rc::gen::inRange(1, 25).as("Number of paintings (n)");
                 int W = *rc::gen::inRange(1, std::numeric_limits<int>::max()).as("Maximum Width (W)");

                 int firstHeight = *rc::gen::inRange(1, 10000000).as("FirstHeight");
                 std::vector<int> heights = *rc::gen::apply(
                         [n, firstHeight]() {
                             std::vector<int> heights;
                             heights.reserve(n);
                             heights.push_back(firstHeight);
                             std::random_device rd;
                             std::mt19937 gen(rd());

                             for (int i = 1; i < n; ++i) {
                                 int maxHeight = heights.back();
                                 std::uniform_int_distribution<> distrib(0, maxHeight);
                                 int nextHeight = distrib(gen);
                                 heights.push_back(nextHeight);
                             }
                             return heights;
                         }
                 ).as("heights");

                 std::vector<int> widths = *rc::gen::container<std::vector<int>>(n, rc::gen::inRange(1, W)).as("widths");

                 // Assertions to ensure correct sizes
                 RC_ASSERT(heights.size() == n);
                 RC_ASSERT(widths.size() == n);

                 // Call program3
                 auto [number_of_platforms3, total_height3, num_paintings_per_platform3] = program3(n, W, heights, widths);
                 int total_number_of_paintings3 = std::accumulate(num_paintings_per_platform3.begin(), num_paintings_per_platform3.end(), 0);

                 // Call program4
                 auto [number_of_platforms4, total_height4, num_paintings_per_platform4] = program4(n, W, heights, widths);
                 int total_number_of_paintings4 = std::accumulate(num_paintings_per_platform4.begin(), num_paintings_per_platform4.end(), 0);

                 // Call program5A
                 auto [number_of_platforms5a, total_height5a, num_paintings_per_platform5a] = program5A(n, W, heights, widths);
                 int total_number_of_paintings5a = std::accumulate(num_paintings_per_platform5a.begin(), num_paintings_per_platform5a.end(), 0);

                 // Call program5B
                 auto [number_of_platforms5b, total_height5b, num_paintings_per_platform5b] = program5B(n, W, heights, widths);
                 int total_number_of_paintings5b = std::accumulate(num_paintings_per_platform5b.begin(), num_paintings_per_platform5b.end(), 0);

                 // Logging all results
                 std::stringstream ss;
                 ss << "Property: Descending\n";
                 ss << "n (" << n << "), W (" << W << "):\n";
                 ss << "  Heights size: " << heights.size() << "\n";
                 ss << "  Widths size: " << widths.size() << "\n";
                 ss << "  Program3 - Platforms: " << number_of_platforms3 << ", Total Height: " << total_height3
                    << ", Total Paintings: " << total_number_of_paintings3 << "\n";
                 ss << "  Program4 - Platforms: " << number_of_platforms4 << ", Total Height: " << total_height4
                    << ", Total Paintings: " << total_number_of_paintings4 << "\n";
                 ss << "  Program5A - Platforms: " << number_of_platforms5a << ", Total Height: " << total_height5a
                    << ", Total Paintings: " << total_number_of_paintings5a << "\n";
                 ss << "  Program5B - Platforms: " << number_of_platforms5b << ", Total Height: " << total_height5b
                    << ", Total Paintings: " << total_number_of_paintings5b << "\n";
                 ss << "  num_paintings_per_platform3: " << vectorToString(num_paintings_per_platform3) << "\n";
                 ss << "  num_paintings_per_platform4: " << vectorToString(num_paintings_per_platform4) << "\n";
                 ss << "  num_paintings_per_platform5a: " << vectorToString(num_paintings_per_platform5a) << "\n";
                 ss << "  num_paintings_per_platform5b: " << vectorToString(num_paintings_per_platform5b) << "\n";

                 std::string log_output = ss.str();
                 RC_LOG(log_output);

                 // Detailed Assertion Logs
                 if (heights.size() != n) {
                     RC_FAIL("heights.size() != n");
                 }
                 if (widths.size() != n) {
                     RC_FAIL("widths.size() != n");
                 }
                 if (total_number_of_paintings3 != total_number_of_paintings4) {
                     RC_FAIL("total_number_of_paintings3 != total_number_of_paintings4");
                 }
                 if (total_number_of_paintings3 != total_number_of_paintings5a) {
                     RC_FAIL("total_number_of_paintings3 != total_number_of_paintings5a");
                 }
                 if (total_number_of_paintings3 != total_number_of_paintings5b) {
                     RC_FAIL("total_number_of_paintings3 != total_number_of_paintings5b");
                 }
                 if (total_height3 != total_height4) {
                     RC_FAIL("total_height3 != total_height4");
                 }
                 if (total_height3 != total_height5a) {
                     RC_FAIL("total_height3 != total_height5a");
                 }
                 if (total_height3 != total_height5b) {
                     RC_FAIL("total_height3 != total_height5b");
                 }
                 if (total_number_of_paintings3 != n) {
                     RC_FAIL("total_number_of_paintings3 != n");
                 }

                 return true;
             });

    // Property 2: Random Minimum
    rc::prop("Random Minimum: Check if all the paintings and total height are consistent across programs 3,4,5A,5B",
             []()
             {
                 int n = *rc::gen::inRange(1, 25).as("Number of paintings (n)");
                 int W = *rc::gen::inRange(1, 10).as("Maximum Width (W)");

                 int firstHeight = *rc::gen::inRange(1, 10000000).as("FirstHeight");
                 std::vector<int> heights = *rc::gen::apply(
                         [n, firstHeight]() {
                             std::vector<int> heights;
                             heights.reserve(n);
                             heights.push_back(firstHeight);

                             std::random_device rd;
                             std::mt19937 gen(rd());

                             // Randomly choose the inflection point k between 0 and n - 1
                             std::uniform_int_distribution<> k_distrib(0, n - 1);
                             int k = k_distrib(gen);

                             // Generate heights for indices 1 to n - 1
                             for (int i = 1; i < n; ++i) {
                                 if (i <= k) {
                                     // Decreasing part: heights[i] ≤ heights[i - 1]
                                     int prevHeight = heights.back();
                                     std::uniform_int_distribution<> distrib(0, prevHeight);
                                     int nextHeight = distrib(gen);
                                     heights.push_back(nextHeight);
                                 } else {
                                     // Increasing part: heights[i] ≥ heights[i - 1]
                                     int prevHeight = heights.back();
                                     // To prevent exceeding firstHeight, set the maximum to firstHeight
                                     std::uniform_int_distribution<> distrib(prevHeight, firstHeight);
                                     int nextHeight = distrib(gen);
                                     heights.push_back(nextHeight);
                                 }
                             }
                             return heights;
                         }
                 ).as("heights");

                 std::vector<int> widths = *rc::gen::container<std::vector<int>>(n, rc::gen::inRange(1, W)).as("widths");

                 // Assertions to ensure correct sizes
                 RC_ASSERT(heights.size() == n);
                 RC_ASSERT(widths.size() == n);

                 // Call program3
                 auto [number_of_platforms3, total_height3, num_paintings_per_platform3] = program3(n, W, heights, widths);
                 int total_number_of_paintings3 = std::accumulate(num_paintings_per_platform3.begin(), num_paintings_per_platform3.end(), 0);

                 // Call program4
                 auto [number_of_platforms4, total_height4, num_paintings_per_platform4] = program4(n, W, heights, widths);
                 int total_number_of_paintings4 = std::accumulate(num_paintings_per_platform4.begin(), num_paintings_per_platform4.end(), 0);

                 // Call program5A
                 auto [number_of_platforms5a, total_height5a, num_paintings_per_platform5a] = program5A(n, W, heights, widths);
                 int total_number_of_paintings5a = std::accumulate(num_paintings_per_platform5a.begin(), num_paintings_per_platform5a.end(), 0);

                 // Call program5B
                 auto [number_of_platforms5b, total_height5b, num_paintings_per_platform5b] = program5B(n, W, heights, widths);
                 int total_number_of_paintings5b = std::accumulate(num_paintings_per_platform5b.begin(), num_paintings_per_platform5b.end(), 0);

                 // Logging all results
                 std::stringstream ss;
                 ss << "Property: Random Minimum\n";
                 ss << "n (" << n << "), W (" << W << "):\n";
                 ss << "  Heights size: " << heights.size() << "\n";
                 ss << "  Widths size: " << widths.size() << "\n";
                 ss << "  Program3 - Platforms: " << number_of_platforms3 << ", Total Height: " << total_height3
                    << ", Total Paintings: " << total_number_of_paintings3 << "\n";
                 ss << "  Program4 - Platforms: " << number_of_platforms4 << ", Total Height: " << total_height4
                    << ", Total Paintings: " << total_number_of_paintings4 << "\n";
                 ss << "  Program5A - Platforms: " << number_of_platforms5a << ", Total Height: " << total_height5a
                    << ", Total Paintings: " << total_number_of_paintings5a << "\n";
                 ss << "  Program5B - Platforms: " << number_of_platforms5b << ", Total Height: " << total_height5b
                    << ", Total Paintings: " << total_number_of_paintings5b << "\n";
                 ss << "  num_paintings_per_platform3: " << vectorToString(num_paintings_per_platform3) << "\n";
                 ss << "  num_paintings_per_platform4: " << vectorToString(num_paintings_per_platform4) << "\n";
                 ss << "  num_paintings_per_platform5a: " << vectorToString(num_paintings_per_platform5a) << "\n";
                 ss << "  num_paintings_per_platform5b: " << vectorToString(num_paintings_per_platform5b) << "\n";

                 std::string log_output = ss.str();
                 RC_LOG(log_output);

                 // Assertions to ensure all programs return the same total_number_of_paintings
                 RC_ASSERT(total_number_of_paintings3 == total_number_of_paintings4);
                 RC_ASSERT(total_number_of_paintings3 == total_number_of_paintings5a);
                 RC_ASSERT(total_number_of_paintings3 == total_number_of_paintings5b);

                 // Assertions to ensure all programs return the same total_height
                 RC_ASSERT(total_height3 == total_height4);
                 RC_ASSERT(total_height3 == total_height5a);
                 RC_ASSERT(total_height3 == total_height5b);

                 // Assertion to ensure total_number_of_paintings equals n
                 RC_ASSERT(total_number_of_paintings3 == n);

                 return true;
             });

    // Property 3: Non-Decreasing
    rc::prop("Non-Decreasing: Check if all the paintings and total height are consistent across programs 3,4,5A,5B",
             []()
             {
                 int n = *rc::gen::inRange(1, 25).as("Number of paintings (n)");
                 int W = *rc::gen::inRange(1, std::numeric_limits<int>::max()).as("Maximum Width (W)");

                 int firstHeight = *rc::gen::inRange(1, 10000000).as("FirstHeight");
                 std::vector<int> heights = *rc::gen::apply(
                         [n, firstHeight]() {
                             std::vector<int> heights;
                             heights.reserve(n);
                             heights.push_back(firstHeight);  // Start with the first height

                             std::random_device rd;
                             std::mt19937 gen(rd());

                             for (int i = 1; i < n; ++i) {
                                 // Generate a height greater than or equal to the previous height
                                 int prevHeight = heights.back();
                                 std::uniform_int_distribution<> distrib(prevHeight, prevHeight + 100); // Adjust range as needed
                                 int nextHeight = distrib(gen);
                                 heights.push_back(nextHeight);
                             }

                             return heights;
                         }
                 ).as("heights");

                 std::vector<int> widths = *rc::gen::container<std::vector<int>>(n, rc::gen::inRange(1, W)).as("widths");

                 // Assertions to ensure correct sizes
                 RC_ASSERT(heights.size() == n);
                 RC_ASSERT(widths.size() == n);

                 // Call program3
                 auto [number_of_platforms3, total_height3, num_paintings_per_platform3] = program3(n, W, heights, widths);
                 int total_number_of_paintings3 = std::accumulate(num_paintings_per_platform3.begin(), num_paintings_per_platform3.end(), 0);

                 // Call program4
                 auto [number_of_platforms4, total_height4, num_paintings_per_platform4] = program4(n, W, heights, widths);
                 int total_number_of_paintings4 = std::accumulate(num_paintings_per_platform4.begin(), num_paintings_per_platform4.end(), 0);

                 // Call program5A
                 auto [number_of_platforms5a, total_height5a, num_paintings_per_platform5a] = program5A(n, W, heights, widths);
                 int total_number_of_paintings5a = std::accumulate(num_paintings_per_platform5a.begin(), num_paintings_per_platform5a.end(), 0);

                 // Call program5B
                 auto [number_of_platforms5b, total_height5b, num_paintings_per_platform5b] = program5B(n, W, heights, widths);
                 int total_number_of_paintings5b = std::accumulate(num_paintings_per_platform5b.begin(), num_paintings_per_platform5b.end(), 0);

                 // Logging all results
                 std::stringstream ss;
                 ss << "Property: Non-Decreasing\n";
                 ss << "n (" << n << "), W (" << W << "):\n";
                 ss << "  Heights size: " << heights.size() << "\n";
                 ss << "  Widths size: " << widths.size() << "\n";
                 ss << "  Program3 - Platforms: " << number_of_platforms3 << ", Total Height: " << total_height3
                    << ", Total Paintings: " << total_number_of_paintings3 << "\n";
                 ss << "  Program4 - Platforms: " << number_of_platforms4 << ", Total Height: " << total_height4
                    << ", Total Paintings: " << total_number_of_paintings4 << "\n";
                 ss << "  Program5A - Platforms: " << number_of_platforms5a << ", Total Height: " << total_height5a
                    << ", Total Paintings: " << total_number_of_paintings5a << "\n";
                 ss << "  Program5B - Platforms: " << number_of_platforms5b << ", Total Height: " << total_height5b
                    << ", Total Paintings: " << total_number_of_paintings5b << "\n";
                 ss << "  num_paintings_per_platform3: " << vectorToString(num_paintings_per_platform3) << "\n";
                 ss << "  num_paintings_per_platform4: " << vectorToString(num_paintings_per_platform4) << "\n";
                 ss << "  num_paintings_per_platform5a: " << vectorToString(num_paintings_per_platform5a) << "\n";
                 ss << "  num_paintings_per_platform5b: " << vectorToString(num_paintings_per_platform5b) << "\n";

                 std::string log_output = ss.str();
                 RC_LOG(log_output);

                 // Assertions to ensure all programs return the same total_number_of_paintings
                 RC_ASSERT(total_number_of_paintings3 == total_number_of_paintings4);
                 RC_ASSERT(total_number_of_paintings3 == total_number_of_paintings5a);
                 RC_ASSERT(total_number_of_paintings3 == total_number_of_paintings5b);

                 // Assertions to ensure all programs return the same total_height
                 RC_ASSERT(total_height3 == total_height4);
                 RC_ASSERT(total_height3 == total_height5a);
                 RC_ASSERT(total_height3 == total_height5b);

                 // Assertion to ensure total_number_of_paintings equals n
                 RC_ASSERT(total_number_of_paintings3 == n);

                 return true;
             });
}



TEST_CASE("Specific test for Example 1: n=7, W=10, heights=[21, 19, 17, 16, 11, 5, 1] , widths=[7, 1, 2, 3, 5, 8, 1]", "[specific][given]") {
    int n = 7;
    int W = 10;

    std::vector<int> heights = {21, 19, 17, 16, 11, 5, 1};
    std::vector<int> widths = {7, 1, 2, 3, 5, 8, 1};

    // Check the size of heights and widths
    REQUIRE(heights.size() == n);
    REQUIRE(widths.size() == n);

    SECTION( "program 1" ) {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program1(n, W, heights, widths);

        // Calculate the total number of paintings from num_paintings_per_platform
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(),
                                                         num_paintings_per_platform.end(), 0);

        // Validate the results
        REQUIRE(number_of_platforms == 3 );
        REQUIRE(num_paintings_per_platform.size()==3);
        REQUIRE(num_paintings_per_platform[0]==3);
        REQUIRE(num_paintings_per_platform[1]==2);
        REQUIRE(num_paintings_per_platform[2]==2);
        REQUIRE(total_height == 42);
        REQUIRE(total_number_of_paintings == n);     // The total number of paintings should equal n
    }

    SECTION( "program 2" ) {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program2(n, W, heights, widths);

        // Calculate the total number of paintings from num_paintings_per_platform
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(),
                                                         num_paintings_per_platform.end(), 0);

        // Validate the results
        REQUIRE(total_height == 42);
        REQUIRE(total_number_of_paintings == n);     // The total number of paintings should equal n
    }

    SECTION( "program 3" ) {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program3(n, W, heights, widths);

        // Calculate the total number of paintings from num_paintings_per_platform
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(),
                                                         num_paintings_per_platform.end(), 0);

        // Validate the results
        REQUIRE(number_of_platforms == 3 );
        REQUIRE(num_paintings_per_platform.size()==3);
        REQUIRE(num_paintings_per_platform[0]==3);
        REQUIRE(num_paintings_per_platform[1]==2);
        REQUIRE(num_paintings_per_platform[2]==2);
        REQUIRE(total_height == 42);
        REQUIRE(total_number_of_paintings == n);     // The total number of paintings should equal n
    }

    SECTION( "program 4" ) {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program4(n, W, heights, widths);

        // Calculate the total number of paintings from num_paintings_per_platform
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(),
                                                         num_paintings_per_platform.end(), 0);

        // Validate the results
        REQUIRE(number_of_platforms == 3 );
        REQUIRE(num_paintings_per_platform.size()==3);
        REQUIRE(num_paintings_per_platform[0]==3);
        REQUIRE(num_paintings_per_platform[1]==2);
        REQUIRE(num_paintings_per_platform[2]==2);
        REQUIRE(total_height == 42);
        REQUIRE(total_number_of_paintings == n);     // The total number of paintings should equal n
    }
}


TEST_CASE("Specific test for int overflow: n=5, W=54138, heights=[963859275, 842725740, 795573189, 666410839, 80312912], widths=[23278, 24267, 26282, 11580, 25721]", "[specific]") {
    int n = 5;
    int W = 54138;

    std::vector<int> heights = {963859275, 842725740, 795573189, 666410839, 80312912};
    std::vector<int> widths = {23278, 24267, 26282, 11580, 25721};

    // Check the size of heights and widths
    REQUIRE(heights.size() == n);
    REQUIRE(widths.size() == n);

    // Variables to store results from each program
    int number_of_platforms2, number_of_platforms3, number_of_platforms4;
    int total_height2, total_height3, total_height4;
    std::vector<int> num_paintings_per_platform2, num_paintings_per_platform3, num_paintings_per_platform4;

    // Call program2
    std::tie(number_of_platforms2, total_height2, num_paintings_per_platform2) = program2(n, W, heights, widths);
    int total_number_of_paintings2 = std::accumulate(num_paintings_per_platform2.begin(), num_paintings_per_platform2.end(), 0);

    // Call program3
    std::tie(number_of_platforms3, total_height3, num_paintings_per_platform3) = program3(n, W, heights, widths);
    int total_number_of_paintings3 = std::accumulate(num_paintings_per_platform3.begin(), num_paintings_per_platform3.end(), 0);

    // Call program4
    std::tie(number_of_platforms4, total_height4, num_paintings_per_platform4) = program4(n, W, heights, widths);
    int total_number_of_paintings4 = std::accumulate(num_paintings_per_platform4.begin(), num_paintings_per_platform4.end(), 0);

    // Logging all results
    std::stringstream ss;
    ss << "Test Case: Example 2\n";
    ss << "n = " << n << ", W = " << W << "\n";
    ss << "Heights: " << vectorToString(heights) << "\n";
    ss << "Widths: " << vectorToString(widths) << "\n\n";

    ss << "Program2:\n";
    ss << "  Number of Platforms: " << number_of_platforms2 << "\n";
    ss << "  Total Height: " << total_height2 << "\n";
    ss << "  Num Paintings per Platform: " << vectorToString(num_paintings_per_platform2) << "\n";
    ss << "  Total Number of Paintings: " << total_number_of_paintings2 << "\n\n";

    ss << "Program3:\n";
    ss << "  Number of Platforms: " << number_of_platforms3 << "\n";
    ss << "  Total Height: " << total_height3 << "\n";
    ss << "  Num Paintings per Platform: " << vectorToString(num_paintings_per_platform3) << "\n";
    ss << "  Total Number of Paintings: " << total_number_of_paintings3 << "\n\n";

    ss << "Program4:\n";
    ss << "  Number of Platforms: " << number_of_platforms4 << "\n";
    ss << "  Total Height: " << total_height4 << "\n";
    ss << "  Num Paintings per Platform: " << vectorToString(num_paintings_per_platform4) << "\n";
    ss << "  Total Number of Paintings: " << total_number_of_paintings4 << "\n";

    INFO(ss.str()); // Using INFO to log information when a test fails

    // Assertions to ensure all programs have positive total heights
    REQUIRE(total_height2 > 0);
    REQUIRE(total_height3 > 0);
    REQUIRE(total_height4 > 0);

    // Assertions to ensure all programs return the same total_height
    REQUIRE(total_height2 == total_height3);
    REQUIRE(total_height2 == total_height4);

    // Assertions to ensure all programs return the same total_number_of_paintings
    REQUIRE(total_number_of_paintings2 == total_number_of_paintings3);
    REQUIRE(total_number_of_paintings2 == total_number_of_paintings4);

    // Assertions to ensure total_number_of_paintings equals n
    REQUIRE(total_number_of_paintings2 == n);
    REQUIRE(total_number_of_paintings3 == n);
    REQUIRE(total_number_of_paintings4 == n);
}


TEST_CASE("Specific test for Example with n=5, W=2, heights=[1, 92, 134, 232, 293], widths=[1,1,1,1,1]", "[specific][given]") {
    // Define the specific inputs
    int n = 5;
    int W = 2;
    std::vector<int> heights = {1, 92, 134, 232, 293};
    std::vector<int> widths = {1, 1, 1, 1, 1};

    // Verify the sizes of heights and widths
    REQUIRE(heights.size() == n);
    REQUIRE(widths.size() == n);

    // Define the expected outputs based on program3 and program4
    int expected_number_of_platforms = 3;
    int expected_total_height = 1+134+293;
    std::vector<int> expected_num_paintings_per_platform = {1, 2, 2};

    // SECTION for program3
    SECTION("Program 3 Verification") {
        auto [num_p3, th3, npp3] = program3(n, W, heights, widths);

        // Log the outputs
        std::stringstream ss;
        ss << "Program3 - Platforms: " << num_p3
           << ", Total Height: " << th3
           << ", Num Paintings per Platform: " << vectorToString(npp3);
        INFO(ss.str());

        // Assertions for program3
        REQUIRE(num_p3 == expected_number_of_platforms);
        REQUIRE(th3 == expected_total_height);
        REQUIRE(npp3 == expected_num_paintings_per_platform);
        REQUIRE(std::accumulate(npp3.begin(), npp3.end(), 0) == n);
    }

        // SECTION for program4
    SECTION("Program 4 Verification") {
        auto [num_p4, th4, npp4] = program4(n, W, heights, widths);

        // Log the outputs
        std::stringstream ss;
        ss << "Program4 - Platforms: " << num_p4
           << ", Total Height: " << th4
           << ", Num Paintings per Platform: " << vectorToString(npp4);
        INFO(ss.str());

        // Assertions for program4
        REQUIRE(num_p4 == expected_number_of_platforms);
        REQUIRE(th4 == expected_total_height);
        REQUIRE(npp4 == expected_num_paintings_per_platform);
        REQUIRE(std::accumulate(npp4.begin(), npp4.end(), 0) == n);
    }

        // SECTION for program2
    SECTION("Program 2 Verification Against Programs 3 and 4") {
        auto [num_p3, th3, npp3] = program3(n, W, heights, widths);

        auto [num_p4, th4, npp4] = program4(n, W, heights, widths);

        auto [num_p2, th2, npp2] = program2(n, W, heights, widths);

        // Log the outputs
        std::stringstream ss;
        ss << "Program2 - Platforms: " << num_p2
           << ", Total Height: " << th2
           << ", Num Paintings per Platform: " << vectorToString(npp2);
        INFO(ss.str());

        // Assertions for program2 to match program3
        REQUIRE(num_p2 == num_p3);
        REQUIRE(num_p2 == num_p4);
        REQUIRE(th2 == th3);
        REQUIRE(th2 == th4);
        REQUIRE(npp2 == npp3);
        REQUIRE(npp2 == npp4);
        REQUIRE(std::accumulate(npp2.begin(), npp2.end(), 0) == n);
    }
}

TEST_CASE("Specific test for Example 3: n=7, W=10, heights=[12, 10, 9, 7, 8, 10, 11] , widths=[3, 2, 3, 4, 3, 2, 3]", "[specific][given]") {
    int n = 7;
    int W = 10;

    std::vector<int> heights = {12, 10, 9, 7, 8, 10, 11};
    std::vector<int> widths = {3, 2, 3, 4, 3, 2, 3};

    // Check the size of heights and widths
    REQUIRE(heights.size() == n);
    REQUIRE(widths.size() == n);

    SECTION("program 2") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program2(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 3);
        REQUIRE(num_paintings_per_platform.size() == 3);
        REQUIRE(num_paintings_per_platform[0] == 3);
        REQUIRE(num_paintings_per_platform[1] == 1);
        REQUIRE(num_paintings_per_platform[2] == 3);
        REQUIRE(total_height == 30);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 3") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program3(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 3);
        REQUIRE(num_paintings_per_platform.size() == 3);
        REQUIRE(num_paintings_per_platform[0] == 3);
        REQUIRE(num_paintings_per_platform[1] == 1);
        REQUIRE(num_paintings_per_platform[2] == 3);
        REQUIRE(total_height == 30);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 4") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program4(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 3);
        REQUIRE(num_paintings_per_platform.size() == 3);
        REQUIRE(num_paintings_per_platform[0] == 3);
        REQUIRE(num_paintings_per_platform[1] == 1);
        REQUIRE(num_paintings_per_platform[2] == 3);
        REQUIRE(total_height == 30);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 5A") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program5A(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 3);
        REQUIRE(num_paintings_per_platform.size() == 3);
        REQUIRE(num_paintings_per_platform[0] == 3);
        REQUIRE(num_paintings_per_platform[1] == 1);
        REQUIRE(num_paintings_per_platform[2] == 3);
        REQUIRE(total_height == 30);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 5B") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program5B(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 3);
        REQUIRE(num_paintings_per_platform.size() == 3);
        REQUIRE(num_paintings_per_platform[0] == 3);
        REQUIRE(num_paintings_per_platform[1] == 1);
        REQUIRE(num_paintings_per_platform[2] == 3);
        REQUIRE(total_height == 30);
        REQUIRE(total_number_of_paintings == n);
    }
}




TEST_CASE("Specific test for n=3, W=4, heights=[2,1,2], widths=[2,1,1]", "[specific][new]") {
    int n = 3;
    int W = 4;

    std::vector<int> heights = {2, 1, 2};
    std::vector<int> widths = {2, 1, 1};

    // Check the size of heights and widths
    REQUIRE(heights.size() == n);
    REQUIRE(widths.size() == n);

    SECTION("program 2") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program2(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(num_paintings_per_platform.size() == 1);
        REQUIRE(num_paintings_per_platform[0] == 3);
        REQUIRE(total_height == 2);                   // Total height is the maximum height on the platform
        REQUIRE(total_number_of_paintings == n);      // Total paintings should equal n
    }

    SECTION("program 3") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program3(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(num_paintings_per_platform.size() == 1);
        REQUIRE(num_paintings_per_platform[0] == 3);
        REQUIRE(total_height == 2);                   // Total height is the maximum height on the platform
        REQUIRE(total_number_of_paintings == n);      // Total paintings should equal n
    }

    SECTION("program 4") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program4(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(num_paintings_per_platform.size() == 1);
        REQUIRE(num_paintings_per_platform[0] == 3);
        REQUIRE(total_height == 2);                   // Total height is the maximum height on the platform
        REQUIRE(total_number_of_paintings == n);      // Total paintings should equal n
    }

    SECTION("program 5A") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program5A(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(num_paintings_per_platform.size() == 1);
        REQUIRE(num_paintings_per_platform[0] == 3);
        REQUIRE(total_height == 2);                   // Total height is the maximum height on the platform
        REQUIRE(total_number_of_paintings == n);      // Total paintings should equal n
    }

    SECTION("program 5B") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program5B(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(num_paintings_per_platform.size() == 1);
        REQUIRE(num_paintings_per_platform[0] == 3);
        REQUIRE(total_height == 2);                   // Total height is the maximum height on the platform
        REQUIRE(total_number_of_paintings == n);      // Total paintings should equal n
    }
}



TEST_CASE("Specific test for n=4, W=4, heights=[2,1,1,1] , widths=[1,1,1,1]", "[specific][new]") {
    int n = 4;
    int W = 4;

    std::vector<int> heights = {2, 1, 1, 1};
    std::vector<int> widths = {1, 1, 1, 1};

    // Check the size of heights and widths
    REQUIRE(heights.size() == n);
    REQUIRE(widths.size() == n);

    SECTION("program 1") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program1(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(total_height == 2);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 2") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program2(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(total_height == 2);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 3") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program3(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(total_height == 2);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 4") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program4(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(total_height == 2);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 5A") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program5A(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(total_height == 2);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 5B") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program5B(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(total_height == 2);
        REQUIRE(total_number_of_paintings == n);
    }
}






TEST_CASE("Specific test for n=4, W=3, heights=[4,2,4,4], widths=[1,1,1,1]", "[specific][new]") {
    int n = 4;
    int W = 3;

    std::vector<int> heights = {4, 2, 4, 4};
    std::vector<int> widths = {1, 1, 1, 1};

    // Check the size of heights and widths
    REQUIRE(heights.size() == n);
    REQUIRE(widths.size() == n);

    SECTION("program 2") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program2(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 2);
        REQUIRE(num_paintings_per_platform.size() == 2);
        REQUIRE(num_paintings_per_platform[0] == 3); // First platform holds paintings 0, 1, and 2
        REQUIRE(num_paintings_per_platform[1] == 1); // Second platform holds painting 3
        REQUIRE(total_height == 8); // Total height should be 8 (4 + 4)
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 3") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program3(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 2);
        REQUIRE(num_paintings_per_platform.size() == 2);
        REQUIRE(total_height == 8);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 4") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program4(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 2);
        REQUIRE(num_paintings_per_platform.size() == 2);
        REQUIRE(num_paintings_per_platform[0] == 3);
        REQUIRE(num_paintings_per_platform[1] == 1);
        REQUIRE(total_height == 8);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 5A") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program5A(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);


        REQUIRE(total_height == 8); // Total height should be 8 (4 + 4)
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 5B") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program5B(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(total_height == 8); // Total height should be 8 (4 + 4)
        REQUIRE(total_number_of_paintings == n);
    }
}


TEST_CASE("Specific test for n=4, W=4, heights=[5, 2, 0, 2], widths=[2, 2, 1, 1]", "[specific][new]") {
    int n = 4;
    int W = 4;

    std::vector<int> heights = {5, 2, 0, 2};
    std::vector<int> widths = {2, 2, 1, 1};

    // Check the size of heights and widths
    REQUIRE(heights.size() == n);
    REQUIRE(widths.size() == n);

    SECTION("program 2") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program2(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 2);
        REQUIRE(num_paintings_per_platform.size() == 2);
        REQUIRE(num_paintings_per_platform[0] == 2);
        REQUIRE(num_paintings_per_platform[1] == 2);
        REQUIRE(total_height == 7);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 3") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program3(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(total_height == 7);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 4") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program4(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(total_height == 7);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 5A") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program5A(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);


        REQUIRE(total_height == 7);
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 5B") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program5B(n, W, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);


        REQUIRE(total_height == 7);
        REQUIRE(total_number_of_paintings == n);
    }
}



TEST_CASE("Specific test for n=11, MAX_WIDTH=8918603, heights and widths given", "[specific][new]") {
    int n = 11;
    int MAX_WIDTH = 8918603;

    std::vector<int> heights = {
            8771901, 8771986, 8772048, 8772118, 8772202,
            8772231, 8772283, 8772378, 8772384, 8772451, 8772513
    };

    std::vector<int> widths = {
            65875, 66632, 9302, 56034, 14955,
            71495, 50238, 45471, 10463, 27732, 85775
    };

    // Check the size of heights and widths
    REQUIRE(heights.size() == n);
    REQUIRE(widths.size() == n);

    SECTION("program 2") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program2(n, MAX_WIDTH, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(num_paintings_per_platform.size() == 1);
        REQUIRE(num_paintings_per_platform[0] == n);
        REQUIRE(total_height == heights.back());
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 3") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program3(n, MAX_WIDTH, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(num_paintings_per_platform.size() == 1);
        REQUIRE(num_paintings_per_platform[0] == n);
        REQUIRE(total_height == heights.back());
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 4") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program4(n, MAX_WIDTH, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(num_paintings_per_platform.size() == 1);
        REQUIRE(num_paintings_per_platform[0] == n);
        REQUIRE(total_height == heights.back());
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 5A") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program5A(n, MAX_WIDTH, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(num_paintings_per_platform.size() == 1);
        REQUIRE(num_paintings_per_platform[0] == n);
        REQUIRE(total_height == heights.back());
        REQUIRE(total_number_of_paintings == n);
    }

    SECTION("program 5B") {
        auto [number_of_platforms, total_height, num_paintings_per_platform] = program5B(n, MAX_WIDTH, heights, widths);
        auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

        REQUIRE(number_of_platforms == 1);
        REQUIRE(num_paintings_per_platform.size() == 1);
        REQUIRE(num_paintings_per_platform[0] == n);
        REQUIRE(total_height == heights.back());
        REQUIRE(total_number_of_paintings == n);
    }
}



TEST_CASE("testing program 1 total number of paintings on each platform= total paintings", "[property]"){



    rc::prop("Check if all the paintings were counted",
             []()
             {
                 int n = *rc::gen::inRange(1, 100).as("Number of paintings (n)");
                 int W = *rc::gen::inRange(1, 1000).as("Maximum Width (W)");

                 auto firstHeight=*rc::gen::inRange(1, 1000).as("FirstHeight");
                 auto heights = *rc::gen::apply(
                         [n,firstHeight]() {
                             std::vector<int> heights;
                             heights.push_back(firstHeight);
                             for (int i = 1; i < n; ++i) {
                                 int maxHeight = heights.back();
                                 std::random_device rd;
                                 std::mt19937 gen(rd());


                                 std::uniform_int_distribution<> distrib(0, maxHeight);

                                 int nextHeight = distrib(gen);
                                 heights.push_back(nextHeight);
                             }
                             return heights;
                         }
                 ).as("heights");

                 auto widths = *rc::gen::container<std::vector<int>>(n, rc::gen::inRange(1, W)).as("widths");



                 RC_ASSERT(heights.size() == n);
                 RC_ASSERT(widths.size() == n);

                 auto[number_of_platforms,total_height, num_paintings_per_platform]=program1(n,W,heights,widths);
                 auto total_number_of_paintings = std::accumulate(num_paintings_per_platform.begin(), num_paintings_per_platform.end(), 0);

                 std::stringstream ss;
                 ss << "n (" << n << "), W (" << W << "):\n";  // n and W at the top, not indented
                 ss << "  Heights size: " << heights.size() << "\n";  // Indented
                 ss << "  Widths size: " << widths.size() << "\n";    // Indented
                 ss << "  Number of platforms: " << number_of_platforms << "\n";  // Indented
                 ss << "  Total height: " << total_height << "\n";    // Indented
                 ss << "  Maximum width: " << W << "\n";  // Indented (repeat Maximum Width)
                 ss << "  Total number of paintings (" << total_number_of_paintings
                    << ") ?= n (" << n << ")\n";  // Indented
                 ss << "  num_paintings_per_platform: " << vectorToString(num_paintings_per_platform) << "\n";  // Indented

                 std::string z = ss.str();
                 RC_LOG(z);


                 RC_ASSERT(number_of_platforms>0);

                 RC_ASSERT(total_number_of_paintings==n);

                 return true;

             });
}


TEST_CASE("Specific test for Example with n=6, W=10, heights=[2,1,8,10,9,7], widths=[1,1,8,1,2,2]", "[specific][given]") {
    // Define the specific inputs
    int n = 6;
    int W = 10;
    std::vector<int> heights = {2, 1, 8, 10, 9, 7};
    std::vector<int> widths = {1, 1, 8, 1, 2, 2};

    // Verify the sizes of heights and widths
    REQUIRE(heights.size() == n);
    REQUIRE(widths.size() == n);

    // Define the expected outputs based on the grouping logic
    int expected_number_of_platforms = 2;
    int expected_total_height = 18; // 8 (Platform1) + 10 (Platform2)
    std::vector<int> expected_num_paintings_per_platform = {3, 3};

    // SECTION for program3
    SECTION("Program3 Verification") {
        auto [num_p3, th3, npp3] = program3(n, W, heights, widths);

        // Log the outputs
        std::stringstream ss;
        ss << "Program3 - Platforms: " << num_p3
           << ", Total Height: " << th3
           << ", Num Paintings per Platform: " << vectorToString(npp3);
        INFO(ss.str());

        // Assertions for program3
        REQUIRE(num_p3 == expected_number_of_platforms);
        REQUIRE(th3 == expected_total_height);
        REQUIRE(npp3 == expected_num_paintings_per_platform);
        REQUIRE(std::accumulate(npp3.begin(), npp3.end(), 0) == n);
    }

        // SECTION for program4
    SECTION("Program4 Verification") {
        auto [num_p4, th4, npp4] = program4(n, W, heights, widths);

        // Log the outputs
        std::stringstream ss;
        ss << "Program4 - Platforms: " << num_p4
           << ", Total Height: " << th4
           << ", Num Paintings per Platform: " << vectorToString(npp4);
        INFO(ss.str());

        // Assertions for program4
        REQUIRE(num_p4 == expected_number_of_platforms);
        REQUIRE(th4 == expected_total_height);
        REQUIRE(npp4 == expected_num_paintings_per_platform);
        REQUIRE(std::accumulate(npp4.begin(), npp4.end(), 0) == n);
    }

        // SECTION for program5B
    SECTION("Program5B Verification") {
        auto [num_p5b, th5b, npp5b] = program5B(n, W, heights, widths);

        // Log the outputs
        std::stringstream ss;
        ss << "Program5B - Platforms: " << num_p5b
           << ", Total Height: " << th5b
           << ", Num Paintings per Platform: " << vectorToString(npp5b);
        INFO(ss.str());

        // Assertions for program5B
        REQUIRE(num_p5b == expected_number_of_platforms);
        REQUIRE(th5b == expected_total_height);
        REQUIRE(npp5b == expected_num_paintings_per_platform);
        REQUIRE(std::accumulate(npp5b.begin(), npp5b.end(), 0) == n);
    }

        // SECTION for program5A
    SECTION("Program5A Verification") {
        auto [num_p5a, th5a, npp5a] = program5A(n, W, heights, widths);

        // Log the outputs
        std::stringstream ss;
        ss << "Program5A - Platforms: " << num_p5a
           << ", Total Height: " << th5a
           << ", Num Paintings per Platform: " << vectorToString(npp5a);
        INFO(ss.str());

        // Assertions for program5A
        REQUIRE(num_p5a == expected_number_of_platforms);
        REQUIRE(th5a == expected_total_height);
        REQUIRE(npp5a == expected_num_paintings_per_platform);
        REQUIRE(std::accumulate(npp5a.begin(), npp5a.end(), 0) == n);
    }

        // SECTION to verify consistency across all programs
    SECTION("Consistency Across Programs 3, 4, 5A, and 5B") {
        auto [num_p3, th3, npp3] = program3(n, W, heights, widths);
        auto [num_p4, th4, npp4] = program4(n, W, heights, widths);
        auto [num_p5a, th5a, npp5a] = program5A(n, W, heights, widths);
        auto [num_p5b, th5b, npp5b] = program5B(n, W, heights, widths);

        // Log the outputs
        std::stringstream ss;
        ss << "Consistency Check:\n";
        ss << "Program3 - Platforms: " << num_p3 << ", Total Height: " << th3 << ", Num Paintings per Platform: " << vectorToString(npp3) << "\n";
        ss << "Program4 - Platforms: " << num_p4 << ", Total Height: " << th4 << ", Num Paintings per Platform: " << vectorToString(npp4) << "\n";
        ss << "Program5A - Platforms: " << num_p5a << ", Total Height: " << th5a << ", Num Paintings per Platform: " << vectorToString(npp5a) << "\n";
        ss << "Program5B - Platforms: " << num_p5b << ", Total Height: " << th5b << ", Num Paintings per Platform: " << vectorToString(npp5b) << "\n";
        INFO(ss.str());

        // Assertions to ensure all programs return the same number_of_platforms
        REQUIRE(num_p3 == num_p4);
        REQUIRE(num_p3 == num_p5a);
        REQUIRE(num_p3 == num_p5b);

        // Assertions to ensure all programs return the same total_height
        REQUIRE(th3 == th4);
        REQUIRE(th3 == th5a);
        REQUIRE(th3 == th5b);

        // Assertions to ensure all programs return the same num_paintings_per_platform
        REQUIRE(npp3 == npp4);
        REQUIRE(npp3 == npp5a);
        REQUIRE(npp3 == npp5b);

        // Assertions to ensure total_number_of_paintings equals n for all programs
        REQUIRE(std::accumulate(npp3.begin(), npp3.end(), 0) == n);
        REQUIRE(std::accumulate(npp4.begin(), npp4.end(), 0) == n);
        REQUIRE(std::accumulate(npp5a.begin(), npp5a.end(), 0) == n);
        REQUIRE(std::accumulate(npp5b.begin(), npp5b.end(), 0) == n);
    }
}




// Property-Based Test to verify consistency across programs 1-5b
TEST_CASE("testing if monotonically non-increasing sequences give the same results for programs 1,2,3,4,5a, and 5b", "[property]") {
    rc::prop("Check if all the paintings and total height are consistent across programs", []() {
        // Generate the number of paintings and maximum width
        int n = *rc::gen::inRange(1, 30).as("Number of paintings (n)");
        int W = *rc::gen::inRange(1, 10000).as("Maximum Width (W)");

        // Generate the first height and create a monotonically non-increasing sequence of heights
        int firstHeight = *rc::gen::inRange(1, 10000).as("FirstHeight");
        std::vector<int> heights = *rc::gen::apply([n, firstHeight]() {
            std::vector<int> heights;
            heights.reserve(n);
            heights.push_back(firstHeight);
            // Initialize random number generator
            static std::random_device rd;
            static std::mt19937 gen(rd());

            for (int i = 1; i < n; ++i) {
                int maxHeight = heights.back();
                std::uniform_int_distribution<> distrib(0, maxHeight);
                int nextHeight = distrib(gen);
                heights.push_back(nextHeight);
            }
            return heights;
        }).as("heights");

        // Generate widths: each width is between 1 and W (inclusive)
        std::vector<int> widths = *rc::gen::container<std::vector<int>>(n, rc::gen::inRange(1, W)).as("widths");

        // Assertions to ensure correct sizes
        RC_ASSERT(heights.size() == n);
        RC_ASSERT(widths.size() == n);

        // Call program1
        auto [number_of_platforms1, total_height1, num_paintings_per_platform1] = program1(n, W, heights, widths);
        int total_number_of_paintings1 = std::accumulate(num_paintings_per_platform1.begin(), num_paintings_per_platform1.end(), 0);

        // Call program2
        auto [number_of_platforms2, total_height2, num_paintings_per_platform2] = program2(n, W, heights, widths);
        int total_number_of_paintings2 = std::accumulate(num_paintings_per_platform2.begin(), num_paintings_per_platform2.end(), 0);

        // Call program3
        auto [number_of_platforms3, total_height3, num_paintings_per_platform3] = program3(n, W, heights, widths);
        int total_number_of_paintings3 = std::accumulate(num_paintings_per_platform3.begin(), num_paintings_per_platform3.end(), 0);

        // Call program4
        auto [number_of_platforms4, total_height4, num_paintings_per_platform4] = program4(n, W, heights, widths);
        int total_number_of_paintings4 = std::accumulate(num_paintings_per_platform4.begin(), num_paintings_per_platform4.end(), 0);

        // Call program5A
        auto [number_of_platforms5a, total_height5a, num_paintings_per_platform5a] = program5A(n, W, heights, widths);
        int total_number_of_paintings5a = std::accumulate(num_paintings_per_platform5a.begin(), num_paintings_per_platform5a.end(), 0);

        // Call program5B
        auto [number_of_platforms5b, total_height5b, num_paintings_per_platform5b] = program5B(n, W, heights, widths);
        int total_number_of_paintings5b = std::accumulate(num_paintings_per_platform5b.begin(), num_paintings_per_platform5b.end(), 0);

        // Logging all results for debugging purposes
        std::stringstream ss;
        ss << "n (" << n << "), W (" << W << "):\n";
        ss << "  Heights size: " << heights.size() << "\n";
        ss << "  Widths size: " << widths.size() << "\n";
        ss << "  Program1 - Platforms: " << number_of_platforms1 << ", Total Height: " << total_height1
           << ", Total Paintings: " << total_number_of_paintings1 << "\n";
        ss << "  Program2 - Platforms: " << number_of_platforms2 << ", Total Height: " << total_height2
           << ", Total Paintings: " << total_number_of_paintings2 << "\n";
        ss << "  Program3 - Platforms: " << number_of_platforms3 << ", Total Height: " << total_height3
           << ", Total Paintings: " << total_number_of_paintings3 << "\n";
        ss << "  Program4 - Platforms: " << number_of_platforms4 << ", Total Height: " << total_height4
           << ", Total Paintings: " << total_number_of_paintings4 << "\n";
        ss << "  Program5A - Platforms: " << number_of_platforms5a << ", Total Height: " << total_height5a
           << ", Total Paintings: " << total_number_of_paintings5a << "\n";
        ss << "  Program5B - Platforms: " << number_of_platforms5b << ", Total Height: " << total_height5b
           << ", Total Paintings: " << total_number_of_paintings5b << "\n";
        ss << "  num_paintings_per_platform1: " << vectorToString(num_paintings_per_platform1) << "\n";
        ss << "  num_paintings_per_platform2: " << vectorToString(num_paintings_per_platform2) << "\n";
        ss << "  num_paintings_per_platform3: " << vectorToString(num_paintings_per_platform3) << "\n";
        ss << "  num_paintings_per_platform4: " << vectorToString(num_paintings_per_platform4) << "\n";
        ss << "  num_paintings_per_platform5A: " << vectorToString(num_paintings_per_platform5a) << "\n";
        ss << "  num_paintings_per_platform5B: " << vectorToString(num_paintings_per_platform5b) << "\n";

        std::string log_output = ss.str();
        RC_LOG(log_output);

        // Assertions to ensure all programs return the same total_number_of_paintings
        RC_ASSERT(total_number_of_paintings1 == total_number_of_paintings2);
        RC_ASSERT(total_number_of_paintings1 == total_number_of_paintings3);
        RC_ASSERT(total_number_of_paintings1 == total_number_of_paintings4);
        RC_ASSERT(total_number_of_paintings1 == total_number_of_paintings5a);
        RC_ASSERT(total_number_of_paintings1 == total_number_of_paintings5b);

        // Assertions to ensure all programs return the same total_height
        RC_ASSERT(total_height1 == total_height2);
        RC_ASSERT(total_height1 == total_height3);
        RC_ASSERT(total_height1 == total_height4);
        RC_ASSERT(total_height1 == total_height5a);
        RC_ASSERT(total_height1 == total_height5b);

        // Assertion to ensure total_number_of_paintings equals n
        RC_ASSERT(total_number_of_paintings1 == n);

        return true;
    });
}
