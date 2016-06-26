#pragma once
#include "stdafx.h"
#include <string>

std::wstring ANSIToUnicode(const std::string & str);

std::string UnicodeToANSI(const std::wstring & str);

std::wstring UTF8ToUnicode(const std::string & str);

std::string UnicodeToUTF8(const std::wstring & str);
