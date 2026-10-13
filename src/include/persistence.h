#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <windows.h>
#include <sal.h>
#include <stdbool.h>

/**
 * @brief Chave de persistência de inicialização do usuário (Run).
 */
#define PERSISTENCE_RUN_KEY L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"

/**
 * @brief Escreve um valor de registro para persistência de inicialização.
 *
 * @param[in] value_name Nome identificador da entrada no Registro.
 * @param[in] exe_path   Caminho completo do binário a ser executado.
 *
 * @return LSTATUS Código Win32 retornado por RegSetValueExW (ERROR_SUCCESS em caso de sucesso).
 */
_Check_return_
LSTATUS SetStartupPersistence(
    _In_z_ LPCWSTR value_name,
    _In_z_ LPCWSTR exe_path
);

/**
 * @brief Remove a chave de inicialização configurada no Registro.
 *
 * @param[in] value_name Nome identificador da entrada no Registro.
 *
 * @return LSTATUS Código Win32 retornado por RegDeleteValueW (ERROR_SUCCESS em caso de sucesso).
 */
_Check_return_
LSTATUS RemoveStartupPersistence(
    _In_z_ LPCWSTR value_name
);

#endif /* PERSISTENCE_H */
