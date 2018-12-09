#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void Read_File(Xtree *xt)
{
    string line,num = "";
    ifstream file("YearPredictionMSD.txt");
    int count = 0;
    if(file.is_open())
    {
        int i=5;
        string linea;
        vector<double> data;
        cout<<data.size()<<endl;

        while(getline(file,linea))
        {
            stringstream stream(linea);
            string valor;

            while(getline(stream,valor,','))
            {
                data.push_back(stod(valor));
            }
            //cout<<data.size()<<endl;
            if(data.size()>0)
            {
                Point temp_point(data);
                xt->x_insert(temp_point);
            }
            
            data.clear();
        }
        cout<<"cabo"<<endl;
        file.close();
    }
    return;
}

int main()
{
    Xtree *xt;
    xt = new Xtree();
    Read_File(xt);
    return 0;
}
