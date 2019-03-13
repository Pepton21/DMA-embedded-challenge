/*
 * payload_generator.c
 *
 *  Created on:  Apr 29, 2014
 *      Author:  Bojan Gruevski
 *      Company: SEAVUS Group
 */

#include "LPC17xx.h"
#include <cr_section_macros.h>
#include <stdlib.h>
#include <string.h>

#include "iap_driver.h"
#include "md5.h"
#include "payload_generator.h"

//#define WRONG_HASH 1

#ifdef WRONG_HASH
#define NUMBER_OF_WRONG_HASHES (4)
uint8_t wrong_hashes_chunk_positions[NUMBER_OF_WRONG_HASHES] = { 3, 8, 11, 15 };
uint8_t wrong_hashes_current_position = 0;
#endif

/**
* Initialize given payload with random data
*/
void seed_payload(uint8_t payload[], uint32_t size, int seed)
{
    int i;

    /* Seed the random number generator */
    srand(seed);

    /* Generate random values for the payload */
    for (i = 0; i < size; ++i)
        payload[i] = rand() % 256;
}

/**
* Calculate MD5 hash for a given payload
*/
void calculate_hash(uint8_t* hash_destination, uint32_t data_size)
{
#ifdef WRONG_HASH
    int i;
#endif
    MD5_CTX ctx;

    /* Initialize the MD5 context */
    MD5_Init(&ctx);

    /* Calculate MD5 hash with iterative calculation */
    MD5_Update(&ctx, &hash_destination[MD5_HASH_SIZE_BYTES], data_size);

    /* Place the hash in the payload block */
    MD5_Final(hash_destination, &ctx);

#ifdef WRONG_HASH
    for (i = 0; i < NUMBER_OF_WRONG_HASHES; ++i)
        if (wrong_hashes_current_position == wrong_hashes_chunk_positions[i]) {
            hash_destination[0] <<= 2;
            break;
        }
#endif
}

/**
* Write to flash the start block
*
* @return IAP status codes
*/
int write_header(void)
{
    e_iap_status iap_status;
    uint8_t block[FLASH_BLOCK_SIZE_4K] = { 0 };
    uint16_t header;
    uint16_t chunks;
    uint32_t size;

    /* Preamble */
    header = FLASH_USER_HEADER_BLOCK_DATA;
    memcpy(&block[0], &header, sizeof(header));

    /* Number of chunks */
    chunks = ((FLASH_USER_SECTORS_4K - 1) * PAYLOAD_BLOCK_PIECES) + ((FLASH_USER_SECTORS_32K - 1) * PAYLOAD_BLOCK_PIECES * PAYLOAD_BLOCK_PIECES_32K);
    memcpy(&block[2], &chunks, sizeof(chunks));

    /* Size of chunks */
    size = PAYLOAD_BLOCK_SIZE;
    memcpy(&block[4], &size, sizeof(size));

    /* Prepare the current sector for writing */
    iap_status = (e_iap_status) iap_prepare_sector(FLASH_USER_HEADER_SECTOR, FLASH_USER_HEADER_SECTOR);
    if (iap_status != CMD_SUCCESS)
        return iap_status;

    /* Write payload block to flash */
    iap_status = (e_iap_status) iap_copy_ram_to_flash(&block, (void *)sector_start_address[FLASH_USER_HEADER_SECTOR], SIZE_4096);
    if (iap_status != CMD_SUCCESS)
        return iap_status;

    return iap_status;
}

/**
* Write to flash the payload blocks
*
* @return IAP status codes
*/
int write_payload(void)
{
    e_iap_status iap_status;
    int index;
    int payload_piece;
    int sub_blocks = 1;
    int sector = FLASH_USER_PAYLOAD_START_SECTOR;
    uint8_t block[FLASH_BLOCK_SIZE_4K] = { 0 };
    uint32_t offset = FLASH_BLOCK_SIZE_4K;
    uint32_t address = sector_start_address[FLASH_USER_PAYLOAD_START_SECTOR];

    /* Seed the payload and calculate its hash */
    while (sector <= FLASH_USER_PAYLOAD_END_SECTOR) {

        /* We need to populate several sub-sectors */
        for (payload_piece = 0; payload_piece < PAYLOAD_BLOCK_PIECES; ++payload_piece) {

#ifdef WRONG_HASH
            ++wrong_hashes_current_position;
#endif

            /* Calculate the start address of the sub-sector */
            index = payload_piece * PAYLOAD_BLOCK_SIZE;

            /* Initialize it with random data */
            seed_payload(&block[index + MD5_HASH_SIZE_BYTES], PAYLOAD_SIZE_BYTES, address + payload_piece);

            /* Calculate the hash of the random data */
            calculate_hash(&block[index], PAYLOAD_SIZE_BYTES);
        }

        /* Prepare the current flash sector for writing */
        iap_status = (e_iap_status) iap_prepare_sector(sector, sector);
        if (iap_status != CMD_SUCCESS)
            return iap_status;

        /* Write payload block to flash sector */
        iap_status = (e_iap_status) iap_copy_ram_to_flash(&block, (void *)address, SIZE_4096);
        if (iap_status != CMD_SUCCESS)
            return iap_status;

        /* Increment flash address */
        address += offset;

        /* If we are currently in 32K flash block we need to populate */
        /* several sub-blocks inside [PAYLOAD_BLOCK_PIECES_32K] */
        if ((sector >= FLASH_SECTOR_16) && (sub_blocks < PAYLOAD_BLOCK_PIECES_32K)) {
            ++sub_blocks;
        } else {
            sub_blocks = 1;
            ++sector;
        }
    }

    return iap_status;
}

/**
* Write to flash the end block
*
* @return IAP status codes
*/
int write_end(void)
{
    uint32_t i;
    e_iap_status iap_status;
    uint8_t block[FLASH_BLOCK_SIZE_4K] = { 0 };

    for (i = 0; i < PAYLOAD_BLOCK_SIZE; ++i)
        block[i] = FLASH_USER_END_BLOCK_DATA;

    /* Prepare the current sector for writing */
    iap_status = (e_iap_status) iap_prepare_sector(FLASH_USER_END_SECTOR, FLASH_USER_END_SECTOR);
    if (iap_status != CMD_SUCCESS)
        return iap_status;

    /* Write payload block to flash */
    iap_status = (e_iap_status) iap_copy_ram_to_flash(&block, (void *)sector_start_address[FLASH_USER_END_SECTOR], SIZE_4096);
    if (iap_status != CMD_SUCCESS)
        return iap_status;

    return iap_status;
}

/**
* Fill flash with the payload
*
* @return IAP status codes
*/
int generator_init(void)
{
    e_iap_status iap_status;

    /* Init the IAP driver */
    iap_init();

    /* Prepare the user sectors for erase */
    iap_status = (e_iap_status) iap_prepare_sector(FLASH_USER_HEADER_SECTOR, FLASH_USER_END_SECTOR);
    if (iap_status != CMD_SUCCESS)
        return iap_status;

    /* Erase all user sectors */
    iap_status = (e_iap_status) iap_erase_sector(FLASH_USER_HEADER_SECTOR, FLASH_USER_END_SECTOR);
    if (iap_status != CMD_SUCCESS)
        return iap_status;

    /* Write header in flash */
    iap_status = (e_iap_status)write_header();
    if (iap_status != CMD_SUCCESS)
        return iap_status;

    /* Write payload in flash */
    iap_status = (e_iap_status)write_payload();
    if (iap_status != CMD_SUCCESS)
        return iap_status;

    /* Write end in flash */
    iap_status = (e_iap_status)write_end();

    return iap_status;
}
