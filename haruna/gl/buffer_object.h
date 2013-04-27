// Ŭnicode please
#pragma once
#include <GL/glew.h>
#include "sora/assert_inc.h"

namespace haruna {;
namespace gl {

	class IndexBufferObject {
	public:
		IndexBufferObject();
		~IndexBufferObject();

		bool Load(const std::vector<unsigned short> &index_list);
		bool Init(const std::vector<unsigned short> &index_list);
		bool Deinit();
		bool IsLoaded() const;

		int Count() const { return count_; }
		bool empty() const { return (count_ == 0); }

		GLuint handle() const { return handle_; }
		operator GLuint() const { return handle_; }
		bool IsBuffer() const { return true; }
		int ElemSize() const { return elem_size_; }

	private:
		GLuint handle_;
		int elem_size_;
		int count_;
	};


	template<typename VertexT>
	class VertexBufferObjectT {
	public:
		typedef VertexT value_type;
		enum {
			elem_size = sizeof(VertexT)
		};

	public:
		VertexBufferObjectT() : count_(0), handle_(0) { }
		~VertexBufferObjectT() {}

		bool Init(const std::vector<VertexT> &vert_list) { return InitWithTypeCheck(vert_list); }
		bool Load(const std::vector<VertexT> &vert_list) { return LoadWithTypeCheck(vert_list); }
		
		template<typename T2>
		bool InitWithTypeCheck(const std::vector<T2> &vert_list) 
		{
			SR_ASSERT(IsLoaded() == false);
			if(vert_list.empty()) {
				return false;
			}
			if(handle_ != 0) {
				return false;
			}
			glGenBuffers(1, &handle_);
			return LoadWithTypeCheck(vert_list);
		}

		template<typename T2>
		bool LoadWithTypeCheck(const std::vector<T2> &vert_list) 
		{
			SR_ASSERT(IsLoaded() == true);
			if(vert_list.empty()) {
				return false;
			}
			if(std::is_same<T2, value_type>::value == false) {
				return false;
			}

			count_ = vert_list.size();
			int size = ElemSize() * count_;

			glBindBuffer(GL_ARRAY_BUFFER, handle_);
			glBufferData(GL_ARRAY_BUFFER, size, &vert_list[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);		
			
			return true;
		}
		
		bool Deinit() 
		{
			if(handle_ == 0) {
				return false;
			}
			glDeleteBuffers(1, &handle_);
			handle_ = 0;
			count_ = 0;
			return true;
		}

		int Count() const { return count_; }
		bool empty() const { return (count_ == 0); }

		GLuint handle() const { return handle_; }
		operator GLuint() const { return handle_; }
		bool IsBuffer() const { return true; }
		int ElemSize() const { return sizeof(value_type); }

		bool IsLoaded() const { return handle_ != 0; }

	private:
		GLuint handle_;
		int count_;
	};
}	// namespace gl
}	// namespace haruna