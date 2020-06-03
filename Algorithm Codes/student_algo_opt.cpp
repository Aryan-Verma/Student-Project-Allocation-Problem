// This is an optimised C++ implementation of Student-oriented Algorithm for Student-Project Allocation problem (SPA) from the research paper- 
// "D.J. Abraham, R.W. Irving, D.F. Manlove, Two algorithms for the Student-Project Allocation problem, Journal of Discrete Algorithms 5 (1) (2007) 73–90".

// By- Aryan Verma, B.Tech CSE, IIT Indore

// Assumption- There is an upper bound to the number of projects to be included in the project preference list of each student, and that number (u_bound) can be of user's choice.

// Please enter -1 at the end of project list for each student, and similarly for student and project lists for each lecturer.

// All the entries in each list are zero-indexed.

// Optimisation is done in the delete operation.

#include<bits/stdc++.h>
using namespace std;
int s_no, l_no, p_no, u_bound;


int worst_student1(int pj, vector<vector<int> > student_preference, int cnt_student_assigned[]){
// To find the worst student assigned to project pj according to L^j_k
	int worst_stud;
	for(int i=0; i < student_preference[pj].size(); i++){
		if(cnt_student_assigned[student_preference[pj][i]]){
			worst_stud = student_preference[pj][i];
		}
	}
	return worst_stud;
}


int worst_student2(int lk, vector<vector<int> > lecturer, int cnt_student_assigned[]){
// To find the worst student assigned to lecturer lk according to the student preference list of that lecturer
	int worst_stud;
	for(int i=0; i < lecturer[lk].size(); i++){
		if(cnt_student_assigned[lecturer[lk][i]]){
			worst_stud = lecturer[lk][i];
		}
	}
	return worst_stud;
}


vector<int> spa_student(vector<vector<int> > student, vector<vector<int> > lecturer, vector<int> lecturer_project, vector<vector<int> > student_preference, vector<vector<int> > hash_stud_pref, vector<vector<int> > hash_stud_pref_check, int proj_cap[], int lec_cap[]){

	vector<int> proj_assign(s_no);  //project assigned to each student
	for(int i=0; i < s_no; i++) proj_assign[i] = -1;
	//Initially each student is free and each project and lecturer are totally unsubscribed
	int cnt_student_assigned[s_no]={0};   //to check if a student is assigned or not
	int cnt_proj[p_no]={0};  //number of students assigned to each project
	int cnt_lect[l_no]={0};  //number of students assigned to each lecturer
	int pj, lk;

	for(int iter=0; iter < u_bound; iter++){   //as a student can have a preference of maximum (u_bound) number of projects
		for(int i=0; i < s_no; i++){
			if(!cnt_student_assigned[i] and student[i].size() > 0){  //if some student s_i is free and has a non-empty list of projects preference
				pj = student[i][0];   // first project on s_i's list
				lk = lecturer_project[student[i][0]];  //lecturer who offers project pj
				proj_assign[i] = pj;  //provisional assign s_i to pj
				cnt_student_assigned[i] = 1;   //1 indicates the student is assigned
				cnt_proj[pj]++;
				cnt_lect[lecturer_project[pj]]++;

				int sr, pt, ind;
				if(cnt_proj[pj] > proj_cap[pj]){  // (pj is over-subscribed)
					sr = worst_student1(pj, student_preference, cnt_student_assigned);  //sr = worst student assigned to pj (according to L^j_k)
					
					//Breaking provisional assignment between sr and pj
					cnt_student_assigned[sr] = 0;
					cnt_proj[pj]--;
					cnt_lect[lecturer_project[pj]]--;
					proj_assign[sr] = -1;
					for(int j=0; j < student_preference[pj].size(); j++){
						if(student_preference[pj][j] == sr){
							ind = j;
							break;
						}
					}
					student_preference[pj].erase(student_preference[pj].begin() + ind);  //removing sr from L^j_k
					for(int j=0; j < student[sr].size(); j++){
						if(student[sr][j] == pj){
							ind = j;
							break;
						}
					}
					student[sr].erase(student[sr].begin() + ind);  //removing pj from project preference list of sr
				}

				else if(cnt_lect[lk] > lec_cap[lk]){  // (lk is over-subscribed)
					sr = worst_student2(lk, lecturer, cnt_student_assigned);  //sr = worst student assigned to lk
					pt = proj_assign[sr];  //project assigned to sr

					//Breaking provisional assignment between sr and pt
					cnt_student_assigned[sr] = 0;
					cnt_proj[pt]--;
					cnt_lect[lecturer_project[pt]]--;
					proj_assign[sr] = -1;
					for(int j=0; j < student_preference[pt].size(); j++){
						if(student_preference[pt][j] == sr){
							ind = j;
							break;
						}
					}
					student_preference[pt].erase(student_preference[pt].begin() + ind);  //removing sr from L^j_k
					for(int j=0; j < student[sr].size(); j++){
						if(student[sr][j] == pt){
							ind = j;
							break;
						}
					}
					student[sr].erase(student[sr].begin() + ind);  //removing pt from preference list of sr
				}

				int ind_sr;
				if(cnt_proj[pj] == proj_cap[pj]){  // (pj is full)
					sr = worst_student1(pj, student_preference, cnt_student_assigned);  //sr = worst student assigned to pj (according to L^j_k)

					for(int j=0; j < student_preference[pj].size(); j++){
						if(student_preference[pj][j] == sr){
							ind_sr = j;   //index of sr in L^j_k
							break;
						}
					}

					//Deleting (st,pj) pairs for each successor st of sr on L^j_k
					for(int j=ind_sr+1; j < student_preference[pj].size(); j++){
						for(int k=0; k < student[student_preference[pj][j]].size(); k++){
							if(student[student_preference[pj][j]][k] == pj){
								ind = k;
								break;
							}
						}
						student[student_preference[pj][j]].erase(student[student_preference[pj][j]].begin() + ind);
						student_preference[pj].erase(student_preference[pj].begin() + ind_sr + 1);
					}
				}

				if(cnt_lect[lk] == lec_cap[lk]){  // (lk is full)

					sr = worst_student2(lk, lecturer, cnt_student_assigned);  //sr = worst student assigned to lk

					for(int j=0; j < lecturer[lk].size(); j++){
						if(lecturer[lk][j] == sr){
							ind_sr = j;
							break;
						}
					}

					//Deleting (st,pu) pairs for each successor st of sr in lecturer list such that pu belongs to all projects under lk which st finds acceptable
					for(int j=ind_sr+1; j < lecturer[lk].size(); j++){
						for(int k=0; k < student[lecturer[lk][j]].size(); k++){
							if(lecturer_project[student[lecturer[lk][j]][k]] == lk){
								if(hash_stud_pref_check[student[lecturer[lk][j]][k]][lecturer[lk][j]] == 1){
									hash_stud_pref_check[student[lecturer[lk][j]][k]][lecturer[lk][j]] == 0;
									int l = hash_stud_pref[student[lecturer[lk][j]][k]][lecturer[lk][j]];
									student_preference[student[lecturer[lk][j]][k]].erase(student_preference[student[lecturer[lk][j]][k]].begin() + l);
								}
								student[lecturer[lk][j]].erase(student[lecturer[lk][j]].begin() + k);
                                k--;
							}
						}
                        lecturer[lk].erase(lecturer[lk].begin() + ind_sr + 1);
					}
				}

			}
		}
	}
	return proj_assign;
}


int main(){

	cout<<"Enter the number of students: ";
	cin>>s_no;
	cout<<"Select maximum number of projects allowed to be included in project preference list: ";
	cin>>u_bound;
	vector<vector<int> > student(s_no);
	for(int i=0; i < s_no; i++){
		cout<<"Enter project preference list (not more than maximum number of projects permissible) of student "<<i<<" : ";
		int a=0;
		while(a!=-1){
			cin>>a;
			if(a!=-1) student[i].push_back(a);
		}
	}

	cout<<"Enter the number of lecturers: ";
	cin>>l_no;
	vector<vector<int> > lecturer(l_no);
	for(int i=0; i < l_no; i++){
		cout<<"Enter student preference list of lecturer "<<i<<" : ";
		int a=0;
		while(a!=-1){
			cin>>a;
			if(a!=-1) lecturer[i].push_back(a);
		}
	}

	cout<<"Enter the number of projects: ";
	cin>>p_no;
	vector<int> lecturer_project(p_no);  //index indicates project number and entry indicates lecturer number
	for(int i=0; i < l_no; i++){
		cout<<"Enter the projects offered by lecturer "<<i<<" : ";
		int a=0;
		while(a!=-1){
			cin>>a;
			if(a!=-1) lecturer_project[a]=i;
		}
	}

	vector<vector<int> > student_preference(p_no);  //for the preference order of students for each project (L^j_k)
	for(int i=0; i < p_no; i++){
		for(int j=0; j < lecturer[lecturer_project[i]].size(); j++){
			for(int k=0; k < student[lecturer[lecturer_project[i]][j]].size(); k++){
				if(student[lecturer[lecturer_project[i]][j]][k]==i){
					student_preference[i].push_back(lecturer[lecturer_project[i]][j]);
					break;
				}
			}
		}
	}

	// Optimised version - using hashing table to reduce time taken by delete operation
	vector<vector<int> > hash_stud_pref(p_no+5, vector<int> (s_no+5,0));  // hashing table for student_preference list
	vector<vector<int> > hash_stud_pref_check(p_no+5, vector<int> (s_no+5,0));  //hashing table to check if a student is included
	// for(int i=0;i<p_no;i++){
	// 	for(int j=0;j<s_no;j++){
	// 		hash_stud_pref[i][j][1]=0;
	// 	}
	// }
	for(int i=0;i<p_no;i++){
		for(int j=0;j<s_no;j++){
			hash_stud_pref_check[i][student_preference[i][j]]=1;  // if that student is included
			hash_stud_pref[i][student_preference[i][j]]=j;  // storing student_preference list position of that student in hash table 
		}
	}


	int proj_cap[p_no], lec_cap[l_no];

	cout<<"Enter student capacity of each project: ";
	for(int i=0; i < p_no; i++) cin>>proj_cap[i];

	cout<<"Enter student capacity of each lecturer: ";
	for(int i=0; i < l_no; i++) cin>>lec_cap[i];

	vector<int> proj_assign = spa_student(student, lecturer, lecturer_project,student_preference, hash_stud_pref, hash_stud_pref_check, proj_cap, lec_cap);
	
	cout<<endl<<endl;
	cout<<"Project and lecturer assigned to each student are: "<<endl;
	cout<<"Student - Project - Lecturer"<<endl;
	for(int i=0; i < s_no; i++){
		if(proj_assign[i] != -1){
			cout<<i<<"         "<<proj_assign[i]<<"         "<<lecturer_project[proj_assign[i]]<<endl;
		}
	}
}