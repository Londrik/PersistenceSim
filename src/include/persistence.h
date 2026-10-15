/**
 * @file persistence.h
 * @brief Win32 Startup Persistence and System Helpers Interface.
 * @details Declara rotinas seguras para resolução de caminhos, validação de diretórios
 *          e manipulação do Registro do Windows (HKCU Run).
 */

#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <stdbool.h>

#if defined(_MSC_VER)
    #include <sal.h>
#else
    #ifndef _Check_return_
        #define _Check_return_
    #endif
    #ifndef _In_z_
        #define _In_z_
    #endif
    #ifndef _Out_writes_z_
        #define _Out_writes_z_(size)
    #endif
    #ifndef _In_range_
        #define _In_range_(low, high)
    #endif
#endif

/**
 * @def PERSISTENCE_RUN_KEY
 * @brief Subchave do Registro para inicialização automática no escopo HKCU.
 */
#define PERSISTENCE_RUN_KEY L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"

/**
 * @brief Obtém o caminho absoluto do executável atual via GetModuleFileNameW.
 *
 * @param[out] buffer Buffer receptor de caracteres largos (wchar_t).
 * @param[in]  size   Capacidade do buffer em número de caracteres.
 *
 * @return DWORD Código de erro do sistema (ERROR_SUCCESS em caso de sucesso).
 * @retval ERROR_SUCCESS             Caminho obtido e terminado em nulo.
 * @retval ERROR_INVALID_PARAMETER   Buffer nulo ou tamanho zero.
 * @retval ERROR_INSUFFICIENT_BUFFER Tamanho insuficiente para o caminho completo.
 */
_Check_return_
DWORD GetCurrentBinaryPath(
    _Out_writes_z_(size) LPWSTR buffer,
    _In_range_(1, MAX_PATH * 2) DWORD size
);

/**
 * @brief Verifica se um diretório existe e o cria caso ausente.
 *
 * @param[in] dir_path Caminho do diretório a verificar/criar.
 *
 * @return DWORD Código de erro do sistema (ERROR_SUCCESS em caso de sucesso).
 * @retval ERROR_SUCCESS           Diretório já existia ou foi criado com sucesso.
 * @retval ERROR_INVALID_PARAMETER dir_path é nulo ou string vazia.
 * @retval Códigos Win32           Falhas retornadas por GetLastError().
 */
_Check_return_
DWORD VerifyAndCreateDirectory(
    _In_z_ LPCWSTR dir_path
);

/**
 * @brief Grava a entrada de persistência na chave Run de HKCU.
 *
 * @details Abre a chave com KEY_SET_VALUE | KEY_QUERY_VALUE e grava
 *          uma string REG_SZ garantindo fechamento do handle via goto cleanup.
 *
 * @param[in] value_name Nome identificador da entrada no Registro.
 * @param[in] exe_path   Caminho completo do binário.
 *
 * @return LSTATUS Código Win32 retornado por RegOpenKeyExW ou RegSetValueExW.
 * @retval ERROR_SUCCESS           Entrada gravada com sucesso.
 * @retval ERROR_INVALID_PARAMETER value_name ou exe_path nulos.
 */
_Check_return_
LSTATUS SetUserRunPersistence(
    _In_z_ LPCWSTR value_name,
    _In_z_ LPCWSTR exe_path
);

/**
 * @brief Remove a entrada de persistência configurada no Registro.
 *
 * @param[in] value_name Nome identificador da entrada a ser deletada.
 *
 * @return LSTATUS Código Win32 retornado por RegOpenKeyExW ou RegDeleteValueW.
 * @retval ERROR_SUCCESS           Entrada removida com sucesso.
 * @retval ERROR_INVALID_PARAMETER value_name é nulo.
 */
_Check_return_
LSTATUS RemoveStartupPersistence(
    _In_z_ LPCWSTR value_name
);

#ifdef __cplusplus
}
#endif

#endif /* PERSISTENCE_H */
