#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include "Xtree.cpp"
using namespace std;

vector<double> Read_Input(string input)
{
    vector<double> data;
    data.clear();
    stringstream stream(input);
    string valor="";
    while(getline(stream,valor,','))
    {
        data.push_back(stod(valor));
    }

    return data;
}

void Read_File(Xtree *xt)
{
    string line,num = "";
    int v = 0;
    ifstream file("YearPredictionMSD.txt");
    //ifstream file("data_3.txt");

    int count = 0;
    if(file.is_open())
    {
        int i=5;
        string linea;
        vector<double> data;
        //cout<<data.size()<<endl;

        while(getline(file,linea))
        {
            v++;
            stringstream stream(linea);
            string valor;

            while(getline(stream,valor,','))
            {
                data.push_back(stod(valor));
            }
            //cout<<data.size()<<endl;
            //cout<<linea<<endl;
            if(data.size()>0)
            {
                xt->insert(data);
            }
            
            data.clear();
        }
        cout<<"Termino ..."<<endl;
        file.close();
    }
    return;
}

int main()
{
    Xtree *xt;
    int n,m;
    vector<double> Point;
    string input="";
    struct  timeval ti,tf;
    double ttime;
    //vector<double> query = {2004,26,-148,-13,-7,17,-21,5,3,2,2,-0,4,55,5492,1704,953,959,788,365,270,524,194,139,375,-434,-627,642,-440,-319,10,38,-30,75,-48,80,183,-178,-594,185,192,-65,-133,13,-24,-122,113,175,542.47140,6.37825,179.58990,137.58951,38.38606,-3.31309,5.48498,-128,-303,188,-108,-69,84,-54,-112,85,38,-277,132,91,-11,-28,-21,205,-22,-52,-122,-0,-56,91,-2,49,-243,23,251,18,157,-27,-137,115,23,-164,51};
    cout<<"Ingrese M: ";  //Pruebas hechas con M = 2000
    cin>>m;
    xt = new Xtree(m);
    gettimeofday(&ti,NULL);   //Tiempo de inicio de indexacion
    Read_File(xt);
    gettimeofday(&tf,NULL);
    ttime = (tf.tv_sec-ti.tv_sec)*1000 + (tf.tv_usec-ti.tv_usec)/1000;
    cout<<"Tiempo de Indexado: "<<ttime/1000<<"s"<<endl;
    
    
    //while(1)
   // {
        cout<<"...Buscar los N mas cercanos..."<<endl;  
        cout<<"Ingrese N : ";
        cin>>n;
        cout<<"Ingrese el punto: ";
        getline(cin,input);
        getline(cin,input);
        cout<<"\n"<<endl;
        Point.clear();
        Point = Read_Input(input);
        cout<<input<<endl;
        cout<<Point.size()<<endl;
        gettimeofday(&ti,NULL);
        xt->kNN_Search(xt->m_head,Point,n);
        gettimeofday(&tf,NULL);
        input = "";
        ttime = (tf.tv_sec-ti.tv_sec)*1000 + (tf.tv_usec-ti.tv_usec)/1000;
        cout<<"Tiempo de consulta: "<<ttime/1000<<"s"<<endl;
    //}
    
    return 0;
}
