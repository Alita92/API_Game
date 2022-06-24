#include "EngineGeneralDirectory.h"
#include <Windows.h>
#include <io.h> // _access() 사용용도

// Static Var
// Static Func

// constructer destructer
EngineGeneralDirectory::EngineGeneralDirectory()
{
	path_.resize(256); // 내부의 문자열을 256개 생성 
						// 내부에 char* = new char[256]

	for (size_t i = 0; i < path_.size(); i++) // 버퍼의 모든 배열에 null 0 을 배치해 준다.
	{
		path_[i] = '\0';
	}

	GetCurrentDirectoryA(static_cast<DWORD>(path_.size()), &path_[0]); // 현재의 작업 경로를 받와 path_ 에 넣어 준다.
	//							버퍼의 길이				  버퍼의 포인터
	size_t stringEnd = path_.find('\0'); // 작업 경로를 받아 논 path_ 에, null 0이 처음으로 들어간 배열을 찾는다
	path_ = path_.substr(0, stringEnd); // 정해진 문자를 특정 자릿수만큼 잘라내는 함수
	path_ += "\\"; // 잘라낸 문자열 마지막에 \\ 를 붙여 사용할 것이다.
}

EngineGeneralDirectory::~EngineGeneralDirectory()
{
}

EngineGeneralDirectory::EngineGeneralDirectory(const EngineGeneralDirectory& _other)
	: EngineGeneralPath(_other)
{

}

EngineGeneralDirectory::EngineGeneralDirectory(EngineGeneralDirectory&& _other) noexcept
{
}

//member Func

std::string EngineGeneralDirectory::GetDirectoryName()
{
	size_t lastWordLocater = path_.rfind("\\");  // rfind() 특정 문자열이 마지막으로 나타나는 위치를 찾는다. 
	if (std::string::npos == lastWordLocater) // std::string::npos <= 반환 값으로 일반적으로 일치하는 항목이 없을 때 사용.. null?
	{
		return path_; // 그냥 그 경로를 리턴해준다.
	}
	// \\ 가 존재한다면...
	size_t beforeLastWordLocator = path_.rfind("\\", lastWordLocater - 1); // 역시 문자열의 마지막-1 의 "\\" 나타나는 위치를 찾아 저장한다.

	std::string resultName = path_.substr(beforeLastWordLocator, lastWordLocater);
	resultName = resultName.substr(1, resultName.size() - 2);
	return resultName;

}

void EngineGeneralDirectory::MovetoParentFolder() // 상위 폴더로 하나 올라가는 함수
{
	size_t lastWordLocater = path_.rfind("\\"); // 끝부분 \\ 가 들어가는 위치를 찾고...
	if (std::string::npos == lastWordLocater)
	{
		return;
	}

	size_t beforeLastWordLocator = path_.rfind("\\", lastWordLocater - 1); // 끝-1 부분 \\ 가 들어가는 위치를 찾고...
	path_ = path_.substr(0, beforeLastWordLocator + 1); // 그 부분만큼을 잘라낸다...

}

bool EngineGeneralDirectory::IsReachedtoRoot() 
{
	int stringCounter = 0;

	for (size_t i = 0; i < path_.size(); i++)
	{
		if (path_[i] == '\\') // 경로의 경계를 만나면...
		{
			++stringCounter;
		}
	}

	if (1 >= stringCounter)
	{
		return true;
	}

	return false;
}

bool EngineGeneralDirectory::MovetoParentFolder(std::string _dirName) // 특정 인자로 받은 디렉토리명을 찾을 때까지 올라가는 함수
{
	EngineGeneralDirectory dirChecker = EngineGeneralDirectory(*this);

	while (true)
	{
		if (_dirName == dirChecker.GetDirectoryName()) // 인자와 일치하는 디렉토리명을 찾았다면... 나가기
		{
			break;
		}

		dirChecker.MovetoParentFolder(); // 아닐 시 계속 재귀함수로서 상위 폴더로 올라가기

		if (true == dirChecker.IsReachedtoRoot())
		{
			return false;
		}
	}
	this->path_ = dirChecker.path_;
	return true;
}

bool EngineGeneralDirectory::IsFolderExists()
{
	if (0 == _access(path_.c_str(), 0))
	{
		return true;
	}

	return false;
}

bool EngineGeneralDirectory::MovetoChildFolder(std::string _dirName) // 특정 자식 폴더로 들어가는 함수
{
	if (0 == _dirName.size())
	{
		return false;
	}

	if (_dirName[0] == '\\')
	{
		_dirName = _dirName.substr(1, _dirName.size());
	}

	if (_dirName[_dirName.size() - 1] == '\\')
	{
		_dirName = _dirName.substr(0, _dirName.size() - 1);
	}

	EngineGeneralDirectory newDir = EngineGeneralDirectory();
	newDir.path_ = this->path_ + _dirName + "\\";

	if (false == newDir.IsFolderExists())
	{
		return false;
	}

	this->path_ += _dirName + "\\";
	return true;
}

std::string EngineGeneralDirectory::GetRelativePath(const std::string _FileName)
{ // 만들어진 경로 버퍼에 찾고자 하는 파일명을 붙여 사용할 수 있는 "실질적인 목표로 했던 함수"
	return path_ + _FileName;
}