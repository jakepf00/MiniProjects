#include<algorithm>
#include<fstream>
#include<iostream>
#include<vector>
using namespace std;

void loadDictionary(string filename, vector<string>& dictionary) {
    ifstream file(filename);
    if (file.is_open()) {
        while (file.good()) {
            string temp;
            file >> temp;
            dictionary.push_back(temp);
        }
    }
}

int wagnerFischer(string word1, string word2) {
    vector<vector<int>> check;
    check.resize(word1.length() + 1, vector<int>(word2.length() + 1));
    for (int i = 0; i < check[0].size(); i++) check[0][i] = i;
    for (int i = 0; i < check.size(); i++) check[i][0] = i;

    for (int i = 1; i < check.size(); i++) {
        for (int j = 1; j < check[i].size(); j++) {
            int add = check[i-1][j] + 1;
            int change = check[i-1][j-1];
            int del = check[i][j-1] + 1;
            if (word1[i-1] != word2[j-1]) change++;
            check[i][j] = min(add, min(change, del));
        }
    }

    return check[word1.length()][word2.length()];
}

vector<pair<string, int>> spellCheck(string word, vector<string> dictionary, int numSuggestions = 10) {
    vector<pair<string, int>> suggestions;
    for (auto correctWord : dictionary) {
        suggestions.push_back(make_pair(correctWord, wagnerFischer(word, correctWord)));
    }
    sort(suggestions.begin(), suggestions.end(), [](pair<string, int> a, pair<string, int> b) {
        return a.second < b.second;
    });
    suggestions.erase(suggestions.begin() + numSuggestions, suggestions.end());
    return suggestions;
}

int main() {
    vector<string> dictionary;
    loadDictionary("Dictionary.txt", dictionary);

    string wordToCheck;
    cout << "> ";
    cin >> wordToCheck;
    while (wordToCheck != "exit") {
        vector<pair<string, int>> correctWords = spellCheck(wordToCheck, dictionary);
        cout << "Top " << correctWords.size() << " suggestions for '" << wordToCheck << "':" << endl;
        for (auto i : correctWords) {
            cout << i.first << " (Distance: " << i.second << ")" << endl;
        }
        cout << "> ";
        cin >> wordToCheck;
    }
}