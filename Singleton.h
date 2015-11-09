//Singleton.h

#ifndef SINGLETON_H
#define SINGLETON_H

template <class Type>
class Singleton
{
private:
	static Type* s_pInstance;

public:
	static Type* Get()
	{
		if (!s_pInstance)
			s_pInstance = new Type;

		return s_pInstance;
	}

	static void DestroySingleton()
	{
		delete s_pInstance;
		s_pInstance = nullptr;
	}
};

template <class Type>
Type* Singleton<Type>::s_pInstance = nullptr;

#endif // !SINGLETON_H
