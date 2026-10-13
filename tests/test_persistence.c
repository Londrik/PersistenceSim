#include "unity.h"
#include "persistence.h"

void setUp(void) {
    /* Setup antes de cada teste */
}

void tearDown(void) {
    /* Cleanup apos cada teste */
}

void test_persistence_null_parameters_should_return_invalid_parameter(void) {
    LSTATUS status = SetStartupPersistence(NULL, L"C:\\dummy.exe");
    TEST_ASSERT_EQUAL_HEX32(ERROR_INVALID_PARAMETER, status);

    status = SetStartupPersistence(L"DummyApp", NULL);
    TEST_ASSERT_EQUAL_HEX32(ERROR_INVALID_PARAMETER, status);

    status = RemoveStartupPersistence(NULL);
    TEST_ASSERT_EQUAL_HEX32(ERROR_INVALID_PARAMETER, status);
}

void test_persistence_full_lifecycle(void) {
    const wchar_t *test_key = L"UnityTestEntry";
    const wchar_t *test_path = L"C:\\Windows\\System32\\calc.exe";

    LSTATUS status_set = SetStartupPersistence(test_key, test_path);
    TEST_ASSERT_EQUAL_HEX32(ERROR_SUCCESS, status_set);

    LSTATUS status_remove = RemoveStartupPersistence(test_key);
    TEST_ASSERT_EQUAL_HEX32(ERROR_SUCCESS, status_remove);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_persistence_null_parameters_should_return_invalid_parameter);
    RUN_TEST(test_persistence_full_lifecycle);
    return UNITY_END();
}
