#pragma once

#include <array>
#include <vector>
#include <ostream>

#ifdef DEBUG
#	include <iostream>
#endif

#include "util.hpp"
#include "debug.hpp"

namespace utki{

// TODO: remove when C++'20 becomes very common.

/**
 * @brief span template class.
 * This class is a wrapper of continuous memory buffer, it encapsulates pointer to memory block and size of that memory block.
 * It does not own the memory.
 * This is a replacement of std::span when C++'20 is not available.
 */
template <class T> class span final{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	
private:
	pointer buf = nullptr;
	size_type bufSize = 0;
	
public:

	span(const span&) = default;
	span& operator=(const span&) = default;
	
	
	/**
	 * @brief Create a span object.
	 * Creates a span object which wraps given memory buffer of specified size.
	 * Note, the memory will not be freed upon this Buffer object destruction,
	 * Buffer does not own the memory.
	 * @param bufPtr - pointer to the memory buffer.
	 * @param bufSize - size of the memory buffer.
	 */
	span(pointer bufPtr, size_type bufSize)noexcept :
			buf(bufPtr),
			bufSize(bufSize)
	{}

	
	span()noexcept{}
	
	/**
	 * @brief Constructor for automatic conversion from nullptr.
     * @param bufPtr - pointer to the memory buffer. Makes not much sense, because size is 0 anyway.
     */
	span(std::nullptr_t)noexcept :
			buf(nullptr),
			bufSize(0)
	{}
	
	/**
	 * @brief get buffer size.
	 * @return number of elements in buffer.
	 */
	size_type size()const noexcept{
		return this->bufSize;
	}

	bool empty()const noexcept{
		return this->size() == 0;
	}

	/**
	 * @brief get size of buffer in bytes.
	 * @return size of array in bytes.
	 */
	size_type size_bytes()const noexcept{
		return this->size() * sizeof(value_type);
	}

	//TODO: deprecated, remove.
	size_type sizeInBytes()const noexcept{
		return this->size_bytes();
	}



	/**
	 * @brief access specified element of the buffer.
	 * Const version of Buffer::operator[].
	 * @param i - element index.
	 * @return reference to i'th element of the buffer.
	 */
	const_reference operator[](size_type i)const noexcept{
		ASSERT(i < this->size())
		return this->buf[i];
	}



	/**
	 * @brief access specified element of the buffer.
	 * @param i - element index.
	 * @return reference to i'th element of the buffer.
	 */
	reference operator[](size_type i)noexcept{
		ASSERT_INFO(i < this->size(), "operator[]: index out of bounds")
		return this->buf[i];
	}



	/**
	 * @brief get pointer to first element of the buffer.
	 * @return pointer to first element of the buffer.
	 */
	iterator begin()noexcept{
		return this->buf;
	}



	/**
	 * @brief get pointer to first element of the buffer.
	 * @return pointer to first element of the buffer.
	 */
	const_iterator begin()const noexcept{
		return this->cbegin();
	}

	const_iterator cbegin()const noexcept{
		return this->buf;
	}


	/**
	 * @brief get pointer to "after last" element of the buffer.
	 * @return pointer to "after last" element of the buffer.
	 */
	iterator end()noexcept{
		return this->buf + this->bufSize;
	}



	/**
	 * @brief get const pointer to "after last" element of the buffer.
	 * @return const pointer to "after last" element of the buffer.
	 */
	const_iterator end()const noexcept{
		return this->cend();
	}
	
	const_iterator cend()const noexcept{
		return this->buf + this->bufSize;
	}

	
	const_reverse_iterator crbegin()const noexcept{
		return const_reverse_iterator(this->end());
	}

	const_reverse_iterator crend()const noexcept{
		return const_reverse_iterator(this->begin());
	}
	
	reverse_iterator rbegin()noexcept{
		return reverse_iterator(this->end());
	}

	const_reverse_iterator rbegin()const noexcept{
		return const_reverse_iterator(this->end());
	}

	reverse_iterator rend()noexcept{
		return reverse_iterator(this->begin());
	}

	const_reverse_iterator rend()const noexcept{
		return const_reverse_iterator(this->begin());
	}
	
	pointer data()noexcept{
		return this->buf;
	}

	const_pointer data()const noexcept{
		return this->buf;
	}
	


	/**
	 * @brief Checks if pointer points somewhere within the buffer.
	 * @param p - pointer to check.
	 * @return true - if pointer passed as argument points somewhere within the buffer.
	 * @return false otherwise.
	 */
	bool overlaps(const_pointer p)const noexcept{
		return this->begin() <= p && p <= (this->end() - 1);
	}



	friend std::ostream& operator<<(std::ostream& s, const span<T>& buf){
		for(auto& e : buf){
			s << e;
		}
		return s;
	}
};


template <class T> inline utki::span<T> make_span(std::nullptr_t){
	return utki::span<T>(nullptr);
}

template <class T> inline utki::span<T> make_span(T* buf, size_t size){
	return utki::span<T>(buf, size);
}

template <class T> inline const utki::span<T> make_span(const T* buf, size_t size){
	return utki::span<T>(const_cast<T*>(buf), size);
}

template <class T, std::size_t array_size> inline utki::span<T> make_span(std::array<T, array_size>& a){
	return make_span(a.size() == 0 ? nullptr : &*a.begin(), a.size());
}

template <class T, std::size_t array_size> inline const utki::span<T> make_span(const std::array<T, array_size>& a){
	return make_span(a.size() == 0 ? nullptr : &*a.begin(), a.size());
}

template <class T> inline utki::span<T> make_span(std::vector<T>& v){
	return make_span(v.size() == 0 ? nullptr : &*v.begin(), v.size());
}

template <class T> inline const utki::span<T> make_span(const std::vector<T>& v){
	return make_span(v.size() == 0 ? nullptr : &*v.begin(), v.size());
}

inline std::string to_string(const utki::span<char>& buf){
	return std::string(buf.data(), buf.size());
}

}
