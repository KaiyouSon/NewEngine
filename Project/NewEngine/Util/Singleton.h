#pragma once
#include <memory>

// 別クラスをシングルトーンにするクラス
template<typename T> class Singleton
{
protected:
	constexpr Singleton() {};
public:
	virtual ~Singleton() {};
private:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton& operator=(const Singleton&&) = delete;

	static std::unique_ptr<T> sInstance;
public:
	static std::unique_ptr<T>& GetInstance()
	{
		if (sInstance.get() == nullptr)
		{
			sInstance.reset(new T());
		}
		return sInstance;
	}
};

template<typename T>
std::unique_ptr<T> Singleton<T>::sInstance = {};