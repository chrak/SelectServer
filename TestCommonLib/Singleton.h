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
	/// ���� �����忡�� �̸� �ʱ�ȭ�ض�...
	static void Init();

	/// �ʱ�ȭ ���ϰ� ���� ����
	static T* GetInstance();

	/// �̱��Ͽ� static ���°� �Ⱦ��ϴ� ������� ���� �����Ͽ����ϴ�.
	static void Release();
};


