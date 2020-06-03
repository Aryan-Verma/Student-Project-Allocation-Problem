   // Student can take only maximum of 1 project 
#include<bits/stdc++.h>
using namespace std;
struct Project{

    int pno;
    int c;      // capacity
};
struct Student{
    int sno;          //Student number
    vector<int> sp;  //Student Preference of Projects
};
struct Lecturer{
	 
	 int Lno;      //Lecturer Number
     int d;          //capacity
     vector<Student> lp;   // Lecturer Preference of Students
     unordered_map<int,bool> lps;   //Lecturer set of Projects
};	
void SPA_instance(Lecturer &L,vector<Lecturer> &Lec,unordered_map<int,pair<int,int>> &M,vector<bool> &visited,int Sno,int Pno,vector<Project> &P)
{
    int flag=0,flag2=0,np,b,index;

    for(int j=0;j<L.lp.size();j++)
    {
        if(Sno==L.lp[j].sno)                                           //The student who got new project pj to p
        {
            index=j+1;
            break;
        }
    }

    for(int k=index;k<L.lp.size();k++)                         // list of students
    {
        for(int f=0;f<L.lp[k].sp.size();f++)                //list of projects of particular student
       {
           if(P[L.lp[k].sp[f]-1].pno==Pno)               //when project matches with some student
           {
                flag=1;
                if(!visited[L.lp[k].sno-1])
                {
                    visited[L.lp[k].sno-1]=true;
                    M[L.lp[k].sno]=make_pair(L.Lno,Pno);                               //add to M
                    L.lp[k].sp.erase(L.lp[k].sp.begin()+f,--(L.lp[k].sp.end()));      // erase upto the last
                    P[L.lp[k].sp[f]-1].c--;                                          //capacity decrement by one 
                    L.d--;
                }
                else
                {
                            
                    if(P[ M[L.lp[k].sno].second-1].c>0)
                        flag2=1; 
                                                 
                    P[ M[L.lp[k].sno].second-1].c++;
                           
                    np = M[ L.lp[k].sno].second;                     // original project assigned
                           
                    b= M[ L.lp[k].sno].first;                        // original lecturer assigned
                           
                    Lec[b-1].d++;
                           
                    M[L.lp[k].sno].first=L.Lno;                  //New Lecturer assigned
                           
                    M[L.lp[k].sno].second=Pno;                  //New project assigned
                           
                    L.d--;
                           
                    P[Pno-1].c--;
                            
                    //erase less preference projects
                    L.lp[k].sp.erase(L.lp[k].sp.begin()+f,--(L.lp[k].sp.end()));  
                                                    
                    //if sno is not the last student for a particular lecturer
                    //call that function again for newly generated free project in the set of some lecturer
                    if(L.lp[k].sno!=Lec[b-1].lp[Lec[b-1].lp.size()-1].sno && flag2==0)     
                       SPA_instance(Lec[b-1],Lec,M,visited,L.lp[k].sno,np,P);   
                                     
                }
            } 
            
          break;   //as soon as assignment done then there is no need to check further for project of assigned student
        }
        if(flag)
         break;    //no need to check further for students because we already call for SPA_instance
    }
}

unordered_map<int,pair<int,int>> SPA_L(vector<Lecturer> L,vector<Student> S,vector<Project>P)
{
	int flag,b,np,flag2=0;
	vector<bool> visited(S.size(),false);  
	unordered_map<int,pair<int,int>> M;    // Format will be Sno, {LNo, Pno}
	
        for(int i=0;i<L.size();i++)          //For Lecturer
       {
    	   for(int j=0;j<L[i].lp.size();j++)     // loop for students list
    	   {
                if(L[i].d>0)
                {
        		   for(int k=0;k<L[i].lp[j].sp.size();k++)      //loop for particular student project preference list
        		   {
        			    if(L[i].lps.find(L[i].lp[j].sp[k])!=L[i].lps.end() && P[L[i].lp[j].sp[k]-1].c>0)
                        {
                            flag=1;
        					if(!visited[L[i].lp[j].sno-1])
        					{
        						visited[L[i].lp[j].sno-1]=true;
        					    M[L[i].lp[j].sno]=make_pair(L[i].Lno,L[i].lp[j].sp[k]);
                                P[L[i].lp[j].sp[k]-1].c--;
                                L[i].d--;
        					    L[i].lp[j].sp.erase(L[i].lp[j].sp.begin()+k,--(L[i].lp[j].sp.end()));  
        					} 
        					else
        					{
        						//if capacity of project is not zero that means it is 
                                //not accepted by the students in the list of lecturer L[b-1] :: b defined below
                                if(P[ M[L[i].lp[j].sno].second-1].c>0)   
                                  flag2=1;                              
                                        
                                P[ M[L[i].lp[j].sno].second-1].c++;
                                
                                L[ M[L[i].lp[j].sno].first-1].d++;                   //assignment deleted 
            		     		
                                b=M[L[i].lp[j].sno].first;                          // original lecturer number
            				    
                                np=M[L[i].lp[j].sno].second;                       // original project number
        					    
                                M[L[i].lp[j].sno].first=L[i].Lno;                // assign new lecturer
                                
                                L[i].d--;                                       //decrement capacity by one
						     	
                                M[L[i].lp[j].sno].second=L[i].lp[j].sp[k];               //assign new project
                                
                                P[L[i].lp[j].sp[k]-1].c--;
    						    
                                L[i].lp[j].sp.erase(L[i].lp[j].sp.begin()+k,--(L[i].lp[j].sp.end()));    
                                //deletion of preference list of student after assignment 
                                

                                 //if newly assigned student is at the last of the list of lecturer them no need 
                                //to check further in lecturer list
                                if(flag2==0&&L[b-1].lp[L[b-1].lp.size()-1].sno!=L[i].lp[j].sno)     
                                {
                                    SPA_instance(L[b-1],L,M,visited,L[i].lp[j].sno,np,P);
                                }
                                
                            } 
    					
                        }
                        if(flag)
                          break;
        			}
        		}
            }
    	}

    return M;	   
}
int main()
{
	int Num_Proj,Num_Student,Num_Lec,Student_No,Num_Proj_Student,Proj_Name;

	int Student_Num_Lec,Proj_Lec,Lec_No,Num_Student_Lec,Num_Proj_Lec,capacity_Proj,capacity_Lec;

	cout<<"Total Number of Projects: ";
	cin>>Num_Proj;
    
    cout<<"Next "<<Num_Proj<<" lines conatains capacity of projects serial number wise"<<endl;

    vector<Project> P(Num_Proj);

    for(int i=0;i<Num_Proj;i++)
    {
        cin>>capacity_Proj;
        P[i].pno=i+1;
        P[i].c=capacity_Proj;
    }

    cout<<"Total Number of Students: ";
    cin>>Num_Student;

    vector<Student> S(Num_Student); // Student Set
    cout<<"Next "<<Num_Student<<" lines will take input int the format number of projects student have,Project Names respectively"<<endl;
    
    for(int i=0;i<Num_Student;i++)
    {
       cin>>Num_Proj_Student;
       
       S[i].sno=i+1; 
       for(int j=0;j<Num_Proj_Student;j++)
       {
       	    cin>>Proj_Name;                   //Proj_Name Preference wise
            S[i].sp.push_back(Proj_Name);  
       }
    }
    cout<<"Total Number Of Lecturers: ";
    cin>>Num_Lec;   
    vector<Lecturer> L(Num_Lec);  // Lecturer Set

    cout<<"Next "<<Num_Lec<<" lines will take input in the format capacity,Number of Students ,Num Projects Lec have,Students list and Project set respectively"<<endl;
    for(int i=0;i<Num_Lec;i++)
    {
        cin>>capacity_Lec>>Num_Student_Lec>>Num_Proj_Lec;

        L[i].Lno=i+1;
        L[i].d=capacity_Lec;

        for(int j=0;j<Num_Student_Lec;j++)
        {
        	cin>>Student_Num_Lec;
        	L[i].lp.push_back(S[Student_Num_Lec-1]);   //List of Students Preference wise
        }
        for(int k=0;k<Num_Proj_Lec;k++)
        {
        	cin>>Proj_Lec;

        	L[i].lps[Proj_Lec]=true;    // List of Projects Lecturer Have...
        }

    }   
     unordered_map<int,pair<int,int>> M= SPA_L(L,S,P);  //calling out the function to find stable matching

     cout<<"Sno"<<"\t"<<"Lno"<<"\t"<<"Pno"<<endl;

     for(auto it=M.begin();it!=M.end();++it)
     {
          cout<<it->first<<"\t"<<it->second.first<<"\t"<<it->second.second<<endl;
     }

    return 0;
}