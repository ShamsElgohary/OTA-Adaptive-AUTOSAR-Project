#include <bits/stdc++.h>    
using namespace std ;


vector<int> initial_permtation_table= {
    58,50,42,34,26,18,10,2,
    60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,
    64,56,48,40,32,24,16,8,
    57,49,41,33,25,17,9,1,
    59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,
    63,55,47,39,31,23,15,7
};

vector<int> permutation_choice_1_table= {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2 ,59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

vector<int> permutation_choice_2_table ={
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32 
    };
 
vector<int> shift_table=
{
    1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};

vector<int> expansion_permutation_table =
{ 
    32, 1, 2, 3, 4, 5, 4, 5,
    6, 7, 8, 9, 8, 9, 10, 11,
    12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25, 24, 25, 26, 27,
    28, 29, 28, 29, 30, 31, 32, 1     
};

vector<int> s1 ={
14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10 ,5, 0,
15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 
};

vector<int> s2 ={
15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9

};

vector<int>s3 ={
10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,
};

vector<int> s4 ={
7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14

};

vector<int> s5 ={
2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3

};

vector<int> s6 ={
12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13

};

vector<int> s7 ={
4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
 
};

vector<int> s8 ={
13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11,
};

vector<vector<int>> s_box_table = {s1,s2,s3,s4,s5,s6,s7,s8};

vector<int> permutation_table ={
16,7,20,21,
29,12,28,17,
1,15,23,26,
5,18,31,10,
2,8,24,14,
32,27,3,9,
19,13,30,6,
22,11,4,25
};

vector <int> inverse_permutation_table ={ 
40, 8, 48, 16, 56, 24, 64, 32,
39, 7, 47, 15, 55, 23, 63, 31,
38, 6, 46, 14, 54, 22, 62, 30,
37, 5, 45, 13, 53, 21, 61, 29,
36, 4, 44, 12, 52, 20, 60, 28,
35, 3, 43, 11, 51, 19, 59, 27,
34, 2, 42, 10, 50, 18, 58, 26,
33, 1, 41, 9, 49, 17, 57, 25 };

vector<int> text_vect(64);
vector<int> key_vect(64);
vector<int> initial_permutation_output(64);
vector<int> permutation_choice_1_output(56);
vector<int> permutation_choice_2_output(48);
vector<int> shift_output(56);
vector<int> expansion_permutation_output(48);
vector<int> xor_output(48);
vector<int> s_box_output(32);
vector<int> permutation_output(32);
vector<int> inverse_permutation_output(64);
vector<int> r(32);
vector<int> l(32);

vector<vector<int>> keys;


void binary_to_dec(unsigned long long x ,vector<int> &output)
{  
    for(int i=0;i <64 ;i++)
    {
        output[63-i] = x%2 ;
        x =x/2;
    }
}

vector <char> h ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void print_hex(vector<int> &v)
{
    unsigned long long value =0;
    string output ="";

    for(int i=0; i<v.size() ;i+=4)
    {
        int value = 0;
        for(int x=i; x<i+4 ;x++)
        {
            value += v[x]*pow(2,(3-(x-i)));
        }
        output += h[value] ;

    }
    cout<<output<<'\n';

}

void left_shift_circular(vector<int> &permutation_choice_1_output,vector<int> &shift_output ,int round)
{   
    round = shift_table[round];
    for(int x=0 ;x<round;x++) 
    {
        for(int i=1 ; i<28 ;i++)
        {
            shift_output[(i-1)] =permutation_choice_1_output[i];
            shift_output[(i-1) +28] =permutation_choice_1_output[i+28];
        }
        shift_output[27] =permutation_choice_1_output[0];
        shift_output[55]=permutation_choice_1_output[28];
        permutation_choice_1_output =shift_output;
    }
}

void Xor(vector<int> &inp1, vector<int> &inp2 ,vector<int> &output)
{
    for(int i=0 ; i<inp1.size() ;i++)
    {
        if(inp1[i]==1)
        {
           output[i]=(inp2[i]==0);
        }
        else{
            output[i]=inp2[i];
        }
    }
}

void s_box(vector<int>&xor_output ,vector<int>&s_box_output)
{
    for(int i = 0 ;i<48 ;i+=6)
    {
        int value=0 ;

            value+= xor_output[i+4]*pow(2,0);
            value+= xor_output[i+3]*pow(2,1);
            value+= xor_output[i+2]*pow(2,2);
            value+= xor_output[i+1]*pow(2,3);
            value+= xor_output[i+5]*pow(2,4);
            value+= xor_output[i]*pow(2,5);

        value = s_box_table[i/6][value];

        int z=(i/6)*4;
        for(int x=0;x <4 ;x++)
        {
            s_box_output[(z+3)-x] = value%2 ;
            value =value/2;
        }
    }
}

void permutaion(vector<int> &input,vector<int> &table ,vector<int> &output)
{
    for(int i=0 ; i<output.size() ;i++)
    {
        output[i] = input[table[i]-1];
    }
}

void round(int round_number ,vector<int>&right ,vector<int>&left)
{
    permutaion(right,expansion_permutation_table, expansion_permutation_output );
    Xor(expansion_permutation_output,keys[round_number],xor_output);
    s_box(xor_output ,s_box_output);
    permutaion(s_box_output,permutation_table ,permutation_output);
    Xor(permutation_output,left,right);
}

void get_keys(unsigned long long key,vector<vector<int>>&keys)
{
    binary_to_dec(key ,key_vect);
    permutaion(key_vect,permutation_choice_1_table,permutation_choice_1_output);
    for(int i=0 ;i<16 ;i++)
    {
        if(i !=0)
            permutation_choice_1_output =shift_output;
        left_shift_circular(permutation_choice_1_output ,shift_output ,i);
        permutaion(shift_output,permutation_choice_2_table ,permutation_choice_2_output);
        keys.push_back(permutation_choice_2_output);
    }
}
int main (int argc , char * argv[])
{   
    if(argc !=4){
        cout<<"input program arguments\n" ;
        return 0;
    }
    string op = argv[1];
    string text_s =argv[2];
    string key_s =argv[3];

    unsigned long long text= stoull(text_s, 0, 16);
    unsigned long long key=  stoull(key_s, 0, 16);
    
    binary_to_dec(text ,text_vect);
    permutaion(text_vect,initial_permtation_table,initial_permutation_output);
    get_keys(key ,keys);
   
 
    if(op == "encrypt")
    {
        for(int roun= 0; roun<16 ; roun++)
        { 
            for(int i=0 ;i<32 ;i++) 
            {
                r[i] = initial_permutation_output[i+32];
                l[i] = initial_permutation_output[i];
            }
            round(roun ,r ,l);
            for(int i=0 ;i<32 ;i++)
            {
                initial_permutation_output[i] = initial_permutation_output[i+32];
                initial_permutation_output[i+32] = r[i];
            }
        }
        vector <int>final (64);
         for(int i=0 ;i<32 ;i++)
        {
            final[i] = initial_permutation_output[i+32];
            final[i+32] = initial_permutation_output[i];
        }
        
        permutaion(final,inverse_permutation_table,inverse_permutation_output);
        cout<<"cipher: ";
        print_hex(inverse_permutation_output);

    }
    else if(op == "decrypt") 
    {
        reverse(keys.begin(),keys.end());
        for(int roun= 0; roun<16 ; roun++)
        { 
            for(int i=0 ;i<32 ;i++) 
            {
                r[i] = initial_permutation_output[i+32];
                l[i] = initial_permutation_output[i];
            }
            round(roun ,r ,l);
            for(int i=0 ;i<32 ;i++)
            {
                initial_permutation_output[i] = initial_permutation_output[i+32];
                initial_permutation_output[i+32] = r[i];
            }

        }
        vector <int>final (64);
         for(int i=0 ;i<32 ;i++)
        {
            final[i] = initial_permutation_output[i+32];
            final[i+32] = initial_permutation_output[i];
        }
        
        permutaion(final,inverse_permutation_table,inverse_permutation_output);
        cout<<"decrypt: ";
        print_hex(inverse_permutation_output);
    }
    else{
        cout<<"not valid operation" ;
    }
    return 0;
}
