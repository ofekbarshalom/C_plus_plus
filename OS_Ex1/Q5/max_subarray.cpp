#include <iostream>
#include <vector>
#include <cstdlib>  // for rand(), atoi()
using namespace std;

// Random vector generator
vector<int> generateRandomVector(int size) {
    vector<int> vec(size);
    for (int i = 0; i < size; ++i)
        vec[i] = rand() % 100 - 25;  // Random numbers in range [-25, 74]
    return vec;
}

// O(n^3) algorithm
int maxSum_N3(const vector<int>& arr) {
    int maxSum = arr[0];
    int n = arr.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            int sum = 0;
            for (int k = i; k <= j; ++k)
                sum += arr[k];
            if (sum > maxSum)
                maxSum = sum;
        }
    }
    return maxSum;
}

// O(n^2) algorithm
int maxSum_N2(const vector<int>& arr) {
    int maxSum = arr[0];
    int n = arr.size();
    for (int i = 0; i < n; ++i) {
        int sum = 0;
        for (int j = i; j < n; ++j) {
            sum += arr[j];
            if (sum > maxSum)
                maxSum = sum;
        }
    }
    return maxSum;
}

// O(n) algorithm (Kadane's)
int maxSum_N(const vector<int>& arr) {
    int maxSoFar = arr[0], maxEndingHere = arr[0];
    for (int i = 1; i < arr.size(); ++i) {
        maxEndingHere = max(arr[i], maxEndingHere + arr[i]);
        maxSoFar = max(maxSoFar, maxEndingHere);
    }
    return maxSoFar;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <seed> <size>\n";
        return 1;
    }

    int seed = atoi(argv[1]);
    int size = atoi(argv[2]);

    srand(seed);

    // Generate random input
    vector<int> data = generateRandomVector(size);

    // Run all three algorithms (for profiling only)
    maxSum_N3(data);
    maxSum_N2(data);
    maxSum_N(data);

    return 0;
}
