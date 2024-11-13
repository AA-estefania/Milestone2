#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <climits>
/* Solution to program 4
* @param n the number of paintings
* @param W the maximum width of the platform
* @param heights the heights of the paintings
* @param widths the widths of the paintings
* @return a tuple containing the number of platforms used, the optimal total height, and the number of paintings on each platform
*/
std::tuple<int, int, std::vector<int>> program4(int n, int W, std::vector<int> heights, std::vector<int> widths){
     std::vector<long> dp(n + 1, LONG_MAX);
    std::vector<long> split(n + 1, -1);

    dp[0] = 0; 

    for (int i = 1; i <= n; ++i) {
        long maxHeight = 0;
        long totalWidth = 0;

        for (int j = i; j >= 1; --j) {
            totalWidth += widths[j - 1];
            if (totalWidth > W) break; 

            maxHeight = std::max(maxHeight, (long)heights[j - 1]);
            if (dp[j - 1] + maxHeight < dp[i]) {
                dp[i] = dp[j - 1] + maxHeight;
                split[i] = j - 1;
            }
        }
    }

    std::vector<int> platforms;
    for (auto i = n; i > 0; i = split[i]) {
        platforms.push_back(i - split[i]);
    }
    std::reverse(platforms.begin(), platforms.end());

    // Replace this with your own result if needed
    // return std::make_tuple(0, 0, std::vector<int>()); // replace with your own result.
    return std::make_tuple(platforms.size(), dp[n], platforms);
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
//    auto result = program4(n, W, heights, widths);
//
//    std::cout << std::get<0>(result) << std::endl;
//    std::cout << std::get<1>(result) << std::endl;
//    for(int i = 0; i < std::get<0>(result); i++){
//        std::cout << std::get<2>(result)[i] << std::endl;
//    }
//    return 0;
//}