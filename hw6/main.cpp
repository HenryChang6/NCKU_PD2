#include <iostream>
#include <fstream> //檔案讀寫
#include <sstream> //字串處理
#include <map> 
#include <vector>
#include <algorithm> // For remove_if function //For transform function
#include <cctype>    // For ispunct function //For tolower function
#include <set>  
#include <math.h> //For log()
#include <cassert>

using namespace std;

// Function to remove punctuation and convert to lower case
string process_word(const string& word);
// Function to build the index from the corpus and count the idf_values of each words manually
void build_index(map< string, set<int> >& index, map<string, double>& idf_values ,ifstream& corpus_file);
// Function to process the queries and print the results
void search_queries(map< string, set<int> >& index, map<string, double>& idf_values, ifstream& query_file);
// Function to calculate the answer
set<int> calculate_intersection(const vector< set<int> >& results);

int main(int argc, char *argv[]) {
	map< string, set<int> > index;
    map<string, double> idf_values;

    assert(argc == 4);

   	ifstream corpus_file(argv[1]);
    build_index(index, idf_values, corpus_file);
   	corpus_file.close();

    ifstream query_file(argv[2]);
    search_queries(index, idf_values, query_file);
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
    int total_docs = 0;
    while(getline(corpus_file,line)){
        stringstream ss(line);
        string id_str , text ,temp;
        // Extract the document ID and the text from the line
        getline(ss, id_str, ',');
        //ws manipulator (which stands for "whitespace") is used here to skip any leading whitespace after the comma and before the starting double quote. 
        getline(ss, temp,'"');
        int id = stoi(id_str);
        ++total_docs;

        // Tokenize the text into words
        string word;
        set<string> words_in_docs;
        while(getline(ss,word,' ')){
            process_word(word);
            if (!word.empty()){
                index[word].insert(id);
                words_in_docs.insert(word);
            } 
        }

        // Update the number of documents each word appears in
        for(auto& word : words_in_docs){
            idf_values[word]++;
        }
    }

    for(auto& idf : idf_values){
        idf.second = log(total_docs/idf.second);
    }
}

void search_queries(map< string, set<int> >& index, map<string, double>& idf_values, ifstream& query_file){
    string line;
    while(getline(query_file,line)){
        stringstream ss(line);
        string word;

        set<string> query_words; // use a set to keep unique query words

        // Tokenize the line into words
        while(ss>>word){
            process_word(word);
            if(!word.empty()) query_words.insert(word);
        }
    }       
}


set<int> calculate_intersection(const vector< set<int> >& results){
    /*
    The set_intersection function is a built-in STL function that computes the intersection
    of two sorted ranges. It requires five arguments: the beginning and end iterators of the
    first range, the beginning and end iterators of the second range, and an output iterator
    where the results are written.
    */
   set<int>intersection = results[0];
   for(int i = 1; i < results.size(); i++){
        set<int> temp;
        set_intersection(intersection.begin(),intersection.end(),results[i].begin(),results[i].end(),inserter(temp,temp.begin()));
        /*inserter(temp, temp.begin()): This is the output iterator where the results of the intersection are written. It's using
        the inserter function to create an insert iterator that inserts new elements at the beginning of the temp set.*/
        intersection.clear();
        intersection.insert(temp.begin(),temp.end());
        /*The move function is used here to transfer ownership of the temp set to the intersection set, rather than copying the elements.*/
   }
    return intersection;
}