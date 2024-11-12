#include <iostream>
#include <vector>
#include <tuple>
#include <climits>
/* Solution to program 3
* @param n the number of paintings
* @param W the maximum width of the platform
* @param heights the heights of the paintings
* @param widths the widths of the paintings
* @return a tuple containing the number of platforms used, the optimal total height, and the number of paintings on each platform
*/
std::tuple<int, int, std::vector<int>> program3(int n, int W, std::vector<int> heights, std::vector<int> widths){
   int minHeight = INT_MAX;
    int bestPlatformCount = 0;
    std::vector<int> bestPaintingsOnPlatforms;

    int totalCombinations = 1 << (n - 1); // 2^(n-1) possible configurations

    // Iterate over all possible ways to divide paintings into platforms
    for (int mask = 0; mask < totalCombinations; mask++) {
        int currentPlatformWidth = 0;
        int currentPlatformHeight = 0;
        int totalHeight = 0;
        int platformCount = 1;
        std::vector<int> paintingsOnPlatforms;
        int paintingCount = 0;

        bool valid = true;

        for (int i = 0; i < n; i++) {
            if (currentPlatformWidth + widths[i] > W) {
                valid = false;
                break;
            }

            // Add painting to the current platform
            currentPlatformWidth += widths[i];
            currentPlatformHeight = std::max(currentPlatformHeight, heights[i]);
            paintingCount++;

            // If we need to start a new platform (based on mask), finalize current platform
            if ((mask & (1 << i)) || i == n - 1) { // Start new platform if bit is set or last painting
                totalHeight += currentPlatformHeight;
                paintingsOnPlatforms.push_back(paintingCount);

                currentPlatformWidth = 0;
                currentPlatformHeight = 0;
                platformCount++;
                paintingCount = 0;
            }
        }

        // Finalize and check if this configuration is valid and has minimal height
        if (valid && totalHeight < minHeight) {
            minHeight = totalHeight;
            bestPlatformCount = platformCount - 1;
            bestPaintingsOnPlatforms = paintingsOnPlatforms;
        }
    }

    return std::make_tuple(bestPlatformCount, minHeight, bestPaintingsOnPlatforms);
}

int main(){
    int n, W;
    std::cin >> n >> W;
    std::vector<int> heights(n);
    std::vector<int> widths(n);
    for(int i = 0; i < n; i++){
        std::cin >> heights[i];
    }
    for(int i = 0; i < n; i++){
        std::cin >> widths[i];
    }
    auto result = program3(n, W, heights, widths);

    std::cout << std::get<0>(result) << std::endl;
    std::cout << std::get<1>(result) << std::endl;
    for(int i = 0; i < std::get<0>(result); i++){
        std::cout << std::get<2>(result)[i] << std::endl;
    }
    return 0; 
}