#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
using namespace std;

const int NUM_DIM  = 91; //Numero de dimensiones por defecto segun el dataset

class Nodo
{
    public:
        bool is_leaf;
        int is_Supernode=1;
        Nodo *parent;

        vector<vector<double>> m_data_children;           /// Valores
        vector<Nodo *> children;
        vector<double> lowerBound;
        vector<double> upperBound;
    
        Nodo()
        {
            this->is_leaf = false;           //No sera hoja , por defecto
            Nodo *parent = nullptr;
        }

        Nodo(vector<double> point)
        {
            this->is_leaf = true;           //Dado que tiene dato (valores)
            this->m_data_children.push_back(point);
            this->lowerBound = point;
            this->upperBound = point;
        }
        
        Nodo(Nodo *n)
        {
            this->is_leaf = false;          
            this->children.push_back(n);
            this->lowerBound = n->lowerBound;
            this->upperBound = n->upperBound;
        }
        
        double cal_MBR(vector<double> a,vector<double> b)
        {
            double v = 1.0;
            for(int i = 0;i < NUM_DIM;i++)            // VE
            {
                v = v * abs(b[i] - a[i]);
            }
            return v;
        }

        double cal_MBR()
        {
            return cal_MBR(upperBound,lowerBound);
        }

        double DefInsData(vector<double> &point)
        {
            vector<double> _lowerBound = this->lowerBound;
            vector<double> _upperBound = this->upperBound;

            for(int i = 0; i < NUM_DIM;i++)
            {
                if(_lowerBound[i] > point[i])
                {
                    _lowerBound[i] = point[i];
                }
                if(_upperBound[i] < point[i])
                {
                    _upperBound[i] = point[i];
                }
            }

            return cal_MBR(_lowerBound,_upperBound);
        }

        double DefInsData(Nodo n)  //
        {
            vector<double> _lowerBound = this->lowerBound;
            vector<double> _upperBound = this->upperBound;

            for(int i = 0 ; i < NUM_DIM;i++)
            {
                if(_lowerBound[i] > n.lowerBound[i])
                {
                    _lowerBound[i] = n.lowerBound[i];
                }
                if(_upperBound[i] < n.lowerBound[i])
                {
                    _upperBound[i] = n.lowerBound[i];
                }

                if(_lowerBound[i] > n.upperBound[i])
                {
                    _lowerBound[i] = n.upperBound[i];
                }
                if(_upperBound[i] < n.upperBound[i])
                {
                    _upperBound[i] = n.upperBound[i];
                }
            }
            return cal_MBR(_lowerBound,upperBound);
        }

        void updateMBR(const vector<double> point)
        {
            for(long i = 0; i < NUM_DIM;i++)
            {
                if(this->lowerBound[i] > point[i])
                {
                    this->lowerBound[i] = point[i];
                }
                if(this->upperBound[i] < point[i])
                {
                    this->upperBound[i] = point[i];
                }
            }
        }

        void updateMBR(Nodo *n)
        {
            for(int i=0;i<NUM_DIM;i++)   //////////VERIFICA
            {

                if(this->lowerBound[i] > n->lowerBound[i])
                {
                    this->lowerBound[i] = n->lowerBound[i];
                }
                if(this->upperBound[i] < n->lowerBound[i])
                {
                    this->upperBound[i] = n->lowerBound[i];
                }

                if(this->lowerBound[i] > n->upperBound[i])
                {
                    this->lowerBound[i] = n->upperBound[i];
                }

                if(this->upperBound[i] < n->upperBound[i])
                {
                    this->upperBound[i] = n->upperBound[i];
                }
            }
        }

        double getDistanceOfPoint(vector<double> _upperPoint,vector<double> _lowerPoint,vector<double> point)
        {
            double distancia = 0.0;
            double component = 0.0;

            for(int i = 0 ; i < NUM_DIM ; i++) // Se comprueba donde se encuentra para cada proyección.
            {
                if((point[i] >= _lowerPoint[i]) and (point[i] <= _upperPoint[i]))
                {
                    component = 0.0;
                }
                else if(point[i] < _lowerPoint[i])
                {
                    component = _lowerPoint[i] - point[i];
                }
                else
                {
                    component = point[i] - _upperPoint[i];
                }

                distancia += component * component;
            }
            return sqrt(distancia);
        }
        
        double getDistanceOfPoint(vector<double> point)
        {
            return getDistanceOfPoint(this->upperBound,this->lowerBound,point);
        }
};

class cmp
{
    public:
        bool operator()(pair<Nodo*,double> a,pair<Nodo*,double> b)
        {
            return a.second > b.second;
        };
};

class Xtree
{
    public:
        Nodo *m_head;
    
        int M;
        Xtree(int m)
        {
            if(m > 100)
            {
                this->M = m; 
                this->m_head = nullptr;
            }
            else
            {
                cout<<"Proporcione un M mas alto"<<endl;
                exit(0);
            }
        }

        double Distancia(const vector<double> &a, const vector<double> &b)
        {
            double distancia = 0.0;
            for(int i = 0;i<NUM_DIM;i++)                                      //VERIFICAR!!!!!!!!!!!!
            {       
                //distancia = distancia + pow(a.data[i]-b.data[i],2);
                distancia = distancia + abs((b[i]-a[i])*(b[i]-a[i]));
            }

            return sqrt(distancia);
        }

        Nodo* chooseLeaf(Nodo *node, vector<double> elem)
		{
			if(node->is_leaf)
				return node;

            double best_area = numeric_limits<double>::max();
            double temp_Area = 0.0;

            
			for (int i = 0; i < node->children.size(); i++)
			{
				temp_Area = node->children[i]->DefInsData(elem) - node->children[i]->cal_MBR();

				if (temp_Area <= best_area)
				{   
					best_area = temp_Area;
					node = node->children[i];
				}
			}
            node->updateMBR(elem);
			return chooseLeaf(node, elem);
		}

        void Select_picks__(Nodo* n,int &a,int &b)
        {
            a = 0;
            b = 0;
            double min_distance_lowerBound = numeric_limits<double>::max();//
            double min_distance_upperBound = numeric_limits<double>::max();//
            double temp_area = 0.0;
            //cout<<"Point: "<<n->m_data_children.size()<<endl;
            for(int i=0 ; i<n->m_data_children.size() ; i++)
            {
                temp_area = Distancia(n->lowerBound,n->m_data_children[i]);
                if(temp_area < min_distance_lowerBound)
                {
                    min_distance_lowerBound = temp_area;
                    a = i;
                }

                temp_area = Distancia(n->upperBound,n->m_data_children[i]);
                if(temp_area < min_distance_upperBound)
                {
                    if(i != a)
                    {
                        min_distance_upperBound = temp_area;
                        b = i;
                    }
                }
            }
        }
        
/*
        int Insert(vector<double> obj,Nodo **new_nodo)
        {
            Nodo *s1,*s2;
            Nodo *follow , *new_son;
            int return_value;

            follow = choose_subtree(obj); //Inplementar
            return_value = follow->insert(obj,&new_son);
        }
*/
        void insert(vector<double> point)
        {
            Nodo** n_p = &m_head;

            if((*n_p) == nullptr)
            {
                *n_p = new Nodo(point);
                return;
            }
            else
            {
                (*n_p) = chooseLeaf((*n_p),point);

                (*n_p)->m_data_children.push_back(point); ///Check Point..................  
                (*n_p)->updateMBR(point);

                if(this->M*(*n_p)->is_Supernode < (*n_p)->m_data_children.size())
                {
                    //cout<<"Divionnnnnn"<<endl;
                    Split_node(n_p);
                }  
                CheckParent(this->m_head);
                return;             
            }
        }

        void CheckParent(Nodo *node)
        {
            if(node == nullptr and node->is_leaf)
                return;
            
            for(int i=0;i<node->children.size();i++)
            {
                node->children[i]->parent = node;
                CheckParent(node->children[i]);
            }
        }

        void Split_node(Nodo** nodo)
        {
            if((*nodo)->is_leaf)   
            {
                int Seed_1 = 0;
                int Seed_2 = 0;
                
                Select_picks__(*nodo,Seed_1,Seed_2);
                vector<double> p_s_I = (*nodo)->m_data_children[Seed_1];
                vector<double> p_s_II = (*nodo)->m_data_children[Seed_2];

                Nodo* regionLeft = new Nodo(p_s_I);  
                Nodo* regionRight = new Nodo(p_s_II);

                for(int i =0;i<(*nodo)->m_data_children.size();i++)   
                {
                    if( i == Seed_1  || i == Seed_2) continue;

                    double area_1 = regionLeft->DefInsData((*nodo)->m_data_children[i]) - regionLeft->cal_MBR();
                    double area_2 = regionRight->DefInsData((*nodo)->m_data_children[i]) - regionRight->cal_MBR();

                    if(area_1 < area_2)
                    {
                        regionLeft->m_data_children.push_back((*nodo)->m_data_children[i]);
                        regionLeft->updateMBR((*nodo)->m_data_children[i]);
                    }
                    else
                    {
                        regionRight->m_data_children.push_back((*nodo)->m_data_children[i]);
                        regionRight->updateMBR((*nodo)->m_data_children[i]);
                    }                    
                }

                if((*nodo) == m_head)  
                {
                    (*nodo)->is_leaf = false;
                    (*nodo)->m_data_children.clear();
                    regionLeft->parent = *nodo;
                    regionRight->parent = *nodo;
                    (*nodo)->children.push_back(regionLeft);
                    (*nodo)->children.push_back(regionRight); //UPDATE REGION
                }
                else
                {
                    (*nodo)->is_leaf = false; //AAD
                    (*nodo)->children.clear(); //ADD
                    Nodo *padre = (*nodo)->parent;
                    regionLeft->parent = padre;
                    regionRight->parent = padre;

                    for(int i=0;i<padre->children.size();i++)
                    {
                        if(padre->children[i] == (*nodo))
                        {
                            //cout<<"Ingreso!!!!!!!!"<<endl;
                            delete padre->children[i];
                            padre->children.erase(padre->children.begin()+i);
                            i = padre->children.size();
                        }
                    }

                    padre->children.push_back(regionLeft);
                    padre->children.push_back(regionRight);

                    bool adjust_tree = true;
                    while(adjust_tree and padre)
                    {
                        
                        if(!(padre->children.size() <= this->M*(padre->is_Supernode)))
                        {
                            adjust_tree = false;
                        }
                        else
                        {
                            padre = padre->parent;
                        }
                    }
                    //padre->is_leaf = false;
                    if(!adjust_tree)
                    {
                        this->Split_node(&padre);  
                    }
                    else
                    {return;}
                }
            }    
            else
            {
             
                int Seed_1 = 0;
                int Seed_2 = 0;   
                
                Select_picks__(*nodo,Seed_1,Seed_2);
                Nodo* regionLeft = new Nodo((*nodo)->children[Seed_1]);
                Nodo* regionRight = new Nodo((*nodo)->children[Seed_2]);

                for(int i=0;i<(*nodo)->children.size();i++)
                {
                    if(i==Seed_1 || i==Seed_2) continue;
                    
                    double area_1 = regionLeft->DefInsData((*nodo)->children[i]) - regionLeft->cal_MBR();
                    double area_2 = regionRight->DefInsData((*nodo)->children[i]) - regionRight->cal_MBR();
                    
                    if(area_1 < area_2)
                    {
                        regionRight->children.push_back((*nodo)->children[i]);
                        regionRight->updateMBR((*nodo)->children[i]);
                    }
                    else
                    {
                        regionLeft->children.push_back((*nodo)->children[i]);
                        regionLeft->updateMBR((*nodo)->children[i]);
                    }
                }

                if(!this->Overlap(regionLeft,regionRight))
                {
                    if(*nodo == this->m_head)
                    {
                        (*nodo)->children.clear();
                        regionLeft->parent = *nodo;
                        regionRight->parent = *nodo;
                        (*nodo)->children.push_back(regionLeft);
                        (*nodo)->children.push_back(regionRight);
                    }
                    else
                    {
                        Nodo* padre = (*nodo)->parent;
                        regionLeft->parent = padre;
                        regionRight->parent = padre;
                        for(int i=0;i<padre->children.size();i++)
                        {
                            if(padre->children[i]==(*nodo))
                            {
                                delete padre->children[i];
                                padre->children.erase(padre->children.begin()+i);
                            }
                        }
                        padre->children.push_back(regionLeft);
                        padre->children.push_back(regionRight);

                        bool adjust_tree = true;
                        while(adjust_tree and padre)
                        {
                            
                            if(!(padre->children.size() <= this->M*(padre->is_Supernode)))
                            {
                                adjust_tree = false;
                            }
                            else
                            {
                                padre = padre->parent;
                            }
                        }
                        if(!adjust_tree)
                        {
                            this->Split_node(&padre);
                        }
                        else{return;}
                    }
                }
                else
                {
                    (*nodo)->children.push_back(regionLeft);
                    (*nodo)->children.push_back(regionRight);

                    (*nodo)->is_Supernode++;
                    Nodo* padre = (*nodo)->parent;
                    bool adjust_tree = true;

                    while(adjust_tree and padre)
                    {
                        
                        if(!(padre->children.size() <= this->M*(padre->is_Supernode)))
                        {
                            adjust_tree = false;
                        }
                        else
                        {
                            padre = padre->parent;
                        }
                    }

                    if(!adjust_tree)
                    {
                        this->Split_node(&padre);
                    }
                    else{return;}
                }
            }
        }

      bool pick_next(Nodo* region_1,Nodo* region_2,const vector<double>& point)
        {
            double area_1 = region_1->DefInsData(point) - region_1->cal_MBR();
            double area_2 = region_2->DefInsData(point) - region_2->cal_MBR();

            if(area_1 < area_2)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        // Superposicon entre regiones 
        bool Overlap(Nodo* region1,Nodo* region2)
        {
            for(int i=0;i<NUM_DIM;i++)
            {
                if((region2->lowerBound[i] < region1->upperBound[i]) or (region1->upperBound[i] > region2->lowerBound[i]))
                {
                    return true; //HAY OVERLAP
                }
            }

            return false; //NO HAY OVERLAP
        }

        //Busqueda

        void kNN_Search(Nodo *root,vector<double> point,int k)
        {

            priority_queue<pair<Nodo*,double> , vector<pair<Nodo*,double>> , cmp> queue;

            queue.push(make_pair(root,0.0));

            int count=0;
            while(!queue.empty() and count < k)
            {
                Nodo*  currentNodo = queue.top().first;
                queue.pop();
            
                if(currentNodo->is_leaf)
                {
                    for(int i=0;i < currentNodo->m_data_children.size() and count < k;i++)
                    {
                        for(int j=0;j<currentNodo->m_data_children[i].size();j++)
                            cout<<currentNodo->m_data_children[i][j]<<",";
                        cout<<"\n"<<endl;
                        count++;
                    }
                }
                else
                {
                    //for(auto childIndex : currentNodo->m_data)
                    for(int i=0;i<currentNodo->children.size();i++)
                    {
                        // Nodo *tempNodo = new Nodo(childIndex);
                        Nodo *tempNodo = currentNodo->children[i];
                     
                        queue.push(make_pair(tempNodo,tempNodo->getDistanceOfPoint(point)));
                    }
                }
                if(currentNodo != m_head)
                {
                    delete currentNodo;
                }
            }
        }
};


/*
int main()
{
    return 0;
}*/