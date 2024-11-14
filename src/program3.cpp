#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <climits>   // For LLONG_MAX
#include <cstdint>   // For fixed-width integer types
#include <bitset>    // For bit manipulation functions
#include <mutex>     // For std::mutex
#include <omp.h>     // For OpenMP directives
/* Solution to program 3
* @param n the number of paintings
* @param W the maximum width of the platform
* @param heights the heights of the paintings
* @param widths the widths of the paintings
* @return a tuple containing the number of platforms used, the optimal total height, and the number of paintings on each platform
*/
std::tuple<int, long long, std::vector<int>> program3(int n, int W, const std::vector<int>& heights, const std::vector<int>& widths) {
    long long minHeight = LLONG_MAX;
    int bestPlatformCount = 0;
    std::vector<int> bestPaintingsOnPlatforms;

    // Use unsigned long long for masks to handle up to 64 bits
    unsigned long long totalCombinations = 1ULL << (n - 1); // 2^(n-1) possible configurations

    // Iterate over all possible ways to divide paintings into platforms
    for (unsigned long long mask = 0; mask < totalCombinations; mask++) {
        long long currentPlatformWidth = 0;
        long long currentPlatformHeight = 0;
        long long totalHeight = 0;
        int platformCount = 1; // Start with one platform
        std::vector<int> paintingsOnPlatforms;
        int paintingCount = 0;

        bool valid = true;

        for (int i = 0; i < n; i++) {
            // Check if adding the current painting exceeds the platform width
            if (currentPlatformWidth + widths[i] > W) {
                valid = false;
                break;
            }

            // Add painting to the current platform
            currentPlatformWidth += widths[i];
            currentPlatformHeight = std::max(currentPlatformHeight, static_cast<long long>(heights[i]));
            paintingCount++;

            // If we need to start a new platform (based on mask)
            if ((i < n - 1) && (mask & (1ULL << i))) { // Use 1ULL to ensure 64-bit shift
                totalHeight += currentPlatformHeight;
                paintingsOnPlatforms.push_back(paintingCount);

                // Reset for the next platform
                currentPlatformWidth = 0;
                currentPlatformHeight = 0;
                platformCount++;
                paintingCount = 0;
            }
        }

        // Finalize the last platform
        totalHeight += currentPlatformHeight;
        paintingsOnPlatforms.push_back(paintingCount);

        // Calculate the total number of paintings
        int numberOfPaintings = std::accumulate(paintingsOnPlatforms.begin(), paintingsOnPlatforms.end(), 0);

        // Check if this configuration is valid and has minimal height
        if (valid && totalHeight < minHeight && numberOfPaintings == n) {
            minHeight = totalHeight;
            bestPlatformCount = platformCount;
            bestPaintingsOnPlatforms = paintingsOnPlatforms; // Replace instead of appending
        }
    }

    // If no valid configuration is found, return indicative values
    if (bestPlatformCount == 0) {
        return std::make_tuple(0, -1, std::vector<int>());
    }

    return std::make_tuple(bestPlatformCount, minHeight, bestPaintingsOnPlatforms);
}

//
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
//    auto result = program3(n, W, heights, widths);
//
//    std::cout << std::get<0>(result) << std::endl;
//    std::cout << std::get<1>(result) << std::endl;
//    for(int i = 0; i < std::get<0>(result); i++){
//        std::cout << std::get<2>(result)[i] << std::endl;
//    }
//    return 0;
//}