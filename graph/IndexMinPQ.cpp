// C++ program for the above approach

#include <bits/stdc++.h>
using namespace std;

template <class T1, class T2,
		class Comparator = less<T2>,
		class Hash = hash<T1> >

class indexed_priority_queue {

	// Storing indices of values using key
	unordered_map<T1, long long int, Hash> m;

	// Container
	vector<pair<T1, T2> > v;

	// Size
	long long numberOfElement;

	// Creating a instance of Comparator class
	Comparator comp;

	// Max Capacity
	long long capacity = LLONG_MAX;

	// Obtaing the index value from hash map
	long long int getValueIndex(T1 key)
	{
		if (m[key] == 0) {
			cout << "No Such Key Exist";
			return -1;
		}
		return v[m[key] - 1];
	}

	// heapify the container
	void heapify(vector<pair<T1, T2> >& v,
				long long int heap_size,
				long long index)
	{
		long long leftChild = 2 * index + 1,
				rightChild = 2 * index + 2,
				suitableNode = index;

		if (leftChild < heap_size
			&& comp(v[suitableNode].second,
					v[leftChild].second)) {
			suitableNode = leftChild;
		}

		if (rightChild < heap_size
			&& comp(v[suitableNode].second,
					v[rightChild].second)) {
			suitableNode = rightChild;
		}

		if (suitableNode != index) {

			// swap the value
			pair<T1, T2> temp = v[index];
			v[index] = v[suitableNode];
			v[suitableNode] = temp;

			// updating the map
			m[v[index].first] = index + 1;
			m[v[suitableNode].first]
				= suitableNode + 1;

			// heapify other affected nodes
			heapify(v, numberOfElement,
					suitableNode);
		}
	}

public:
	indexed_priority_queue()
	{
		numberOfElement = 0;
		m.clear();
		v.clear();
	}

	void push(T1 key, T2 value)
	{
		if (numberOfElement == capacity) {
			cout << "Overflow";
			return;
		}
		if (m[key] != 0) {
			cout << "Element Already Exists";
			return;
		}

		// Adding element
		v.push_back(make_pair(key, value));
		numberOfElement++;
		m[key] = numberOfElement;

		long long index = numberOfElement - 1;

		// Comparing to parent node
		while (index != 0
			&& comp(v[(index - 1) / 2].second,
					v[index].second)) {

			// swap the value
			pair<T1, T2> temp = v[index];
			v[index] = v[(index - 1) / 2];
			v[(index - 1) / 2] = temp;

			// updating the map
			m[v[index].first] = index + 1;
			m[v[(index - 1) / 2].first]
				= (index - 1) / 2 + 1;

			// updating index in map
			index = (index - 1) / 2;
		}
	}

	void pop()
	{
		if (numberOfElement == 0) {
			cout << "UnderFlow";
			return;
		}

		// Removing element
		v.erase(v.begin());
		numberOfElement--;
		heapify(v, numberOfElement, 0);
	}

	pair<T1, T2> top() { return v[0]; }

	long long int size() { return numberOfElement; }

	bool empty() { return numberOfElement == 0; }

	bool contains(T1 key){
        return m.find (key)!=m.end();
    }

    void changeAtKey(T1 key, T2 value)
	{
		if (m[key] == 0) {
			cout << "No Such Key Exist";
			return;
		}
		long long index = m[key] - 1;
		v[index].second = value;

		// Comparing to child nodes
		heapify(v, numberOfElement, index);

		// Comparing to Parent Node
		while (index != 0
			&& comp(v[(index - 1) / 2].second,
					v[index].second)) {

			// swap the value
			pair<T1, T2> temp = v[index];
			v[index] = v[(index - 1) / 2];
			v[(index - 1) / 2] = temp;

			// updating the map
			m[v[index].first] = index + 1;
			m[v[(index - 1) / 2].first]
				= (index - 1) / 2 + 1;

			// updating index in map
			index = (index - 1) / 2;
		}
	}
};

void display(indexed_priority_queue<int, int> IPQ)
{
	indexed_priority_queue<int, int> temp = IPQ;
	while (!IPQ.empty()) {
		pair<int, int> tmp;
		tmp = IPQ.top();
		IPQ.pop();
		cout << "( " << tmp.first << ", "
			<< tmp.second << " ) ";
	}
	cout << '\n';
}