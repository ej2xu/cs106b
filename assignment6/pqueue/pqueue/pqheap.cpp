#include <iostream>
#include "pqueue.h"
#include "genlib.h"

/* 	int *arr;
	int capacity;
	int count;

	void maxHeapify(int arr[], int parent, int size);
	void expandCapacity();
	void swap(int &one, int &two); */
	PQueue::PQueue() {
		capacity = 10;
		arr = new int[capacity];
		count = 0;
	}

	PQueue::~PQueue() {
		delete[] arr;
	}

	bool PQueue::isEmpty() {
		return count == 0;
	}

	int PQueue::size() {
		return count;
	}

	void PQueue::enqueue(int newValue) {
		if (count == capacity) expandCapacity();
		arr[count++] = newValue;
		int child = count - 1, parent = count / 2 - 1;
		while (parent >= 0 && arr[child] > arr[parent]) {
			swap(arr[child], arr[parent]);
			child = parent;
			parent = (child + 1) / 2 - 1; 
		}
	}

	int PQueue::dequeueMax() {
		if (count == 0)
			Error("Tried to dequeue max from an empty pqueue!");
		int max = arr[0];
		arr[0] = arr[--count];
		maxHeapify(arr, 0, count);
		return max;
	}

	void PQueue::maxHeapify(int arr[], int parent, int size) {
		int lchild = 2 * (parent + 1) - 1;
		int	largest, rchild = lchild + 1;
		if (lchild < size && arr[lchild] > arr[parent])
			largest = lchild;
		else largest = parent;
		if (rchild < size && arr[rchild] > arr[largest])
			largest = rchild;
		if (largest != parent) {
			swap(arr[largest], arr[parent]);
			maxHeapify(arr, largest, size);
		}
	}

	int PQueue::bytesUsed() {
		return sizeof(*this) + sizeof(int) * capacity;
	}

	string PQueue::implementationName() {
		return "heap";
	}

	void PQueue::printDebuggingInfo() {
		cout << "------------------ START DEBUG INFO ------------------" << endl;
		cout << "Pqueue contains " << count << " entries" << endl;
		for (int i = 0; i < count; i++) 
			cout << arr[i] << " ";
		cout << endl;
		cout << "------------------ END DEBUG INFO ------------------" << endl;
	}

	void PQueue::swap(int &one, int &two) {
			int tmp = one;
			one = two;
			two = tmp;
	}

	void PQueue::expandCapacity() {
		int *oldArr = arr;
		capacity *= 2;
		arr = new int[capacity];
		for (int i = 0; i < count; i++) {
			arr[i] = oldArr[i];
		}
		delete[] oldArr;
	}
