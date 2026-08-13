#pragma once
#include "pch.h"
#include <string>

namespace FileDialog
{
    // 파일 열기 다이얼로그 (기본 필터: 모든 파일)
    std::wstring Open(const wchar_t* filter = L"All Files (*.*)\0*.*\0");
    
    // 파일 저장 다이얼로그 (기본 필터: 모든 파일)
    std::wstring Save(const wchar_t* filter = L"All Files (*.*)\0*.*\0");
}
