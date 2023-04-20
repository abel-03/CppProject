#include <iostream>

int* BinarySearch(int* arr, int* end, int target) {
    int last_index = end - arr;
    int first_index = 0;
    int middle = (end - arr) / 2;
    if (*end <= target) {
        return end;
    }
    if (*arr == target) {
        return arr;
    }
    while (middle > 1) {
        middle = (last_index - first_index) / 2;
        if (arr[first_index + middle] == target) {
            return arr + first_index + middle;
        }
        arr[first_index + middle] < target ? first_index += middle: last_index = first_index + middle;  
    }
    return arr + last_index;
}
int main() {
    int len_arr;
    int num_arr[5];
    int num_call;
    int t;
    std::cin >> len_arr;
    for (int l = 0; l < len_arr; l++) {
        std::cin >> num_arr[l];
    }
    std::cin >> t;
    std::cout << *BinarySearch(num_arr, num_arr + len_arr - 1, t);
    return 0;
}
