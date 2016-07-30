#include <iostream>
#include "pqueue.h"
#include "genlib.h"

template <typename ElemType>
	PQueue<ElemType>::PQueue(int (*cmpfn)(ElemType, ElemType)) {
		cmp = cmpfn;
		capacity = 10;
		arr = new ElemType[capacity];
		count = 0;
	}

template <typename ElemType>
	PQueue<ElemType>::~PQueue() {
		delete[] arr;
	}

template <typename ElemType>
	bool PQueue<ElemType>::isEmpty() const {
		return count == 0;
	}

template <typename ElemType>
	int PQueue<ElemType>::size() const {
		return count;
	}

template <typename ElemType>
	void PQueue<ElemType>::clear() {
		count = 0;
	}

template <typename ElemType>
	void PQueue<ElemType>::enqueue(ElemType newValue) {
		if (count == capacity) expandCapacity();
		arr[count++] = newValue;
		int child = count - 1, parent = count / 2 - 1;
		while (parent >= 0 && cmp(arr[child], arr[parent]) > 0) {
			swap(arr[child], arr[parent]);
			child = parent;
			parent = (child + 1) / 2 - 1; 
		}
	}

template <typename ElemType>
	ElemType PQueue<ElemType>::dequeueMax() {
		if (count == 0)
			Error("Tried to dequeue max from an empty pqueue!");
		ElemType max = arr[0];
		arr[0] = arr[--count];
		maxHeapify(arr, 0, count);
		return max;
	}

template <typename ElemType>
	void PQueue<ElemType>::maxHeapify(ElemType arr[], int parent, int size) {
		int lchild = 2 * (parent + 1) - 1;
		int	largest, rchild = lchild + 1;
		if (lchild < size && cmp(arr[lchild], arr[parent]) > 0)
			largest = lchild;
		else largest = parent;
		if (rchild < size && cmp(arr[rchild], arr[largest]) > 0)
			largest = rchild;
		if (largest != parent) {
			swap(arr[largest], arr[parent]);
			maxHeapify(arr, largest, size);
		}
	}

template <typename ElemType>
	void PQueue<ElemType>::printDebuggingInfo() const {
		cout << "------------------ START DEBUG INFO ------------------" << endl;
		cout << "Pqueue contains " << count << " entries" << endl;
		for (int i = 0; i < count; i++) 
			cout << arr[i] << " ";
		cout << endl;
		cout << "------------------ END DEBUG INFO ------------------" << endl;
	}

template <typename ElemType>
	void PQueue<ElemType>::swap(ElemType &one, ElemType &two) {
			ElemType tmp = one;
			one = two;
			two = tmp;
	}

template <typename ElemType>
	void PQueue<ElemType>::expandCapacity() {
		ElemType *oldArr = arr;
		capacity *= 2;
		arr = new ElemType[capacity];
		for (int i = 0; i < count; i++) {
			arr[i] = oldArr[i];
		}
		delete[] oldArr;
	}

template <typename ElemType> 
    PQueue<ElemType>::PQueue(const PQueue &src) {
		deepCopy(src);					
	}

template <typename ElemType> 
    PQueue<ElemType> &PQueue<ElemType>::operator=(const PQueue &src) {
		if (this != &src) {
			delete[] arr;
			deepCopy(src);			
		}
		return *this;
	}

template <typename ElemType> 
void PQueue<ElemType>::deepCopy(const PQueue &src) {
	arr = new ElemType[src.count];
	for (int i = 0; i < src.count; i++) {
		arr[i] = src.arr[i];
	}
	count = src.count;
	capacity = src.count;
}