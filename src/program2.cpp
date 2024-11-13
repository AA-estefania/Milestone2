#include <iostream>
#include <vector>
#include <tuple>
#include <limits>

using namespace std;
/* Solution to program 2
* @param n the number of paintings
* @param W the maximum width of the platform
* @param heights the heights of the paintings
* @param widths the widths of the paintings
* @return a tuple containing the number of platforms used, the optimal total height, and the number of paintings on each platform
*/
std::tuple<int, int, std::vector<int>> program2(int n, int MAX_WIDTH, const std::vector<int>& heights, const std::vector<int>& widths) {
    int totalCost = 0;
    std::vector<int> totalPaintings; // Total paintings on each platform

    int current_max_height = heights[0];
    int index = 0;
    int increasing_index = -1; // To store the index where increasing starts

    int total_counted=0;

    // Process from start until increasing sequence is detected
    while (index < n) {

        totalPaintings.push_back(0);
        int currentWidth = 0;
        int platform_height = heights[index];
        bool locallyIncreasing= false;
        current_max_height=heights[index];
        while (index < n && currentWidth + widths[index] <= MAX_WIDTH) {
            if (heights[index] > current_max_height) {
                // Detected increasing sequence
                locallyIncreasing= false;
                increasing_index = index;
                break;
            }
            if(not locallyIncreasing and index and heights[index]>heights[index-1]){
                locallyIncreasing= true;
                increasing_index = index;
            }
            currentWidth += widths[index];
            totalPaintings.back() += 1;
            index++;
        }

        total_counted+=totalPaintings.back();

        totalCost += platform_height;

        if(total_counted==n){
            return std::make_tuple(totalPaintings.size(), totalCost, totalPaintings);
        }

        if (increasing_index != -1) {
            // Break outer loop if increasing sequence detected
            if(not locallyIncreasing) {
                total_counted -= totalPaintings.back();
                totalCost -= platform_height;
                totalPaintings.pop_back();
            }
            break;
        }

        if (index >= n ) {
            // All paintings processed
            return std::make_tuple(totalPaintings.size(), totalCost, totalPaintings);
        }
        if(locallyIncreasing){
            break;
        }
    }

    // Now, process from end backwards to increasing_index
    index = n - 1;
    int increasing_index_totalPaintings=totalPaintings.size();
    while (total_counted<n and index) {
        totalPaintings.insert(totalPaintings.begin()+increasing_index_totalPaintings,0);
        int currentWidth = 0;
        current_max_height = heights[index];

        while (total_counted<n and index > 0 && currentWidth + widths[index] <= MAX_WIDTH) {
            currentWidth += widths[index];
            totalPaintings[increasing_index_totalPaintings] += 1;
            index--;
            total_counted+=1;
        }

        totalCost += current_max_height;

    }

    if(total_counted<n and not index){
        if(totalPaintings[increasing_index_totalPaintings]>=MAX_WIDTH){
            totalPaintings.insert(totalPaintings.begin()+increasing_index_totalPaintings,0);
            current_max_height=0;
        }
        totalPaintings[increasing_index_totalPaintings] += 1;
        if(heights[0]>current_max_height){
            totalCost+=heights[0]-current_max_height;
        }
    }

    return std::make_tuple(totalPaintings.size(), totalCost, totalPaintings);
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
//    auto result = program2(n, W, heights, widths);
//
//    std::cout << std::get<0>(result) << std::endl;
//    std::cout << std::get<1>(result) << std::endl;
//    for(int i = 0; i < std::get<0>(result); i++){
//        std::cout << std::get<2>(result)[i] << std::endl;
//    }
//    return 0;
//}