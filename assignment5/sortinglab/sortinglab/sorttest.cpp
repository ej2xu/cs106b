#include <iostream>
#include <string>
#include "vector.h"
#include "genlib.h"
#include "random.h"
#include "simpio.h"
#include "set.h"

template <typename Type>
	void Swap(Type &one, Type &two) {
		Type tmp = one;
		one = two;
		two = tmp;
	}

template <typename Type>
	int OperatorComp(Type one, Type two) {
		if (one == two) return 0;
		return (one < two ? -1 : 1);
	}

int stringCmp(string one, string two) {
	if (one.length() < two.length()) return -1;
	if (one.length() > two.length()) return 1;
	return OperatorComp(one, two);
} 

int setCmp(Set<int> one, Set<int> two) {
	Set<int>::Iterator itOne = one.iterator();
	Set<int>::Iterator itTwo = two.iterator();
	int sumOne, sumTwo;
	sumOne = sumTwo = 0;
	while (itOne.hasNext())
		sumOne += itOne.next();
	while (itTwo.hasNext())
		sumTwo += itTwo.next();
	return OperatorComp(sumOne, sumTwo);
}

template <typename Type>
	void maxHeapify(Vector<Type> &v, int parent, int size, int (cmpFn)(Type, Type)) {
		int lchild = 2 * (parent + 1) - 1;
		int	largest, rchild = lchild + 1;
		if (lchild < size && cmpFn(v[lchild], v[parent]) > 0)
			largest = lchild;
		else largest = parent;
		if (rchild < size && cmpFn(v[rchild], v[largest]) > 0)
			largest = rchild;
		if (largest != parent) {
			Swap(v[largest], v[parent]);
			maxHeapify(v, largest, size, cmpFn);
		}
	}

template <typename Type>
	void buildMaxHeap(Vector<Type> &v, int (cmpFn)(Type, Type)) {
		int size = v.size();
		for (int i = size / 2 - 1; i >= 0; i--)
			maxHeapify(v, i, size, cmpFn);
	}

template <typename Type>
	void heapSort(Vector<Type> &v, int (cmpFn)(Type, Type) = OperatorComp) {
		buildMaxHeap(v, cmpFn);
		int size = v.size();
		for (int i = size - 1; i >= 1; i--) {
			Swap(v[i], v[0]);
			maxHeapify(v, 0, i, cmpFn);
		}
	}

template <typename Type>
	void bubbleSort(Vector<Type> &nums, int (cmpFn)(Type, Type) = OperatorComp) {
		bool sorted = false;
		int size = nums.size();
		while (!sorted) {
			sorted = true;
			for (int i = 0; i < size - 1; i++) {
				if (cmpFn(nums[i], nums[i + 1]) > 0) {
					Swap(nums[i], nums[i + 1]);
					sorted = false;
				}
			}
		}
	}

int main() {
/*	Vector<int> nums;
	Randomize();
	while (true) {
		int num = RandomInteger(0, 20);
		if (num == 0) break;
		nums.add(num);
	}
	heapSort(nums);
	for (int i = 0; i < nums.size(); i++)
		cout << nums[i] << endl; 

	Vector<string> strings;
	string s;
	while (true) {
		s = GetLine();
		if (s.length() == 0) break;
		strings.add(s);
	}
	heapSort(strings, stringCmp);
	for (int i = 0; i < strings.size(); i++)
		cout << strings[i] << endl; */

	Vector<Set<int> > integers;
	int tmp;
	Set<int> tmps;
	while (true) {
		tmp = GetInteger();
		if (tmp == -1) {
			integers.add(tmps);
			tmps.clear();
		} else if (tmp == 999) break;
		else tmps.add(tmp);
	}
	heapSort(integers, setCmp);
	for (int i = 0; i < integers.size(); i++) {
		Set<int>::Iterator it = integers[i].iterator();
		while (it.hasNext())
			cout << it.next() << ", ";
		cout << endl;
	}
	return 0;
}