#include<iostream>
#include<vector>


using namespace std;
int insert_flag = 0;
//creating heap.......................................
void minheapify(int i,vector<pair<int,int>>& nums){
    int n = nums.size();
    while(i<(n/2)){
        int left = nums[2*i+1].first;
        int right = ((2*i+2)<n?nums[2*i+2].first:-1);

        int min_e = nums[i].first;
        if(min_e>left)min_e = left;
        if(right!=-1 && min_e>right)min_e = right;
        if(min_e==left){
            pair<int,int> temp = nums[i];
            nums[i] = nums[2*i+1];
            nums[2*i+1] = temp;
            i = 2*i+1;
        }else if(min_e==right){
            pair<int,int> temp = nums[i];
            nums[i] = nums[2*i+2];
            nums[2*i+2] = temp;
            i = 2*i+2;
        }else
        {   
            insert_flag = 1;
            break;
        }
        
    }
}
void insert(int ele,int index,vector<pair<int,int>>&nums){
    nums.push_back({ele,index});
    int n = nums.size();
    insert_flag = 0;
    for(int i = (n/2)-1;i>=0;i/=2){

        minheapify(i,nums);
        if(i==0 || insert_flag)break;
    }
}
pair<int,int> delete_min(vector<pair<int,int>> & nums){
    pair<int,int> a = nums[0];
    nums[0] = nums[nums.size()-1];
    nums.pop_back();
    minheapify(0,nums);
    return a;
}
void create_min_heap(vector<pair<int,int>>& nums){

    int n = nums.size();

    for(int i = (n/2)-1;i>=0;i--){

        minheapify(i,nums);
    }
}

// merge_sort.........................................................
void merge(int p,int mid,int q,vector<int>&nums){
    vector<int> a(q-p+1);
    int i = p;
    int j = mid+1;
    int k = 0;
    while(i<=mid && j<=q){
        if(nums[i]<nums[j]){
            a[k++] = nums[i++];
        }else
        {
            a[k++] = nums[j++];
        }
        
    }
    while(i<=mid) a[k++] = nums[i++];
    while(j<=q)a[k++] = nums[j++];
    k = 0;
    for(int l = p;l<=q;l++){
        nums[l] = a[k++];
    }
    
}
void merge_sort(int p,int q,vector<int>&nums){

    if(p<q){
        int mid = p + (q-p)/2;
        //cout << p << " " << mid << " " << q << endl;
        merge_sort(p,mid,nums);
        merge_sort(mid+1,q,nums);
        merge(p,mid,q,nums);
    } 
    
}
//refilling container.........
bool refill_container(int limit,vector<vector<int>>& op_container,int op_val,FILE* file_value){
        
        op_container[op_val].clear();
        int j = 0;
        int d;
         while (j<limit)
        {
            if(fscanf(file_value, "%d,", &d) !=EOF){
                op_container[op_val].push_back(d);
                j++;
            }
            else{
                return false;
            }
        }
        return true;
}
int main(){

    int k = 100;
    int size_of_chunk = 100000;
    int count_files = 0;
    FILE* input_file = fopen("input.txt","r");

    FILE* chunk_file;
    
    for(int i = 0;i<k-1;i++){
        vector<int> data;
        int flag = 0;
        int j = 0;
        int d;
        while (j<size_of_chunk)
        {
            if(fscanf(input_file, "%d,", &d) !=EOF){
                data.push_back(d);
                j++;
            }
            else{
                flag = 1;
                break;
            }
        }
        
        
        merge_sort(0,data.size()-1,data);
        
        string fileName = "chunk"+to_string(i)+".txt";
        count_files++;
        chunk_file = fopen(fileName.c_str(),"w");
        for(int  l = 0;l<data.size();l++){
            fprintf(chunk_file,"%d,",data[l]);
        }
        fclose(chunk_file);
        if(flag) break;
        
    }
    int d;
    int j = 0;
    vector<int> data;
    while ((fscanf(input_file, "%d,", &d) !=EOF))
    {
        
        data.push_back(d);
    }
        
        
        if(data.size()>0){
            merge_sort(0,data.size()-1,data);
            //sort(data.begin(),data.end());
            string fileName = "chunk"+to_string(k-1)+".txt";
            count_files++;
            chunk_file = fopen(fileName.c_str(),"w");
            for(int  l = 0;l<data.size();l++){
                fprintf(chunk_file,"%d,",data[l]);
            }
            fclose(chunk_file);

        }
        
    
    fclose(input_file);

    int limit = 50000;

    vector<vector<int>> op_container(count_files);//hold value from each file

    FILE* chunk[count_files];//pointer to each file
    //taking data from each file upto a limit or EOF
    for(int i = 0;i<count_files;i++){
        string f_name = "chunk"+to_string(i)+".txt";
        chunk[i] = fopen(f_name.c_str(),"r");

        int j = 0;
        int d;
         while (j<limit)
        {
            if(fscanf(chunk[i], "%d,", &d) !=EOF){
                op_container[i].push_back(d);
                j++;
            }
            else{
                break;
            }
            
        }
    }

    vector<pair<int,int>> min_heap;// to pick min value each time
    //insering first element of each vector into heap
    for(int i = 0;i<count_files;i++){
        min_heap.push_back({op_container[i][0],i});
    }
    create_min_heap(min_heap);// converting into heap
    
    vector<int> output;//hold final data taht to be inserted into final file
    vector<int> index(count_files,0);// hold index of each vector while traversing

    //
    FILE* output_file = fopen("output.txt","w");
    while (min_heap.size())
    {
        
        while (output.size()<3)
        {   
            pair<int,int> value = delete_min(min_heap);
            output.push_back(value.first);
            
            //increment the pointer of the vector to which min_value of heap belongs to
            index[value.second]++;
            
            //came here as the vector in the container has been traversed and needs to be refilled with new file values
            
            if(index[value.second]>=op_container[value.second].size())
            {
                index[value.second] = 0;
                if(!refill_container(limit,op_container,value.second,chunk[value.second])){
                    fclose(chunk[value.second]);
                    count_files--;
                }
                
            }
            //inserting into min_heap only when vector is not empty
            if(op_container[value.second].size()>0)
            {   
                
                insert(op_container[value.second][index[value.second]],value.second,min_heap);
            }
            if(min_heap.size()==0){
                break;
            }
           
        }

        for(int  l = 0;l<output.size();l++){
            fprintf(output_file,"%d,",output[l]);
        }
        output.clear();

        
    }
    
    fclose(output_file);
}