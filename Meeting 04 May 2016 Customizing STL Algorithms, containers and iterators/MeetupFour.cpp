// MeetupFour.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <numeric>
#include <functional>

// #include <boost/algorithm/cxx11/copy_if.hpp>

#include "meetingfour.h"

void Test_ContPIterator()
{
	ContPIterator<int, 10> container;

	int n = 0;
	std::generate(container.begin(), container.end(), [&n] { return n++; });

	PrintContainer(container);

	std::transform(container.begin(), container.end(), container.begin(),
		[](int& c) { return c + 5; });

	std::sort(container.begin(), container.end(), std::greater<>());

	PrintContainer(container);
}

void Test_ContInsert()
{
	ContInsert<int> container;

	std::fill_n(std::inserter<ContInsert<int>>(container, 0), 5, 1);

	PrintContainer(container);

	std::transform(container.begin(), container.end(), container.begin(),
		[](int& n) { return n * 5 + (RandomBool() ? 5 : 10 ); });

	PrintContainer(container);
}

void Test_ContPushBack()
{
	ContPushBack<int> container;

	std::vector<int> v(20);
	int n = 0;
	std::generate(v.begin(), v.end(), [&n] { return n++; });

	std::copy_if(v.begin(), v.end(),
		std::back_inserter<ContPushBack<int>>(container),
		[](int n) { return n % 2 == 0; });

	PrintContainer(container);

	std::for_each(container.begin(), container.end(),
								[](int& n) { n = n % 3; });

	PrintContainer(container);
}

void Test_WierdContainer()
{
	WierdContainer<int> container;

	// add 5 ones
	std::fill_n(std::inserter<WierdContainer<int>>(container, container.begin()), 5, 1);

	PrintContainer(container);

	// add 0 to 19 in a vector
	std::vector<int> v(20);	int n = 0;
	std::generate(v.begin(), v.end(), [&n] { return n++; });

	// copy to container only the even ones
	std::copy_if(v.begin(), v.end(), std::inserter<WierdContainer<int>>(container, container.begin()), [](int n) { return n % 2 == 0; });

	PrintContainer(container);

	// add 5 to each element
	std::transform(container.begin(), container.end(), container.begin(),
		[](int& c) { return c + 5; });

	PrintContainer(container);

	// check if all are positive
	bool isPositive = std::any_of(container.begin(), container.end(), [](auto& n) { return n > 0; });

	// slide them forward by 1 element
	std::rotate(container.begin(), ++container.begin(), container.end());

	PrintContainer(container);

	// sum all elements
	int sum = std::accumulate(container.begin(), container.end(), 0);
	std::cout << sum << '\n';

	// find first adjesten that first is greater than second
	auto it = std::adjacent_find(container.begin(), container.end(), std::greater<int>());

	std::cout << *it << '\n';
	WierdContainer<int> container2;

	// use boost! algorithm copy until the element that is equal to 17
	//boost::algorithm::copy_until(container.begin(), container.end(), std::inserter<WierdContainer<int>>(container2, container2.begin()), [](int i) { return i == 17; });
	//PrintContainer(container2);
}

void Test_Algorithm() 
{
	std::vector<int> v({ 1,1,1,1,1,2,1,1,2,1,1 });

	// { 1,1,1,1,1,2,1,1,2,1,1 }
	bool bIsVector1 = is_every_D_after_first_occurrence(v.begin(), v.end(), 3, 2);	// true
	// { 1,1,1,1,1,2,1,1,2,1,1 }
	bool bIsVector2 = is_every_D_after_first_occurrence(v.begin(), v.end(), 3, 1);	// true

	std::list<int> l({ 1,1,2,1,1,2,1,1,2,1,1,3 });
	// { 1,1,2,1,1,2,1,1,2,1,1,3 }
	bool bIsList1 = is_every_D_after_first_occurrence(l.begin(), l.end(), 3, 2);	// false
	// { 1,1,2,1,1,2,1,1,2,1,1,3 }
	bool bIsList2 = is_every_D_after_first_occurrence(l.begin(), l.end(), 3, 1);	// true
}


int main()
{
	std::cout << "\n Test_ContPIterator\n\n";
	Test_ContPIterator();
	std::cout << "\n Test_ContInsert \n\n";
	Test_ContInsert();
	std::cout << "\n Test_ContPushBack \n\n";
	Test_ContPushBack();
	std::cout << "\n Test_WierdContainer  \n\n";
	Test_WierdContainer();
	std::cout << "\n Test_Algorithm  \n\n";
	Test_Algorithm();

	return 0;
}

