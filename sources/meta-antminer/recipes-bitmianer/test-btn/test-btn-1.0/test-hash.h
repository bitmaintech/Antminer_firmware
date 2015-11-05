#ifndef TEST_HASH__
#define TEST_HASH__
#define CHAIN_SIZE	16
#define	BM_GET_STATUS	0x53
#define	BM_STATUS_DATA	0xa1

#define	DETECT_GET		0x02
#define 	GET_CHIP_ST	0x01
struct BITMAIN_GET_STATUS {
	uint8_t token_type;
	uint8_t version;
	uint16_t length;

	uint8_t chip_status_eft      :1;
	uint8_t detect_get		:1;
	uint8_t reserved1            :6;

	/*uint8_t rchipd_eft;*/
	uint8_t test_hash; //0xba
	uint8_t reserved[2];
	//uint32_t reg_data;
	uint8_t chip_address;
	uint8_t reg_addr;
	uint16_t crc;
}__attribute__((packed, aligned(4)));
typedef struct BITMAIN_GET_STATUS*	BITMAIN_GET_STATUS_P;

struct BITMAIN_STATUS_DATA {
    //struct BITMAIN_STATUS_DATA_HEADER status_header;
    uint8_t data_type;
	uint8_t version;
    uint16_t length;

    uint8_t chip_value_eft : 1;
    uint8_t reserved1 : 7;
	uint8_t chain_num;
	uint16_t fifo_space;
    uint32_t hw_version;
	uint8_t fan_num;
    uint8_t temp_num;
	uint16_t fan_exist;
	uint32_t temp_exist;
    /*uint8_t rchipregval_eft;*/
    //uint8_t fifo_space;
	uint32_t nonce_err;
	uint32_t reg_value;
	uint32_t chain_asic_exist[CHAIN_SIZE][8];
    uint32_t chain_asic_status[CHAIN_SIZE][8];
    uint8_t chain_asic_num[CHAIN_SIZE];
    uint8_t temp[16];
    uint8_t fan[16];
    uint16_t crc;
} __attribute__((packed, aligned(4)));


#endif
