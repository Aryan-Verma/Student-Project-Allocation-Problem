#include<bits/stdc++.h>
using namespace std;
struct Project{

    int pno;
    int c;      // capacity
};
struct Student{
    int sno;
    vector<int> sp;  //Student Preference of Projects
};
struct Lecturer{
	 
	 int Lno;
     int d;          //capacity
     vector<Student> lp;   // Lecturer Preference of Students
     vector<int> lps;   //Lecturer set of Projects
};	
void SPA_instance(Lecturer &L,vector<Lecturer> &Lec,vector<pair<int,pair<int,int>>> &M,vector<bool> &visited,int Sno,int Pno,vector<Project>&P)
{
    int flag=0,flag2=0,z,x;
    for(int j=0;j<L.lp.size();j++)
    {
        if(Sno==L.lp[j].sno)   //The student who got new project pj to p
        {
            for(int k=j+1;k<L.lp.size();k++)        // list of students
            {
                for(int f=0;f<L.lp[k].sp.size();f++)   //list of projects of particular student
                {
                    if(P[L.lp[k].sp[f]-1].pno==Pno)               //when project matches with some student
                    {
                         flag=1;
                        if(!visited[L.lp[k].sno])
                        {
                            visited[L.lp[k].sno]=true;
                            M.push_back(make_pair(L.Lno,make_pair(L.lp[k].sno,Pno)));  //add to M
                            L.lp[k].sp.erase(L.lp[k].sp.begin()+f,--(L.lp[k].sp.end()));       // erase upto the last
                            P[L.lp[k].sp[f]-1].c--;  //capacity decrement by one 
                            L.d--;
                        }
                        else
                        {
                            for(int a=0;a<M.size();a++)
                            {
                                if(M[a].second.first==L.lp[k].sno)
                                {   
                                    if(P[M[f].second.second-1].c>0)
                                         flag2=1; 
                                    
                                    
                                    P[M[f].second.second-1].c++;
                                    z=M[a].second.second;     // original project assigned
                                    x=M[a].first-1;           // original lecturer assigned
                                    Lec[x].d++;
                                    M[a].first=L.Lno;         //New Lecturer assigned
                                    M[a].second.second=Pno;      //New project assigned
                                    L.d--;
                                    P[Pno-1].c--;
                                    L.lp[k].sp.erase(L.lp[k].sp.begin()+f,--(L.lp[k].sp.end()));  //erase less preference projects
                                                    
                                    
                                    if(M[a].second.first!=Lec[x].lp[Lec[x].lp.size()-1].sno&& flag2==0)     //if sno is not of last student for a particular lecturer
                                       SPA_instance(Lec[x],Lec,M,visited,M[a].second.first,z,P);   //call that function again for newly generated project in set of some lecturer
                                     
                                     break;  //to keep out checking in M
                                }
                            }
                        }
                        break;   //as soon as assignment done then there is no need to check further for project of assigned student
                    
                    }
    
                }
              if(flag==1)
                 break;    //no need to check further for students because we already call for SPA_instance
            }
        }
        if(flag==1)
        break;    // to keep out from the outermost loop
    }
}

vector<pair<int,pair<int,int>>> SPA_L(vector<Lecturer> L,vector<Student> S,vector<Project>P)
{
	int flag,b,np,flag2=0;
	vector<bool> visited(S.size()+1,false);
	vector<pair<int,pair<int,int>>> M;
	
    for(int i=0;i<L.size();i++)          //For Lecturer
    {
    	for(int j=0;j<L[i].lp.size();j++)     // loop for students list
    	{
            if(L[i].d>0)
            {
        		 flag=0;
                
        		for(int k=0;k<L[i].lp[j].sp.size();k++)      //loop for particular student project preference list
        		{
        			for(int l=0;l<L[i].lps.size();l++)        // set of lecturer project 
        			{

        				if((L[i].lps[l]==L[i].lp[j].sp[k]) && P[L[i].lp[j].sp[k]-1].c>0)
        				{
        					   flag=1;
        					if(!visited[L[i].lp[j].sno])
        					{
        						visited[L[i].lp[j].sno]=true;
        					    M.push_back(make_pair(L[i].Lno,make_pair(L[i].lp[j].sno,L[i].lps[l])));
                                P[L[i].lp[j].sp[k]-1].c--;
                                L[i].d--;
        					    L[i].lp[j].sp.erase(L[i].lp[j].sp.begin()+k,--(L[i].lp[j].sp.end()));
        					    
        					} 
        					else
        					{
        						for(int f=0;f<M.size();f++)   //checking for student that is already visited
        						{
        							if(M[f].second.first==L[i].lp[j].sno)  // when match found
        							{
                                        if(P[M[f].second.second-1].c>0)
                                         flag2=1;        //if capacity of project is not zero that means it cannot be used further or less demanded project
                                        
                                           P[M[f].second.second-1].c++;
                                           L[M[f].first-1].d++; //assignment deleted
            							   b=M[f].first;     // original lecturer number
            							   np=M[f].second.second;  // original project number
            							   M[f].first=L[i].Lno;     // assign new lecturer
                                           L[i].d--;                     //decrement capacity by one
            						     	M[f].second.second=P[L[i].lps[l]-1].pno;   //assign new project
                                            P[L[i].lps[l]-1].c--;
            							    L[i].lp[j].sp.erase(L[i].lp[j].sp.begin()+k,--(L[i].lp[j].sp.end()));    //deletion of preference list of student after assignment 
                                            if(flag2==0&&L[b-1].lp[L[b-1].lp.size()-1].sno!=M[f].second.first)
                                            {
                                                SPA_instance(L[b-1],L,M,visited,L[i].lp[j].sno,np,P);
        								        break;
                                            }
                                    
                                    } 
        						}
        					}
                            break;
        				}
        			}
        		  if(flag==1)
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
    cout<<"Next "<<Num_Student<<" lines will take input int the format Student_No,number of projects student have,Project Names respectively"<<endl;
    
    for(int i=0;i<Num_Student;i++)
    {
       cin>>Student_No>>Num_Proj_Student;
       
       S[i].sno=Student_No; 
       for(int j=0;j<Num_Proj_Student;j++)
       {
       	    cin>>Proj_Name;                   //Proj_Name Preference wise
            S[i].sp.push_back(Proj_Name);  
       }
    }
    cout<<"Total Number Of Lecturers: ";
    cin>>Num_Lec;   
    vector<Lecturer> L(Num_Lec);  // Lecturer Set

    cout<<"Next "<<Num_Lec<<" lines will take input in the format Lec_No,capacity,Number of Students ,Num Projects Lec have,Students list and Project set respectively"<<endl;
    for(int i=0;i<Num_Lec;i++)
    {
        cin>>Lec_No>>capacity_Lec>>Num_Student_Lec>>Num_Proj_Lec;

        L[i].Lno=Lec_No;
        L[i].d=capacity_Lec;

        for(int j=0;j<Num_Student_Lec;j++)
        {
        	cin>>Student_Num_Lec;
        	L[i].lp.push_back(S[Student_Num_Lec-1]);   //List of Students Preference wise
        }
        for(int k=0;k<Num_Proj_Lec;k++)
        {
        	cin>>Proj_Lec;

        	L[i].lps.push_back(Proj_Lec);    // List of Projects Lecturer Have...
        }

    }   
     vector<pair<int,pair<int,int>>> M= SPA_L(L,S,P);
     cout<<"Lno"<<"\t"<<"Sno"<<"\t"<<"Pno"<<endl;
     for(int i=0;i<M.size();i++)
     {
          cout<<M[i].first<<"\t"<<M[i].second.first<<"\t"<<M[i].second.second<<endl;
     }

    return 0;
}