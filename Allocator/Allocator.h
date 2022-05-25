#pragma once
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<queue>

typedef uint8_t b1;
typedef uint16_t b2;
typedef uint32_t b4;
typedef uint64_t b8;

template<class U>
class Block
{
public:
	U* p = nullptr;
	bool used = false;
	Block* next = nullptr;
	Block* prev = nullptr;
};

template<typename T>
struct Queue
{
	Block<T>* start = nullptr;
	Block<T>* end = nullptr;

	void push_back(T* element)
	{
		if (end == nullptr)
		{
			end = new Block<T>;
			start = end;
			end->p = element;
		}
		else
		{
			Block<T>* p = new Block<T>;
			end->next = p;
			p->prev = end;
			end = p;
			end->p = element;
		}
	}

	bool empty()
	{
		if (start == nullptr)
			return true;
		else
			return false;
	} 

};


template<typename T>
class Allocator
{
public:
	typedef T value_type;
	typedef T* pointer;
	typedef void* vp;

private:

	Queue<T> blocks;
	Queue<b1> byte_1;
	Queue<b2> bytes_2;
	Queue<b4> bytes_4;
	Queue<b8> bytes_8;

	pointer memory;

public:

	Allocator()
	{
		int amount = 100000;
		memory = static_cast<pointer>(malloc(amount * sizeof(T)));

		pointer p = memory;
		for (int i = 0; i < amount; i++)
		{
			blocks.push_back(p);
			p++;
		}
		set_blocks();
	}

	template<typename U>
	Allocator(Allocator<U>& allocator)
	{
		int amount = 100000;
		memory = static_cast<pointer>(malloc(amount * sizeof(T)));

		pointer p = memory;
		for (int i = 0; i < amount; i++)
		{
			blocks.push_back(p);
			p++;
		}
		set_blocks();
	}

	pointer allocate(int N)
	{
		try
		{
		if (N * sizeof(T) == sizeof(T))
		{
			if (blocks.start != nullptr)
			{
			Block<T>* pt = blocks.start;
			while (pt != nullptr)
			{
				if (!pt->used)
				{
					pt->used = true;
					return (pt->p);
				}
				pt = pt->next;
			}
			}
		}
		else if (N * sizeof(T) == 1)
		{
			if (byte_1.start != nullptr)
			{
			Block<b1>* pt = byte_1.start;
			while (pt != nullptr)
			{
				if (!pt->used)
				{
					pt->used = true;
					return reinterpret_cast<pointer>(pt->p);
				}
				pt = pt->next;
			}
			}
		}
		else if (N * sizeof(T) == 2)
		{
			if (bytes_2.start != nullptr)
			{
			Block<b2>* pt = bytes_2.start;
			while (pt != nullptr)
			{
				if (!pt->used)
				{
					pt->used = true;
					return reinterpret_cast<pointer>(pt->p);
				}
				pt = pt->next;
			}
			}
		}
		else if (N * sizeof(T) == 3 || N * sizeof(T) == 4)
		{
			if (bytes_4.start != nullptr)
			{
			Block<b4>* pt = bytes_4.start;
			while (pt != nullptr)
			{
				if (!pt->used)
				{
					pt->used = true;
					return reinterpret_cast<pointer>(pt->p);
				}
				pt = pt->next;
			}
			}
		}
		else if (N * sizeof(T) > 4 && N * sizeof(T) <= 8)
		{
			if (bytes_8.start != nullptr)
			{
			Block<b8>* pt = bytes_8.start;
			while (pt != nullptr)
			{
				if (!pt->used)
				{
					pt->used = true;
					return reinterpret_cast<pointer>(pt->p);
				}
				pt = pt->next;
			}
			}
		}

		throw "No such block";
		}
		catch (const char* message)
		{
			std::cout << message;
		}
	}

	template<typename U>
	void deallocate(U* point, int n)
	{
		if (sizeof(U) == 1)
		{
			if (byte_1.start != nullptr)
			{
			Block<U>* pt = (Block<U>*)byte_1.start;
			while (pt != nullptr )
			{
				if (pt->p == point)
				{
					pt->used = false;
					return;
				}
				pt = pt->next;
			}
			}
		}
		else if (sizeof(U) == 2)
		{
			if (bytes_2.start != nullptr)
			{
			Block<U>* pt = (Block<U>*)bytes_2.start;
			while (pt != nullptr)
			{
				if (pt->p == point)
				{
					pt->used = false;
					return;
				}
				pt = pt->next;
			}
			}
		}
		else if (sizeof(U) == 4)
		{
			if (bytes_4.start != nullptr)
			{
			Block<U>* pt = (Block<U>*)bytes_4.start;
			while (pt != nullptr)
			{
				if (pt->p == point)
				{
					pt->used = false;
					return;
				}
				pt = pt->next;
			}
			}
		}
		else if (sizeof(U) == 8)
		{
			if (bytes_8.start != nullptr)
			{
			Block<U>* pt = (Block<U>*)bytes_8.start;
			while (pt != nullptr)
			{
				if (pt->p == point)
				{
					pt->used = false;
					return;
				}
				pt = pt->next;
			}
			}
		}

		if (sizeof(U) == sizeof(T))
		{
			Block<U>* pt = (Block<U>*)blocks.start;
			while (pt != nullptr)
			{
				if (pt->p == point)
				{
					pt->used = false;
					return;
				}
				pt = pt->next;
			}
		}
	}

	void set_blocks(int b1_count = 50, int b2_count = 50, int b4_count = 50, int b8_count = 50)
	{
		b1* p_1 = (b1*)memory;

		for (int i = 0; i < b1_count; i++)
		{
			byte_1.push_back(p_1);
			p_1++;
		}

		b2* p_2 = (b2 *) p_1;

		for (int i = 0; i < b2_count; i++)
		{
			bytes_2.push_back(p_2);
			p_2++;
		}

		b4* p_3 = (b4*)p_2;

		for (int i = 0; i < b4_count; i++)
		{
			bytes_4.push_back(p_3);
			p_3++;
		}

		b8* p_4 = (b8*)p_3;

		for (int i = 0; i < b8_count; i++)
		{
			bytes_8.push_back(p_4);
			p_4++;
		}
	}

	void* get_memory(int N)
	{
		try
		{
			if (N == 1)
			{
				if (byte_1.start == nullptr)
					throw "No such block";

				Block<b1>*p = byte_1.start;
				while (p != nullptr)
				{
					if (!p->used)
					{
						p->used = true;
						return p->p;
					}
				}
			}
			else if (N == 2)
			{
				if (bytes_2.start == nullptr)
					throw "No such block";

				Block<b2>* p = bytes_2.start;
				while (p != nullptr)
				{
					if (!p->used)
					{
						p->used = true;
						return p->p;
					}
				}
			}
			else if (N == 3 || N == 4)
			{
				if (bytes_4.start == nullptr)
					throw "No such block";

				Block<b4>* p = bytes_4.start;
				while (p != nullptr)
				{
					if (!p->used)
					{
						p->used = true;
						return p->p;
					}
				}
			}
			else if (N > 4 && N <= 8)
			{
				if (bytes_4.start == nullptr)
					throw "No such block";

				Block<b8>* p = bytes_8.start;
				while (p != nullptr)
				{
					if (!p->used)
					{
						p->used = true;
						return p->p;
					}
				}
			}

			throw "No such block";
		}
		catch (const char* message)
		{
			std::cout << message;
		}
	}

};







































//class Allocator
//{
//private:
//	struct Block
//	{
//		void* memory;
//		bool used = true;
//	};
//
//	std::map<int, std::vector<Block>> blocks;
//
//public:
//	Allocator(int size, int amount)
//	{
//		for (int i = 0; i < amount; i++)
//		{
//			Block block;
//			block.memory = malloc(size);
//			blocks[size].push_back(block);
//		}
//	}
//
//	Allocator(int N)
//	{
//		std::map<int, std::vector<Block>>::iterator it = blocks.begin();
//		while ()
//	}


//	~Allocator()
//	{
//		
//	}
//};
//
