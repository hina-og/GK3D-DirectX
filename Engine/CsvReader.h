#pragma once
#include <string>
#include <vector>

class CsvReader {
public:
	CsvReader() {};
	CsvReader(std::string filename);
	~CsvReader();
	void Load(std::string filename);
	//�c
	int GetLines();
	//��
	int GetColumns(int line);
	std::string GetString(int line, int column);
	int GetInt(int line, int column);
	float GetFloat(int line, int column);
private:
	struct LINEREC {
		std::vector<std::string> record;
	};
	std::vector<LINEREC> all;
};
