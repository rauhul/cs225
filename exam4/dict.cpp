#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <cmath>

using namespace std;

pair<int, int> getPair(int arr[], int n, int sum) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                if (arr[i]*arr[i] + arr[j]*arr[j] == sum) {
                    return make_pair(arr[i], arr[j]);
                }
            }
        }    
    }
    return make_pair(0, 0);
}
