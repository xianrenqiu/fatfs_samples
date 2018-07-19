#include "ff.h"
#include "diskio.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define DIR_NAME        "/abc"
#define FILE_NAME       "/abc/file1.txt"
#define FILE_VAULE      "1234567"

#define fatfs_assert(cond)                                            \
    do                                                                \
    {                                                                 \
        if (cond != RES_OK)                                           \
        {                                                             \
            printf("assert: '" #cond "' [%d, %d]\n", __LINE__, cond); \
            return -1;                                                \
        }                                                             \
    } while (0)

#if (USE_RAM == 0)
void creat_disk(void)
{
    char buf[SEC_SIZE] = {0, };
    int f = open(DISK_NAME, O_CREAT | O_SYNC | O_RDWR, S_IRUSR | S_IWUSR);
    for (int i = 0; i < SEC_COUNT; i++) {
        write(f, buf, SEC_SIZE);
    }
    close(f);
}
#endif

int main(void)
{
    printf("\n  fatfs sample use %s.\n\n", USE_RAM ? "RAM" : "Disk");

#if (USE_RAM == 0)
    creat_disk();
#endif

    /** mkfs */
    uint8_t work[FF_MAX_SS];
    fatfs_assert(f_mkfs("", FM_SFD | FM_FAT, 0, work, FF_MAX_SS));
    printf("  0. creat fatfs system success\n\n");

    /** mount */
    FATFS fs;
    fatfs_assert(f_mount(&fs, "", 1));

    /** make dir */
    fatfs_assert(f_mkdir(DIR_NAME));
    printf("  1. mkdir success, dir name: %s\n\n", DIR_NAME);

    /** open file */
    int n;
    FIL f;
    fatfs_assert(f_open(&f, FILE_NAME, FA_CREATE_ALWAYS|FA_READ|FA_WRITE));
    printf("  2. open %s file success\n", FILE_NAME);
    fatfs_assert(f_write(&f, FILE_VAULE, strlen(FILE_VAULE), &n));
    printf("  3. write %s to %s file success\n\n", FILE_VAULE, FILE_NAME);
    fatfs_assert(f_sync(&f));
    fatfs_assert(f_close(&f));
    
    /** read dir */
    DIR d;
    FILINFO fn;
    fatfs_assert(f_opendir(&d, DIR_NAME));
    printf("  4. dir name: %s\n", d.fn);
    for (;;) {
        if (f_readdir(&d, &fn) != FR_OK || fn.fname[0] == '\0') 
            break;
        printf("  5. file name: %s\n\n", fn.fname);
    }
    fatfs_assert(f_closedir(&d));

    /** read file */
    fatfs_assert(f_open(&f, FILE_NAME, FA_OPEN_EXISTING|FA_READ));
    printf("  6. open %s file success\n", FILE_NAME);
    fatfs_assert(f_read(&f, work, 9, &n));
    printf("  7. read %s from %s file success\n\n", work, FILE_NAME);
    fatfs_assert(f_close(&f));

    return 0;
}