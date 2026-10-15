/**
 * @file persistence.h
 * @brief Win32 Registry Startup Persistence Interface.
 * @details Declara rotinas e macros para manipulação de entradas de persistência
 *          no Registro do Windows (chave Run do HKCU).
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
    /* Fallbacks para compatibilidade com GCC/Clang (MinGW-w64) */
    #ifndef _Check_return_
        #define _Check_return_
    #endif
    #ifndef _In_z_
        #define _In_z_
    #endif
#endif

/**
 * @def PERSISTENCE_RUN_KEY
 * @brief Caminho da chave do Registro para inicialização automática no escopo HKCU.
 */
#define PERSISTENCE_RUN_KEY L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"

/**
 * @brief Adiciona ou atualiza uma entrada de inicialização automática no Registro.
 *
 * @details Abre a chave @ref PERSISTENCE_RUN_KEY em HKCU com KEY_SET_VALUE e
 *          grava uma string REG_SZ apontando para o caminho do executável.
 *
 * @param[in] value_name Nome identificador único da chave no Registro (não nulo, null-terminated).
 * @param[in] exe_path   Caminho completo e qualificado do executável (não nulo, null-terminated).
 *
 * @return LSTATUS Código de status da API Win32 (LSTATUS / LONG).
 * @retval ERROR_SUCCESS           Entrada configurada com sucesso.
 * @retval ERROR_INVALID_PARAMETER Pelo menos um dos ponteiros fornecidos é nulo.
 * @retval Códigos de erro Win32   Retornados diretamente por RegOpenKeyExW ou RegSetValueExW.
 */
_Check_return_
LSTATUS SetStartupPersistence(
    _In_z_ LPCWSTR value_name,
    _In_z_ LPCWSTR exe_path
);

/**
 * @brief Remove a entrada de persistência configurada no Registro.
 *
 * @details Abre a chave @ref PERSISTENCE_RUN_KEY em HKCU com KEY_SET_VALUE e
 *          invoca RegDeleteValueW para desativar a persistência.
 *
 * @param[in] value_name Nome identificador da entrada a ser deletada (não nulo, null-terminated).
 *
 * @return LSTATUS Código de status da API Win32 (LSTATUS / LONG).
 * @retval ERROR_SUCCESS           Entrada removida com sucesso.
 * @retval ERROR_INVALID_PARAMETER O ponteiro `value_name` fornecido é nulo.
 * @retval Códigos de erro Win32   Retornados diretamente por RegOpenKeyExW ou RegDeleteValueW.
 */
_Check_return_
LSTATUS RemoveStartupPersistence(
    _In_z_ LPCWSTR value_name
);

#ifdef __cplusplus
}
#endif

#endif /* PERSISTENCE_H */
