#ifndef  __CONFIG_H_
#define  __CONFIGH_


//#define S3_WRITE //Read and write s3 configuration files
//#define S3_IP   //Using S3 IP to operation
//#define S3_single_quotes       //Find the S3 key with a single quotation marks in the configuration file, differentiate between here。if the config of S3 have single quotes

//#define OPENWRT   //S3,R1...
#define BB_BLACK    //S4,S5,S4+,S5+...

#define S4_IP     //Using S4 IP to operation
#define S4_WRITE    //Read and write s4 configuration files

#ifndef MINERTYPE
#define MINERTYPE 5 //s5 default
#endif // MINERTYPE
#ifndef ETH
#define ETH "eth0"
#endif // ETH

//#define PC_IP    // for PC test
#endif
