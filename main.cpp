#include <iostream>
#include "functions.h"

using namespace std;

int main() {
    vector<std::vector<string>> ratings_content = read_ratings_csv();
    vector<vector<string>> predictions_content = read_predictions_csv();
    unordered_map<int, unordered_map<int, double>> movieMatrix;
    unordered_map<int, unordered_map<int, double>> itemMatrix;
    unordered_map<int, double> meanValues;
    unordered_map<int, double> meanValues_item;


    for (int i = 1; i < ratings_content.size(); ++i) {
        int userId = stoi(ratings_content[i][0]);
        int itemId = stoi(ratings_content[i][1]);
        double rating = stod(ratings_content[i][2]);
        movieMatrix[userId][itemId] = rating;
    }

    for (int i = 1; i < ratings_content.size(); ++i) {
        int itemId = stoi(ratings_content[i][1]);
        int userId = stoi(ratings_content[i][0]);
        double rating = stod(ratings_content[i][2]);
        itemMatrix[itemId][userId] = rating;
    }


    meanValues = get_means(movieMatrix);

    meanValues_item = get_means(itemMatrix);



    ofstream submission;
    submission.open("submission.csv");
    submission << "ID,Predicted\n";
    predictions_content.erase(predictions_content.begin());
    for (auto &i: predictions_content) {
        double pred = predict(stoi(i[0]), stoi(i[1]), itemMatrix, movieMatrix,
                              meanValues_item, meanValues);
        cout << i[0] << " " << pred << endl;
        submission << i[0] << "," << pred << "\n";
    }

    submission.close();


    return 0;
}
