#pragma once
#include "stdafx.h"
#include "../Public Variables/Extern.hpp"

class SaveFile {
public:
	static void Load(const std::string path);
	static void Save(const std::string path);
private:
	//static void CreateFile(const std::string& path);

	static std::fstream s_file;
};