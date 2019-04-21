#pragma once
#include "stdafx.h"
#include "Singleton.h"


template<typename T>
T* CSingleton<T>::pInstance = NULL;

template<typename T>
CSingleton<T>::CSingleton()
{
}

template<typename T>
CSingleton<T>::~CSingleton()
{
}

template<typename T>
CSingleton<T>::CSingleton(const CSingleton& val_)
{
}

template<typename T>
const CSingleton<T>& CSingleton<T>::operator=(const CSingleton<T>& val_)
{
}

template<typename T>
void CSingleton<T>::DerivedInit()
{
}

template<typename T>
void CSingleton<T>::DerivedRelease()
{
}

template<typename T>
void CSingleton<T>::Init()
{
	pInstance = ALLOCATE_NEW(T);
	auto derived = static_cast<CSingleton<T>*>(pInstance);
	derived->DerivedInit();
}

template<typename T>
T* CSingleton<T>::GetInstance()
{
	return pInstance;
}

template<typename T>
void CSingleton<T>::Release()
{
	auto derived = reinterpret_cast<CSingleton<T>*>(pInstance);
	derived->DerivedRelease();
	SAFE_DELETE(pInstance);
}