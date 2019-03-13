/*
 * payload_generator.h
 *
 *  Created on:  Apr 29, 2014
 *      Author:  Bojan Gruevski
 *      Company: SEAVUS Group
 */

#ifndef PAYLOAD_GENERATOR_H_
#define PAYLOAD_GENERATOR_H_

/**    Table for start address of sectors
 *
 *        LPC1769 internal flash memory sector numbers and addresses
 *
 *        LPC1769 flash memory addresses and sector number/size
 *        Table 568 "Sectors in a LPC17xx device", Section 5. "Sector numbers", User Manual
 *
 *        0x00000000 - 0x0007FFFF        flash (29 sectors)
 *
 *      Sector0:     0x00000000 - 0x00000FFF        4K
 *      Sector1:     0x00001000 - 0x00001FFF        4K
 *      Sector2:     0x00002000 - 0x00002FFF        4K
 *      Sector3:     0x00003000 - 0x00003FFF        4K
 *      Sector4:     0x00004000 - 0x00004FFF        4K
 *      Sector5:     0x00005000 - 0x00005FFF        4K
 *      Sector6:     0x00006000 - 0x00006FFF        4K
 *      Sector7:     0x00007000 - 0x00007FFF        4K
 *      Sector8:     0x00008000 - 0x00008FFF        4K
 *      Sector9:     0x00009000 - 0x00009FFF        4K
 *      Sector10:    0x0000A000 - 0x0000AFFF        4K
 *      Sector11:    0x0000B000 - 0x0000BFFF        4K
 *      Sector12:    0x0000C000 - 0x0000CFFF        4K
 *      Sector13:    0x0000D000 - 0x0000DFFF        4K
 *      Sector14:    0x0000E000 - 0x0000EFFF        4K
 *      Sector15:    0x0000F000 - 0x0000FFFF        4K
 *
 *      Sector16:    0x00010000 - 0x00017FFF        32K
 *      Sector17:    0x00018000 - 0x0001FFFF        32K
 *      Sector18:    0x00020000 - 0x00027FFF        32K
 *      Sector19:    0x00028000 - 0x0002FFFF        32K
 *      Sector20:    0x00030000 - 0x00037FFF        32K
 *      Sector21:    0x00038000 - 0x0003FFFF        32K
 *      Sector22:    0x00040000 - 0x00047FFF        32K
 *      Sector23:    0x00048000 - 0x0004FFFF        32K
 *      Sector24:    0x00050000 - 0x00057FFF        32K
 *      Sector25:    0x00058000 - 0x0005FFFF        32K
 *      Sector26:    0x00060000 - 0x00067FFF        32K
 *      Sector27:    0x00068000 - 0x0006FFFF        32K
 *      Sector28:    0x00070000 - 0x00077FFF        32K
 *      Sector29:    0x00078000 - 0x0007FFFF        32K
 */

/* 4K Size sectors */
#define     FLASH_SECTOR_0_ADDRESS      0x00000000
#define     FLASH_SECTOR_1_ADDRESS      0x00001000
#define     FLASH_SECTOR_2_ADDRESS      0x00002000
#define     FLASH_SECTOR_3_ADDRESS      0x00003000
#define     FLASH_SECTOR_4_ADDRESS      0x00004000
#define     FLASH_SECTOR_5_ADDRESS      0x00005000
#define     FLASH_SECTOR_6_ADDRESS      0x00006000
#define     FLASH_SECTOR_7_ADDRESS      0x00007000
#define     FLASH_SECTOR_8_ADDRESS      0x00008000
#define     FLASH_SECTOR_9_ADDRESS      0x00009000
#define     FLASH_SECTOR_10_ADDRESS     0x0000A000
#define     FLASH_SECTOR_11_ADDRESS     0x0000B000
#define     FLASH_SECTOR_12_ADDRESS     0x0000C000
#define     FLASH_SECTOR_13_ADDRESS     0x0000D000
#define     FLASH_SECTOR_14_ADDRESS     0x0000E000
#define     FLASH_SECTOR_15_ADDRESS     0x0000F000

/* 32K Size sectors */
#define     FLASH_SECTOR_16_ADDRESS     0x00010000
#define     FLASH_SECTOR_17_ADDRESS     0x00018000
#define     FLASH_SECTOR_18_ADDRESS     0x00020000
#define     FLASH_SECTOR_19_ADDRESS     0x00028000
#define     FLASH_SECTOR_20_ADDRESS     0x00030000
#define     FLASH_SECTOR_21_ADDRESS     0x00038000
#define     FLASH_SECTOR_22_ADDRESS     0x00040000
#define     FLASH_SECTOR_23_ADDRESS     0x00048000
#define     FLASH_SECTOR_24_ADDRESS     0x00050000
#define     FLASH_SECTOR_25_ADDRESS     0x00058000
#define     FLASH_SECTOR_26_ADDRESS     0x00060000
#define     FLASH_SECTOR_27_ADDRESS     0x00068000
#define     FLASH_SECTOR_28_ADDRESS     0x00070000
#define     FLASH_SECTOR_29_ADDRESS     0x00078000

/* 4K Size sectors */
#define     FLASH_SECTOR_0        0
#define     FLASH_SECTOR_1        1
#define     FLASH_SECTOR_2        2
#define     FLASH_SECTOR_3        3
#define     FLASH_SECTOR_4        4
#define     FLASH_SECTOR_5        5
#define     FLASH_SECTOR_6        6
#define     FLASH_SECTOR_7        7
#define     FLASH_SECTOR_8        8
#define     FLASH_SECTOR_9        9
#define     FLASH_SECTOR_10       10
#define     FLASH_SECTOR_11       11
#define     FLASH_SECTOR_12       12
#define     FLASH_SECTOR_13       13
#define     FLASH_SECTOR_14       14
#define     FLASH_SECTOR_15       15

/* 32K Size sectors */
#define     FLASH_SECTOR_16       16
#define     FLASH_SECTOR_17       17
#define     FLASH_SECTOR_18       18
#define     FLASH_SECTOR_19       19
#define     FLASH_SECTOR_20       20
#define     FLASH_SECTOR_21       21
#define     FLASH_SECTOR_22       22
#define     FLASH_SECTOR_23       23
#define     FLASH_SECTOR_24       24
#define     FLASH_SECTOR_25       25
#define     FLASH_SECTOR_26       26
#define     FLASH_SECTOR_27       27
#define     FLASH_SECTOR_28       28
#define     FLASH_SECTOR_29       29

#define     FLASH_USER_SECTORS                  (25)
#define     FLASH_USER_SECTORS_4K               (12)
#define     FLASH_USER_SECTORS_32K              (13)
#define     FLASH_USER_HEADER_SECTOR            (4)
#define     FLASH_USER_PAYLOAD_START_SECTOR     (5)
#define     FLASH_USER_PAYLOAD_END_SECTOR       (27)
#define     FLASH_USER_END_SECTOR               (28)
#define     FLASH_BLOCK_SIZE_4K                 (4 * 1024)
#define     FLASH_BLOCK_SIZE_32K                (32 * 1024)

#define     PAYLOAD_TINY_SIZE           (240)
#define     PAYLOAD_SMALL_SIZE          (496)
#define     PAYLOAD_MEDIUM_SIZE         (1008)
#define     PAYLOAD_LARGE_SIZE          (2032)
#define     PAYLOAD_SIZE_BYTES          PAYLOAD_TINY_SIZE

#define     MD5_HASH_SIZE_BYTES         (16)
#define     MD5_BUFFER_SIZE_BYTES       (64)

#define     PAYLOAD_BLOCK_SIZE          (PAYLOAD_SIZE_BYTES + MD5_HASH_SIZE_BYTES)
#define     PAYLOAD_BLOCK_PIECES        (FLASH_BLOCK_SIZE_4K / PAYLOAD_BLOCK_SIZE)
#define     PAYLOAD_BLOCK_PIECES_32K    (FLASH_BLOCK_SIZE_32K / FLASH_BLOCK_SIZE_4K)

#define     FLASH_USER_END_BLOCK_DATA       (0xAB)
#define     FLASH_USER_HEADER_BLOCK_DATA    (0xABBA)

static unsigned int sector_start_address[] = {
    (unsigned int) FLASH_SECTOR_0_ADDRESS,
    (unsigned int) FLASH_SECTOR_1_ADDRESS,
    (unsigned int) FLASH_SECTOR_2_ADDRESS,
    (unsigned int) FLASH_SECTOR_3_ADDRESS,
    (unsigned int) FLASH_SECTOR_4_ADDRESS,
    (unsigned int) FLASH_SECTOR_5_ADDRESS,
    (unsigned int) FLASH_SECTOR_6_ADDRESS,
    (unsigned int) FLASH_SECTOR_7_ADDRESS,
    (unsigned int) FLASH_SECTOR_8_ADDRESS,
    (unsigned int) FLASH_SECTOR_9_ADDRESS,
    (unsigned int) FLASH_SECTOR_10_ADDRESS,
    (unsigned int) FLASH_SECTOR_11_ADDRESS,
    (unsigned int) FLASH_SECTOR_12_ADDRESS,
    (unsigned int) FLASH_SECTOR_13_ADDRESS,
    (unsigned int) FLASH_SECTOR_14_ADDRESS,
    (unsigned int) FLASH_SECTOR_15_ADDRESS,
    (unsigned int) FLASH_SECTOR_16_ADDRESS,
    (unsigned int) FLASH_SECTOR_17_ADDRESS,
    (unsigned int) FLASH_SECTOR_18_ADDRESS,
    (unsigned int) FLASH_SECTOR_19_ADDRESS,
    (unsigned int) FLASH_SECTOR_20_ADDRESS,
    (unsigned int) FLASH_SECTOR_21_ADDRESS,
    (unsigned int) FLASH_SECTOR_22_ADDRESS,
    (unsigned int) FLASH_SECTOR_23_ADDRESS,
    (unsigned int) FLASH_SECTOR_24_ADDRESS,
    (unsigned int) FLASH_SECTOR_25_ADDRESS,
    (unsigned int) FLASH_SECTOR_26_ADDRESS,
    (unsigned int) FLASH_SECTOR_27_ADDRESS,
    (unsigned int) FLASH_SECTOR_28_ADDRESS,
    (unsigned int) FLASH_SECTOR_29_ADDRESS
};

/**
* Fill flash with the payload
*
* @return IAP status codes
*/
int generator_init(void);

#endif /* PAYLOAD_GENERATOR_H_ */
