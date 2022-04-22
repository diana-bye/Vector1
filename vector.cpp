#include "vector.h"

Vector::Vector(const Value* rawArray, const size_t size, float coef ) {
    _size = size;
    _capacity = size;
    _multiplicativeCoef = coef;
    _data = new Value[_size];
    for(size_t i = 0; i < _size; i++){
        _data[i] = rawArray[i];
    }
}

Vector::Vector(const Vector& other) { 
    size_t _size = other._size;
    size_t _capacity = other._size;
    _data = new Value[other._size];
	float  _multiplicativeCoef = other._multiplicativeCoef;
    for(size_t i = 0; i < other._size; i++) {
        _data[i] = other._data[i];
    }
}

Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        reserve(other._size);
        _size = other._size;
        _capacity = other._size;
        _multiplicativeCoef = other._multiplicativeCoef;
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = other._data[i];
        }
    }
    return *this;
}

Vector::Vector(Vector&& other) noexcept {
    _data = other._data;
    _size = other._size;
    _capacity = other._size;
    _multiplicativeCoef = other._multiplicativeCoef;
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
    other._multiplicativeCoef = 0;
}

Vector& Vector::operator=(Vector&& other) noexcept {
    if (this != &other) {
        _data = other._data;
        _size = other._size;
        _capacity = other._size;
        _multiplicativeCoef = other._multiplicativeCoef;
        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;
        other._multiplicativeCoef = 0;
    }
    return *this;
}

Vector::~Vector() {
	delete[] _data;
}

void Vector::pushBack(const Value& value) {
    insert(value, _size);
}
	
void Vector::pushFront(const Value& value) {
     insert(value, 0);   
}

void Vector::insert(const Value& value, size_t pos) {
    insert(&value, 1, pos);
}

void Vector::insert(const Value* values, size_t size, size_t pos) {
    if(pos > _size) {
		throw std::out_of_range("pos > _size");
		return;
	}
    _size += size;
    if (_size > _capacity) {
        if (_capacity == 0) {
        _capacity = 1*_multiplicativeCoef;
        }
        while (_size > _capacity) {
            _capacity = _capacity * _multiplicativeCoef;
        }
        Value* tmp = new Value [_capacity];
        for (size_t i = 0; i < _size - size; i++) {
            tmp[i] = _data[i];
        }
        delete[] _data;
        _data = tmp;
    }
    for(size_t i = _size - 1; i > pos; i--) {
		_data[i] = std::move(_data[i - size]);
	}
	for(size_t i = 0; i < size; i++) {
		_data[pos + i] = values[i];
	}
}

void Vector::insert(const Vector& vector, size_t pos) {
    insert(vector._data, vector._size, pos);
}

void Vector::popBack(){
    if(_size > 0) {
		_size--;
	}
	else {
		throw std::out_of_range("size = 0");
	}
}

void Vector::popFront(){
    erase(0);
}

void Vector::erase(size_t pos, size_t count){
    if(pos >= _size) {
		throw std::out_of_range("pos >= _size");
		return;
	}
	if(pos + count > _size) {
		count = _size - pos;
	}
	_size -= count;
	for(size_t i = pos; i < _size; i++) {
		_data[i] = std::move(_data[i + count]);
	}
}

void Vector::eraseBetween(size_t beginPos, size_t endPos){
    if(beginPos >= endPos) {
		throw std::out_of_range("beginPos >= endPos");
		return;
	}
    erase(beginPos, endPos - beginPos);
}

size_t Vector::size() const{
    return _size;
};

size_t Vector::capacity() const{
    return _capacity;
}

 double Vector::loadFactor() const{
     return (double)_size/_capacity;
 }

Value& Vector::operator[](size_t idx) {
    return _data[idx];
}

const Value& Vector::operator[](size_t idx) const{
    return _data[idx];
}

long long Vector::find(const Value& value) const{
    for (size_t i = 0; i < _size; i++) {
		if (_data[i] == value) {
	        return i;
	    }
	}
	return -1;
}

void Vector::reserve(size_t capacity) {
    if (capacity > _capacity) {
        Value* result = new Value[capacity];
        for (size_t i = 0; i < _size; i++) {
            result[i] =  _data[i];
        }
        delete[] _data;
        _data = result;
        _capacity = capacity;
    }
}

void Vector::shrinkToFit() {
    if(_capacity > _size) {
        _capacity = _size;
    }
    Value* result = new Value[_capacity];
    for (size_t i = 0; i < _size; i++) { 
        result[i] = _data[i];
    }
    delete[] _data;
    _data = result;  
}

Vector::Iterator::Iterator(Value* ptr) {
    _ptr = ptr;
}

Value& Vector::Iterator::operator*() {
    return *_ptr;
}

const Value& Vector::Iterator::operator*() const {
    return *_ptr;
}

Value* Vector::Iterator::operator->(){
    return _ptr;
}

const Value* Vector::Iterator::operator->() const {
    return _ptr;
}
    
Vector::Iterator Vector::Iterator::operator++() {
    ++_ptr;
    return *this;
}
    
Vector::Iterator  Vector::Iterator::operator++(int) {
    Vector::Iterator oldValue(*this);
    ++(*this);
    return oldValue;
}
    
bool Vector::Iterator::operator==(const Iterator& other) const {
    return _ptr == other._ptr;
}
    
bool Vector::Iterator::operator!=(const Iterator& other) const {
    return !(_ptr == other._ptr);
}

Vector::Iterator Vector::begin() {
    Vector::Iterator head(_data);
    return head;
}

Vector::Iterator   Vector::end() {
    Vector::Iterator tail(_data + _size);
    return tail;
}