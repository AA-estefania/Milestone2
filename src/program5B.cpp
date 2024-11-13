#include <iostream>
#include <vector>
#include <tuple>
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



std::tuple<int, int, std::vector<int>> program5B(int n, int MAX_WIDTH, std::vector<int> heights, std::vector<int> widths){
    auto distance_matrix= generateCumulativeWidths(widths); //O(n^2)
    std::vector<long> totalCosts(n+1,0);
    std::vector<int> numberOfPaintingsPerShelf;


    for(int end_index=0;end_index<n;++end_index){
        std::vector<long>possibleCosts;
        for (int start_index = end_index; start_index >=0; --start_index) {
            if(distance_matrix[start_index][end_index-start_index]<=MAX_WIDTH){
                auto cost = getMaxInSubrange(heights,start_index, end_index)+totalCosts[start_index];
                possibleCosts.push_back(cost);
            }
            else{
                break;
            }
        }
        totalCosts[end_index+1] = *std::min_element(possibleCosts.begin(), possibleCosts.end());
    }

    //calculate the heights
    int last_index=n;
    for(int i=last_index-1; i>=0; --i){
        if(totalCosts[last_index]-totalCosts[i]==heights[i] or not totalCosts[i]){
            numberOfPaintingsPerShelf.push_back(last_index - i);
            last_index=i;
        }
    }

    std::reverse(numberOfPaintingsPerShelf.begin(), numberOfPaintingsPerShelf.end());

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
//    auto result = program5B(n, W, heights, widths);
//
//    std::cout << std::get<0>(result) << std::endl;
//    std::cout << std::get<1>(result) << std::endl;
//    for(int i = 0; i < std::get<0>(result); i++){
//        std::cout << std::get<2>(result)[i] << std::endl;
//    }
//    return 0;
//}