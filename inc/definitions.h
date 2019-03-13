/*
 * definitions.h
 *
 *  Created on: Jan 5, 2016
 *  Authors: Petar Tonkovikj, Petar Jovanovski, Ebrar Islam
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_
#endif /* DEFINITIONS_H_ */

/* definitions of frequently used variables */
#define HASH_SIZE 16
#define VALID_PREAMBLE 0xABBA
#define VALID_FOOTER 0xABABABABABABABABULL
#define BYTE_WIDTH 0x0
#define HALFWORD_WIDTH 0x01
#define WORD_WIDTH 0x02
#define TRANSFER_WIDTH WORD_WIDTH
#define HEADER_ADDRESS 0x4000
#define RAM_BLOCK_SIZE (2048* ((1 << TRANSFER_WIDTH) | TRANSFER_WIDTH))
#define TRANSFER_SIZE (RAM_BLOCK_SIZE >> TRANSFER_WIDTH)
#define PART_STARTING_ADDRESS 0x5000

/* definition of global variable */
extern volatile uint8_t transfer_finished;

/* definitions of functions */
uint16_t get_preamble();
uint16_t get_number_of_parts();
uint32_t get_part_size();
uint64_t get_eight_bytes(uint8_t* location);
uint64_t get_footer(uint32_t part_size, uint16_t no_of_parts);
void calculate_part_hash(uint8_t* part, uint32_t part_size);
void DMA_init();
void DMA_transfer(uint8_t* src_addr, uint8_t* dest_addr, uint16_t transfer_size);
void transfer_to_RAM(uint8_t* src_addr, uint8_t* dest_addr, uint16_t* no_parts,
		uint16_t* block_capacity, uint32_t* part_size);
uint8_t verify_block(uint8_t* block_addr, uint16_t* block_capacity,
		uint16_t* parts_to_verify, uint32_t* part_size);
uint8_t verify();

