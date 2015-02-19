#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <fstream>


int main()
{
    uchar k[8]={0x13,0x34,0x57,0x79,0x9B,0xBC,0xDF,0xF1},M[8]={0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};
    stringstream plain,cipher;
    cout<<endl<<plain.good()<<endl;
    for(int i=0;i<8;i++){
        plain<<M[i];
    }
    cout<<endl<<plain.good()<<endl;
    //plain.seekg(0);
    des_ofb_crypt(plain,k,k,cipher);
    cout<<endl<<plain.good()<<endl;
    print_block(cipher);
    plain.clear();
    plain.str("");
    cout<<plain.good()<<endl;
    des_ofb_crypt(cipher,k,k,plain);
    cout<<plain.good()<<endl;
    print_block(plain);
    return 0;
}

