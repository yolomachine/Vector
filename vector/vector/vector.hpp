#pragma once
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <initializer_list>
#include <iterator>
#include "allocator.hpp"

namespace stl_compatible {

	class exception : public std::exception {};

	template<typename T, typename A = allocator<T>>
	class vector {
	public:
		typedef std::size_t size_type;
		typedef T value_type;
		typedef A allocator_type;

		struct iterator : public std::iterator<std::random_access_iterator_tag, T> {
		public:

			iterator() : ptr(nullptr) {};
			iterator(pointer ptr) : ptr(ptr) {};
			iterator(iterator& other) : ptr(other.ptr) {};

			iterator& operator=(const iterator&);

			iterator& operator++();
			iterator& operator--();
			iterator operator++(int);
			iterator operator--(int);
			iterator& operator+=(difference_type);
			iterator& operator-=(difference_type);
			difference_type operator-(const iterator&) const;
			iterator operator+(difference_type) const;
			iterator operator-(difference_type) const;

			reference operator*() const;
			pointer operator->() const;
			reference operator[](difference_type) const;

			bool operator==(const iterator&) const;
			bool operator!=(const iterator&) const;
			bool operator>(const iterator&) const;
			bool operator<(const iterator&) const;
			bool operator>=(const iterator&) const;
			bool operator<=(const iterator&) const;

		private:
			pointer ptr;

		};

		vector() {};
		vector(const vector<T>&);
		vector(size_type);
		vector(size_type, const T&);
		vector(std::initializer_list<T>);

		template<typename InputIterator>
		vector(InputIterator, InputIterator);

		~vector();

		vector<T, A>& operator=(const vector<T, A>&);
		vector<T, A>& operator=(vector<T, A>&&);
		vector<T, A>& operator=(std::initializer_list<T>);

		iterator begin() noexcept;
		iterator end() noexcept;

		size_type size() const noexcept;
		size_type max_size() const noexcept;
		void resize(size_type);
		void resize(size_type, const T&);
		size_type capacity() const noexcept;
		bool empty() const noexcept;
		void reserve(size_type);
		void shrink_to_fit();

		T& operator[](size_type) const;
		T& at(size_type);
		T& front();
		T& back();
		T* data() noexcept;

		void assign(size_type, const T&);
		void assign(std::initializer_list<T>);
		void push_back(const T&);
		void push_back(T&&);
		void pop_back();
		iterator insert(iterator, const T&);
		iterator insert(iterator, size_type, const T&);
		iterator insert(iterator, std::initializer_list<T>);
		iterator erase(iterator);
		iterator erase(iterator, iterator);
		void swap(vector<T, A>&);
		void clear() noexcept;

		template<typename... Types>
		iterator emplace(iterator&, Types&&...);

		template<typename... Types>
		iterator emplace_back(Types&&...);

		template<typename InputIterator>
		void assign(InputIterator, InputIterator);

		template<typename InputIterator>
		iterator insert(iterator, InputIterator, InputIterator);

		A get_allocator() const noexcept;

	private:
		A allocator;
		T* _begin = nullptr, *_last = nullptr, *_end = nullptr;

		const int allocation_multiplier = 2;

		void reallocate(size_type s);
		void initialize(iterator, iterator);
		void destroy(iterator, iterator);
	};

	template<typename T, typename A>
	vector<T, A>::vector(const vector<T>& other) {
		reallocate(other.size());
		for (size_type i = 0; i < other.size(); ++i) allocator.construct(_begin + i, other[i]);
	}

	template<typename T, typename A>
	vector<T, A>::vector(size_type new_size) {
		reallocate(new_size);
		_last = _begin + new_size;
		initialize(begin(), end());
	}

	template<typename T, typename A>
	vector<T, A>::vector(size_type new_size, const T& value) {
		assign(new_size, value);
	}

	template<typename T, typename A>
	vector<T, A>::vector(std::initializer_list<T> il) {
		assign(il.begin(), il.end());
	}

	template<typename T, typename A>
	template<typename InputIterator>
	vector<T, A>::vector(InputIterator first, InputIterator last) {
		assign(first, last);
	}

	template<typename T, typename A>
	vector<T, A>::~vector() {
		destroy(begin(), end());
		allocator.deallocate(_begin, capacity());
	}

	template<typename T, typename A>
	vector<T, A>& vector<T, A>::operator=(const vector<T, A>& other) {
		if (this == &other) return *this;
		erase(begin(), end());
		if (other.size() > capacity()) reallocate(other.size());
		for (size_type i = 0; i < other.size(); ++i) allocator.construct(_begin + i, other[i]);
		_last = _begin + other.size();
		return *this;
	}

	template<typename T, typename A>
	vector<T, A>& vector<T, A>::operator=(vector<T, A>&& other) {
		if (this == &other) return *this;
		clear();
		swap(other);
		return *this;
	}

	template<typename T, typename A>
	vector<T, A>& vector<T, A>::operator=(std::initializer_list<T> other) {
		assign(other.begin(), other.end());
		return *this;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator vector<T, A>::begin() noexcept {
		return iterator(_begin);
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator vector<T, A>::end() noexcept {
		return iterator(_last);
	}

	template<typename T, typename A>
	void vector<T, A>::assign(size_type n, const T& value) {
		erase(begin(), end());
		if (n > capacity()) reallocate(n);
		for (size_type i = 0; i < n; ++i) allocator.construct(_begin + i, value);
		_last = _begin + n;
	}

	template<typename T, typename A>
	void vector<T, A>::assign(std::initializer_list<T> il) {
		assign(il.begin(), il.end());
	}

	template<typename T, typename A>
	void vector<T, A>::push_back(const T& value) {
		size_type new_size = size() + 1;
		if (new_size >= capacity()) reallocate(new_size);
		allocator.construct(_last++, T(value));
	}

	template<typename T, typename A>
	void vector<T, A>::push_back(T&& value) {
		size_type new_size = size() + 1;
		if (new_size >= capacity()) reallocate(new_size);
		std::swap(*(_last++), value);
	}

	template<typename T, typename A>
	void vector<T, A>::pop_back() {
		allocator.destroy(_last);
		--_last;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator vector<T, A>::insert(iterator it, const T& value) {
		size_type new_size = size() + 1;
		size_type index = it - begin();
		if (new_size > capacity()) reallocate(new_size);
		it = begin() + index;
		std::copy_backward(it, end(), it + size() - index + 1);
		*it = value;
		++_last;
		return it;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator vector<T, A>::insert(iterator it, size_type n, const T& value) {
		size_type new_size = size() + n;
		size_type index = it - begin();
		if (new_size > capacity()) reallocate(new_size);
		it = begin() + index;
		std::copy_backward(it, end(), it + n + size() - index);
		std::fill(it, it + n, value);
		_last = _begin + new_size;
		return it;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator vector<T, A>::insert(iterator it, std::initializer_list<T> il) {
		return insert(it, il.begin(), il.end());
	}

	template<typename T, typename A>
	template<typename InputIterator>
	typename vector<T, A>::iterator vector<T, A>::insert(typename vector<T, A>::iterator from, InputIterator first, InputIterator last) {
		typename std::iterator_traits<InputIterator>::difference_type n = std::distance(first, last);
		size_type new_size = size() + n;
		size_type index = from - begin();
		if (new_size > capacity()) reallocate(new_size);
		from = begin() + index;
		std::copy_backward(from, end(), from + n + size() - index);
		std::copy(first, last, from);
		_last = _begin + new_size;
		return from;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator vector<T, A>::erase(iterator it) {
		allocator.destroy(&*it);
		std::copy(it + 1, end(), it);
		--_last;
		return it;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator vector<T, A>::erase(iterator first, iterator last) {
		for (iterator it = first; it != last; ++it) allocator.destroy(&*it);
		if (end() > last + 1) std::copy(last + 1, end(), first + 1);
		_last -= std::distance(first, last);
		return first;
	}

	template<typename T, typename A>
	void vector<T, A>::swap(vector<T, A>& other) {
		std::swap(_begin, other._begin);
		std::swap(_last, other._last);
		std::swap(_end, other._end);
		std::swap(allocator, other.allocator);
	}

	template<typename T, typename A>
	void vector<T, A>::clear() noexcept {
		allocator.deallocate(_begin, capacity());
		_begin = _last = _end = nullptr;
	}

	template<typename T, typename A>
	A vector<T, A>::get_allocator() const noexcept {
		return allocator;
	}

	template<typename T, typename A>
	void vector<T, A>::resize(size_type new_size) {
		size_type index = size();
		if (new_size > capacity()) reallocate(new_size);
		_last = _begin + new_size;
		if (index < size()) initialize(begin() + index, end());
		else destroy(end(), begin() + index);
	}

	template<typename T, typename A>
	void vector<T, A>::resize(size_type new_size, const T& val) {
		erase(begin(), end());
		assign(new_size, val);
	}

	template<typename T, typename A>
	void vector<T, A>::reserve(size_type new_size) {
		if (new_size > capacity()) reallocate(new_size);
	}

	template<typename T, typename A>
	void vector<T, A>::shrink_to_fit() {
		reallocate(size() / 2);
	}

	template<typename T, typename A>
	typename vector<T, A>::size_type vector<T, A>::capacity() const noexcept {
		return _end - _begin;
	}

	template<typename T, typename A>
	typename vector<T, A>::size_type vector<T, A>::size() const noexcept {
		return _last - _begin;
	}

	template<typename T, typename A>
	typename vector<T, A>::size_type vector<T, A>::max_size() const noexcept {
		return (size_type)(-1) / sizeof(T);
	}

	template<typename T, typename A>
	inline bool vector<T, A>::empty() const noexcept {
		return size() == 0;
	}

	template<typename T, typename A>
	inline T& vector<T, A>::at(size_type i) {
		if (i >= size()) throw std::out_of_range("vector subscript out of range");
		return _begin[i];
	}

	template<typename T, typename A>
	T& vector<T, A>::front() {
		return *(begin());
	}

	template<typename T, typename A>
	T& vector<T, A>::back() {
		return *(end() - 1);
	}

	template<typename T, typename A>
	T* vector<T, A>::data() noexcept {
		return _begin;
	}

	template<typename T, typename A>
	T& vector<T, A>::operator[](size_type i) const {
		return _begin[i];
	}

	template<typename T, typename A>
	inline void vector<T, A>::reallocate(size_type new_size) {
		T* new_start = allocator.allocate(new_size * 2);
		if (new_start == _begin) return;
		if (_begin) {
			try {
				std::copy(_begin, _begin + std::min(size(), new_size), new_start);
			}
			catch (...) {
				allocator.deallocate(new_start, new_size * 2);
				throw exception();
			}
			allocator.deallocate(_begin, capacity());
		}
		size_type my_size = size();
		_begin = new_start;
		_last = new_start + my_size;
		_end = _begin + new_size * 2;
	}

	template<typename T, typename A>
	void vector<T, A>::initialize(iterator first, iterator last) {
		for (iterator it = first; it != last; ++it) allocator.construct(&*it, T());
	}

	template<typename T, typename A>
	void vector<T, A>::destroy(iterator  first, iterator last) {
		for (iterator it = first; it != last; ++it) allocator.destroy(&*it);
	}

	template<typename T, typename A>
	template<typename ...Types>
	typename vector<T, A>::iterator vector<T, A>::emplace(iterator& it, Types && ...args) {
		size_type new_size = size() + 1;
		size_type index = it - begin();
		if (new_size > capacity()) reallocate(new_size);
		it = begin() + index;
		std::copy_backward(it, end(), it + size() - index + 1);
		allocator.construct(&*it, std::forward<Types>(args)...);
		return it;
	}

	template<typename T, typename A>
	template<typename ...Types>
	typename vector<T, A>::iterator vector<T, A>::emplace_back(Types&& ...args) {
		return emplace(end(), std::forward<Types>(args)...);
	}

	template<typename T, typename A>
	template<typename InputIterator>
	void vector<T, A>::assign(InputIterator first, InputIterator last) {
		erase(begin(), end());
		typename std::iterator_traits<InputIterator>::difference_type new_size = std::distance(first, last);
		if (capacity() < new_size) reallocate(new_size);
		for (iterator it = begin(); first != last; ++it, ++first) allocator.construct(&*it, T(*first));
		_last = _begin + new_size;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator& vector<T, A>::iterator::operator=(const iterator& other) {
		ptr = other.ptr;
		return *this;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator& vector<T, A>::iterator::operator++() {
		++ptr;
		return *this;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator& vector<T, A>::iterator::operator--() {
		--ptr;
		return *this;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator vector<T, A>::iterator::operator++(int) {
		iterator temp(*this);
		++ptr;
		return temp;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator vector<T, A>::iterator::operator--(int) {
		iterator temp(*this);
		--ptr;
		return temp;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator& vector<T, A>::iterator::operator+=(difference_type n) {
		ptr += n;
		return *this;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator& vector<T, A>::iterator::operator-=(difference_type n) {
		ptr -= n;
		return *this;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator::difference_type vector<T, A>::iterator::operator-(const iterator& other) const {
		return ptr - other.ptr;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator vector<T, A>::iterator::operator+(difference_type n) const {
		return iterator(ptr + n);
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator vector<T, A>::iterator::operator-(difference_type n) const {
		return iterator(ptr - n);
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator::reference vector<T, A>::iterator::operator*() const {
		return *ptr;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator::pointer vector<T, A>::iterator::operator->() const {
		return ptr;
	}

	template<typename T, typename A>
	typename vector<T, A>::iterator::reference vector<T, A>::iterator::operator[](difference_type i) const {
		return ptr[i];
	}

	template<typename T, typename A>
	bool vector<T, A>::iterator::operator==(const iterator& other) const {
		return ptr == other.ptr;
	}

	template<typename T, typename A>
	bool vector<T, A>::iterator::operator!=(const iterator& other) const {
		return !(*this == other);
	}

	template<typename T, typename A>
	bool vector<T, A>::iterator::operator>(const iterator& other) const {
		return ptr > other.ptr;
	}

	template<typename T, typename A>
	bool vector<T, A>::iterator::operator<(const iterator& other) const {
		return ptr < other.ptr;
	}

	template<typename T, typename A>
	bool vector<T, A>::iterator::operator>=(const iterator& other) const {
		return ptr >= other.ptr;
	}

	template<typename T, typename A>
	bool vector<T, A>::iterator::operator<=(const iterator& other) const {
		return ptr <= other.ptr;
	}
}