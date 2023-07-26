
/**
 * main.c
 */
#include <F28x_Project.h>
#include <string.h>
#include <stdint.h>
#define FLASH_RUN 1
#define SRAM_RUN 2
#define RUN_TYPE SRAM_RUN
#if RUN_TYPE==FLASH_RUN
    extern Uint16 RamfuncsLoadStart;
    extern Uint16 RamfuncsLoadSize;
    extern Uint16 RamfuncsRunStart;
#endif


int main(void)
{
    // 初始化系统控制
    InitSysCtrl();
    return 0;
}
