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

	static std::unique_ptr<T> instance_;
public:
	inline static std::unique_ptr<T>& GetInstance()
	{
		if (instance_.get() == nullptr)
		{
			instance_.reset(new T());
		}
		return instance_;
	}
};

template<typename T>
std::unique_ptr<T> Singleton<T>::instance_ = {};