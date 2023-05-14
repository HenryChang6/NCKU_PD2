#include <iostream>
#include <fstream> //檔案讀寫
#include <sstream> //字串處理
#include <unordered_map> 
#include <unordered_set> 
#include <vector>
#include <algorithm> // For remove_if function //For transform function
#include <cctype>    // For ispunct function //For tolower function
#include <set>  

using namespace std;

// Function to remove punctuation and convert to lower case
//string process_word(string word);
string process_word(const string& word);
// Function to build the index from the corpus
void build_index(unordered_map< string, set<int> >& index, ifstream& corpus_file);
// Function to process the queries and print the results
void search_queries(unordered_map< string, set<int> >& index, ifstream& query_file);
// Function to calculate the answer
set<int> calculate_intersection(const vector< set<int> >& results);


int main(int argc, char *argv[]) {
    /*An inverted index is a data structure used to store mapping from words to their 
    location in a document (or set of documents). In this case, I used an unordered_map
    to implement the inverted index where each word is a key and the value is a set of 
    IDs of the documents containing that word.*/
    /*unordered_map is used because it provides O(1) average complexity for search, insert, and delete operations, which 
    makes it highly efficient for indexing. It is an implementation of a hash table that does not maintain any order of elements.*/
    //The set was used to keep the IDs unique and sorted.
    unordered_map< string, set<int> > index;

    ifstream corpus_file(argv[1]);
    build_index(index, corpus_file);
    corpus_file.close();

    ifstream query_file(argv[2]);
    search_queries(index, query_file);
    query_file.close();

    return 0;
}

// string process_word(string word){
//     //`remove_if` is an algorithm provided by the C++ Standard Library
//     //`::ispunct` is a function pointer that refers to the ispunct function from the C Standard Library. //Checkout function pointer
//     /*
//     The remove_if algorithm scans through each character in the word string and moves all characters that satisfy the 
//     condition (::ispunct) to the beginning of the range. It returns an iterator pointing to the new end of the range, after 
//     the non-punctuation characters have been moved.
//     */
//     word.erase(remove_if(word.begin(),word.end(),::ispunct),word.end()); 
//     //The transform function in C++ is used to apply a specified operation on each element of a given range and store the result in another range or the same range itself. 
//     /*
//     The third arguments(word.begin()),specifies the output range where the transformed values will be stored.
//     In this case, the transformed values will be written back to the same range, effectively modifying the original word string.
//     */
//     transform(word.begin(),word.end(),word.begin(),::tolower);
//     return word;
// }

string process_word(const string& word){
    string processed_word;
    for (char c : word) {
        if (isalpha(c)) {
            processed_word += tolower(c);
        }
    }
    return processed_word;
}

void build_index(unordered_map< string, set<int> >& index, ifstream& corpus_file){
    string line;
    while(getline(corpus_file,line)){
        stringstream ss(line);
        string id_str , text;
        // Extract the document ID and the text from the line
        getline(ss, id_str, ',');
        //ws manipulator (which stands for "whitespace") is used here to skip any leading whitespace after the comma and before the starting double quote. 
        getline(ss>>ws, text);
        int id = stoi(id_str);

        // Tokenize the text into words
        string word;
        stringstream ss_text(text);
        while(ss_text >> word){
            word = process_word(word);
            if (!word.empty()) index[word].insert(id);
        }
    }
}

void search_queries(unordered_map< string, set<int> >& index, ifstream& query_file){
    string line;
    while(getline(query_file,line)){
        stringstream ss(line);
        string word;
        vector< set<int> > results;
        set<string> searched_words; //use to avoid same query

        // Tokenize the line into words
        while(ss>>word){
            word = process_word(word);

            //Try to avoid same query
            if(searched_words.count(word) == 0){
                searched_words.insert(word);
            }else{
                continue;
            }

            // If the word is in the index, add its IDs to the results.
            if(index.find(word) != index.end()){
                results.push_back(index[word]);
            }else{
                // If the word is not in the index, break and print(cuz每個words都需要符合).
                cout<<-1<<endl;
                results.clear();
                break;
            }
        }

        if(!results.empty()){
            set<int>intersection = calculate_intersection(results);
            //Print the result
            for(int id:intersection){
                cout<<id<<" ";
            }
            cout<<endl;
        }
    }
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

//version2
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
        intersection = move(temp);
        /*The move function is used here to transfer ownership of the temp set to the intersection set, rather than copying the elements.*/
   }
    return intersection;
}