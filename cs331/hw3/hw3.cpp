// Programmed by Jonah Brooks //

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

const int NEG = 0;
const int POS = 1;

struct feature {
    // count[featureTrue|False][reviewGood|Bad]
    int count[2][2];
    double p[2][2];

    feature() { 
        count[NEG][NEG] = 0; count[NEG][POS] = 0;
        count[POS][NEG] = 0; count[POS][POS] = 0;
        p[NEG][NEG] = 0.0;   p[NEG][POS] = 0.0;
        p[POS][NEG] = 0.0;   p[POS][POS] = 0.0;
    }
};

int main(int argc, char *argv[])
{
    // Things I will need:
    //      Number of training lines
    //      Number of training lines that are "good"
    //      Number of training lines that are "bad"
    //      Number of lines with feature A=true and "good"
    //      Number of lines with feature A=false and "good"
    //      Number of lines with feature A=true and "bad"
    //      Number of lines with feature A=false and "bad"

    if (argc != 5)
    {
        std::cout << "Please enter the following arguments: "
                  << "< training data file > "
                  << "< training label file > "
                  << "< testing data file > "
                  << "< testing label file >" 
                  << std::endl;
        return 1;
    }
    
    std::ifstream trainData;
    trainData.open(argv[1]);
    if (trainData.fail()) { std::cout << "Error reading file 1\n"; return 2;}
    
    std::ifstream trainLabel;
    trainLabel.open(argv[2]);
    if (trainLabel.fail()) { std::cout << "Error reading file 2\n"; return 2;}
    
    std::ifstream testData;
    testData.open(argv[3]);
    if (testData.fail()) { std::cout << "Error reading file 3\n"; return 2;}
    
    std::ifstream testLabel;
    testLabel.open(argv[4]);
    if (testLabel.fail()) { std::cout << "Error reading file 4\n"; return 2;}


    int featureCount = 0;
    int reviewTypeCount[2];
    reviewTypeCount[NEG] = 0; // Negative reviews
    reviewTypeCount[POS] = 0; // Positive reviews
    double reviewTypeProb[2];
    reviewTypeProb[NEG] = 0.0; //Percent chance that a review is negative
    reviewTypeProb[POS] = 0.0; //Percent chance that a review is positive
    std::string nextLabel;
    int nextLabelValue = 0;
    std::string nextReview;
    int nextReviewValue = 0;
    std::stringstream nextReviewParser;

    //Parse through training data and label
    //Tally and categorize 

    std::cout << "counting feature elements from word list" << std::endl;

    std::string stringForCounting;
    std::getline(trainData,stringForCounting); // Ditch the list of words
    std::getline(testData,stringForCounting);
    std::stringstream streamForCounting(stringForCounting);
    while(!streamForCounting.eof())
    {
        std::string meaninglessToken;
        std::getline(streamForCounting,meaninglessToken,',');
        featureCount++;
    }
    
    std::cout << "counting feature data from train" << std::endl;

    std::vector<feature> trainingFeatures(featureCount);
    std::vector<feature> testingFeatures(featureCount);
    std::vector<int> nextReviewFeatures(featureCount,0);
    
    while(std::getline(trainData,nextReview) && std::getline(trainLabel,nextLabel))
    {
        if(nextLabel == "pos")
        {
            reviewTypeCount[POS]++;
            nextLabelValue = POS;
        }
        else
        {
            reviewTypeCount[NEG]++;
            nextLabelValue = NEG;
        }

        nextReviewParser.clear();
        nextReviewParser.str(nextReview);
        int index = 0;
        while(!nextReviewParser.eof())
        {
            // Grab next value and drop the comma
            nextReviewParser >> nextReviewValue;
            if(nextReviewParser.peek() == ',')
            {
                nextReviewParser.ignore(1);
            }
            
            // Increment the correct count for this case
            trainingFeatures[index].count[nextReviewValue][nextLabelValue]++;
            index++;
        }
    }

    
    
    std::cout << "filling in probability tables" << std::endl;
    // Fill in tables for each feature
    for(int f = 0; f < featureCount; f++)
    {
        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 2; j++)
            {   // Using Dirichlet Priors
                trainingFeatures[f].p[i][j] = log(double((trainingFeatures[f].count[i][j] + 1))) - log(double(reviewTypeCount[j] + 2));
            }
        }           
    }
    reviewTypeProb[NEG] = log(double(reviewTypeCount[NEG]+1)) - log(double(reviewTypeCount[NEG] + reviewTypeCount[POS] + 2));
    reviewTypeProb[POS] = log(double(reviewTypeCount[POS]+1)) - log(double(reviewTypeCount[NEG] + reviewTypeCount[POS] + 2));


    int positiveTestReviews = 0;
    int identifiedPositives = 0;
    int negativeTestReviews = 0;
    int identifiedNegatives = 0;

    std::cout << "counting and predicting data from test" << std::endl;

    while(std::getline(testData,nextReview) && std::getline(testLabel,nextLabel))
    {
        if(nextLabel == "pos")
        {
            positiveTestReviews++;
            nextLabelValue = POS;
        }
        else
        {
            negativeTestReviews++;
            nextLabelValue = NEG;
        }

        nextReviewParser.clear();
        nextReviewParser.str(nextReview);
        if(nextReviewParser.peek() == ',')
        {
            nextReviewParser.ignore(1);
        }
        nextReviewValue = 0;
        int i = 0;
        while(!nextReviewParser.eof())
        {
            // Grab next value and drop the comma
            nextReviewParser >> nextReviewValue;
            if(nextReviewParser.peek() == ',')
            {
                nextReviewParser.ignore(1);
            }
            // Increment the correct count for this case
            testingFeatures[i].count[nextReviewValue][nextLabelValue]++;
            nextReviewFeatures[i] = nextReviewValue;
            i++;
        }
        double positivePrediction = reviewTypeProb[POS];
        double negativePrediction = reviewTypeProb[NEG];
        for(int f = 0; f < featureCount; f++)
        {
            positivePrediction += trainingFeatures[f].p[nextReviewFeatures[f]][POS];
            negativePrediction += trainingFeatures[f].p[nextReviewFeatures[f]][NEG];
        } 
        if(positivePrediction > negativePrediction)
        {
            if(nextLabel == "pos")
            {
                identifiedPositives++;
            }
        }
        else
        {
            if(nextLabel == "neg")
            {
                identifiedNegatives++;
            }
        }
    }

    std::cout << "Test data prediction results: " << std::endl;
    std::cout << "Positive: " << (double(identifiedPositives)*100/double(positiveTestReviews)) << "%\t" << identifiedPositives << "/" << positiveTestReviews << std::endl;
    std::cout << "Negative: " << (double(identifiedNegatives)*100/double(negativeTestReviews)) << "%\t" << identifiedNegatives << "/" << negativeTestReviews << std::endl;
    
    std::cout << "\npredicting data from train\n";
    positiveTestReviews = 0;
    identifiedPositives = 0;
    negativeTestReviews = 0;
    identifiedNegatives = 0;

    trainData.clear();
    trainLabel.clear();
    trainData.seekg(0,trainData.beg);
    std::getline(trainData,nextReview);
    trainLabel.seekg(0,trainLabel.beg);
    while(std::getline(trainData,nextReview) && std::getline(trainLabel,nextLabel))
    {
        if(nextLabel == "pos")
        {
            positiveTestReviews++;
            nextLabelValue = POS;
        }
        else
        {
            negativeTestReviews++;
            nextLabelValue = NEG;
        }
        nextReviewParser.clear();
        nextReviewParser.str(nextReview);
        if(nextReviewParser.peek() == ',')
        {
            nextReviewParser.ignore(1);
        }
        nextReviewValue = 0;
        int i = 0;
        while(!nextReviewParser.eof())
        {
            // Grab next value and drop the comma
            nextReviewParser >> nextReviewValue;
            if(nextReviewParser.peek() == ',')
            {
                nextReviewParser.ignore(1);
            }
            nextReviewFeatures[i] = nextReviewValue;
            i++;
        }

        double positivePrediction = reviewTypeProb[POS];
        double negativePrediction = reviewTypeProb[NEG];
        for(int f = 0; f < featureCount; f++)
        {
            positivePrediction += trainingFeatures[f].p[nextReviewFeatures[f]][POS];
            negativePrediction += trainingFeatures[f].p[nextReviewFeatures[f]][NEG];
        } 
        if(positivePrediction > negativePrediction)
        {
            if(nextLabel == "pos")
            {
                identifiedPositives++;
            }
        }
        else
        {
            if(nextLabel == "neg")
            {
                identifiedNegatives++;
            }
        }
    }

    std::cout << "Training data prediction results: " << std::endl;
    std::cout << "Positive: " << (double(identifiedPositives)*100/double(positiveTestReviews)) << "%\t" << identifiedPositives << "/" << positiveTestReviews << std::endl;
    std::cout << "Negative: " << (double(identifiedNegatives)*100/double(negativeTestReviews)) << "%\t" << identifiedNegatives << "/" << negativeTestReviews << std::endl;

    return 0;
}
