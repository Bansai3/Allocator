#pragma once
#include<iostream>
#include<stdlib.h>

template<typename T>
struct Block
{
	T* p = nullptr;
	size_t size = 0;
	bool state = false;
};

template<typename T>
struct Node
{
	T value;
	Node* prev = nullptr;
	Node* next = nullptr;
};

template<typename T>
struct Queue
{
	Node<T>* start = nullptr;
	Node<T>* end = nullptr;

	void push_back(T& value)
	{
		if (end == nullptr)
		{
			Node<T>* p = new Node<T>;
			p->value = value;
			end = p;
			start = p;
		}
		else
		{
			Node<T>* p = new Node<T>;
			p->value = value;
			end->next = p;
			p->prev = end;
			end = p;
		}
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

	Queue<Queue<Block<void>>> blocks;

public:

	Allocator()
	{
		for (int i = 0; i < 9; i++)
		{
			Queue<Block<void>> b;
			blocks.push_back(b);
		}
		set_blocks();
	}

	template<typename U>
	Allocator(Allocator<U>& allocator)
	{
		for (int i = 0; i < 9; i++)
		{
			Queue<Block<void>> b;
			blocks.push_back(b);
		}
		set_blocks();
	}

	pointer allocate(int N)
	{
		try
		{
			Node<Queue<Block<void>>>* pt = blocks.start;

			while (pt != nullptr)
			{
			if (pt->value.start != nullptr)
			{
			if (N * sizeof(T) <= pt->value.start->value.size)
			{
				Node<Block<void>>* p = pt->value.start;
				while (p != nullptr)
				{
					if (p->value.state == false)
					{
						p->value.state = true;
						return static_cast<pointer>(p->value.p);
					}
					p = p->next;
				}
			}
			}
			pt = pt->next;
			}
			throw "No such block";
		}
		catch (const char* message)
		{
			std::cout << message;
		}
	}

	template<typename U>
	void deallocate(U* point, int N)
	{
		Node<Queue<Block<void>>>* pt = blocks.start;
		while (pt != nullptr)
		{
			if (N * sizeof(T) <= pt->value.start->value.size)
				break;
			pt = pt->next;
		}

		if (pt == nullptr)
			return;

		Node<Block<void>>* p = pt->value.start;

		while (p != nullptr)
		{
			if (p->value.p == point)
			{
				p->value.state = false;
				return;
			}
			p = p->next;
		}

	}

	void set_blocks(int b1 = 10, int b2 = 10, int b4 = 10, int b8 = 10, int b16 = 10, 
					int b32 = 10, int b64 = 10, int b128 = 10, int b256 = 10)
	{
		Node<Queue<Block<void>>>* pt = blocks.start;

		add_block(1, b1, pt);
		add_block(2, b2, pt);
		add_block(4, b4, pt);
		add_block(8, b8, pt);
		add_block(16, b16, pt);
		add_block(32, b32, pt);
		add_block(64, b64, pt);
		add_block(128, b128, pt);
		add_block(256, b256, pt);

	}

	void add_block(int size, int blocks_amount, Node<Queue<Block<void>>>*& pt)
	{
		for (int i = 0; i < blocks_amount; i++)
		{
			Block<void> block;
			block.p = malloc(size);
			block.size = size;
			pt->value.push_back(block);
		}
		pt = pt->next;
	}
	~Allocator() = default;
};




