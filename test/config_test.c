#include "config_test.h"
#include "config.h"
#include <stdio.h>
#include <string.h>

static const char CONFIG_FILE[] = "/data/local/tmp/config_test.conf";
static const char CONFIG_FILE_CONTENT[] =
"                                                                                    \n\
first_key=value                                                                      \n\
                                                                                     \n\
# Device ID (DID) configuration                                                      \n\
[DID]                                                                                \n\
                                                                                     \n\
# Record Number: 1, 2 or 3 - maximum of 3 records                                    \n\
recordNumber = 1                                                                     \n\
                                                                                     \n\
# Primary Record - true or false (default)                                           \n\
# There can be only one primary record                                               \n\
primaryRecord = true                                                                 \n\
                                                                                     \n\
# Vendor ID '0xFFFF' indicates no Device ID Service Record is present in the device  \n\
# 0x000F = Broadcom Corporation (default)                                            \n\
#vendorId = 0x000F                                                                   \n\
                                                                                     \n\
# Vendor ID Source                                                                   \n\
# 0x0001 = Bluetooth SIG assigned Device ID Vendor ID value (default)                \n\
# 0x0002 = USB Implementer's Forum assigned Device ID Vendor ID value                \n\
#vendorIdSource = 0x0001                                                             \n\
                                                                                     \n\
# Product ID & Product Version                                                       \n\
# Per spec DID v1.3 0xJJMN for version is interpreted as JJ.M.N                      \n\
# JJ: major version number, M: minor version number, N: sub-minor version number     \n\
# For example: 1200, v14.3.6                                                         \n\
productId = 0x1200                                                                   \n\
version = 0x1111                                                                     \n\
                                                                                     \n\
# Optional attributes                                                                \n\
#clientExecutableURL =                                                               \n\
#serviceDescription =                                                                \n\
#documentationURL =                                                                  \n\
                                                                                     \n\
# Additional optional DID records. Bluedroid supports up to 3 records.               \n\
[DID]                                                                                \n\
[DID]                                                                                \n\
version = 0x1436                                                                     \n\
";

static void config_test_setup_file(void) {
    FILE *fp = fopen(CONFIG_FILE, "wt");
    fwrite(CONFIG_FILE_CONTENT, 1, sizeof(CONFIG_FILE_CONTENT), fp);
    fclose(fp);
}

int test_config_new_empty(void) {
    config_t *config = config_new_empty();
    if (!config) return 0;
    config_free(config);
    return 1;
}

int test_config_new_no_file(void) {
    config_t *config = config_new("/meow");
    if (config) { config_free(config); return 0; }
    config_free(config);
    return 1;
}

int test_config_new(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    config_free(config);
    return 1;
}

int test_config_free_null(void) {
    config_free(NULL);
    return 1;
}

int test_config_has_section(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    if (!config_has_section(config, "DID")) { config_free(config); return 0; }
    config_free(config);
    return 1;
}

int test_config_has_key_in_default_section(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    if (!config_has_key(config, CONFIG_DEFAULT_SECTION, "first_key")) { config_free(config); return 0; }
    if (strcmp(config_get_string(config, CONFIG_DEFAULT_SECTION, "first_key", "meow"), "value") != 0) { config_free(config); return 0; }
    config_free(config);
    return 1;
}

int test_config_has_keys(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    if (!config_has_key(config, "DID", "recordNumber")) { config_free(config); return 0; }
    if (!config_has_key(config, "DID", "primaryRecord")) { config_free(config); return 0; }
    if (!config_has_key(config, "DID", "productId")) { config_free(config); return 0; }
    if (!config_has_key(config, "DID", "version")) { config_free(config); return 0; }
    config_free(config);
    return 1;
}

int test_config_no_bad_keys(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    if (config_has_key(config, "DID_BAD", "primaryRecord")) { config_free(config); return 0; }
    if (config_has_key(config, "DID", "primaryRecord_BAD")) { config_free(config); return 0; }
    if (config_has_key(config, CONFIG_DEFAULT_SECTION, "primaryRecord")) { config_free(config); return 0; }
    config_free(config);
    return 1;
}

int test_config_get_int_version(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    if (config_get_int(config, "DID", "version", 0) != 0x1436) { config_free(config); return 0; }
    config_free(config);
    return 1;
}

int test_config_get_int_default(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    if (config_get_int(config, "DID", "primaryRecord", 123) != 123) { config_free(config); return 0; }
    config_free(config);
    return 1;
}

int test_config_remove_section(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    if (!config_remove_section(config, "DID")) { config_free(config); return 0; }
    if (config_has_section(config, "DID")) { config_free(config); return 0; }
    if (config_has_key(config, "DID", "productId")) { config_free(config); return 0; }
    config_free(config);
    return 1;
}

int test_config_remove_section_missing(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    if (config_remove_section(config, "not a section")) { config_free(config); return 0; }
    config_free(config);
    return 1;
}

int test_config_remove_key(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    if (config_get_int(config, "DID", "productId", 999) != 0x1200) { config_free(config); return 0; }
    if (!config_remove_key(config, "DID", "productId")) { config_free(config); return 0; }
    if (config_has_key(config, "DID", "productId")) { config_free(config); return 0; }
    config_free(config);
    return 1;
}

int test_config_remove_key_missing(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    if (config_get_int(config, "DID", "productId", 999) != 0x1200) { config_free(config); return 0; }
    if (!config_remove_key(config, "DID", "productId")) { config_free(config); return 0; }
    if (config_get_int(config, "DID", "productId", 999) != 999) { config_free(config); return 0; }
    config_free(config);
    return 1;
}

int test_config_section_begin(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    const config_section_node_t *section = config_section_begin(config);
    if (!section) { config_free(config); return 0; }
    const char *section_name = config_section_name(section);
    if (!section_name) { config_free(config); return 0; }
    if (strcmp(section_name, CONFIG_DEFAULT_SECTION) != 0) { config_free(config); return 0; }
    config_free(config);
    return 1;
}

int test_config_section_next(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    const config_section_node_t *section = config_section_begin(config);
    if (!section) { config_free(config); return 0; }
    section = config_section_next(section);
    if (!section) { config_free(config); return 0; }
    const char *section_name = config_section_name(section);
    if (!section_name) { config_free(config); return 0; }
    if (strcmp(section_name, "DID") != 0) { config_free(config); return 0; }
    config_free(config);
    return 1;
}

int test_config_section_end(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    const config_section_node_t * section = config_section_begin(config);
    section = config_section_next(section);
    section = config_section_next(section);
    if (section != config_section_end(config)) { config_free(config); return 0; }
    config_free(config);
    return 1;
}

int test_config_save_basic(void) {
    config_test_setup_file();
    config_t *config = config_new(CONFIG_FILE);
    if (!config) return 0;
    if (!config_save(config, CONFIG_FILE)) { config_free(config); return 0; }
    config_free(config);
    return 1;
} 