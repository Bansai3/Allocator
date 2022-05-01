#include"Allocator_2.h"
#include<vector>
#include<string>
#include<list>
#include<deque>
#include<chrono>


int main()
{
	auto s = std::chrono::high_resolution_clock::now();

	std::vector<int, Allocator<int>> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	auto e = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> d = e - s;
	std::cout << d.count() << std::endl;

/////////////////////////////////////////////////////////////

	auto s_2 = std::chrono::high_resolution_clock::now();

	std::vector<int> v_2;
	v_2.push_back(1);
	v_2.push_back(2);
	v_2.push_back(3);

	auto e_2 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> d_2 = e_2 - s_2;
	std::cout << d.count() << std::endl;



	/*std::list<int, Allocator<int>> l;
	std::deque<int, Allocator<int>> d; */

	/*Allocator<int> g;

	l.push_back(1);
	l.push_back(1);
	l.push_back(1);*/

	return 0;
}