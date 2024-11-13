#include <iostream>
#include <vector>
#include <tuple>
#include <map>
/* Solution to program 1
* @param n the number of paintings
* @param W the maximum width of the platform
* @param heights the heights of the paintings
* @param widths the widths of the paintings
* @return a tuple containing the number of platforms used, the optimal total height, and the number of paintings on each platform
*/
std::tuple<int, int, std::vector<int>> program1(int n, int MAX_WIDTH, std::vector<int> heights, std::vector<int> widths){
    auto totalCost=heights[0];
    std::vector<int> totalpaintings; //total painting on each platform

    auto currentWidth=widths[0];
    for(auto index=0; index<n;){
        totalpaintings.push_back(0);
        while(currentWidth<=MAX_WIDTH and index<n){
            currentWidth+=widths[++index];
            totalpaintings[totalpaintings.size()-1]+=1;
        }
        if(index>=n){
            break;
        }
        currentWidth=widths[index];
        totalCost+=heights[index];
    }

    return std::make_tuple(totalpaintings.size(), totalCost, totalpaintings); // replace with your own result.
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
//    auto result = program1(n, W, heights, widths);
//
//    std::cout << std::get<0>(result) << std::endl;
//    std::cout << std::get<1>(result) << std::endl;
//    for(int i = 0; i < std::get<0>(result); i++){
//        std::cout << std::get<2>(result)[i] << std::endl;
//    }
//    return 0;
//}