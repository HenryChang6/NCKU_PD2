#include <iostream>
#include <fstream> //檔案讀寫
#include <sstream> //字串處理
#include <map> 
#include <unordered_set> 
#include <vector>
#include <algorithm> // For remove_if function //For transform function
#include <cctype>    // For ispunct function //For tolower function
#include <set>  

using namespace std;

// Function to remove punctuation and convert to lower case
string process_word(const string& word);
// Function to build the index from the corpus
void build_index(map< string, set<int> >& index, ifstream& corpus_file);
// Function to process the queries and print the results
void search_queries(map< string, set<int> >& index, ifstream& query_file);
// Function to calculate the answer
set<int> calculate_intersection(const vector< set<int> >& results);

int main(int argc, char *argv[]) {
	map< string, set<int> > index;

   	ifstream corpus_file(argv[1]);
    build_index(index, corpus_file);
   	corpus_file.close();

    ifstream query_file(argv[2]);
    search_queries(index, query_file);
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

void build_index(map< string, set<int> >& index, ifstream& corpus_file){
    string line;
    while(getline(corpus_file,line)){
        stringstream ss(line);
        string id_str , text ,temp;
        // Extract the document ID and the text from the line
        getline(ss, id_str, ',');
        //ws manipulator (which stands for "whitespace") is used here to skip any leading whitespace after the comma and before the starting double quote. 
        getline(ss, temp,'"');
        int id = stoi(id_str);

        // Tokenize the text into words
        string word;
        while(getline(ss,word,' ')){
            process_word(word);
            if (!word.empty()) index[word].insert(id);
        }
    }
}

void search_queries(map< string, set<int> >& index, ifstream& query_file){
    string line;
    while(getline(query_file,line)){
        stringstream ss(line);
        string word;
        vector< set<int> > results;
        set<string> searched_words; //use to avoid same query

        // Tokenize the line into words
        while(ss>>word){
            process_word(word);

            //Try to avoid same query
            if(searched_words.count(word) == 0){
                searched_words.insert(word);
            } else {
                continue;
            }

            // If the word is in the index, add its IDs to the results.
            if(index.count(word)){
                results.push_back(index[word]);
            }else{
                // If the word is not in the index, print and break(cuz每個words都需要符合).
                cout<<-1<<endl;
                results.clear();
                break;
            }
        }

        if(!results.empty()){
            set<int>intersection = calculate_intersection(results);
            //Print the result
            if (intersection.size()==0) {
                cout << -1 << endl;
            } else {
                for(auto& id:intersection){
                    if(id == *intersection.rbegin()) cout << id;
                    else cout << id << " ";
                }
                cout<<endl;
            }
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

//version 1
//set<int> calculate_intersection(const vector< set<int> >& results){

    // Compute the intersection of the document IDs in the results
    /*
    Concept: calculate 0 1 to be one intersection, and calculate the section and 2, which will be the 
    intersection of 0 1 2. Calculate so on.  
    */
    // set<int>intersection = results[0];
    // for(int i = 1; i < results.size(); i++){
    //     set<int> temp;
    //     for(int id:results[i]){
    //         if(intersection.find(id)!=intersection.end()) 
    //             temp.insert(id);
    //     }
    //     intersection = temp;
    // }
    // return intersection;
//}