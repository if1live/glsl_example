// Ŭnicode please
#include "stdafx.h"
#include "sora/assert_inc.h"
#include "buffer_object.h"

using namespace std;

namespace haruna {;
namespace gl {
	
	IndexBufferObject::IndexBufferObject()
		: count_(0), elem_size_(0), handle_(0)
	{ 
	}
	IndexBufferObject::~IndexBufferObject() 
	{
	}

	bool IndexBufferObject::IsLoaded() const 
	{ 
		return handle_ != 0;
	}
	bool IndexBufferObject::Deinit() 
	{
		if(handle_ == 0) {
			return false;
		}
		glDeleteBuffers(1, &handle_);
        handle_ = 0;
		elem_size_ = 0;
		count_ = 0;
		return true;
	}

	bool IndexBufferObject::Load(const std::vector<unsigned short> &index_list) { 
		SR_ASSERT(IsLoaded() == true);
		if(index_list.empty()) {
			return false;
		}

		count_ = index_list.size();
		elem_size_ = sizeof(index_list[0]);
		int size = count_ * elem_size_;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, &index_list[0], GL_STATIC_DRAW);
		//unbind
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);		
		return true;
	}

	bool IndexBufferObject::Init(const std::vector<unsigned short> &index_list) {
		//typedef IndexContainer::value_type IndexElemType;
		//static_assert(std::is_same<unsigned short, IndexElemType>::value, "not unsigned short index");
		if(handle_ != 0) {
			return false;
		}
		SR_ASSERT(IsLoaded() == false);
		glGenBuffers(1, &handle_);
		return Load(index_list);
	}

}	// namespace gl
}	// namespace haruna