#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <map> 
#include <vector>
#include<algorithm>
#include <list>
#include <unistd.h>
using namespace std;


//检取单词
void check_words(char * line,map<string,int> & wordlist){
    string cf=",.;:'?!()/|\"*{}+-";
    string str;
    char *p;
    size_t pos;   //找到位置
    strtok(line,"\n");
    strtok(line,"\r\n");
    p=strtok(line," ");
    while(p!=NULL){
        str=p;
        pos = str.find_first_of(cf);  //找特殊的标点符号
        while(pos!=string::npos)
        {   //删除单个字符
            str.erase(pos,1);
            //再找下一个单个的字符
           pos = str.find_first_of(cf); 
        }
        if(str!="\n" && str.length()>0 && atoi(str.c_str())==0){
            // cout<<"check_words str:"<<str<<endl;
            if(wordlist.count(str)>0){
                wordlist[str]++;
            }else{
                wordlist[str]=1;
            }
        }
        wordlist.erase("0");
        // printf("map key: %s, value:%d\n", p,wordlist.count(str));
        p=strtok(NULL," ");
    }
}


// 排序规则
bool cmp(const pair<string,int> a,pair<string,int>b){
    return a.second>b.second;
}

// 过滤虚词和特殊字符
bool FunctionWord(string key){
    list<std::string> function_word={
        "a","is","the","this","of","=","to","in","end",
        "as","from","for","with","that","have","by",
        "on","upon","about","above","across","among",
        "ahead","after","an","although","at","also",
        "along","around","always","away","any","up",
        "under","until","before","between","beyond",
        "behind","because","what","when","would","could",
        "who","whom","whose","which","where","why",
        "without","whether","down","during","despite",
        "over","off","only","other","out","then","through",
        "throughout","that","these","those","there","therefore",
        "till","some","such","since","so","can","many","much",
        "more","may","might","must","ever","even","every",
        "each","A","or","and"
    };
    for (list<string>::iterator it = function_word.begin(); it != function_word.end(); ++it) {
        if(*it==key){
            return false;
        }
                
    }
    return true;
}

struct WORDS{
    char key[200];
    int value;
    struct list_head list;
};


int main(int argc, char *argv[]){
    int ret;
    int top=50;
    char name[255];
    if (argc<2)
    {
        printf("参数\n");
        printf("-f: 输入文件名\n");
        printf("[-n 5]: 输出排名前5的单词\n");
        exit(0);
    }
    while ((ret = getopt(argc, argv, ":f:n:")) != -1){
        switch (ret) {
            case 'f':
                strcpy(name,optarg);
                break;
            case 'n':
                top=atoi(optarg);
                break;
            default:
                // printf ("option: %c\n", ret);
                printf("参数\n");
                printf("-f: 输入文件名\n");
                printf("[-n 5]: 输出排名前5的单词\n");
                exit(0);
        }
    }
    
    struct list_head head, *plist;
    struct WORDS *w;
    map<string,int> wordlist;

    INIT_LIST_HEAD(&head);
    struct WORDS *wlist; 
    FILE *fp = NULL;
    char buff[255];
    fp = fopen(name, "r");

    //获取单词
    while(fgets(buff, 255, (FILE*)fp)!=NULL){
        check_words(buff,wordlist);
    }
    vector<pair<string,int>>v(wordlist.begin(),wordlist.end());
    sort(v.begin(),v.end(),cmp);
    for (auto it : v) {
        if(FunctionWord(it.first)){
            // cout << it.first << " " << it.second << endl;
            // 存入list_head链表
            wlist=(struct WORDS *)malloc(sizeof(struct WORDS));
            strcpy(wlist->key,it.first.c_str());
            wlist->value=it.second;
            list_add_tail(&wlist->list, &head);
        }

    }

    // 遍历链表
    printf("%20s 次数 top %d\n","单词",top);
    list_for_each(plist, &head){
        if(top-- == 0)break;
        struct WORDS *pnode=list_entry(plist, struct WORDS, list);
        printf("%18s %d\n", pnode->key,pnode->value);
    }

}