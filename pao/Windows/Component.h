#pragma once

#include "windows.h"

template <class T> class Chain
{
	T*	next;
public:
	void Add(T* src){
		T* p = this;
		while (p->next){
			p = p->next;
		}
		p->next = src;
	}
	void AddQuick(T* src){
		src->next = next;
		next = src;
	}
};

//class Component : public Chain<Component> {
class Component {
public:
	virtual void Init(HWND hWnd) = 0;
	virtual void Update() = 0;
	virtual void Final() = 0;
};
