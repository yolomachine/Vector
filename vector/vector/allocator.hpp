#pragma once
#include <list>

namespace stl_compatible {

	template<typename T>
	class allocator;

	template<typename T>
	struct memory_pool {

		typedef std::list<typename allocator<T>::memory_block> blocks_list;

		blocks_list memory;
		typename allocator<T>::pointer free_space;
	    typename allocator<T>::size_type default_size = 1000000;

		memory_pool() {
			free_space = (typename allocator<T>::pointer) ::operator new(sizeof(T) * default_size);
			memory = { typename allocator<T>::memory_block(free_space, default_size, true) };
		}
	};

	template<typename T>
	memory_pool<T>& alloc() {
		static memory_pool<T> free_store;
		return free_store;
	}

	template<typename T>
	class allocator {
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef size_t size_type;

		allocator() {};
		allocator(const allocator<T>&) {};
		template<typename U>
		allocator(const allocator<U>&) {};

		allocator<T>& operator=(const allocator<T>&) { return *this; };
		pointer allocate(size_type n);
		void deallocate(pointer p, size_type n);
		void destroy(pointer p);

		template <typename... Types>
		void construct(pointer p, Types&&... t) {
			::new (static_cast<void*>(p)) value_type(std::forward<Types>(t)...);
		};

		struct memory_block {

			pointer begin;
			size_type length;
			bool is_free;

			memory_block(pointer begin, size_type length, bool is_free)
				: begin(begin), length(length), is_free(is_free) {};
		};

	private:
		memory_pool<T>& _memory = alloc<T>();
	};

	template<typename T>
	void allocator<T>::destroy(pointer p) { p->~value_type(); };

	template<typename T>
	typename allocator<T>::pointer allocator<T>::allocate(size_type n) {
		for (auto it = _memory.memory.rbegin(); it != _memory.memory.rend(); ++it) {
			if (it->is_free && it->length == n) {
				it->is_free = false;
				return it->begin;
			}
			else if (it->is_free && it->length > n) {
				auto block = it.base();
				auto res = _memory.memory.emplace(--block, it->begin, n, false);
				it->begin = it->begin + n;
				it->length = it->length - n;
				return res->begin;
			}
		}
		throw std::bad_alloc();
	};

	template<typename T>
	void allocator<T>::deallocate(pointer p, size_type n) {
		if (p == nullptr) return;
		for (auto it = _memory.memory.begin(); it != _memory.memory.end(); ++it) {
			if (it->begin == p && it->length == n) {
				it->is_free = true;
				if (it != _memory.memory.begin()) {
					auto buff = it;
					for (--buff; buff != _memory.memory.begin();) {
						if (!buff->is_free) break;
						it->begin = buff->begin;
						it->length = it->length + buff->length;
						buff = _memory.memory.erase(buff);
						--buff;
					}
				}
				auto buff = it;
				for (++buff; buff != _memory.memory.end();) {
					if (!buff->is_free) break;
					it->length = it->length + buff->length;
					buff = _memory.memory.erase(buff);
				}
			}
		}
	};

	template<typename T, typename U>
	bool operator==(const allocator<T>&, const allocator<U>&) { return true; }

	template<typename T, typename U>
	bool operator!=(const allocator<T>&, const allocator<U>&) { return false; }

}