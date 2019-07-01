#pragma once

#ifndef __HASHTABLE_HPP
#define __HASHTABLE_HPP



template<typename T1, typename T2>
class hashItem;

template<typename T1, typename T2>
class hashTable
{
	using  String = std::string;
	template<typename TT>
	using  List =  std::list<TT>;

	static_assert(sizeof(int) == sizeof(T1) || sizeof(String) == sizeof(T1),
		"T type is not the specified Key including int and string");
public:
	hashTable();
	~hashTable();

	void insert(const T1& key, const T2& value);
	T2 values(const T1& key);
	List<T2> values();
	T2 operator[] (const T1& key);
	T2 operator() (const T1& key);
	void remove(const T1& key);
	void clear();
	bool contains(const T1& key);
	bool isEmpty();
	int count();
private:
	int gethash(T1 key);
	template<typename TT1>
	static int gethash_(const TT1& key) {
		return 0;
	}
	template<>
	static int gethash_<int>(const int& key)
	{
		return key;
	}
	template<>
	static int gethash_<String>(const String& key)
	{
		int seed = 131; // 31 131 1313 13131 131313 etc..
		int hash = 0;
		String Key = key;
		const char * str = Key.c_str();
		while (*str)
		{
			hash = hash * seed + (*str++);
		}
		return (hash & 0x7FFFFFFF);
	}

	bool putitem(hashItem<T1, T2> *item,hashItem<T1, T2> ** container,int capacity);
	void resize(int newsize);
private:
	int _capacity;
	int _maxcapacity;
	int _currentsize;
	static int _initalcapacity;
	static float _loadFactor;

	
	hashItem<T1, T2> ** _container;
};
template<typename T1, typename T2>
class hashItem
{
public:
	hashItem(const T1& key, const T2& value) : _key(key), _value(value) {};

	const T1& getKey() {
		return _key;
	}

	const T2& getValue() {
		return _value;
	}

	void setValue(const T2& value) {
		_value = value;
	}

	hashItem<T1, T2>* next() {
		return _next;
	}

	void setNext(hashItem<T1, T2>* next) {
		_next = next;
	}

private:
	T1 _key;
	T2 _value;
	hashItem<T1, T2>* _next;
};

template<typename T1, typename T2>
int hashTable<T1, T2>::_initalcapacity = 16;

template<typename T1, typename T2>
float hashTable<T1, T2>::_loadFactor = 0.75f;

template<typename T1, typename T2>
hashTable<T1, T2>::hashTable() :
	_capacity(_initalcapacity), _maxcapacity((int)(_initalcapacity*_loadFactor)), _currentsize(0){
	_container = new hashItem<T1, T2>* [_initalcapacity]();
}

template<typename T1, typename T2>
hashTable<T1,T2>::~hashTable(){
	clear();

	delete[] _container;
}

template<typename T1, typename T2>
void hashTable<T1, T2>::insert(const T1& key,const T2& value){
	if (_currentsize >= _maxcapacity) {
		resize(_capacity * 2);
	}
	hashItem<T1,T2> *item = new hashItem<T1,T2>(key,value);
	if (putitem(item, _container,_capacity)==true) {
		++ _currentsize;
	}
}

template<typename T1, typename T2>
T2 hashTable<T1, T2>::values(const T1& key) {
	int hash = gethash(key);
	int index = hash & (_capacity - 1);

	hashItem<T1, T2>* temp = _container[index];

	while (temp != NULL) {
		if (temp->getKey() == key) {
			return temp->getValue();
		}
		else {
			temp = temp->next();
		}
	}
	return NULL;
}

template<typename T1, typename T2>
hashTable<T1, T2>::List<T2> hashTable<T1, T2>::values() {
	List<T2> res;
	for (int i = 0; i < _capacity; ++i) {
		hashItem<T1, T2> * item = _container[i];
		while (item != NULL) {
			res.push_back(item->getValue());
			item = item->next();
		}
	}
	return res;
}


template<typename T1, typename T2>
T2 hashTable<T1, T2>::operator[](const T1& key){
	return values(key);
}

template<typename T1, typename T2>
T2 hashTable<T1, T2>::operator()(const T1& key){
	return values(key);
}

template<typename T1, typename T2>
void hashTable<T1, T2>::remove(const T1 & key){
	int hash = gethash(key);
	int index = hash & (_capacity - 1);

	if (_container[index]->getKey() == key){
		hashItem<T1, T2>* next = _container[index]->next();
		delete _container[index];
		_container[index] = next;
		--_currentsize;
		return;
	}
	else {
		hashItem<T1, T2>* temp = _container[index];
		while (temp->next()!= NULL) {
			if (temp->next()->getKey() == key) {
				hashItem<T1, T2>* next = temp->next()->next();
				delete temp->next();
				temp->setNext(next);
				--_currentsize;
				return;
			}
			temp = temp->next();
		}
	}
	if (_currentsize <= _capacity / 4) {
		resize(_capacity / 2);
	}
}

template<typename T1, typename T2>
void hashTable<T1, T2>::clear(){
	for (int i = 0; i < _capacity; ++i) {
		hashItem<T1, T2> * item = _container[i];
		while (item != NULL) {
			hashItem<T1, T2> * next = item->next();
			delete item;
			item = next;
		}
	}
	
	_currentsize = 0;
	_capacity = _initalcapacity;
	_maxcapacity = (int)(_capacity * _loadFactor);
}

template<typename T1, typename T2>
bool hashTable<T1, T2>::contains(const T1& key) {
	return values(key) != NULL;
}


template<typename T1, typename T2>
bool hashTable<T1, T2>::isEmpty() {
	return _currentsize == NULL;
}


template<typename T1, typename T2>
int hashTable<T1, T2>::count() {
	return _currentsize;
}

template<typename T1, typename T2>
int hashTable<T1, T2>::gethash(T1 key) {
	return gethash_(key);
}

template<typename T1, typename T2>
bool hashTable<T1, T2>::putitem(hashItem<T1, T2>* item, hashItem<T1, T2>** container, int capacity){
	int hash = gethash(item->getKey());
	int index = hash & (capacity - 1);// hash % _capacity;
	
	if (container[index] == NULL) {
		container[index] = item;
		return true;
	}
	else {
		hashItem<T1, T2>* temp = container[index];
		while (temp != NULL) {
			if ((item->getValue() == temp->getValue()) && (item->getKey() == temp->getKey())) {
				return false;
			}
			else if ((item->getKey() == temp->getKey()) && (item->getValue() != temp->getValue())) {
				temp->setValue(item->getValue());
				return true;
			}
			else {
				if (temp->next() == NULL) {
					temp->setNext(item);
					return true;
				}
				temp = temp->next();
			}
		}
	}
	return false;
}

template<typename T1, typename T2>
void hashTable<T1, T2>::resize(int newsize){
	hashItem<T1, T2> ** newcontainer = new hashItem<T1, T2>*[newsize]();
	for (int i = 0; i < _capacity; ++i) {
		hashItem<T1, T2> * item = _container[i];
		while (item != NULL) {
			item->setNext(NULL);
			putitem(item, newcontainer, newsize);
			item = item->next();
		}
	}
	_container = newcontainer;
	_capacity = newsize;
	_maxcapacity = (int)(_capacity * _loadFactor);
}

#endif // __HASHTABLE_HPP


