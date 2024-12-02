#include <init.h>
#include <windows.h>
#include <fstream>
#include <iostream>

static std::streambuf* CinBuffer, *CoutBuffer, *CerrBuffer;
static std::fstream ConsoleInput, ConsoleOutput, ConsoleError;

static void RedirectIO() {
    CinBuffer = std::cin.rdbuf();
    CoutBuffer = std::cout.rdbuf();
    CerrBuffer = std::cerr.rdbuf();
    
    ConsoleInput.open("CONIN$", std::ios::in);
    ConsoleOutput.open("CONOUT$", std::ios::out);
    ConsoleError.open("CONOUT$", std::ios::out);

    std::cin.rdbuf(ConsoleInput.rdbuf());
    std::cout.rdbuf(ConsoleOutput.rdbuf());
    std::cerr.rdbuf(ConsoleError.rdbuf());
}

static void ResetIO() {
    ConsoleInput.close();
    ConsoleOutput.close();
    ConsoleError.close();

    std::cin.rdbuf(CinBuffer);
    std::cout.rdbuf(CoutBuffer);
    std::cerr.rdbuf(CerrBuffer);

    CinBuffer = nullptr;
    CoutBuffer = nullptr;
    CerrBuffer = nullptr;
}

static void StartWin(LPVOID hModule) {
    AllocConsole();
    RedirectIO();
    {
        std::string path;
        path.resize(FILENAME_MAX, 0);
        auto path_size(GetModuleFileName(static_cast<HMODULE>(hModule), &path.front(), FILENAME_MAX));
        path.resize(path_size);

        init(path);

        std::cout << "Press any key to continue..." << std::endl;
        std::cin.get();
    }
    ResetIO();
    FreeConsole();
    FreeLibraryAndExitThread(static_cast<HMODULE>(hModule), 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(StartWin), hModule, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}
