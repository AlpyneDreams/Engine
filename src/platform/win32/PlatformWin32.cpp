
#include "platform/Platform.h"
#include <string>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <commdlg.h>

namespace engine
{
    std::string Platform::FilePicker(const char* startIn = nullptr)
    {
        OPENFILENAME ofn;
        char filename[1024];

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;

        ofn.lpstrFile = filename;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(filename);
        
        //open.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
        //open.nFilterIndex = 1;

        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;

        // Windows tends to override this for convenience anyway
        ofn.lpstrInitialDir = startIn;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn)) {
            return std::string(ofn.lpstrFile);
        } else {
            return std::string();
        }
    }
}