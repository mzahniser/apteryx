/* AutoVector.h
Michael Zahniser, 08/09/07

Header file for the AutoVector templated class, which stores a set of auto_ptrs and gives
access to the objects they point to, while ensuring that the auto_ptr copy semantics don't
mess up the contents of the vector.
*/

#ifndef __MZ_AUTO_VECTOR_H__
#define __MZ_AUTO_VECTOR_H__


#include <memory>

using std::auto_ptr;



template <class Type>
class AutoVector {
public:
	AutoVector()
	: begin(new auto_ptr<Type>[1]), it(begin), end(begin + 1) {}
	
	// Assignment does _not_ copy vector contents.
	AutoVector(const AutoVector &autoVector)
	: begin(new auto_ptr<Type>[1]), it(begin), end(begin + 1) {}
	
	AutoVector &operator=(const AutoVector &autoVector)
	{
		Free(begin, it);
		delete [] begin;
		
		begin = new auto_ptr<Type>[1];
		it = begin;
		end = begin + 1;
		
		return *this;
	}

	~AutoVector()
	{
		Free(begin, it);
		delete [] begin;
	}
	
	void Add(auto_ptr<Type> data)
	{
		if(it == end)
		{
			size_t size = Size();
			auto_ptr<Type> *newBegin = new auto_ptr<Type>[2 * size];
			
			Copy(newBegin, begin, end);
			
			delete [] begin;
			begin = newBegin;
			it = begin + size;
			end = begin + 2 * size;
		}
		
		*it = data;
		++it;
	}
	
	Type &operator[](int index) { return **(begin + index); }
	const Type &operator[](int index) const { return **(begin + index); }
	
	size_t Size() const { return (it - begin); }
	
	
private:
	static void Copy(auto_ptr<Type> *newBegin, auto_ptr<Type> *begin, auto_ptr<Type> *end)
	{
		for(; begin != end; ++begin, ++newBegin)
			*newBegin = *begin;
	}
	
	static void Free(auto_ptr<Type> *begin, auto_ptr<Type> *end)
	{
		for(; begin != end; ++begin)
			begin->reset();
	}
	
	
private:
	auto_ptr<Type> *begin;
	auto_ptr<Type> *it;
	auto_ptr<Type> *end;
};

#endif
