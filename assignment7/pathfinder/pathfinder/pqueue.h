#ifndef _pqueue_h
#define _pqueue_h

#include "genlib.h"
#include "cmpfn.h"

template <typename ElemType> 
	class PQueue {
	  public:

		PQueue(int (cmpfn)(ElemType, ElemType) = OperatorCmp);

		~PQueue();

		bool isEmpty() const;

		int size() const;

		void clear();

		void enqueue(ElemType newElem);

		ElemType dequeueMax();
		
		void printDebuggingInfo() const;

		 
		PQueue(const PQueue &src);

		PQueue &operator=(const PQueue &src);

	  private:
	  /* implementation of heap structure */
		ElemType *arr;
		int capacity;
		int count;

		int (*cmp)(ElemType, ElemType);
		void maxHeapify(ElemType arr[], int parent, int size);
		void expandCapacity();
		void swap(ElemType &one, ElemType &two);
		void deepCopy(const PQueue &src);
	};

#include "pqheap.cpp"

#endif