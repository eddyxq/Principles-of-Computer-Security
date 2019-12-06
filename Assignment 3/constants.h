#ifndef _CONSTANTS_H__
#define _CONSTANTS_H__

// sizes of things

#define KEYLEN   32   // 256 bits
#define IVLEN    16   // 128 bits
#define MACLEN   32   // 256 bits
#define BLOCKLEN 16   // 128 bits
#define CTLEN    1024

// filenames

#define ORACLE1A "./oracle"
#define ORACLE1B "./oracle"
#define ORACLE2A "./oracle"
#define ORACLE2B "./oracle"

// return values

#define ERR_BADPAD 'P' // output by oracleXa
#define ERR_BADMAC 'M' // output by oracleXa
#define ERR_BADCT  'B' // output by oracleXb
#define ERR_OK     'O' // output by oracleXa and oracleXb

// artificial delays

#define SLEEP_BADPAD 10000
#define SLEEP_BADMAC 50000

#endif
