#include <stdio.h>
#include "persistence.h"

int wmain(void) {
    wchar_t current_exe[MAX_PATH];
    DWORD path_status = GetCurrentBinaryPath(current_exe, MAX_PATH);

    if (path_status != ERROR_SUCCESS) {
        wprintf(L"[-] Falha ao obter caminho do binario: erro 0x%08lX\n", path_status);
        return 1;
    }

    wprintf(L"[+] Binario localizado: %ls\n", current_exe);

    const wchar_t *entry_name = L"PersistenceSimulatorApp";
    LSTATUS status = SetUserRunPersistence(entry_name, current_exe);

    if (status != ERROR_SUCCESS) {
        wprintf(L"[-] Falha ao configurar persistencia: erro 0x%08lX\n", status);
        return 1;
    }

    wprintf(L"[+] Persistencia configurada com sucesso em HKCU Run.\n");

    LSTATUS cleanup_status = RemoveStartupPersistence(entry_name);
    if (cleanup_status != ERROR_SUCCESS) {
        wprintf(L"[-] Falha ao remover entrada de persistencia: erro 0x%08lX\n", cleanup_status);
        return (int)cleanup_status;
    }

    wprintf(L"[+] Limpeza executada com sucesso.\n");
    return 0;
}
