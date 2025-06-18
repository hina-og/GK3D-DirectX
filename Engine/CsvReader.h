#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>

class CsvReader {
public:
	CsvReader() {};
	CsvReader(std::string filename);
	~CsvReader();
	void Load(std::string filename);
	//èc
	int GetLines();
	//â°
	int GetColumns(int line);
	std::string GetString(int line, int column);
	int GetInt(int line, int column);
	float GetFloat(int line, int column);
	DirectX::XMFLOAT2 GetFloat2(int line, int column);
	DirectX::XMFLOAT3 GetFloat3(int line, int column);
	DirectX::XMFLOAT4 GetFloat4(int line, int column);
private:
	struct LINEREC {
		std::vector<std::string> record;
	};
	std::vector<LINEREC> all;
};
