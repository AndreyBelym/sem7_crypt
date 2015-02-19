#ifndef DES_H
#define DES_H
#include <iostream>
typedef unsigned char uchar;
void create_key(uchar *key0,uchar keys[16][6]);
void des_ofb_crypt(std::iostream &src,uchar key[8],uchar c0_[8],std::iostream& dst);
#endif DES_H
