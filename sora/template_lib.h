// Ŭnicode please
#pragma once

#include "assert_inc.h"

namespace sora {;
/// @brief second에 포인터를 저장하는 연관 컨테이너가
/// 보관하는 포인터 해제 및 적절히 비우기
template<typename AssocType>
void DestroyDict(AssocType *dict)
{
	typename AssocType::iterator it = dict->begin();
	typename AssocType::iterator endit = dict->end();
	for ( ; it != endit ; it++) {
		delete(it->second);
	}
	dict->clear();
}

/// @brief 포인터를 저장하는 컨테이너의 내용 delete 및 비우기
template<typename SeqType>
void DestroyList(SeqType *list) 
{
	typename SeqType::iterator it = list->begin();
	typename SeqType::iterator endit = list->end();
	for ( ; it != endit ; it++) {
		delete(*it);
	}
	list->clear();
}

template<typename T>
void SafeDelete(T* &ptr) 
{
	if(ptr != NULL) {
		delete(ptr);
		ptr = NULL;
	}
}

template<typename T>
void SafeDeleteArray(T* &ptr) 
{
	if(ptr != NULL) {
		delete[](ptr);
		ptr = NULL;     
	}
}

template<typename T>
void SafeDeleteWithNullCheck(T* &ptr) 
{
	SR_ASSERT(ptr != NULL && "null지우기는 언어상은 지원하나 아마도 잘못 짜여져있을 가능성이 있다");
	delete(ptr);
	ptr = NULL;
}

template<typename T>
void SafeDeleteArrayWithNullCheck(T* &ptr) 
{
	SR_ASSERT(ptr != NULL && "null지우기는 언어상은 지원하나 아마도 잘못 짜여져있을 가능성이 있다");
	delete[](ptr);
	ptr = NULL;
}

template<typename T>
void CallConstructor(T *ptr) 
{
	new(ptr) T;
}

template<typename T>
void CallDestructor(T *ptr) 
{
	ptr->~T();
}

template<typename T, int N>
int GetArraySize(T (&arr)[N]) 
{
	return N;
}

/////////////////////////////////////////
template<typename T>
class Singleton {
public:
	static T& GetInstance() 
	{
		if (ctx_ == 0) {
			ctx_ = new T();
		}
		return *ctx_;
	}

	static void DestroyInstance() 
	{
		if (ctx_ != 0) {
			delete(ctx_);
			ctx_ = 0; 
		}
	}
	static bool IsCreated() { return (ctx_ != 0); }

protected:
	Singleton() 
	{
		SR_ASSERT(ctx_ == 0);
		long offset = (long)(T*)(1) - (long)(Singleton*)(T*)(1);
		ctx_ = (T*)((long)(this + offset));
	}
	~Singleton() 
	{
		SR_ASSERT(ctx_ != 0);
		ctx_ = 0;
	}
	Singleton(const Singleton &o);
	Singleton operator=(const Singleton &o);
	static T *ctx_;
};
template<typename T>
T* Singleton<T>::ctx_ = 0;
////////////////////////////////////////////
template<typename T>
class SharedObject {
public:
	static T& GetInstance()
	{
		if (ctx_ == NULL) {
			inner_allcated_ = true;
			ctx_ = new T();
		}
		return *ctx_;
	}
	static void DestoryInstance() 
	{
		if (ctx_ != NULL && inner_allcated_) {
			delete(ctx_);
			inner_allcated_ = false;
		}
		ctx_ = NULL;
	}

	static bool IsCreated() { return (ctx_ != NULL); }

	SharedObject() 
	{
		if (SharedObject::ctx_ == NULL) {
			SharedObject::ctx_ = static_cast<T*>(this);
		} else {
			//@TODO
			//SR_ASSERT(!"shared obj cannot exist 2 obj");
		}
	}
	~SharedObject() 
	{
		SharedObject::ctx_ = NULL;
	}
private:
	static T *ctx_;
	static bool inner_allcated_;
};
template<typename T>
T *SharedObject<T>::ctx_ = NULL;
template<typename T>
bool SharedObject<T>::inner_allcated_ = false;

//mpl 같은거 할떄 필요한것들
template<typename T>
struct Type2Type {
	typedef T Type;
};

}	//namespace sora
