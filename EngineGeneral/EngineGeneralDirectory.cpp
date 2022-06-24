#include "EngineGeneralDirectory.h"
#include <Windows.h>
#include <io.h> // _access() ���뵵

// Static Var
// Static Func

// constructer destructer
EngineGeneralDirectory::EngineGeneralDirectory()
{
	path_.resize(256); // ������ ���ڿ��� 256�� ���� 
						// ���ο� char* = new char[256]

	for (size_t i = 0; i < path_.size(); i++) // ������ ��� �迭�� null 0 �� ��ġ�� �ش�.
	{
		path_[i] = '\0';
	}

	GetCurrentDirectoryA(static_cast<DWORD>(path_.size()), &path_[0]); // ������ �۾� ��θ� �޿� path_ �� �־� �ش�.
	//							������ ����				  ������ ������
	size_t stringEnd = path_.find('\0'); // �۾� ��θ� �޾� �� path_ ��, null 0�� ó������ �� �迭�� ã�´�
	path_ = path_.substr(0, stringEnd); // ������ ���ڸ� Ư�� �ڸ�����ŭ �߶󳻴� �Լ�
	path_ += "\\"; // �߶� ���ڿ� �������� \\ �� �ٿ� ����� ���̴�.
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
	size_t lastWordLocater = path_.rfind("\\");  // rfind() Ư�� ���ڿ��� ���������� ��Ÿ���� ��ġ�� ã�´�. 
	if (std::string::npos == lastWordLocater) // std::string::npos <= ��ȯ ������ �Ϲ������� ��ġ�ϴ� �׸��� ���� �� ���.. null?
	{
		return path_; // �׳� �� ��θ� �������ش�.
	}
	// \\ �� �����Ѵٸ�...
	size_t beforeLastWordLocator = path_.rfind("\\", lastWordLocater - 1); // ���� ���ڿ��� ������-1 �� "\\" ��Ÿ���� ��ġ�� ã�� �����Ѵ�.

	std::string resultName = path_.substr(beforeLastWordLocator, lastWordLocater);
	resultName = resultName.substr(1, resultName.size() - 2);
	return resultName;

}

void EngineGeneralDirectory::MovetoParentFolder() // ���� ������ �ϳ� �ö󰡴� �Լ�
{
	size_t lastWordLocater = path_.rfind("\\"); // ���κ� \\ �� ���� ��ġ�� ã��...
	if (std::string::npos == lastWordLocater)
	{
		return;
	}

	size_t beforeLastWordLocator = path_.rfind("\\", lastWordLocater - 1); // ��-1 �κ� \\ �� ���� ��ġ�� ã��...
	path_ = path_.substr(0, beforeLastWordLocator + 1); // �� �κи�ŭ�� �߶󳽴�...

}

bool EngineGeneralDirectory::IsReachedtoRoot() 
{
	int stringCounter = 0;

	for (size_t i = 0; i < path_.size(); i++)
	{
		if (path_[i] == '\\') // ����� ��踦 ������...
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

bool EngineGeneralDirectory::MovetoParentFolder(std::string _dirName) // Ư�� ���ڷ� ���� ���丮���� ã�� ������ �ö󰡴� �Լ�
{
	EngineGeneralDirectory dirChecker = EngineGeneralDirectory(*this);

	while (true)
	{
		if (_dirName == dirChecker.GetDirectoryName()) // ���ڿ� ��ġ�ϴ� ���丮���� ã�Ҵٸ�... ������
		{
			break;
		}

		dirChecker.MovetoParentFolder(); // �ƴ� �� ��� ����Լ��μ� ���� ������ �ö󰡱�

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

bool EngineGeneralDirectory::MovetoChildFolder(std::string _dirName) // Ư�� �ڽ� ������ ���� �Լ�
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
{ // ������� ��� ���ۿ� ã���� �ϴ� ���ϸ��� �ٿ� ����� �� �ִ� "�������� ��ǥ�� �ߴ� �Լ�"
	return path_ + _FileName;
}