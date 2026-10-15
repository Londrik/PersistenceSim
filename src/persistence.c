#include "persistence.h"

_Check_return_
DWORD GetCurrentBinaryPath(
    _Out_writes_z_(size) LPWSTR buffer,
    _In_range_(1, MAX_PATH * 2) DWORD size
) {
    if (buffer == NULL || size == 0) {
        return ERROR_INVALID_PARAMETER;
    }

    SetLastError(ERROR_SUCCESS);
    DWORD length = GetModuleFileNameW(NULL, buffer, size);

    if (length == 0) {
        DWORD err = GetLastError();
        return (err != ERROR_SUCCESS) ? err : ERROR_CAN_NOT_COMPLETE;
    }

    /*
     * Se length == size, o Windows trunca o caminho ou não adiciona o terminador nulo.
     * Além disso, GetLastError() retorna ERROR_INSUFFICIENT_BUFFER em versões modernas.
     */
    if (length >= size || GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        buffer[size - 1] = L'\0';
        return ERROR_INSUFFICIENT_BUFFER;
    }

    buffer[length] = L'\0';
    return ERROR_SUCCESS;
}

_Check_return_
DWORD VerifyAndCreateDirectory(
    _In_z_ LPCWSTR dir_path
) {
    if (dir_path == NULL || dir_path[0] == L'\0') {
        return ERROR_INVALID_PARAMETER;
    }

    DWORD attributes = GetFileAttributesW(dir_path);

    if (attributes != INVALID_FILE_ATTRIBUTES) {
        if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
            return ERROR_SUCCESS;
        }
        /* O caminho existe, mas é um arquivo comum */
        return ERROR_DIRECTORY;
    }

    DWORD last_error = GetLastError();
    if (last_error != ERROR_FILE_NOT_FOUND && last_error != ERROR_PATH_NOT_FOUND) {
        return last_error;
    }

    if (!CreateDirectoryW(dir_path, NULL)) {
        return GetLastError();
    }

    return ERROR_SUCCESS;
}

_Check_return_
LSTATUS SetUserRunPersistence(
    _In_z_ LPCWSTR value_name,
    _In_z_ LPCWSTR exe_path
) {
    HKEY hKey = NULL;
    LSTATUS status = ERROR_SUCCESS;

    if (value_name == NULL || exe_path == NULL) {
        return ERROR_INVALID_PARAMETER;
    }

    /* Abertura exigindo KEY_SET_VALUE | KEY_QUERY_VALUE conforme requisito */
    status = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        PERSISTENCE_RUN_KEY,
        0,
        KEY_SET_VALUE | KEY_QUERY_VALUE,
        &hKey
    );

    if (status != ERROR_SUCCESS) {
        goto cleanup;
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

cleanup:
    if (hKey != NULL) {
        RegCloseKey(hKey);
        hKey = NULL;
    }

    return status;
}

_Check_return_
LSTATUS RemoveStartupPersistence(
    _In_z_ LPCWSTR value_name
) {
    HKEY hKey = NULL;
    LSTATUS status = ERROR_SUCCESS;

    if (value_name == NULL) {
        return ERROR_INVALID_PARAMETER;
    }

    status = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        PERSISTENCE_RUN_KEY,
        0,
        KEY_SET_VALUE,
        &hKey
    );

    if (status != ERROR_SUCCESS) {
        goto cleanup;
    }

    status = RegDeleteValueW(hKey, value_name);

cleanup:
    if (hKey != NULL) {
        RegCloseKey(hKey);
        hKey = NULL;
    }

    return status;
}
