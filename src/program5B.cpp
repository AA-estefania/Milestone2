#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <limits>
/* Solution to program 5B
* @param n the number of paintings
* @param MAX_WIDTH the maximum width of the platform
* @param heights the heights of the paintings
* @param widths the widths of the paintings
* @return a tuple containing the number of platforms used, the optimal total height, and the number of paintings on each platform
*/

std::vector<std::vector<long>> generateCumulativeWidths(const std::vector<int>& widths) {
    int n = widths.size();
    std::vector<std::vector<long>> cumulativeWidths;

    // Iterate over each possible start index
    for (int start = 0; start < n; ++start) {
        std::vector<long> currentRow; // Holds cumulative widths for the current start index
        long sum = 0;

        // Iterate over each possible end index from 'start' to 'n-1'
        for (int end = start; end < n; ++end) {
            sum += widths[end];       // Add the width at the current end index
            currentRow.push_back(sum); // Store the cumulative sum
        }

        cumulativeWidths.push_back(currentRow); // Add the row to the outer vector
    }

    return cumulativeWidths;
}

int getMaxInSubrange(const std::vector<int>& vec, int start, int end) {

    // Calculate the maximum element in the subrange [start, end]
    // Note: std::max_element's second argument is one past the last element, hence (end + 1)
    return *std::max_element(vec.begin() + start, vec.begin() + end + 1);
}



std::tuple<int, int, std::vector<int>> program5B(int n, int MAX_WIDTH, const std::vector<int>& heights, const std::vector<int>& widths) {
    // Initialize totalCosts and shelf_start_indices
    std::vector<long> totalCosts(n + 1, std::numeric_limits<long>::max());
    std::vector<int> shelf_start_indices(n + 1, -1);
    totalCosts[0] = 0; // Base case: cost of zero paintings is zero

    std::vector<int> cumulative_widths(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        cumulative_widths[i + 1] = cumulative_widths[i] + widths[i];
    }

    for (int end_index = 0; end_index < n; ++end_index) {
        int max_height = 0;
        for (int start_index = end_index; start_index >= 0; --start_index) {
            int width = cumulative_widths[end_index + 1] - cumulative_widths[start_index];
            if (width > MAX_WIDTH) {
                break; // Cannot include more paintings on this shelf
            }
            max_height = std::max(max_height, heights[start_index]);
            long cost = totalCosts[start_index] + max_height;
            if (cost < totalCosts[end_index + 1]) {
                totalCosts[end_index + 1] = cost;
                shelf_start_indices[end_index + 1] = start_index;
            }
        }
    }

    // Reconstruct the number of paintings per shelf
    std::vector<int> numberOfPaintingsPerShelf;
    int index = n;
    while (index > 0) {
        int shelf_start = shelf_start_indices[index];
        int num_paintings_on_shelf = index - shelf_start;
        numberOfPaintingsPerShelf.push_back(num_paintings_on_shelf);
        index = shelf_start; // Move to the previous shelf
    }
    std::reverse(numberOfPaintingsPerShelf.begin(), numberOfPaintingsPerShelf.end());

    int total_height = totalCosts[n];

    return std::make_tuple(numberOfPaintingsPerShelf.size(), total_height, numberOfPaintingsPerShelf);
}
//int main(){
//    int n, W;
//    std::cin >> n >> W;
//    std::vector<int> heights(n);
//    std::vector<int> widths(n);
//    for(int i = 0; i < n; i++){
//        std::cin >> heights[i];
//    }
//    for(int i = 0; i < n; i++){
//        std::cin >> widths[i];
//    }
//    auto result = program5B(n, W, heights, widths);
//
//    std::cout << std::get<0>(result) << std::endl;
//    std::cout << std::get<1>(result) << std::endl;
//    for(int i = 0; i < std::get<0>(result); i++){
//        std::cout << std::get<2>(result)[i] << std::endl;
//    }
//    return 0;
//}