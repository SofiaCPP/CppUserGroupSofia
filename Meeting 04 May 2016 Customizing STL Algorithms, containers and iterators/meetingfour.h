#pragma once

// Help functions 

// returning true or false 
inline bool RandomBool()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::bernoulli_distribution d(0.50);

	return d(gen);
}

// prints the container
template<class T> void PrintContainer(const T& container)
{
	for (const auto& i : container)
		std::cout << i << ' ';

	std::cout << '\n';
}


// Simple container with pointers as iterators
template<class T, int N = 100>
class ContPIterator
{
	T data[N];

public:
	auto begin() { return &data[0]; }
	auto end() { return &data[N]; }	 // we never write here

	auto begin() const { return &data[0]; }
	auto end() const { return &data[N]; }	// we never write here

};

template<class T>
struct ContInsert {

	using iterator = int;
	using value_type = T;

	iterator insert(iterator, const T&  value)
	{
		m_Vect.push_back(value);
		return 0;
	}
	auto begin()  { return m_Vect.begin(); }
	auto end()  { return m_Vect.end(); }

	auto begin() const { return m_Vect.cbegin(); }
	auto end() const { return m_Vect.cend(); }

private:
	std::vector<T> m_Vect;
};

template<class T>
struct ContPushBack {

	using value_type = T;

	void push_back(const T& value)
	{
		m_Vect.push_back(value);
	}

	auto begin() { return m_Vect.begin(); }
	auto end() { return m_Vect.end(); }

	auto begin() const { return m_Vect.cbegin(); }
	auto end() const { return m_Vect.cend(); }
private:
	std::vector<T> m_Vect;
};

template<class T>
class WierdNode {
	std::unique_ptr<WierdNode<T>> left;
	std::unique_ptr<WierdNode<T>> right;
	T value;
	WierdNode<T>* parent;

public:
	WierdNode(const T& v, WierdNode<T>* p) : value(v), parent(p) {}
	T& Value() { return value; }

	void insert(const T& value)
	{
		if (RandomBool())
		{
			if (left)
				left->insert(value);
			else
				left = std::make_unique<WierdNode<T>>(value, this);
		}
		else
		{
			if (right)
				right->insert(value);
			else
				right = std::make_unique<WierdNode<T>>(value, this);
		}
	}

	WierdNode<T>* next()
	{
		if (left)
			return left.get();

		if (right)
			return right.get();
			
		return parent->next(this);
	}

private:

	WierdNode<T>* next(WierdNode<T>* child)
	{
		if (left.get() == child)
			if (right)
				return right.get();

		if (parent == nullptr)
			return nullptr;
		else
			return parent->next(this);
	}

};

template<class Container>
class WierdIterator : public std::iterator<std::forward_iterator_tag, typename Container::value_type>
{
public:
	using value_type = typename Container::value_type;
	using  _Myiter  = WierdIterator<Container>;
	using  _Nodeptr = typename Container::_Nodeptr;

	WierdIterator()
		: _Ptr(nullptr)
	{	// construct with null node pointer
	}

	WierdIterator(_Nodeptr _Pnode)
		: _Ptr(_Pnode)
	{
	}

	value_type& operator*() 
	{	// return designated value
		return _Ptr->Value();
	}

	value_type* operator->() 
	{	// return pointer to class object
		return &_Ptr->Value();
	}

	_Myiter& operator++()
	{	// preincrement
		_Ptr = _Ptr != nullptr ? _Ptr->next() : nullptr;
		return (*this);
	}

	_Myiter operator++(int)
	{	// postincrement
		_Myiter _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	bool operator==(const _Myiter& _Right) const
	{	// test for iterator equality
		return (_Ptr == _Right._Ptr);
	}

	bool operator!=(const _Myiter& _Right) const
	{	// test for iterator inequality
		return (!(*this == _Right));
	}

	_Nodeptr _Ptr;	// pointer to node
};

template<class T>
class WierdContainer {

	std::unique_ptr<WierdNode<T>> root;

public:
	using value_type = T;
	using iterator = WierdIterator<WierdContainer<T>>;
	using _Nodeptr = WierdNode<T>*;

	iterator insert(iterator, const T&  value)
	{
		if (root)
			root->insert(value);
		else
		{
			root = std::make_unique<WierdNode<T>>(value, nullptr);
		}

		return iterator();
	}
 
	iterator begin() const 
	{
		return iterator(root.get());
	}

	iterator end() const 
	{
		return iterator();
	}
};

template <class Iter>
bool is_every_D_impl(std::random_access_iterator_tag, Iter _First, Iter _Last, size_t D)
{
	if (_First == _Last)
		return false;

	auto N = *_First;
	do
	{
		if (_Last - _First <= D)
			return true;

		_First += D; 		

		if (N != *_First)
			return false;

	} while (true);
}

template <class Iter>
bool is_every_D_impl(std::forward_iterator_tag, Iter _First, Iter _Last, size_t D)
{
	if (_First == _Last)
		return false;

	auto N = *_First;
	do
	{
		for (auto i = 0; i < D; ++i)
			if (++_First == _Last)
				return true;

		if (N != *_First)
			return false;

	} while (true);
}

template <class Iter>
bool is_every_D(Iter _First, Iter _Last, size_t D)
{
	return is_every_D_impl(typename std::iterator_traits<Iter>::iterator_category(), _First, _Last, D);
}

//template <class Iter>
//bool is_every_D(Iter _First, Iter _Last, size_t D)
//{
//	if (_First == _Last)
//		return false;
//
//	auto N = *_First;
//	do
//	{
//		for (auto i = 0; i < D; ++i)
//			if (++_First == _Last)
//				return true;
//
//		if (N != *_First)
//			return false;
//
//	} while (true);
//}
//

template <class Iter, class Type>
bool is_every_D_after_first_occurrence(Iter _First, Iter _Last, size_t D, Type N)
{
	auto _Found = std::find(_First, _Last, N);

	return is_every_D(_Found, _Last, D);
}	


//template <class Iter, class Type>
//bool is_every_D_after_first_occurrence(Iter _First, Iter _Last, size_t D, Type N)
//{
//	auto _Found = std::find(_First, _Last, N);
//
//	if (_First == _Last)
//		return false;
//
//	do 
//	{
//		for (auto i = 0; i < D; ++i)
//			if (++_Found == _Last)
//				return true;
//
//		if (N != *_Found)
//			return false;
//
//	} while (true);
//}
//

