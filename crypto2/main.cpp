#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <mgl2/mgl.h>

using namespace std;

void test1(int n=100,int mod=RAND_MAX,unsigned int seed=time(0)){
    map<int,int> freq;
    srand(seed);
    for(int i=0;i<n;++i){
        ++freq[rand()%mod];
    }
    for(auto i=freq.begin();i!=freq.end();++i){
        cout<<i->first<<": "<<i->second<<endl;
    }
}

void test2(int mod=RAND_MAX,unsigned int seed=time(0)){
    srand(seed);
    int i=1;
    for(int r = rand()%mod;r!=rand()%mod;i++);
    cout<<i<<endl;
}
void test3(int n=100,int mod=RAND_MAX,unsigned int seed=time(0)){
    vector<int> v;
    vector<double> res;
    srand(seed);
    double s1=0,s2=0,avg1=0,avg2=0;
    for(int i=0;i<n;i++){
        int t=rand()%mod;
        v.push_back(t);
        s1+=t;
    }
    s2=s1;avg1=s1/n;
    for(int i=0;i<n/4;i++){
        v.push_back(rand()%mod);
    }
    for(int i=1;i<=n/4;i++){
        s2+=v[i+n-1]-v[i-1];
        avg2=s2/n;
        double a=0,b=0,c=0;
        for(int j=0;j<n;j++){
            c+=(v[j]-avg1)*(v[i+j]-avg2);
            a+=(v[j]-avg1)*(v[j]-avg1);
            b+=(v[i+j]-avg2)*(v[i+j]-avg2);
        }
        res.push_back(c/sqrt(a*b));
    }
    for(int i=0;i<n/4;i++)
        cout<<i+1<<": "<<res[i]<<endl;
}

void test4(string filename="hist.png",int n=100,int mod=RAND_MAX,unsigned int seed=time(0)){
    map<int,int> freq;
    vector<int> numbers;
    vector<float> counts;
    srand(seed);
    for(int i=0;i<n;++i){
        ++freq[rand()%mod];
    }
    for(auto i=freq.begin();i!=freq.end();++i){
        numbers.push_back(i->first);
        counts.push_back((float)i->second/n);
    }
    mglGraph gr;
    mglData a,b;
    a.Set(numbers); b.Set(counts);
    gr.SetRange('x',a.Minimal(),a.Maximal()+1);
    gr.SetRange('y',0,b.Maximal());
    mglData hist=gr.Hist(a,b);
    gr.Axis("xy");
    gr.Bars(hist);
    gr.WriteFrame(filename.c_str());
}

int main()
{
    int mod,n; string filename;
    cout<<"Исследование равномерности распределения: "<<endl;
    cout<<"Введите количество отсчетов: "; cin>>n;
    cout<<"Введите модуль генератора: "; cin>>mod;
    test1(n,mod);
    cout<<"Исследование периода: "<<endl;
    cout<<"Введите модуль генератора: "; cin>>mod;
    test2(mod);
    cout<<"Исследование автокоррелляции: "<<endl;
    cout<<"Введите количество отсчетов: "; cin>>n;
    cout<<"Введите модуль генератора: "; cin>>mod;
    test3(n,mod);
    cout<<"Построение гистограммы частоты случайных чисел:"<<endl;
    cout<<"Введите количество отсчетов: "; cin>>n;
    cout<<"Введите модуль генератора: "; cin>>mod;
    cout<<"Введите имя файла для гистограммы: ";cin>>filename;
    test4(filename,n,mod);
    return 0;
}

