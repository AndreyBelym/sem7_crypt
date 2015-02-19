#include <iostream>
#include <cstdio>
#include <sstream>
#include <iomanip>
#include <ctime>

typedef unsigned long long uint;

const uint SIZE=(sizeof(uint))*8,
           SIZE2=SIZE/2,
           POW2=uint(1)<<SIZE2;
const uint MAX_SIZE=10000;
using namespace std;
inline void addc(uint& a,const uint& b,uint& carry){
    a+=b;
    if(a<b)
        ++carry;
}

void long_len_correct(uint *src,uint &len){
    for(uint *ptr=src+len-1;!(*ptr);ptr--,len--);
}
void long_clear(uint *src,uint len){
    for(;len;src++,len--)
        *src=0;
}

void mult1(uint a,uint b,uint *res,uint& car){
    uint r0=(a%POW2)*(b%POW2),
         r01=(a%POW2)*(b>>SIZE2),
         r10=(a>>SIZE2)*(b%POW2),
         r1=(a>>SIZE2)*(b>>SIZE2);
    uint car0=0;
    addc(res[0],r0,car0);
    addc(res[0],(r01%POW2)<<SIZE2,car0);
    addc(res[0],(r10%POW2)<<SIZE2,car0);

    addc(res[1],car0,car);
    addc(res[1],r1,car);
    addc(res[1],r01>>SIZE2,car);
    addc(res[1],r10>>SIZE2,car);
}

void long_mult(uint *a, uint la,uint *b, uint lb, uint *res,uint &len){
    uint i,j,car0=0,car1=0,car2=0,maxi=la+lb-1;
    long_clear(res,la+lb);
    for(i=0;i<=maxi;i++){
        addc(res[i],car0,car1);
        for(j=0;j<=i;j++){
            if(j>=la)
                break;
            if((i-j)>=lb)
                continue;
            mult1(a[j],b[i-j],res+i,car2);
        }
        car0=car1;car1=car2;car2=0;
    }
    len=maxi+1;
    if(car0){
        len=maxi+1;
        res[len]=car0;
        len++;
    }
    if(car1){
        len=maxi+2;
        res[len]=car1;
        len++;
    }
    if(car2){
        len=maxi+3;
        res[len]=car2;
        len++;
    }
    long_len_correct(res,len);
}

void print_long(uint* num,uint len){
    for(uint *ptr=num+len-1;len;--ptr,--len){
        cout<<hex<<setw(sizeof(uint)*2)<<setfill('0')<<*ptr;
    }
    cout<<endl;
}

void long_copy(uint *src,uint len,uint* dst){
    for(;len;src++,dst++,len--)
        *dst=*src;
}



uint high_bit(uint n){
    return (n&(uint(1)<<(SIZE-1)))>>(SIZE-1);
}

uint last_bit(uint n){
    return n&1;
}

void long_lsh(uint *res,uint &len){
    uint hb=0,hb0=0;
    for(uint i=len;i;i--,res++){
        hb=high_bit(*res);
        *res=((*res)<<1)+hb0;
        hb0=hb;
    }
    if(hb0){
        len++;
        *res=hb0;
    }
}

void long_rsh(uint *res,uint &len){
    uint lb=0,lb0=0;
    for(uint i=len,*ptr=res+len-1;i;i--,ptr--){
        lb=last_bit(*ptr);
        *ptr=((*ptr)>>1)+(lb0<<(SIZE-1));
        lb0=lb;
    }
    if(!res[len-1]){
        len--;
    }
}

void long_inc(uint *res,uint &len){
    uint car=0,car0=0;uint i;
    addc(*res,1,car0);
    res++;
    for(i=len-1;i&&car0;i--,res++){
        addc(*res,car0,car);
        car0=car;car=0;
    }
    if(!i&&car0){
        len++;
        *res=car0;
    }
}

bool long_le(uint *a,uint la,uint *b,uint lb){
    if(la<lb)
        return true;
    else if(la>lb)
        return false;
    else{
        uint *p1=a+la-1,*p2=b+lb-1;
        for(;la&&*p1==*p2;la--,p1--,p2--);
        return !la||*p1<*p2;
    }
}

bool long_lt(uint *a,uint la,uint *b,uint lb){
    if(la<lb)
        return true;
    else if(la>lb)
        return false;
    else{
        uint *p1=a+la-1,*p2=b+lb-1;
        for(;la&&*p1==*p2;la--,p1--,p2--);
        return la&&*p1<*p2;
    }
}

void long_long_pow(uint* src0,uint l0,uint *pow0,uint lp0,uint* res0,uint &l){
    l=l0; uint *t; uint *res=res0;
    uint *src=new uint[MAX_SIZE],
            *pow=new uint[MAX_SIZE];
    long_clear(src,MAX_SIZE);
    long_clear(pow,MAX_SIZE);
    pow[0]=2; uint lp=1;
    long_copy(src0,l0,src);

    while(long_le(pow,lp,pow0,lp0)){
        long_clear(res,2*l);
        long_mult(src,l,src,l,res,l);
        t=res;res=src;src=t;
        long_lsh(pow,lp);
    }
    long_rsh(pow,lp);
    while(long_lt(pow,lp,pow0,lp0)){
        long_clear(res,l+l0);
        long_mult(src,l,src0,l0,res,l);
        long_inc(pow,lp);
        t=res;res=src;src=t;
    }
    if(src!=res0)
        long_copy(src,l,res);
    else{
        t=res;res=src;src=t;
    }
    delete src;
}

void long_pow(uint* src0,uint l0,uint pow0,uint* res0,uint &l){
    l=l0; uint *t; uint *res=res0;
    uint *src=new uint[MAX_SIZE];
    long_clear(src,MAX_SIZE);

    uint pow=2;
    long_copy(src0,l0,src);

    while(pow<=pow0){
        long_clear(res,2*l);
        long_mult(src,l,src,l,res,l);
        t=res;res=src;src=t;
        pow*=2;
    }
    pow/=2;
    while(pow<pow0){
        long_clear(res,l+l0);
        long_mult(src,l,src0,l0,res,l);
        pow++;
        t=res;res=src;src=t;
    }
    if(src!=res0)
        long_copy(src,l,res);
    else{
        t=res;res=src;src=t;
    }
    delete src;
}
void long_from_string(const string &str,uint *res,uint &len){
    uint block_size=SIZE/4;
    int i; len=0;
    for(i=str.length()-block_size;i>=0;i-=block_size){
        istringstream istr(str.substr(i,block_size));
        istr>>hex>>*res;
        len++;res++;
    }
    if((i+block_size)>0){
        istringstream istr(str.substr(0,i+block_size));
        istr>>hex>>*res;
        len++;res++;
    }
}

void input_long(uint *res,uint &len){
    string s;
    cin>>s;
    long_clear(res,len);
    long_from_string(s,res,len);
}

void mod2n(uint* a,uint &len_a,uint n){
    if(len_a*SIZE<=n)
        return;
    uint m=n%SIZE;
    len_a=n/SIZE;
    if(m){
        a[len_a]%=uint(1)<<m;
        len_a++;
    }
}

void key(uint* m,uint len_m,uint *x,uint len_x,uint* y,uint &len_y,uint key_len){
    long_long_pow(m,len_m,x,len_x,y,len_y);
    mod2n(y,len_y,key_len);
}

void gen_key(){
    uint m[MAX_SIZE],x[MAX_SIZE],y[MAX_SIZE];
    uint len_m=MAX_SIZE,len_x=MAX_SIZE,len_y;
    uint key_len;
    clock_t t1,t2;
    cout<<"Введите длину ключа."<<endl;
    cin>>key_len;
    cout<<"Введите базу/открытый ключ."<<endl;
    input_long(m,len_m);
    cout<<"Введите секретное число."<<endl;
    input_long(x,len_x);
    t1=clock();
    key(m,len_m,x,len_x,y,len_y,key_len);
    t2=clock();
    cout<<"Полученный ключ:"<<endl;
    print_long(y,len_y);
    cout<<"Время формирования ключа: "<<double(t2-t1)/CLOCKS_PER_SEC<<" с."<<endl;
    char ch;
    cout<<"Продолжить формирование общего ключа? (Y/N)"<<endl;
    cin>>ch;
    if(ch=='Y'){
        cout<<"Введите второе секретное число."<<endl;
        input_long(x,len_x);
        t1=clock();
        key(y,len_y,x,len_x,m,len_m,key_len);
        t2=clock();
        cout<<"Полученный общий ключ:"<<endl;
        print_long(m,len_m);
        cout<<"Время формирования ключа: "<<double(t2-t1)/CLOCKS_PER_SEC<<" с."<<endl;
    }
}




void check_key(){
    uint m[MAX_SIZE],x[MAX_SIZE],x1[MAX_SIZE],x2[MAX_SIZE],y[MAX_SIZE];
    uint len_m=MAX_SIZE,len_x,len_x1=MAX_SIZE,len_x2=MAX_SIZE,len_y;
    uint key_len;
    clock_t t1,t2;
    cout<<"Введите длину ключа."<<endl;
    cin>>key_len;
    cout<<"Введите базу/открытый ключ."<<endl;
    input_long(m,len_m);
    cout<<"Введите секретное число 1."<<endl;
    input_long(x1,len_x1);
    cout<<"Введите секретное число 2."<<endl;
    input_long(x2,len_x2);
    long_mult(x1,len_x1,x2,len_x2,x,len_x);
    t1=clock();
    key(m,len_m,x,len_x,y,len_y,key_len);
    t2=clock();
    cout<<"Полученный ключ:"<<endl;
    print_long(y,len_y);
    cout<<"Время формирования ключа: "<<double(t2-t1)/CLOCKS_PER_SEC<<" с."<<endl;
}

int main()
{
    int sel;
    cout<<"Получение ключей по схеме Диффи-Хеллмана."<<endl;
    cout<<"1. Формирование ключей"<<endl;
    cout<<"2. Проверка общего ключа"<<endl;
    cin>>sel;
    switch (sel) {
    case 1:
        gen_key();
        break;
    case 2:
        check_key();
        break;
    default:
        exit(-1);
    }
    return 0;
}

