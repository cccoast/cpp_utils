#include <iostream>
#include <stdio.h>
#include <memory>
#include <stdlib.h>

#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>

using namespace std;

inline int cmp(const void*a, const void* b){
    return strcmp(*(char **)a,*(char **)b);
}

inline int bcmp(const void*a, const void* b){
    return strcmp((char *)a,*(char **)b);
}

int find_not_in(){

    const int L = 25;
    const int N = 14000000;

    int len1,len2;

    char** id_second;
    id_second = (char **)malloc(sizeof(char*) * N);
    for(int i = 0; i < N; i++){
        id_second[i] = (char *)malloc(sizeof(char) * L);
    }

    char** result;
    result = (char **)malloc(sizeof(char*) * 10000000);

    cout << "inited!" << endl;

    char* buff = (char *)malloc(sizeof(char) * 30);

    char first_path[128] = "/media/xudi/coding/tmp/11.csv";
    char second_path[128] = "/media/xudi/coding/tmp/22.csv";
    char result_path[128] = "/media/xudi/coding/tmp/result.csv";

    FILE* fp1 = fopen(first_path,"r");
    FILE* fp2 = fopen(second_path,"r");

    if( fp1 == NULL || fp2 == NULL)
    {
        cout << " can not open file " << endl;
    }

    int i = 0;
    int re;
    fscanf(fp2,"%s",id_second[i]);
    while( (re = fscanf(fp2,"%s",id_second[i++])) != EOF){
    }
    len2 = i -1;
    cout << "2 finished , len = " << len2 << endl;

    qsort(id_second,len2,sizeof(char*),cmp);

    puts("sorted");
    int j = 0;
    fscanf(fp1,"%s",buff);
    while( (re = fscanf(fp1,"%s",buff)) != EOF){
        char* rp = (char *)bsearch(buff,id_second,len2,sizeof(char*),bcmp);
        if(rp == NULL){
            result[j] = (char *)malloc(sizeof(char) * L);
            strcpy(result[j++],buff);
        }
    }
    cout << "count = " << j << endl;

    fclose(fp1);
    fclose(fp2);

    FILE* fout = fopen(result_path,"w+");
    for(i = 0; i < j; i++){
        fputs((const char*)result[i],fout);
        fputs("\n",fout);
        free(result[i]);
    }

    for(int i = 0; i < N; i++){
        free(id_second[i]);
    }
    free(id_second);
    fclose(fout);

    return 0;

}

int example ()
{
    char* pItem;
    char* key = "example";
    char strs[][20] = {"some","example","strings","here"};
    char* strvalues[4];
    for ( int i = 0; i < 4; i++){
        strvalues[i] = strs[i];
    }

    /* sort elements in array: */
    qsort (strvalues, 4, sizeof(char*), cmp);

    /* search for the key: */
    pItem = (char*) bsearch (key, strvalues, 4, sizeof(char*), bcmp);

    if (pItem!=NULL)
    printf ("%s is in the array.\n",*(char **)pItem);
    else
    printf ("%s is not in the array.\n",key);
    return 0;
}

int cpp_quick_sort(){

    vector<string> second;

    char* buff = (char *)malloc(sizeof(char) * 30);

    char first_path[128] = "/media/xudi/coding/tmp/11.csv";
    char second_path[128] = "/media/xudi/coding/tmp/22.csv";
    char result_path[128] = "/media/xudi/coding/tmp/result.csv";

    FILE* fp1 = fopen(first_path,"r");
    FILE* fp2 = fopen(second_path,"r");
    FILE* fout = fopen(result_path,"w+");

    if( fp1 == NULL || fp2 == NULL)
    {
        cout << " can not open file " << endl;
    }

    int re;
    fscanf(fp2,"%s",buff);
    while( (re = fscanf(fp2,"%s",buff)) != EOF){
        second.push_back(string(buff));
    }
    cout << "2 finished , len = " << second.size() << endl;

    sort(second.begin(),second.end());

    puts("sorted");

    int count = 0;
    fscanf(fp1,"%s",buff);
    while( (re = fscanf(fp1,"%s",buff)) != EOF){
        auto s = string(buff);
        auto iter = binary_search(second.begin(),second.end(),s);
        if(!iter){
            fputs((const char*)s.c_str(),fout);
            fputs("\n",fout);
            count++;
        }
    }

    cout << "count = " << count << endl;
    fclose(fout);
    fclose(fp1);
    fclose(fp2);

    return 0;
}

int binary_tree(){

    set<string> second;

    char* buff = (char *)malloc(sizeof(char) * 30);

    char first_path[128] = "/media/xudi/coding/tmp/11.csv";
    char second_path[128] = "/media/xudi/coding/tmp/22.csv";
    char result_path[128] = "/media/xudi/coding/tmp/result.csv";

    FILE* fp1 = fopen(first_path,"r");
    FILE* fp2 = fopen(second_path,"r");
    FILE* fout = fopen(result_path,"w+");

    if( fp1 == NULL || fp2 == NULL)
    {
        cout << " can not open file " << endl;
    }

    int re;
    fscanf(fp2,"%s",buff);
    while( (re = fscanf(fp2,"%s",buff)) != EOF){
        second.insert(string(buff));
    }
    cout << "2 finished , len = " << second.size() << endl;

    int count = 0;
    fscanf(fp1,"%s",buff);
    while( (re = fscanf(fp1,"%s",buff)) != EOF){
        auto s = string(buff);
        auto iter = second.find(s);
        if(iter == second.end()){
            fputs((const char*)s.c_str(),fout);
            fputs("\n",fout);
            count++;
        }
    }

    cout << "count = " << count << endl;
    fclose(fout);
    fclose(fp1);
    fclose(fp2);

    return 0;
}

int test(){
    //binary_tree();
    cpp_quick_sort();
    //find_not_in();
    return 0;
}
