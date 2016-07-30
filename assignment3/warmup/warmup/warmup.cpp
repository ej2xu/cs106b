#include <iostream>
#include "genlib.h"
#include "simpio.h"
#include "vector.h"

bool recSum(Vector<int> & nums, int targetSum, int index) {
	if (targetSum == 0)
		return true;
	if (index >= nums.size())
		return false;
	if (recSum(nums, targetSum - nums[index], index + 1)) {
		cout << nums[index] << ", ";
		return true;
	}
	return recSum(nums, targetSum, index + 1);
}

bool altRecSum(Vector<int> & nums, int targetSum, int index) {
	if (targetSum == 0)
		return true;
	for (int i = index; i < nums.size(); i++) {
		if (altRecSum(nums, targetSum - nums[i], i + 1))
			return true;
	}
	return false;
}
bool sum(Vector<int> & nums, int targetSum) {
	return altRecSum(nums, targetSum, 0);
}

void printInBinary(int num) {
	if (num == 0) return;
	printInBinary(num / 2);
	cout << num % 2;
}

int main() {
	Vector<int> nums;
	while(true) {
		cout << "test for canmakesum: ";
		int num = GetInteger();
		if (num == 0) break;
		nums.add(num);
	}
	while(true) {
		cout << "test for canmakesum: ";
		int targetSum = GetInteger();
		if (targetSum == 0) break;
		cout << "test result: " << sum(nums, targetSum) << endl;
	}

	return 0;
}