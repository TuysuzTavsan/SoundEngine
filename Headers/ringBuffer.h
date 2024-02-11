#pragma once

#include <cstdint>
#include <stdexcept>
#include <utility>

/*
Classic ring buffer implementation. This buffer will be used as a request buffer.
Every request will be stored in the buffer. AudioManager will manage requests.
Caution: You should use AnyItem method to check if there are available requests to process.
Otherwise you might cause seg fault or read a std::moved memory.
*/
template<typename T>
struct RingBuffer
{
private:
	static const std::uint8_t CAPACITY = 255; /* static constant buffer size. */

	std::uint8_t head;	/* Index to consume an item.*/
	std::uint8_t tail;	/* Index to insert an item.*/
	T buffer[CAPACITY];	/* Data buffer which has the size of CAPACITY constant. see RingBuffer::CAPACITY*/

public:

	RingBuffer()
		:
		head{ 0 },
		tail{ 0 },
		buffer{}
	{

	}

	/*
	Will return false if there are not item available.
	Otherwise true.
	*/
	bool AnyItem()
	{
		return (head == tail ? false : true);
	}

	/*
	Insert a new item to the ring buffer.
	*/
	void Insert(const T item)
	{
		if ((tail + 1) % CAPACITY == head)
			throw std::logic_error("Request overflow! See MAX_REQUEST.");
		
		buffer[tail] = item;
		tail = (tail + 1) % CAPACITY;
	}

	/*
	Get the next available item.
	You should only use this method after a call to AnyItem to ensure safety.
	*/
	T Pop()
	{
		if (head == tail)
			throw std::logic_error("Nothing to pop. See AnyPending method!");
		T temp = std::move(buffer[head]);
		head = (head + 1) % CAPACITY;
		return temp;
	}

};