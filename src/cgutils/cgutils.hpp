#ifndef _CG_UTILS_HPP
#define _CG_UTILS_HPP

#include <string>
#include <memory>
using typename std::string;

#include <iostream>
#include <map>

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
public:
	virtual void visit(T &object) const  = 0;
};

/**
 * Templated singleton pattern
 * Derived classes must friend Singleton<> so that its private
 * constructor is available in this scope
 */
template<typename T>
class Singleton
{
protected:
	Singleton() {};
	virtual ~Singleton() {};

public:

	static T& getInstance()
	{
		if (!Singleton<T>::instance)
		{
			// allocate singleton, run only once
			Singleton<T>::instance = new T();
		}

		return *(Singleton<T>::instance);
	}

private:
	static T *instance;

 	/** 
 	 * Private copy constructor to prevent copy construction
 	 */
	Singleton( Singleton const & ) {};

	/** 
	 * Private operator to prevent assignment
	 */
	Singleton &operator=( Singleton const & ) {};
};

template< typename T >
T *Singleton<T>::instance        = NULL;



template <typename K, typename T>
class ResourceManager : public Singleton<ResourceManager<K, T>>
{
	/**
	 * Must be a friend class of the singleton base
	 */
	friend class Singleton<ResourceManager<K, T>>;

protected:
	typedef std::map<K, T*> entity_map;
	ResourceManager() {};

public:

	/**
	 * Adds a resource of type T into this manager.
	 * @param key key for inserted element
	 * @param r resource instance to be added
	 */
	void add(const K &key, T &r)
	{
		if (resources.find(key) != resources.end())
		{
			//std::cout << "Warning: trying to add duplicate key " << key << std::endl;
			return;
		}

		//std::cout << " inserting('" << key << "') " << r << std::endl;
		resources[key] = &r;
	}

	/**
	 * get a resource from this manager
	 * @param  key key for the resource
	 * @return     ref to the resource
	 */
	T* get(const K& key)
	{
		if(resources.find(key) != resources.end())
		{
			return resources[key];
		}

		return NULL;
	}

	/**
	 * fnv1 hash (http://isthe.com/chongo/tech/comp/fnv/) using recommended 
	 * seed and prime
	 * @param  str string to be hashed
	 * @return     	hashed int
	 */
	int calcHash(const std::string &str)
	{
		int prime = 0x01000193; //   16777619
		int hash = 0x811C9DC5; // the seed - 2166136261

		for (char c : str)
		{
			hash = (c ^ hash) * prime;
		}

		return hash;
	}

	int size() const
	{
		return resources.size();
	}

private:
	entity_map resources;

};

} /* namespace cgutils */
#endif /* _CG_UTILS_HPP */
