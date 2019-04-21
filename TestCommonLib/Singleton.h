#pragma once

template<typename T>
class CSingleton
{
private:
	static T* pInstance;

protected:
	CSingleton();
	virtual ~CSingleton();

private:
	CSingleton(const CSingleton& val_);
	const CSingleton& operator=(const CSingleton& val_);

protected:
	virtual void DerivedInit();
	virtual void DerivedRelease();

public:
	/// 메인 쓰레드에서 미리 초기화해라...
	static void Init();

	/// 초기화 안하고 쓰면 뻑남
	static T* GetInstance();

	/// 싱글턴에 static 쓰는거 싫어하는 사람들을 위해 마련하였읍니다.
	static void Release();
};


