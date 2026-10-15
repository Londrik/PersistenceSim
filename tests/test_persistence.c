#include "unity.h"
#include "persistence.h"

void setUp(void) {}
void tearDown(void) {}

void test_GetCurrentBinaryPath_null_or_zero_buffer_should_fail(void) {
    wchar_t buf[MAX_PATH];
    TEST_ASSERT_EQUAL_HEX32(ERROR_INVALID_PARAMETER, GetCurrentBinaryPath(NULL, MAX_PATH));
    TEST_ASSERT_EQUAL_HEX32(ERROR_INVALID_PARAMETER, GetCurrentBinaryPath(buf, 0));
}

void test_GetCurrentBinaryPath_insufficient_buffer(void) {
    wchar_t tiny_buf[4];
    DWORD status = GetCurrentBinaryPath(tiny_buf, 4);
    TEST_ASSERT_EQUAL_HEX32(ERROR_INSUFFICIENT_BUFFER, status);
}

void test_GetCurrentBinaryPath_success(void) {
    wchar_t full_path[MAX_PATH * 2];
    DWORD status = GetCurrentBinaryPath(full_path, MAX_PATH * 2);
    TEST_ASSERT_EQUAL_HEX32(ERROR_SUCCESS, status);
    TEST_ASSERT_TRUE(wcslen(full_path) > 0);
}

void test_VerifyAndCreateDirectory_invalid_param(void) {
    TEST_ASSERT_EQUAL_HEX32(ERROR_INVALID_PARAMETER, VerifyAndCreateDirectory(NULL));
    TEST_ASSERT_EQUAL_HEX32(ERROR_INVALID_PARAMETER, VerifyAndCreateDirectory(L""));
}

void test_VerifyAndCreateDirectory_lifecycle(void) {
    const wchar_t *test_dir = L"C:\\windows\\temp\\persist_sim_test_dir";
    DWORD status = VerifyAndCreateDirectory(test_dir);
    TEST_ASSERT_EQUAL_HEX32(ERROR_SUCCESS, status);

    /* Idempotência: rodar novamente em diretório existente deve retornar sucesso */
    status = VerifyAndCreateDirectory(test_dir);
    TEST_ASSERT_EQUAL_HEX32(ERROR_SUCCESS, status);

    RemoveDirectoryW(test_dir);
}

void test_persistence_null_parameters_should_return_invalid_parameter(void) {
    LSTATUS status = SetUserRunPersistence(NULL, L"C:\\dummy.exe");
    TEST_ASSERT_EQUAL_HEX32(ERROR_INVALID_PARAMETER, status);

    status = SetUserRunPersistence(L"DummyApp", NULL);
    TEST_ASSERT_EQUAL_HEX32(ERROR_INVALID_PARAMETER, status);

    status = RemoveStartupPersistence(NULL);
    TEST_ASSERT_EQUAL_HEX32(ERROR_INVALID_PARAMETER, status);
}

void test_persistence_full_lifecycle(void) {
    const wchar_t *test_key = L"UnityTask3Entry";
    const wchar_t *test_path = L"C:\\Windows\\System32\\notepad.exe";

    LSTATUS status_set = SetUserRunPersistence(test_key, test_path);
    TEST_ASSERT_EQUAL_HEX32(ERROR_SUCCESS, status_set);

    LSTATUS status_remove = RemoveStartupPersistence(test_key);
    TEST_ASSERT_EQUAL_HEX32(ERROR_SUCCESS, status_remove);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_GetCurrentBinaryPath_null_or_zero_buffer_should_fail);
    RUN_TEST(test_GetCurrentBinaryPath_insufficient_buffer);
    RUN_TEST(test_GetCurrentBinaryPath_success);
    RUN_TEST(test_VerifyAndCreateDirectory_invalid_param);
    RUN_TEST(test_VerifyAndCreateDirectory_lifecycle);
    RUN_TEST(test_persistence_null_parameters_should_return_invalid_parameter);
    RUN_TEST(test_persistence_full_lifecycle);
    return UNITY_END();
}
