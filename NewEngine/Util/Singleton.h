#pragma once
#include <memory>

template<typename T> class Singleton
{
protected:
	Singleton() {};
public:
	virtual ~Singleton() {};
private:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton& operator=(const Singleton&&) = delete;

	static std::unique_ptr<T> instance;
public:
	inline static std::unique_ptr<T>& GetInstance()
	{
		if (instance.get() == nullptr)
		{
			instance.reset(new T());
		}
		return instance;
	}
};

template<typename T>
std::unique_ptr<T> Singleton<T>::instance = {};