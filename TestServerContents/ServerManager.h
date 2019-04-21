#pragma once

#include <map>
#include "CommonDef.h"
#include "Singleton.hpp"
#include "SessionManager.h"
#include "NetworkMode.h"



class CServerBase;
class CServerManager :
	public CSingleton<CServerManager>
{
public:
	typedef std::map<ServerType, CServerBase*> SERVER_MAP;

private:
	SERVER_MAP m_ServerMap;

private:
	virtual void DerivedInit() override;
	virtual void DerivedRelease() override;

public:
	bool RegistServer(CNetworkMode::SConfigInfo const& info_);

	bool RemoveServer(ServerType const serverType_);

	bool Start();

	void Close();
};




