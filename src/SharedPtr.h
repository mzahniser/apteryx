/* SharedPtr.h
Michael Zahniser, 08/09/07

Header file for the SharedPtr templated class, a simple reference-counted pointer
that is not thread safe.
*/

#ifndef __MZ_SHARED_PTR_H__
#define __MZ_SHARED_PTR_H__



template <class Type>
class SharedPtr {
public:
	SharedPtr() : data(NULL), count(NULL) {}
	explicit SharedPtr(Type *ptr) : data(ptr), count(ptr ? new int(1) : NULL) {}
	SharedPtr(const SharedPtr &ptr) : data(ptr.data), count(ptr.count) { Retain(); }
	~SharedPtr() { Release(); }
	
	SharedPtr &operator=(const SharedPtr &ptr)
	{
		// Check for self-assignment.
		if(this == &ptr)
			return *this;
		
		Release();
		
		data = ptr.data;
		count = ptr.count;
		
		Retain();
		
		return *this;
	}
	
	Type &operator*() { return *data; }
	const Type &operator*() const { return *data; }
	
	Type *operator->() { return data; }
	const Type *operator->() const { return data; }
	
	operator bool() const { return (data != 0); }
	bool operator!() const { return !static_cast<bool>(*this); }
	
	bool operator==(const SharedPtr &ptr) const { return (data == ptr.data); }
	bool operator!=(const SharedPtr &ptr) const { return !(*this == ptr); }
	bool operator==(const Type *ptr) const { return (data == ptr); }
	bool operator!=(const Type *ptr) const { return !(*this == ptr); }
	friend bool operator==(const Type *ptr, const SharedPtr &sp) { return (sp.data == ptr); }
	friend bool operator!=(const Type *ptr, const SharedPtr &sp) { return !(sp == ptr); }
	
	void Reset() { Release(); }
	void Swap(SharedPtr &ptr) {
		int *tmpCount = count; count = ptr.count; ptr.count = tmpCount;
		Type *tmpData = data; data = ptr.data; ptr.data = tmpData; }
	
	
private:
	void Retain() { if(count) ++*count; }
	void Release() { if(count && !--*count) { delete count; delete data; } count = NULL; data = NULL; }
	
	
private:
	int *count;
	Type *data;
};

#endif