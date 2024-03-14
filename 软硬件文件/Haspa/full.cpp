#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bitset>

struct Solution {
    std::bitset<11> variables;
    int gain;
    int s;
};

bool compareSolutions(const Solution& a, const Solution& b) {
    return a.gain > b.gain;
}

int main() {
    std::vector<Solution> solutions;
    Solution bestSolution;

    for (int i = 0; i < (1 << 11); i++) {
        Solution solution;
        solution.variables = std::bitset<11>(i);

        // Evaluate the gain and s for the current solution
        solution.gain = solution.variables[0] * 2 + solution.variables[1] * 2 + solution.variables[2] * 3 +
                        solution.variables[3] * 3 + solution.variables[4] * 2 + solution.variables[5] * 6 +
                        solution.variables[6] * 1 + solution.variables[7] * 2 + solution.variables[8] * 7 +
                        solution.variables[9] * 3 + solution.variables[10] * 11;

        solution.s = solution.variables[0] * 4 + solution.variables[1] * 2 + solution.variables[2] * 4 +
                     solution.variables[3] * 7 + solution.variables[4] * 5 + solution.variables[5] * 3 +
                     solution.variables[6] * 4 + solution.variables[7] * 2 + solution.variables[8] * 3 +
                     solution.variables[9] * 5 + solution.variables[10] * 5;

        // Check if the solution satisfies the constraint
        if (solution.s <= 20) {
            solutions.push_back(solution);
        }
    }

    // Sort the solutions based on gain in descending order
    std::sort(solutions.begin(), solutions.end(), compareSolutions);

    if (!solutions.empty()) {
        bestSolution = solutions[0];
        std::cout << "Best Solution: ";
        for (int i = 0; i < 11; i++) {
            std::cout << bestSolution.variables[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Gain: " << bestSolution.gain << std::endl;
        std::cout << "S: " << bestSolution.s << std::endl;
    } else {
        std::cout << "No feasible solution found." << std::endl;
    }

    return 0;
}