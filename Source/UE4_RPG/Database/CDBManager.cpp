#include "CDBManager.h"
#include <xdevapi.h>
#include "Global.h"


UCDBManager::UCDBManager()
{
}

UCDBManager::~UCDBManager()
{
	Disconnect();
}

bool UCDBManager::Connect(const FString& Host, int32 Port, const FString& UserName, const FString& Password, const FString& Schema)
{
	std::string strHost = TCHAR_TO_UTF8(*Host);
	std::string strUserName = TCHAR_TO_UTF8(*UserName);
	std::string strPassword = TCHAR_TO_UTF8(*Password);
	std::string strSchema = TCHAR_TO_UTF8(*Schema);
	
	try {
		m_Session = new mysqlx::Session(strHost, Port, strUserName, strPassword);
		m_SchemaDB = new mysqlx::Schema(m_Session->getSchema(strSchema));
		CLog::Log(TEXT("DBConnect Success."));
		return true;
	}
	catch (const mysqlx::Error& err)
	{
		FString ErrorMsg = TEXT("Mysql Error : %s") + FString(err.what());
		CLog::Log(ErrorMsg);
		return false;
	}
	catch (std::exception& ex)
	{
		FString ErrorMsg = TEXT("Standard Exception : %s") + FString(ex.what());
		CLog::Log(ErrorMsg);
		return false;
	}
	catch (...)
	{
		CLog::Log(TEXT("Connect Unknown Exception Error."));
		return false;
	}

	return false;
}

bool UCDBManager::InsertUserTable(const FString& InTableName, const FString& InUserName, const FString& InPassword)
{
	if (nullptr == m_Session)
	{
		CLog::Log(TEXT("Unknown Exception Error."));
		
		return false;
	}

	try
	{
		mysqlx::Table Table = m_SchemaDB->getTable(TCHAR_TO_UTF8(*InTableName));

		Table.insert("username", "password").values(TCHAR_TO_UTF8(*InUserName), TCHAR_TO_UTF8(*InPassword)).execute();

		CLog::Log(TEXT("DBInsert Success."));
		return true;
	}
	catch(const mysqlx::Error& err)
	{
		FString ErrorMsg = TEXT("Data Insert Fail Error : %s") + FString(err.what());
		CLog::Log(ErrorMsg);
		return false;
	}
	catch (...)
	{
		CLog::Log(TEXT("DBInsert Unknown Exception Error."));
		return false;
	}

	return false;
}

bool UCDBManager::Disconnect()
{
	if (m_Session)
	{
		delete m_Session;
		m_Session = nullptr;

		CLog::Log(TEXT("DB Session Close."));
	}
	return false;
}
