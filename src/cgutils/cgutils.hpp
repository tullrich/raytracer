#ifndef _CG_UTILS_HPP
#define _CG_UTILS_HPP

#include <string>
#include <memory>
using typename std::string;

#include <iostream>

namespace cgutils
{

#ifndef NDEBUG
  #include <cassert>
  #define CGUTILS_ASSERT(condition) \
  { \
      if(!(condition)) \
      { \
          std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__; \
          std::cerr << " inside " << __FUNCTION__ << std::endl; \
          std::cerr << "Condition: " << " "; \
          abort(); \
      } \
  } 
#else
  #define CGUTILS_ASSERT(condition) (condition)
#endif

struct CallBack
{
	virtual void callback() = 0;
};

template<typename T, void (T::*F)()>
struct CallBackImpl : CallBack
{
	void* p;
	typedef T* obj_ptype;

	CallBackImpl(T &obj)
	{
		p = &obj;
	};

	virtual void callback()
	{
		(((obj_ptype)p)->*F)();
	}
};

template<void (*F)()>
struct StaticCallBackImpl : CallBack
{

	StaticCallBackImpl()
	{};

	virtual void callback()
	{
		(*F)();
	}
};

template<typename T>
class Visitor
{
	virtual void visit(T &object)  = 0;
};

/**
 * Templated singled that returns T
 */
template<typename T>
class Singleton
{
private:
	Singleton() {};
public:

	static T& getInstance()
	{
		if (!instance)
		{
			// allocate singleton, run only once
			instance = std::shared_ptr<T>(new T());
		}

		return *instance;
	}

private:
	static T *instance;
};

} /* namespace cgutils */
#endif /* _CG_UTILS_HPP */
