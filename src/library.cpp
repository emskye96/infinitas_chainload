#include <string>
#include <fstream>
#include <windows.h>
#include <filesystem>

TCHAR module_path[MAX_PATH] = {};

extern "C" __declspec(dllexport) HRESULT Direct3DCreate9Ex(UINT SDKVersion, class IDirect3D9Ex** unnamedParam2)
{
	auto library_list = std::wifstream(
		// Strip the filename part from the module path, then add 'chainload.txt'.
		std::filesystem::path(module_path).remove_filename().append("chainload.txt")
	);

	if (library_list.is_open())
	{
		// Read filenames from each line and call LoadLibrary.
		for (std::wstring line; std::getline(library_list, line);)
		{
			// Treat lines starting with '#' as comments.
			if (line.empty() || line.starts_with(L"#"))
				continue;

			LoadLibrary(line.c_str());
		}
	}

	// Try to find the real d3d9.dll library in the system directory.
	TCHAR system_dir[MAX_PATH];
	GetSystemDirectory(system_dir, MAX_PATH);

	// Load the real d3d9.dll, get the 'Direct3DCreate9Ex' function, call it, and return the result.
	const auto real_d3d9 = LoadLibrary(std::filesystem::path(system_dir).append("d3d9.dll").c_str());
	return reinterpret_cast<decltype(&Direct3DCreate9Ex)>(GetProcAddress(real_d3d9, "Direct3DCreate9Ex"))(SDKVersion, unnamedParam2);
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID)
{
	// Store the filename of this library for later.
	if (reason == DLL_PROCESS_ATTACH)
		GetModuleFileName(module, module_path, MAX_PATH);

	return TRUE;
}