// Programmed by Jonah Brooks //

#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>

int main(int argc, char *argv[])
{

    std::map<std::string,int> vocabMap;
    std::vector<std::string> cleanVect;
    
    std::vector<std::set<std::string>*> trainReviews;
    std::vector<std::vector<bool>*> trainReviewTally;
    int trainReviewCount = 0;
    
    std::vector<std::set<std::string>*> testReviews;
    std::vector<std::vector<bool>*> testReviewTally;
    int testReviewCount = 0;
    
    std::string nextWord;
    std::string nextLine;
    std::stringstream nextLineStream;

    std::ifstream rawVocabFile;
    std::ifstream rawTrainFile;
    std::ifstream rawTestFile;
    std::ifstream stopwordsFile;
    
    std::ofstream cleanVocabFile; 
    std::ofstream cleanTrainFile;
    std::ofstream cleanTestFile;

    // Fill vocabMap with all words
    rawVocabFile.open("raw.vocabulary.txt");
    while(!rawVocabFile.eof())
    {
        rawVocabFile >> nextWord;
        vocabMap[nextWord] = 0;
    }
    rawVocabFile.close();

    // Tally vocabMap occurrences in train data
    rawTrainFile.open("raw.train.txt");
    while(!rawTrainFile.eof())
    {
        rawTrainFile >> nextWord;
        if(vocabMap.count(nextWord) > 0)
        {
            vocabMap[nextWord]++;
        }
    }
    rawTrainFile.close();

    // Tally vocabMap occurrences in test data
    rawTestFile.open("raw.test.txt");
    while(!rawTestFile.eof())
    {
        rawTestFile >> nextWord;
        if(vocabMap.count(nextWord) > 0)
        {
            vocabMap[nextWord]++;
        }
    }
    rawTestFile.close();
    
    // Floor the frequency of stopwords so they are removed next step
    stopwordsFile.open("stoplist.txt");
    while(!stopwordsFile.eof())
    {
        stopwordsFile >> nextWord;
        if(vocabMap.count(nextWord) > 0)
        {
            vocabMap[nextWord] = 0;
        }
    }
    stopwordsFile.close();

    // Fill output file and vector with non stopwords with frequencies greater than 3
    cleanVocabFile.open("clean.vocabulary.txt");
    for(std::map<std::string,int>::const_iterator i = vocabMap.begin(); i != vocabMap.end(); i++)
    {
        if (i->second > 3)
        {
            cleanVocabFile << i->first << std::endl;
            cleanVect.push_back(i->first);
        }
    }
    cleanVocabFile.close();

    // iterate through lines of train or test
    // pull line out of file
    // parse words into a map
    // repeat

    // gather sets of words for each review in train
    rawTrainFile.clear();
    rawTrainFile.open("raw.train.txt");
    while(!rawTrainFile.eof())
    {
        trainReviews.push_back(new std::set<std::string>);
        trainReviewTally.push_back(new std::vector<bool>);
        trainReviewCount++;
        std::getline(rawTrainFile,nextLine);
        nextLineStream.clear();
        nextLineStream.str(nextLine);
        while(!nextLineStream.eof())
        {
            nextLineStream >> nextWord;
            trainReviews[trainReviewCount - 1]->insert(nextWord);
        }
    }
    rawTrainFile.close();

    // gather sets of words for each review in test
    rawTestFile.clear();
    rawTestFile.open("raw.test.txt");
    while(!rawTestFile.eof())
    {
        testReviews.push_back(new std::set<std::string>);
        testReviewTally.push_back(new std::vector<bool>);
        testReviewCount++;
        std::getline(rawTestFile,nextLine);
        nextLineStream.clear();
        nextLineStream.str(nextLine);
        while(!nextLineStream.eof())
        {
            nextLineStream >> nextWord;
            testReviews[testReviewCount - 1]->insert(nextWord);
        }
    }
    rawTestFile.close();
   
    // Then iterate through vocab, search and set for each line
    nextLineStream.clear();
    nextLineStream.str("");
    for(std::vector<std::string>::const_iterator iter = cleanVect.begin(); iter != cleanVect.end(); iter++)
    {
        nextLineStream << (*iter) << ",";
        
        for(int i = 0; i < trainReviewCount; i++)
        {
            trainReviewTally[i]->push_back(trainReviews[i]->count((*iter)));
        }
        for(int i = 0; i < testReviewCount; i++)
        {
            testReviewTally[i]->push_back(testReviews[i]->count((*iter)));
        }
        
    }

    // Insert the first line into each file
    cleanTrainFile.open("training.txt");
    cleanTestFile.open("testing.txt");
    cleanTrainFile << nextLineStream.str() << std::endl;
    cleanTestFile << nextLineStream.str() << std::endl;

    // Then go through each tally and write lines to the files
    for(std::vector<std::vector<bool>*>::const_iterator reviewIter = trainReviewTally.begin(); reviewIter != trainReviewTally.end(); reviewIter++)
    {
        nextLineStream.clear();
        nextLineStream.str("");
        std::vector<bool> *reviewVect = (*reviewIter);
        for(std::vector<bool>::const_iterator tallyIter = reviewVect->begin(); tallyIter != reviewVect->end(); tallyIter++)
        {
            nextLineStream << (*tallyIter) << ",";
        }
        cleanTrainFile << nextLineStream.str() << std::endl;
    }
    cleanTrainFile.close();

    for(std::vector<std::vector<bool>*>::const_iterator reviewIter = testReviewTally.begin(); reviewIter != testReviewTally.end(); reviewIter++)
    {
        nextLineStream.clear();
        nextLineStream.str("");
        std::vector<bool> *reviewVect = (*reviewIter);
        for(std::vector<bool>::const_iterator tallyIter = reviewVect->begin(); tallyIter != reviewVect->end(); tallyIter++)
        {
            nextLineStream << (*tallyIter) << ",";
        }
        cleanTestFile << nextLineStream.str() << std::endl;
    }
    cleanTestFile.close();

    std::cout << trainReviews.size();

    // Free allocated memory
    for(int i = 0; i < trainReviewCount; i++)
    {
        delete trainReviews[i];
        delete trainReviewTally[i];
    }
    for(int i = 0; i < testReviewCount; i++)
    {
        delete testReviews[i];
        delete testReviewTally[i];
    }

    return 0;
}
