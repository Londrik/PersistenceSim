#include "persistence.h"

_Check_return_
LSTATUS SetStartupPersistence(
    _In_z_ LPCWSTR value_name,
    _In_z_ LPCWSTR exe_path
) {
    if (value_name == NULL || exe_path == NULL) {
        return ERROR_INVALID_PARAMETER;
    }

    HKEY hKey = NULL;
    LSTATUS status = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        PERSISTENCE_RUN_KEY,
        0,
        KEY_SET_VALUE,
        &hKey
    );

    if (status != ERROR_SUCCESS) {
        return status;
    }

    DWORD data_size = (DWORD)((wcslen(exe_path) + 1) * sizeof(wchar_t));

    status = RegSetValueExW(
        hKey,
        value_name,
        0,
        REG_SZ,
        (const BYTE*)exe_path,
        data_size
    );

    RegCloseKey(hKey);
    return status;
}

_Check_return_
LSTATUS RemoveStartupPersistence(
    _In_z_ LPCWSTR value_name
) {
    if (value_name == NULL) {
        return ERROR_INVALID_PARAMETER;
    }

    HKEY hKey = NULL;
    LSTATUS status = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        PERSISTENCE_RUN_KEY,
        0,
        KEY_SET_VALUE,
        &hKey
    );

    if (status != ERROR_SUCCESS) {
        return status;
    }

    status = RegDeleteValueW(hKey, value_name);

    RegCloseKey(hKey);
    return status;
}
