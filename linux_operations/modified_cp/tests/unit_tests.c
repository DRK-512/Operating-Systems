#include "unity.h"
#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


// Each test gets its own temporary directory so that
// filesystem tests do not interfere with each other.

static char test_dir[] = "/tmp/filecopy_test_XXXXXX";

void setUp(void) {
        // No setup needed
}

void tearDown(void) {
        // No teardown needed
}

// Helper to check whether a file exists.
static int test_file_exists(const char *path) {
        struct stat info;
        return stat(path, &info) == 0;
}

// Helper to read a file and compare its contents.
static void assert_file_contents(const char *path, const char *expected) {
        FILE *file = fopen(path, "rb");
        TEST_ASSERT_NOT_NULL(file);

        char buffer[1024];

        size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file);
        buffer[bytes_read] = '\0';

        TEST_ASSERT_EQUAL_STRING(expected, buffer);

        fclose(file);
}

// Test that copy_files() creates the requested number
// of copies with the correct contents.
void test_copy_files_creates_multiple_copies(void) {
        char source[256];
        char destination[256];
        char copy1[256];
        char copy2[256];
        char copy3[256];

        snprintf(source, sizeof(source), "%s/test.txt", test_dir);
        snprintf(destination, sizeof(destination), "%s", test_dir);
        snprintf(copy1, sizeof(copy1), "%s/test1.txt", test_dir);
        snprintf(copy2, sizeof(copy2), "%s/test2.txt", test_dir);
        snprintf(copy3, sizeof(copy3), "%s/test3.txt", test_dir);

        FILE *file = fopen(source, "w");
        TEST_ASSERT_NOT_NULL(file);

        fputs("Hello Unity!", file);
        fclose(file);

        TEST_ASSERT_EQUAL_INT(0, copy_files(source, destination, 3, 0));

        TEST_ASSERT_TRUE(test_file_exists(copy1));
        TEST_ASSERT_TRUE(test_file_exists(copy2));
        TEST_ASSERT_TRUE(test_file_exists(copy3));

        assert_file_contents(copy1, "Hello Unity!");
        assert_file_contents(copy2, "Hello Unity!");
        assert_file_contents(copy3, "Hello Unity!");

        remove(source);
        remove(copy1);
        remove(copy2);
        remove(copy3);
}

// Regression test for the .bashrc case.
// So a leading '.' should be treated as part of the filename,
// not as the extension separator.
void test_copy_files_handles_hidden_files(void) {
        char source[256];
        char copy1[256];
        char copy2[256];

        snprintf(source, sizeof(source), "%s/.bashrc", test_dir);
        snprintf(copy1, sizeof(copy1), "%s/.bashrc1", test_dir);
        snprintf(copy2, sizeof(copy2), "%s/.bashrc2", test_dir);

        FILE *file = fopen(source, "w");
        TEST_ASSERT_NOT_NULL(file);

        fputs("hidden file", file);
        fclose(file);

        TEST_ASSERT_EQUAL_INT(0, copy_files(source, test_dir, 2, 0));

        TEST_ASSERT_TRUE(test_file_exists(copy1));
        TEST_ASSERT_TRUE(test_file_exists(copy2));

        TEST_ASSERT_FALSE(test_file_exists(
                test_dir + 1)); /* Prevent accidental 1.bashrc assumption */

        assert_file_contents(copy1, "hidden file");
        assert_file_contents(copy2, "hidden file");

        remove(source);
        remove(copy1);
        remove(copy2);
}

// Test that copy_files() refuses to overwrite an existing file.
void test_copy_files_does_not_overwrite_existing_file(void) {
        char source[256];
        char destination[256];
        char existing_copy[256];

        snprintf(source, sizeof(source), "%s/source.txt", test_dir);
        snprintf(destination, sizeof(destination), "%s", test_dir);
        snprintf(existing_copy, sizeof(existing_copy), "%s/source1.txt", test_dir);

        FILE *file = fopen(source, "w");
        TEST_ASSERT_NOT_NULL(file);
        fputs("original", file);
        fclose(file);

        file = fopen(existing_copy, "w");
        TEST_ASSERT_NOT_NULL(file);
        fputs("do not overwrite", file);
        fclose(file);

        TEST_ASSERT_NOT_EQUAL(0, copy_files(source, destination, 1, 0));

        /* Make sure the existing file was not changed. */
        assert_file_contents(existing_copy, "do not overwrite");

        remove(source);
        remove(existing_copy);
}

// Test copying a file with a normal extension.
void test_copy_files_preserves_extension(void) {
        char source[256];
        char copy1[256];
        char copy2[256];

        snprintf(source, sizeof(source), "%s/document.txt", test_dir);
        snprintf(copy1, sizeof(copy1), "%s/document1.txt", test_dir);
        snprintf(copy2, sizeof(copy2), "%s/document2.txt", test_dir);

        FILE *file = fopen(source, "w");
        TEST_ASSERT_NOT_NULL(file);

        fputs("document contents", file);
        fclose(file);

        TEST_ASSERT_EQUAL_INT(0, copy_files(source, test_dir, 2, 0));

        TEST_ASSERT_TRUE(test_file_exists(copy1));
        TEST_ASSERT_TRUE(test_file_exists(copy2));

        assert_file_contents(copy1, "document contents");
        assert_file_contents(copy2, "document contents");

        remove(source);
        remove(copy1);
        remove(copy2);
}

// Test that an invalid copy count is rejected by parse_args().
void test_parse_args_rejects_invalid_number(void) {
        char *argv[] = {
                "filecopy",
                "-s",
                "test.txt",
                "-n",
                "0",
                NULL
        };

        int argc = 5;
        int nCopies = 1;
        int verbose = 0;
        char *srcPath = NULL;
        char *destPath = NULL;

        TEST_ASSERT_NOT_EQUAL(
                0,
                parse_args(argc, argv,
                           &nCopies,
                           &verbose,
                           &srcPath,
                           &destPath));

        free(srcPath);
        free(destPath);
}

// Test that parse_args() accepts the default number of copies.
void test_parse_args_accepts_source(void) {
        char source[256];

        snprintf(source, sizeof(source), "%s/input.txt", test_dir);

        FILE *file = fopen(source, "w");
        TEST_ASSERT_NOT_NULL(file);
        fputs("test", file);
        fclose(file);

        char *argv[] = {
                "filecopy",
                "-s",
                source,
                NULL
        };

        int argc = 3;
        int nCopies = 1;
        int verbose = 0;
        char *srcPath = NULL;
        char *destPath = NULL;

        TEST_ASSERT_EQUAL_INT(
                0,
                parse_args(argc, argv,
                           &nCopies,
                           &verbose,
                           &srcPath,
                           &destPath));

        TEST_ASSERT_EQUAL_INT(1, nCopies);
        TEST_ASSERT_EQUAL_INT(0, verbose);
        TEST_ASSERT_NOT_NULL(srcPath);
        TEST_ASSERT_NOT_NULL(destPath);

        free(srcPath);
        free(destPath);
        remove(source);
}

// Unity test runner.
int main(void) {
        UNITY_BEGIN();

        RUN_TEST(test_copy_files_creates_multiple_copies);
        RUN_TEST(test_copy_files_handles_hidden_files);
        RUN_TEST(test_copy_files_does_not_overwrite_existing_file);
        RUN_TEST(test_copy_files_preserves_extension);
        RUN_TEST(test_parse_args_rejects_invalid_number);
        RUN_TEST(test_parse_args_accepts_source);

        return UNITY_END();
}