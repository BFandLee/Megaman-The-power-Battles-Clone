#include "pch.h"
#include "FileDialog.h"
#include <commdlg.h> // OPENFILENAME 구조체를 위해 필요

namespace FileDialog
{
    std::wstring Open(const wchar_t* filter)
    {
        OPENFILENAMEW ofn;
        wchar_t szFile[260];
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = nullptr; // 소유자 윈도우는 nullptr로 둬서 독립적인 팝업으로 띄움
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0'; // 첫 글자를 NULL로 초기화해야 됨
        ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        
        // OFN_NOCHANGEDIR: 파일 선택 시 현재 작업 디렉토리가 바뀌는 것을 방지 (리소스 로딩 에러 방지)
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameW(&ofn) == TRUE)
        {
            return std::wstring(ofn.lpstrFile);
        }
        return L""; // 취소 시 빈 문자열 반환
    }

    std::wstring Save(const wchar_t* filter)
    {
        OPENFILENAMEW ofn;
        wchar_t szFile[260];
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = nullptr;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        
        // OFN_OVERWRITEPROMPT: 덮어쓰기 시 경고창 띄움
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        if (GetSaveFileNameW(&ofn) == TRUE)
        {
            return std::wstring(ofn.lpstrFile);
        }
        return L"";
    }
}
