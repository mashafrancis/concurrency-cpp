#include <iostream>
#include <atomic>
#include <memory> //for shared_ptr
using namespace std;

template<typename T>
class concurrent_stack
{
	struct Node
	{
		T t;
		shared_ptr<Node> next;
	};
	shared_ptr<Node> head;
	concurrent_stack(concurrent_stack &) = delete;
	void operator=(concurrent_stack &) = delete;

public:
	concurrent_stack() = default;
	~concurrent_stack() = default;

	class reference
	{
		shared_ptr<Node> p;
	public:
		reference(shared_ptr<Node> p_)
			: p{p_}
		{}
		T &operator*()
		{ return p->t; }
		T *operator->()
		{ return &p->t; }
	};

	auto find(T t) const
	{
		auto p = atomic_load(&head);
		while (p && p->t != t)
			p = p->next;
		return reference(move(p));
	}

	auto front() const
	{
		return reference(atomic_load(&head));
	}

	void push_front(T t)
	{
		auto p = make_shared<Node>();
		p->t = t;
		p->next = atomic_load(&head);
		while (p && !atomic_compare_exchange_weak(&head, &p->next, p)) {}
	}

	void pop_front()
	{
		auto p = atomic_load(&head);
		while (p && !atomic_compare_exchange_weak(&head, &p, p->next)) {}
	}

};

int main()
{
	concurrent_stack<int> cS;
	cS.push_front(3);
	cS.push_front(6);
	cS.find(6);
	cS.pop_front();
	cS.front();
}
