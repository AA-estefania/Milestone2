#include <iostream>
#include <vector>
#include <tuple>
/* Solution to program 5A
* @param n the number of paintings
* @param MAX_WIDTH the maximum width of the platform
* @param heights the heights of the paintings
* @param widths the widths of the paintings
* @return a tuple containing the number of platforms used, the optimal total height, and the number of paintings on each platform
*/

auto getMaxInSubrange2(const std::vector<int>& vec, int start, int end) {

    // Calculate the maximum element in the subrange [start, end]
    // Note: std::max_element's second argument is one past the last element, hence (end + 1)
    return *std::max_element(vec.begin() + start, vec.begin() + end + 1);
}

auto calculateNumberOfValidIndicies(int end_index,const int MAX_WIDTH, const std::vector<int>& widths){
    auto current_width=0;
    int iterations=0;
    for(;end_index-iterations>=0 and current_width<MAX_WIDTH;++iterations){
        current_width+=widths[end_index-iterations];
    }
    if(current_width>MAX_WIDTH){
        iterations--;
    }
    return iterations;
}

long recursivelyFindBestAnswer(std::vector<long>& cost_cache, int index, const int MAX_WIDTH,
                               const std::vector<int>& heights, const std::vector<int>& widths,
                               std::vector<int>& shelf_start_indices) {
    if (index < 0) {
        return 0;
    }

    if (cost_cache[index] != -1) {
        return cost_cache[index];
    }

    long result = std::numeric_limits<long>::max();
    int shelf_width = 0;
    int shelf_height = 0;

    for (int start_index = index; start_index >= 0; --start_index) {
        shelf_width += widths[start_index];
        if (shelf_width > MAX_WIDTH) {
            break;
        }
        shelf_height = std::max(shelf_height, heights[start_index]);
        long cost_before_shelf = (start_index > 0) ? recursivelyFindBestAnswer(cost_cache, start_index - 1, MAX_WIDTH, heights, widths, shelf_start_indices) : 0;
        long total_cost = cost_before_shelf + shelf_height;

        if (total_cost < result) {
            result = total_cost;
            shelf_start_indices[index] = start_index;
        }
    }

    cost_cache[index] = result;
    return result;
}


std::vector<int> getNumPaintingsPerShelf(const std::vector<int>& shelf_start_indices, int n) {
    std::vector<int> numberOfPaintingsPerShelf;
    int i = n - 1;

    while (i >= 0) {
        int shelf_start = shelf_start_indices[i];
        int num_paintings_on_shelf = i - shelf_start + 1;
        numberOfPaintingsPerShelf.push_back(num_paintings_on_shelf);
        i = shelf_start - 1; // Move to the previous shelf
    }

    // Reverse the result to get the correct order
    std::reverse(numberOfPaintingsPerShelf.begin(), numberOfPaintingsPerShelf.end());
    return numberOfPaintingsPerShelf;
}

std::tuple<int, int, std::vector<int>> program5A(int n, int MAX_WIDTH, std::vector<int> heights, std::vector<int> widths){
    std::vector<long>totalCosts(n,-1);
    std::vector<int> shelf_start_indices(n, -1);

    recursivelyFindBestAnswer(totalCosts, n - 1, MAX_WIDTH, heights, widths, shelf_start_indices);

    std::vector<int> numberOfPaintingsPerShelf = getNumPaintingsPerShelf(shelf_start_indices, n);

    return std::make_tuple(numberOfPaintingsPerShelf.size(), totalCosts.back(), numberOfPaintingsPerShelf);
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
//    auto result = program5A(n, W, heights, widths);
//
//    std::cout << std::get<0>(result) << std::endl;
//    std::cout << std::get<1>(result) << std::endl;
//    for(int i = 0; i < std::get<0>(result); i++){
//        std::cout << std::get<2>(result)[i] << std::endl;
//    }
//    return 0;
//}