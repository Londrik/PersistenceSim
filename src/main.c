#include <stdio.h>
#include <windows.h>
#include "persistence.h"

int wmain(int argc, wchar_t *argv[]) {
    (void)argc;
    (void)argv;

    const wchar_t *app_name = L"PersistenceSimApp";
    wchar_t current_path[MAX_PATH];

    DWORD length = GetModuleFileNameW(NULL, current_path, MAX_PATH);
    if (length == 0 || length >= MAX_PATH) {
        wprintf(L"[-] Falha ao obter caminho do modulo: %lu\n", GetLastError());
        return 1;
    }

    wprintf(L"[*] Simulando persistencia HKCU Run para: %ls\n", current_path);

    LSTATUS status = SetStartupPersistence(app_name, current_path);
    if (status == ERROR_SUCCESS) {
        wprintf(L"[+] Chave de persistencia criada com sucesso.\n");
    } else {
        wprintf(L"[-] Falha ao criar chave: %ld\n", status);
        return (int)status;
    }

    status = RemoveStartupPersistence(app_name);
    if (status == ERROR_SUCCESS) {
        wprintf(L"[+] Chave removida com sucesso (Cleanup).\n");
    } else {
        wprintf(L"[-] Falha no cleanup da chave: %ld\n", status);
        return (int)status;
    }

    return 0;
}
