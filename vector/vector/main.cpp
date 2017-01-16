#define _SCL_SECURE_NO_WARNINGS
#define CATCH_CONFIG_RUNNER
#define BENCHPRESS_CONFIG_MAIN
#define PAUSE getchar();getchar()
#include "allocator.hpp"
#include "vector.hpp"
#include "catch.hpp"
#include <ctime>
#include <iostream>
#include "benchpress.hpp"
#include "cxxopts.hpp"

SCENARIO("Vectors can be created") {

	WHEN("Pass no arguments") {
		stl_compatible::vector<int> v;
		THEN("Vector is empty") {
			REQUIRE(v.size() == 0);
			REQUIRE(v.capacity() == 0);
			REQUIRE(v.empty());
		}
	}

	WHEN("Pass single integer") {
		stl_compatible::vector<int> v(5);
		THEN("Vector has the exact number of elements, bigger or equal capacity") {
			REQUIRE(v.size() == 5);
			REQUIRE(v.capacity() >= 5);
		}
	}

	WHEN("Pass single integer and a value") {
		stl_compatible::vector<int> v(5, 1u);
		THEN("Vector has the exact number of elements, bigger or equal capacity and all items are equal to value") {
			REQUIRE(v.size() == 5);
			REQUIRE(v.capacity() >= 5);
			for (std::size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == 1);
		}
	}

	WHEN("Pass two iterators") {
		stl_compatible::vector<int> src = { 1, 2, 3, 4, 5 };
		stl_compatible::vector<int> v(src.begin(), src.end());
		THEN("Vector has all the items between first and last iterators, bigger or equal capacity") {
			REQUIRE(v.size() == src.size());
			REQUIRE(v.capacity() >= src.size());
			for (size_t i = 0; i < src.size(); ++i) REQUIRE(v[i] == src[i]);
		}
	}

	WHEN("Pass initializer list") {
		stl_compatible::vector<int> v{ 1, 2, 3, 4, 5 };
		THEN("Vector has all the items from list, bigger or equal capacity") {
			REQUIRE(v.size() == 5);
			REQUIRE(v.capacity() >= 5);
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == i + 1);
		}
	}
}

SCENARIO("Vectors can be assigned") {

	WHEN("Assign vector to initializer list") {
		stl_compatible::vector<int> v;
		v = { 1, 2, 3, 4, 5 };
		THEN("Vector has all the items from list, bigger or equal capacity") {
			REQUIRE(v.size() == 5);
			REQUIRE(v.capacity() >= 5);
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == i + 1);
		}
	}

	WHEN("Assign vector to vector") {
		stl_compatible::vector<int> src = { 1, 2, 3, 4, 5 };
		stl_compatible::vector<int> v;
		v = src;
		THEN("First vector has all the items from second, second vector is unaffected") {
			REQUIRE(src.size() == 5);
			REQUIRE(v.size() == 5);
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == src[i]);
		}
	}

	WHEN("Move assign vector to vector") {
		stl_compatible::vector<int> src = { 1, 2, 3, 4, 5 };
		stl_compatible::vector<int> v;
		v = std::move(src);
		THEN("First vector has all the items from second") {
			REQUIRE(v.size() == 5);
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == i + 1);
		}
	}

	WHEN("Assign vector to the same vector") {
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5 };
		v = v;
		v = std::move(v);
		THEN("Nothimg changes") {
			REQUIRE(v.size() == 5);
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == i + 1);
		}
	}
}

SCENARIO("Vector can be resized") {

	WHEN("Pass integer") {
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5 };
		v.resize(3);
		THEN("Vector has the exact size, bigger or equal capacity") {
			REQUIRE(v.size() == 3);
			REQUIRE(v.capacity() >= 3);
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == i + 1);
		}
	}

	WHEN("Pass integer and value") {
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5 };
		v.resize(3, 1);
		THEN("Vector has the exact size, bigger or equal capacity and all items are equal to value") {
			REQUIRE(v.size() == 3);
			REQUIRE(v.capacity() >= 3);
			for (stl_compatible::vector<int>::iterator it = v.begin(); it != v.end(); ++it) REQUIRE(*it == 1);
		}
	}
}

SCENARIO("Vector can reserve items") {

	WHEN("Pass integer") {
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5 };
		v.reserve(20);
		THEN("Vector doen't change size, all items are unaffected, capacity is equal or greater then passed integer") {
			REQUIRE(v.size() == 5);
			REQUIRE(v.capacity() >= 20);
			for (size_t i = 0; i < v.size(); ++i) v[i] = i + 1;
		}
	}
}

SCENARIO("Vector can be modified") {

	WHEN("Assign function passing integer and value") {
		stl_compatible::vector<int> v;
		v.assign(5, 1u);
		THEN("Vector has the exact number of elements, bigger ot equal capacity and all items are equal to value") {
			REQUIRE(v.size() == 5);
			REQUIRE(v.capacity() >= 5);
			for (std::size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == 1);
		}
	}

	WHEN("Assign function passing initializer list") {
		stl_compatible::vector<int> v;
		v.assign({ 1, 2, 3, 4, 5 });
		THEN("Vector has all the items from list, bigger or equal capacity") {
			REQUIRE(v.size() == 5);
			REQUIRE(v.capacity() >= 5);
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == i + 1);
		}
	}

	WHEN("Assign funtion passing two iterators") {
		stl_compatible::vector<int> src{ 1, 2, 3, 4, 5 };
		stl_compatible::vector<int> v;
		v.assign(src.begin(), src.end());
		THEN("Vector has all the items between first and last iterators, bigger or equal capacity") {
			REQUIRE(v.size() == src.size());
			REQUIRE(v.capacity() >= src.size());
			for (size_t i = 0; i < src.size(); ++i) REQUIRE(v[i] == src[i]);
		}
	}

	WHEN("Push items back") {
		stl_compatible::vector<int> src = { 1, 3, 5, 2, 4 };
		stl_compatible::vector<int> v;
		for (stl_compatible::vector<int>::iterator it = src.begin(); it != src.end(); ++it) v.push_back(*it);
		THEN("Vector has all pushed items in the same order") {
			REQUIRE(v.size() == src.size());
			for (size_t i = 0; i < src.size(); ++i) REQUIRE(src[i] == v[i]);
		}
	}

	WHEN("Move push items back") {
		stl_compatible::vector<int> src = { 1, 3, 5, 2, 4 };
		stl_compatible::vector<int> v;
		for (stl_compatible::vector<int>::iterator it = src.begin(); it != src.end(); ++it) v.push_back(std::move(*it));
		THEN("Vector has all pushed items") {
			REQUIRE(v.size() == src.size());
		}
	}

	WHEN("Pop items") {
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5 };
		for (int i = 0; i < 3; ++i) v.pop_back();
		THEN("Vector size is redused by the number of poped items, unpoped items are unaffected") {
			REQUIRE(v.size() == 2);
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == i + 1);
		}
	}

	WHEN("Insert one item") {
		stl_compatible::vector<int> v = { 1, 2, 4, 5 };
		v.insert(v.begin() + 2, 3);
		THEN("Item is inserted into position, other items are the same") {
			REQUIRE(v.size() == 5);
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == i + 1);
		}
	}

	WHEN("Insert same items") {
		stl_compatible::vector<int> res = { 1, 2, 3, 3, 3, 4, 5 };
		stl_compatible::vector<int> v = { 1, 2, 4, 5 };
		v.insert(v.begin() + 2, 3, 3u);
		THEN("Items are inserted starting from position, other items are the same") {
			REQUIRE(v.size() == 7);
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == res[i]);
		}
	}

	WHEN("Insert iterator range") {
		stl_compatible::vector<int> src = { 2, 3, 4 };
		stl_compatible::vector<int> v = { 1, 5 };
		v.insert(v.begin() + 1, src.begin(), src.end());
		THEN("Items are inserted starting from position, other items are the same") {
			REQUIRE(v.size() == 5);
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == i + 1);
		}
	}

	WHEN("Insert initializer list") {
		stl_compatible::vector<int> v = { 1, 5 };
		v.insert(v.begin() + 1, { 2, 3, 4 });
		THEN("Items are inserted starting from position, other items are the same") {
			REQUIRE(v.size() == 5);
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == i + 1);
		}
	}

	WHEN("Erase one element") {
		stl_compatible::vector<int> v = { 1, 2, 3, 3, 4, 5 };
		v.erase(v.begin() + 2);
		THEN("Item is erased at position, other iterms are the same") {
			REQUIRE(v.size() == 5);
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == i + 1);
		}
	}

	WHEN("Erase elements") {
		stl_compatible::vector<int> v = { 1, 2, 3, 3, 3, 3, 4, 5 };
		v.erase(v.begin() + 2, v.begin() + 5);
		THEN("Item is erased at position, other iterms are the same") {
			REQUIRE(v.size() == 5);
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == i + 1);
		}
	}

	WHEN("Swap vectors") {
		stl_compatible::vector<int> v = { 5, 4, 3, 2, 1 };
		stl_compatible::vector<int> src = { 1, 2, 3, 4, 5 };
		v.swap(src);
		THEN("Vectors velues are swaped") {
			for (size_t i = 0; i < v.size(); ++i) REQUIRE(v[i] == i + 1);
			for (size_t i = 0; i < src.size(); ++i) REQUIRE(src[i] == 5 - i);
		}
	}

	WHEN("Clear vector") {
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5 };
		v.clear();
		THEN("Vector is empty with 0 capacity") {
			REQUIRE(v.empty());
			REQUIRE(v.capacity() == 0);
			REQUIRE(v.size() == 0);
		}
	}

	WHEN("Shrink vector") {
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5, 6 };
		v.shrink_to_fit();
		REQUIRE(v.capacity() == 6);
		REQUIRE(v.size() == 6);
	}

	WHEN("Emplace item back") {
		stl_compatible::vector<stl_compatible::vector<int>> v;
		v.emplace_back(4, 1u);
		stl_compatible::vector<int> a = v.front();
		THEN("Item is created and inserted") {
			for (stl_compatible::vector<int>::iterator it = a.begin(); it != a.end(); ++it) REQUIRE(*it == 0);
		}
	}

	WHEN("Emplace item at positiion") {
		stl_compatible::vector<stl_compatible::vector<int>> v(3, stl_compatible::vector<int>(3, 2u));
		stl_compatible::vector<int> a = *(v.emplace(v.begin() + 1, 3, 1u));
		THEN("Item is created and inserted at position") {
			for (stl_compatible::vector<int>::iterator it = a.begin(); it != a.end(); ++it) REQUIRE(*it == 0);
		}
	}
}

SCENARIO("Iterators") {

	WHEN("Forward iteration") {
		int i = 1;
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5 };
		THEN("Items are forward") {
			for (stl_compatible::vector<int>::iterator it = v.begin(); it != v.end(); ++it, ++i) REQUIRE(*it == i);
		}
	}

	WHEN("Backward iteration") {
		int i = 5;
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5 };
		THEN("Items are backwards") {
			for (stl_compatible::vector<int>::iterator it = v.end() - 1; it >= v.begin(); --it, --i) REQUIRE(*it == i);
		}
	}

	WHEN("Random access") {
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5 };
		stl_compatible::vector<int*> a;
		int b = 8;
		a.push_back(&b);
		THEN("Items can be accessed") {
			REQUIRE(*(v.begin() + 2) == 3);
			REQUIRE(*(v.end() - 2) == 4);
			REQUIRE(v.begin()[2] == 3);
			stl_compatible::vector<int>::iterator it = v.begin();
			it += 2;
			REQUIRE(*it == 3);
			it -= 2;
			REQUIRE(*it == 1);
			REQUIRE(**(a.begin().operator->()) == 8);
			it++;
			REQUIRE(*it == 2);
			it--;
			REQUIRE(*it == 1);
		}
	}

	WHEN("Comparision") {
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5 };
		stl_compatible::vector<int>::iterator first = v.begin(), last = v.end();
		REQUIRE(first < last);
		REQUIRE(first == first);
		REQUIRE(last > first);
		REQUIRE(last >= last);
		REQUIRE(last >= first);
		REQUIRE(first <= last);
		REQUIRE(first <= first);
		REQUIRE(first != last);
	}
}

SCENARIO("Vector data access") {

	WHEN("At access") {
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5 };
		REQUIRE(v.at(2) == 3);
	}

	WHEN("Front and back access") {
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5 };
		REQUIRE(v.front() == 1);
		REQUIRE(v.back() == 5);
	}

	WHEN("Data access") {
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5 };
		REQUIRE(*(v.data()) == 1);
	}

	WHEN("Vector size") {
		stl_compatible::vector<int> v = { 1, 2, 3, 4, 5 };
		REQUIRE(v.max_size() == (size_t)(-1) / (sizeof(int)));
	}

	WHEN("Allocator access") {
		stl_compatible::vector<int> v = { 1, 2, 3 };
		typename stl_compatible::vector<int>::allocator_type a = v.get_allocator();
		REQUIRE(a == v.get_allocator());
		REQUIRE_FALSE(a != v.get_allocator());
	}
}

SCENARIO("Vector exceptions") {

	class T {
	public:

		int a = 1;

		class TException : public std::exception {};

		T& operator=(const T&) {
			throw TException();
		}

	};

	WHEN("At out of range") {
		stl_compatible::vector<int> v = { 1, 2, 3 };
		THEN("Exception") {
			REQUIRE_THROWS(v.at(3));
		}
	}

	WHEN("Bad alloc") {
		stl_compatible::vector<int> v;
		THEN("Exception") {
			REQUIRE_THROWS(v.reserve((size_t)(-1)));
		}
	}

	WHEN("Exception on copy") {
		stl_compatible::vector<T> v(1, T());
		THEN("Exception") {
			REQUIRE_THROWS_AS(v.push_back(T()), stl_compatible::exception);
		}
	}

	WHEN("Bad allocation") {
		THEN("Exception") {
			REQUIRE_THROWS_AS(stl_compatible::vector<int> v(10000000), std::bad_alloc);
		}
	}
}

bool obj_destroyed = false;

SCENARIO("Object destroyed") {

	class Destroyable {
	public:
		~Destroyable() {
			obj_destroyed = true;
		}
	};

	WHEN("Vector resize") {
		stl_compatible::vector<Destroyable> v(5);
		v.resize(3);
		THEN("Object destroyed") {
			REQUIRE(obj_destroyed);
		}
	}

	WHEN("Vector erase") {
		stl_compatible::vector<Destroyable> v(5);
		obj_destroyed = false;
		v.erase(v.begin() + 1);
		THEN("Object destroyed") {
			REQUIRE(obj_destroyed);
		}
	}

	WHEN("Vector erase range") {
		stl_compatible::vector<Destroyable> v(5);
		obj_destroyed = false;
		v.erase(v.begin() + 1, v.end());
		THEN("Object destroyed") {
			REQUIRE(obj_destroyed);
		}
	}
}

SCENARIO("Object without default constructor") {

	class stl_compatibleInt {
	public:

		int a;

		stl_compatibleInt() = delete;
		stl_compatibleInt(int a) : a(a) {};

	};

	WHEN("Emplace object") {
		stl_compatible::vector<stl_compatibleInt> v;
		v.reserve(10);
		v.emplace_back(1);
		THEN("Object is emplaced") {
			REQUIRE(v.front().a == 1);
		}
	}

	WHEN("Object pushed back") {
		stl_compatible::vector<stl_compatibleInt> v;
		v.reserve(10);
		v.push_back(stl_compatibleInt(1));
		THEN("Object is emplaced") {
			REQUIRE(v.front().a == 1);
		}
	}
}

SCENARIO("Vector iterators can be used with STL functions") {

	WHEN("Constructing STL vector") {
		stl_compatible::vector<int> src = { 1, 2, 3, 4, 5 };
		std::vector<int> v(src.begin(), src.end());
		THEN("STL vector is constructed") {
			for (size_t i = 0; i < v.size(); ++i) v[i] == i + 1;
		}
	}

	WHEN("Vector is sorted using std::sort") {
		stl_compatible::vector<int> v = { 5, 3, 2, 4, 1 };
		std::sort(v.begin(), v.end());
		THEN("Vector is sorted") {
			for (size_t i = 0; i < v.size(); ++i) v[i] == i + 1;
		}
	}
}

SCENARIO("Allocator can be used by std::vector") {

	WHEN("Using memory pool allocator") {
		std::vector<int, stl_compatible::allocator<int>> v = { 1, 2, 3, 4 ,5 };
		THEN("Vector is created and contain elements") {
			for (size_t i = 0; i < v.size(); ++i) v[i] == i + 1;
		}
	}

	WHEN("Using standard allocator") {
		std::vector<int, stl_compatible::allocator<int>> v = { 1, 2, 3, 4, 5 };
		THEN("Vector is created and contain elements") {
			for (size_t i = 0; i < v.size(); ++i) v[i] == i + 1;
		}
	}
}

BENCHMARK("stl_compatible::vector, stl_compatible::allocator", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i) {
		stl_compatible::vector<int> v;
		for (size_t i = 0; i < 100000; ++i) v.push_back(i);
	}
})

typedef stl_compatible::vector<int, stl_compatible::allocator<int>> vector_standard_allocator;

BENCHMARK("stl_compatible::vector, stl_compatible::allocator", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i) {
		vector_standard_allocator v;
		for (size_t i = 0; i < 100000; ++i) v.push_back(i);
	}
})

typedef stl_compatible::vector<int, std::allocator<int>> vector_std_allocator;

BENCHMARK("stl_compatible::vector, std::allocator", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i) {
		vector_std_allocator v;
		for (size_t i = 0; i < 100000; ++i) v.push_back(i);
	}
})

BENCHMARK("std::vector, std::allocator", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i) {
		std::vector<int> v;
		for (size_t i = 0; i < 100000; ++i) v.push_back(i);
	}
})

typedef std::vector<int, stl_compatible::allocator<int>> std_vector_memory_pool_allocator;

BENCHMARK("std::vector, stl_compatible::allocator", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i) {
		std_vector_memory_pool_allocator v;
		for (size_t i = 0; i < 100000; ++i) v.push_back(i);
	}
})

typedef std::vector<int, stl_compatible::allocator<int>> std_vector_standard_allocator;

BENCHMARK("std::vector, stl_compatible::allocator", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i) {
		std_vector_standard_allocator v;
		for (size_t i = 0; i < 100000; ++i) v.push_back(i);
	}
})

int main(int argc, char** argv) {
	int result = Catch::Session().run(argc, argv);
	result = run(1, argv);
	PAUSE;
	return result;
}