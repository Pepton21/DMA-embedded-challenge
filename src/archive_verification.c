/*
AllCodes.c
 *
 *  Created on: Jan 5, 2016
 *  Authors: Petar Tonkovikj, Petar Jovanovski, Ebrar Islam
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include "md5.h"
#include "iap_driver.h"
#include "payload_generator.h"
#include "definitions.h"
#include "leds.h"

/* declaration of a global variable that indicates whether a transfer has finished */
volatile uint8_t transfer_finished = 0;

/**
* DMA interrupt handler
*/
void DMA_IRQHandler(void) {
	/* clear the flags causing the interrupt */
	LPC_GPDMA->DMACIntTCClear |= 0xFF;
	LPC_GPDMA -> DMACIntErrClr |= 0xFF;

	/* indicate that the transfer has finished */
	transfer_finished = 1;

}

/**
* Get a 16-bit integer from a given starting location
*
* @param location		Starting location of the integer
*
* @return the 16-bit integer
*/
uint16_t get_two_bytes (uint8_t* location) {
	return ((uint16_t)*(location + 1) << 8) | *location;
}


/**
* Get a 32-bit integer from a given starting location
*
* @param location		Starting location of the integer
*
* @return the 32-bit integer
*/
uint32_t get_four_bytes(uint8_t* location) {
	return ((uint32_t)*(location + 3) << 24) | ((uint32_t)*(location + 2) << 16) |
			((uint16_t)*(location + 1) << 8) | *location;
}

/**
* Get a 64-bit integer from a given starting location
*
* @param location		Starting location of the integer
*
* @return the 64-bit integer
*/
uint64_t get_eight_bytes(uint8_t* location) {
    return  ((uint64_t)*(location + 7) << 56) | ((uint64_t)*(location + 6) << 48) |
    		((uint64_t)*(location + 5) << 40) | ((uint64_t)*(location + 4) << 32) |
			((uint32_t)*(location + 3) << 24) | ((uint32_t)*(location + 2) << 16) |
			((uint16_t)*(location + 1) << 8)  | *location;
}

/**
* Get the preamble of the archive
*
* @return the 16-bit preamble
*/
uint16_t get_preamble() {
	return get_two_bytes(HEADER_ADDRESS);
}

/**
* Get the number of parts within the archive
*
* @return the 16-bit value for the number of parts
*/
uint16_t get_number_of_parts() {
	return get_two_bytes(HEADER_ADDRESS + 2);
}

/**
* Get the part size (size of each part within the archive)
*
* @return the 32-bit value for part size
*/
uint32_t get_part_size() {
	return get_four_bytes(HEADER_ADDRESS + 4);
}

/**
* Get the footer of the archive
*
*@param part_size		Size of a single part
*@param no_of_parts		Number of parts in the archive
*
* @return the 64-bit value for the footer
*/
uint64_t get_footer(uint32_t part_size, uint16_t no_of_parts) {
	return get_eight_bytes(PART_STARTING_ADDRESS + no_of_parts * part_size);
}

/**
* Calculate the hash of a given part
*
* @param part address
* @param part size
*/
void calculate_part_hash(uint8_t* part, uint32_t part_size) {
	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, &part[HASH_SIZE], part_size);
	MD5_Final(part, &ctx);
}

/**
* DMA initialization
*/
void DMA_init() {
	/* power up GPDMA */
	LPC_SC -> PCONP |= 1 << 29;

	/* enable GPDMA */
	LPC_GPDMA -> DMACConfig |= 1;

	/* clear the Interrupt Terminal Count Register */
	LPC_GPDMA -> DMACIntErrClr |= 0xFF;

	/* clear the Interrupt Error Status */
	LPC_GPDMA -> DMACIntTCClear |= 0xFF;

	/* enable interrupt requests */
	NVIC_EnableIRQ(DMA_IRQn);
}

/**
* Initiate a DMA transfer
*
* @param src_addr			Source address of of the transfer
* @param dest_addr			Destination address of the transfer
* @param transfer_size
*/
void DMA_transfer(uint8_t* src_addr, uint8_t* dest_addr, uint16_t transfer_size) {

	/* set source and destination address */
	LPC_GPDMACH0->DMACCSrcAddr = src_addr;
	LPC_GPDMACH0->DMACCDestAddr = dest_addr;

	/* not using linked list */
	LPC_GPDMACH0->DMACCLLI = 0;

	/* set the control register (burst size 1 by default) */
	LPC_GPDMACH0->DMACCControl = (transfer_size) | (TRANSFER_WIDTH << 18) |
			(TRANSFER_WIDTH << 21) | (1 << 26) | (1 << 27) | (1 << 31);

	/* indicate that the transfer has not finished and enable channel */
	transfer_finished = 0;
	LPC_GPDMACH0->DMACCConfig = 0x0C001;

}

/**
* Calculate the transfer size and initiate a DMA transfer
*
* @param src_addr				Source address of of the transfer
* @param dest_addr				Destination address of the transfer
* @param parts_to_transfer		Number of parts left to transfer
* @param block_capacity			Number of parts a block can store
* @param part_size				Size of a single part
*/
void transfer_to_RAM(uint8_t* src_addr, uint8_t* dest_addr, uint16_t* parts_to_transfer,
		uint16_t* block_capacity, uint32_t* part_size){

	/* initiate a DMA transfer for the appropriate transfer size */
	if (*parts_to_transfer < *block_capacity) {
		DMA_transfer(src_addr, dest_addr, (*parts_to_transfer * *part_size) >> TRANSFER_WIDTH);
		*parts_to_transfer = 0;
	}
	else {
		DMA_transfer(src_addr, dest_addr, TRANSFER_SIZE);
		*parts_to_transfer -= *block_capacity;
	}
}

/**
* Verify a block (check whether the hashes are correct)
*
* @param block_addr			Address of the block to be verified
* @param block_capacity		Number of parts a block can store
* @param parts_to_verify	Number of parts left to verify
* @param part_size			Size of a single part
*
* @return hashes are correct or hashes are not correct
*/
uint8_t verify_block(uint8_t* block_addr, uint16_t* block_capacity,
		uint16_t* parts_to_verify, uint32_t* part_size) {

	/* declare and initialize auxiliary variables */
	uint16_t i;
	uint8_t hash_of_part[16];
	uint16_t needed_verifications = (*parts_to_verify < *block_capacity)? (*parts_to_verify) : (*block_capacity);

	/* verify all parts in the block */
	for (i=0; i<needed_verifications; i++) {

		/* save the given hash of the part */
		memcpy(hash_of_part, block_addr, HASH_SIZE);

		/* calculate the correct hash of the part */
		calculate_part_hash(block_addr, *part_size - HASH_SIZE);

		/* compare the given hash with the correct hash */
		if (memcmp(hash_of_part, block_addr, HASH_SIZE)) return 0;
		block_addr += *part_size;
	}

	/* update the number of parts left to verify */
	*parts_to_verify -= needed_verifications;
	return 1;
}

/**
* Verify the archive
*/
uint8_t verify() {

	/* declaration of needed variables */
	uint16_t no_parts, parts_to_verify, parts_to_transfer, block_capacity;
	uint32_t part_size;

	/* declare blocks where the parts are transfered to */
	uint8_t block1[RAM_BLOCK_SIZE], block2[RAM_BLOCK_SIZE];

	/* declare and initialize the address of the part that should be transfered next */
	uint8_t* part_addr = PART_STARTING_ADDRESS;

	/* declare and initialize a flag that indicates in which block to
	 * store data from the DMA transfer and which block to verify */
	uint8_t flag = 0;

	/* initialization of variables */
	part_size = get_part_size();
	block_capacity = RAM_BLOCK_SIZE / part_size;
	no_parts = get_number_of_parts();
	parts_to_verify = parts_to_transfer = no_parts;

	/* initialize the DMA controller */
	DMA_init();

	/* initial DMA transfer done while checking preamble and footer */
	transfer_to_RAM(part_addr, block1, &parts_to_transfer, &block_capacity, &part_size);

	/* check preamble and footer */
	if (get_preamble() != VALID_PREAMBLE) return 0;
	if (get_footer(part_size, no_parts) != VALID_FOOTER) return 0;

	/* wait for the transfer to finish */
	while(!transfer_finished);

	/* verify parts until all are verified (parts_to_verify != 0) */
	while (parts_to_verify){

		/* wait for the transfer to finish */
		while(!transfer_finished);

		/* increment the pointer of the part that should be transfered next */
		part_addr += RAM_BLOCK_SIZE;

		/* start DMA transfer to block not being verified */
		if (parts_to_transfer)
			transfer_to_RAM(part_addr, (flag)? block1 : block2, &parts_to_transfer, &block_capacity, &part_size);

		/* verify block while transferring, if verification is not correct return 0 */
		if (!verify_block((!flag)? block1 : block2, &block_capacity, &parts_to_verify, &part_size)) return 0;

		/* invert flag (change which block to verify and which block to transfer to */
		flag ^= 1;
	}

	/* if loop finishes (all parts successfully verified) return 1 */
	return 1;
}
