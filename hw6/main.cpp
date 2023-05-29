#pragma GCC optimize(3)
#pragma GCC target("avx")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-fwhole-program")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-fstrict-overflow")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-skip-blocks")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("-funsafe-loop-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")

#include <iostream>
#include <fstream> //檔案讀寫
#include <sstream> //字串處理
#include <map> 
#include <queue>
#include <utility> //for pair<>
#include <algorithm> // For remove_if function //For transform function
#include <cctype>    // For ispunct function //For tolower function
#include <set>  
#include <cmath> 
#include <cassert>

using namespace std;

// Function to remove punctuation and convert to lower case
string process_word(const string& word);
// Function to build the index from the corpus and count the idf_values of each words manually
void build_index(map< string, set<int> >& index, map<string, double>& idf_values ,ifstream& corpus_file);
// Function to process the queries and print the results
void search_queries(map< string, set<int> >& index, map<string, double>& idf_values, ifstream& query_file, int output_num);

struct compare{
    bool operator ()(const pair<int,double>& p1, const pair<int,double>& p2) {
        if(p1.second != p2.second){
            return (p1.second < p2.second);
        }
        return (p1.first > p2.first);
    };
};

int main(int argc, char *argv[]) {

    ios_base::sync_with_stdio(false);
    cin.tie(0);

	map< string, set<int> > index;
    map<string, double> idf_values;

   	ifstream corpus_file(argv[1]);
    build_index(index, idf_values, corpus_file);
   	corpus_file.close();

    ifstream query_file(argv[2]);
    search_queries(index, idf_values, query_file, stoi(argv[3]));
    query_file.close();

    return 0;
}

void process_word(string& word){
    string processed_word;
    for (auto &c : word) {
        if (isalpha(c)) {
            processed_word += tolower(c);
        }
    }
    word = processed_word;
}

void build_index(map< string, set<int> >& index, map<string, double>& idf_values ,ifstream& corpus_file){
    string line;
    int total_corpus = 0;
    while(getline(corpus_file,line)){
        stringstream ss(line);
        string id_str , text ,temp;
        // Extract the document ID and the text from the line
        getline(ss, id_str, ','); 
        getline(ss, temp,'"');
        int id = stoi(id_str);
        ++total_corpus;

        // Tokenize the text into words
        string word;
        set<string> words_in_corpus;
        while(getline(ss,word,' ')){
            process_word(word);
            if (!word.empty()){
                index[word].insert(id);
                words_in_corpus.insert(word);
            } 
        }
        // Update the number of documents each word appears in
        for(auto& word : words_in_corpus){
            idf_values[word]++;
        }
    }
    //count the idf_value
    for(auto& idf : idf_values){
        idf.second = log10((double)total_corpus/idf.second);
        // cout<<idf.first<<" "<<idf.second<<endl;
    }
}

void search_queries(map< string, set<int> >& index, map<string, double>& idf_values, ifstream& query_file, int output_num){
    string line;
    while(getline(query_file,line)){
        stringstream ss(line);
        string word;
        map<int, double> idf_sum;
        // Tokenize the queries
        while(ss>>word){
            process_word(word);
            // Add this query's idf_value into each corpuses which contains this query 
            if(idf_values.count(word)){
                double idf = idf_values[word];
                for(auto& id : index[word]) idf_sum[id]+=idf;
            }
        }
        //start sorting and output it

        priority_queue<pair<int, double>, vector< pair<int, double> >, compare> pq;
        int count_zero_idf = 0;
        for(const auto& item : idf_sum){
            pq.push(item);
            if(item.second == 0) count_zero_idf++; 
        }
        //算出共有幾個IDF值非零的id 
        int none_zero_num = pq.size() - count_zero_idf;
        //特例： 若所有的idf都是0
        if(pq.size() == count_zero_idf){
            for(int i = 0; i < output_num;++i){
                if(i == (output_num - 1)) cout<<"-1"<<endl;
                else cout<<"-1"<<" ";
            }
            continue;
        }
        //特例：如果idf不為0的元素小於要印出來個數，其他用-1補上
        if(none_zero_num < output_num){
            int fill_negative_one = output_num - none_zero_num;
            
            for(int i = 0; i < none_zero_num; ++i){
                auto pq_item = pq.top();
                cout<<pq_item.first<<" ";
                pq.pop();
            }
            for(int i = 0; i < fill_negative_one; ++i){
                if(i == fill_negative_one-1) cout<<"-1"<<endl;
                else cout<<"-1"<<" ";
            }
            continue;
        }


        for(int i = 0; i < output_num && !pq.empty(); ++i){
            auto pq_item = pq.top();
            if(i != (output_num - 1)) cout<<pq_item.first<<" ";
            else cout<<pq_item.first<<endl;
            pq.pop();
        }
    }
}