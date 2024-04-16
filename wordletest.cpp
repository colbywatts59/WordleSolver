#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

/*
 * Best average score is 3.6885
*/
 
string findBestWord(vector<string> words);
string findFillerWord(vector<string> words, vector<char> letterList);
vector<string> updateWords(string guessPlaces, string guess, vector<string> words);
vector<string> updateCorrectWords(string guessPlaces, string guess, vector<string> correctWords);
string validateGuess(string guessPlaces, string guess);
 
int main()
{
    vector<string> words;
    vector<string> originalWords;
    vector<string> correctWords;
    vector<char> letterList;
    vector<int> solvedList;
    int incorrectIndex;
   
    ifstream file;
    file.open("wordledictionary.txt");
 
    string word;
 
    while(!file.eof())
    {
        getline(file, word);
        originalWords.push_back(word);
    }
    file.close();

    for(int wordNum = 0; wordNum < originalWords.size(); wordNum++){
        cout << wordNum << "\n";
        bool solved = false;
        int numGuesses = 0;
        string correct = originalWords[wordNum];
        words = originalWords;
        correctWords = words;
        bool fillerWord = false;

        //cout << "The word is " << correct << "\n";
    
        while(!solved)
        {
            string guess;
            string guessPlaces = "xxxxx";
            string bestWord;
            numGuesses++;
            if((correctWords.size() >= 1 && correctWords.size() <= 8 && !fillerWord) || words.size() == 0)
            {
                //cout << "Using correct words" << correctWords.size() << "\n";
                bestWord = findBestWord(correctWords);
            }
            else if(!fillerWord)
            {
                //cout << "Using regular words " << words.size() << "\n";
                bestWord = findBestWord(correctWords);
            }
            else
            {
                //cout << "Using filler word\n";
                bestWord = findFillerWord(originalWords, letterList);
                fillerWord = false;
            }
            //cout << "The best word is: " << bestWord << endl;
            //cout << "Enter your guess:" << endl;
            guess = bestWord;

            for(int k = 0; k < 5; k++){
                if(guess[k] == correct[k]){
                    guessPlaces[k] = 'c';
                } else if(correct.find(guess[k]) != string::npos){
                    guessPlaces[k] = 'w';
                } else{
                    guessPlaces[k] = 'x';
                }
            }
            //cout << bestWord << " has guess places " << guessPlaces << "\n";
    
            //cout << endl << "Use" << endl << "'x' for letters that aren't in the word," << endl << "'w' for letters in the wrong place," << endl << "'c' for letters in the correct place" << endl << "to enter the result:" << endl;
            //cin >> guessPlaces;
            if(guessPlaces == "ccccc")
            {
                //cout << "Solved " << correct << " in " << numGuesses << " guesses.\n";
                solvedList.push_back(numGuesses);
                solved = true;
            }
            else
            {
                words = updateWords(guessPlaces, guess, words);
                correctWords = updateCorrectWords(guessPlaces, guess, correctWords);
                int incorrectLetters = 0;
                for(int i = 0; i < 5; i++){
                    if(guessPlaces[i] != 'c'){
                        incorrectIndex = i;
                        incorrectLetters++;
                    }
                }
                //cout << "incorrect: " << incorrectLetters << "\n";
                if(incorrectLetters <= 1 && correctWords.size() > 2){
                    fillerWord = true;
                    for(int i = 0; i < correctWords.size(); i++){
                        letterList.push_back(correctWords[i][incorrectIndex]);
                    }
                }
            }
        }
    }
    double totalGuesses = 0;
    for(int i = 0; i < solvedList.size(); i++){
        totalGuesses += solvedList[i];
    }
    cout << "The average number of guesses to solve is: " << totalGuesses/solvedList.size() << "\n";
    return 0;
}

string findFillerWord(vector<string> words, vector<char> letterList){
    string bestWord = "";
    int letterMisses = 0;
    // for(int i = 0; i < letterList.size(); i++){
    //     cout << letterList[i] << "\n";
    // }
    while(bestWord == ""){

        for(int i = 0; i < words.size(); i++){
            int currMisses = 0;
            for(int j = 0; j < letterList.size(); j++){
                if(words[i].find(letterList[j]) == string::npos){
                    currMisses++;
                }
            }
                if(currMisses <= letterMisses){
                    //cout << words[i] << endl;
                    return words[i];
                }
        }
        letterMisses++;
    }
    return "";
}
 
string findBestWord(vector<string> words)
{
    int letterFreq[26][5] = {0};
    // for(int i = 0; i < words.size(); i++){
    //     cout << words[i] << "\n";
    // }
 
    // Determining the frequency of each letter in each position
    for(int i = 0; i < words.size(); i++)
    {
        for(int j = 0; j < 5; j++)
        {
            char letter = words[i][j];
            int index = (int)letter % 32 - 1;
 
            letterFreq[index][j]++;
        }           
    }
 
    int maxFreq[5] = {0};
 
    // Finding the max frequency of each letter spot (0-4)
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 26; j++)
        {
            if(letterFreq[j][i] > maxFreq[i])
            {
                maxFreq[i] = letterFreq[j][i];
            }
        }
    }
 
    vector<int> score;
   
    // Determining the score for each word
    for(int i = 0; i < words.size(); i++)
    {
        score.push_back(0);
        //cout << endl;
        for(int j = 0; j < 5; j++)
        {
            int index = (int)words[i][j] % 32 - 1;
            //cout << maxFreq[j] << " - " << letterFreq[index][j] << endl;
            score[i] += (maxFreq[j] - letterFreq[index][j]);
        }
        //cout << "The score for " << words[i] << " is: " << score[i] << endl;
    } 
 
    int bestScore = score[0];
    int bestScoreInd = 0;
 
    for(int i = 0; i < score.size(); i++)
    {
        if(score[i] < bestScore)
        {
            bestScore = score[i];
            bestScoreInd = i;
        }
    }
    return words[bestScoreInd];
}

vector<string> updateCorrectWords(string guessPlaces, string guess, vector<string> words){
    
    //cout << "UPDATING CORECT WORDS\n";
    guessPlaces = validateGuess(guessPlaces, guess);

    for(int i = 0; i < words.size(); i++) {
        // Removing words based on different conditions
        for(int j = 0; j < 5; j++) {
            bool removeWord = false;
            if(guessPlaces[j] == 'c') {
                // Removing words that don't have green letters in the right spot
                if(words[i][j] != guess[j]) {
                    removeWord = true;
                }
            }

            if(guessPlaces[j] == 'w') {
                // Removing words that have yellow letters in the same spot
                if(words[i][j] == guess[j]) {
                    removeWord = true;
                }
                // Removing words that have yellow letters in different spots
                bool found = false;
                for(int k = 0; k < 5; k++) {
                    if(words[i][k] == guess[j] && k != j) {
                        found = true;
                        break;
                    }
                }
                if(!found) {
                    removeWord = true;
                }
            }
            if(guessPlaces[j] == 'x') {
                // Removing words with letters that aren't in the word
                for(int k = 0; k < 5; k++) {
                    if(words[i][k] == guess[j]) {
                        removeWord = true;
                    }
                }
            }
            if(removeWord){
                //cout << "Removing " << words[i] << endl;
                words.erase(words.begin() + i);
                i--; // Adjust the index since we erased an element
                break; // Move to the next word
            }
        }
    }

    // for(int i = 0; i < words.size(); i++) {
    //     cout << words[i] << endl;
    // }

    // cout << endl;

    return words;
}
vector<string> updateWords(string guessPlaces, string guess, vector<string> words) {
    guessPlaces = validateGuess(guessPlaces, guess);
    // for(int i = 0; i < words.size(); i++) {
    //     cout << words[i] << endl;
    // }

    for(int i = 0; i < words.size(); i++) {
        // Removing words based on different conditions
        for(int j = 0; j < 5; j++) {
            bool removeWord = false;
            if(guessPlaces[j] == 'c') {
                if(words[i][j] == guess[j]) {
                    removeWord = true;
                }
            }

            if(guessPlaces[j] == 'w') {
                // Removing words that have yellow letters in the same spot
                if(words[i][j] == guess[j]) {
                    removeWord = true;
                }
                // Removing words that have yellow letters in different spots
                bool found = false;
                for(int k = 0; k < 5; k++) {
                    if(words[i][k] == guess[j] && k != j) {
                        found = true;
                        break;
                    }
                }
                if(!found) {
                    removeWord = true;
                }
            }
            if(guessPlaces[j] == 'x') {
                // Removing words with letters that aren't in the word
                for(int k = 0; k < 5; k++) {
                    if(words[i][k] == guess[j]) {
                        removeWord = true;
                    }
                }
            }
            if(removeWord){
                //cout << "Removing " << words[i] << endl;
                words.erase(words.begin() + i);
                i--; // Adjust the index since we erased an element
                break; // Move to the next word
            }
        }
    }

    // for(int i = 0; i < words.size(); i++) {
    //     cout << words[i] << endl;
    // }

    // cout << endl;

    return words;
}

string validateGuess(string guessPlaces, string guess){
    vector<char> temp;
    for(int i = 0; i < 5; i++){
        if(guessPlaces[i] == 'c' || guessPlaces[i] == 'w'){
            temp.push_back(guess[i]);
            //cout << guess[i] << "\n";
        }
    }
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < temp.size(); j++){
            if(temp[j] == guess[i] && guessPlaces[i] == 'x'){
                guessPlaces[i] = 'w';
            }
        }
    }
    //cout << guessPlaces << "\n";
    return guessPlaces;
}
