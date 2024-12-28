#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

double pearsonCorr(const vector<double>& X, const vector<double>& Y) {
    const int n = X.size();
    if (n < 2) return 0.0;
    
    double sum_X = 0.0, sum_Y = 0.0, sum_XY = 0.0;
    double squareSum_X = 0.0, squareSum_Y = 0.0;

    for (int i = 0; i < n; i++) {
        sum_X += X[i];
        sum_Y += Y[i];
        sum_XY += X[i] * Y[i];
        squareSum_X += X[i] * X[i];
        squareSum_Y += Y[i] * Y[i];
    }

    double numerator = (n * sum_XY - sum_X * sum_Y);
    double denominator = sqrt((n * squareSum_X - sum_X * sum_X) * 
                            (n * squareSum_Y - sum_Y * sum_Y));
    
    if (denominator == 0.0) return 0.0;
    double corr = numerator / denominator;
    return (isnan(corr) || corr < 0) ? 0.0 : corr;
}

double predictRating(int userId, int itemId,
                    const unordered_map<int, unordered_map<int, double>>& userRatings,
                    const unordered_map<int, unordered_map<int, double>>& itemRatings) {
    if (userRatings.find(userId) == userRatings.end()) {
        // If user not found, return global mean or item mean
        double sum = 0.0, count = 0.0;
        for (const auto& [_, ratings] : itemRatings) {
            for (const auto& [_, rating] : ratings) {
                sum += rating;
                count += 1;
            }
        }
        return count > 0 ? sum / count : 3.0; // Default to 3.0 if no data
    }

    // Find similar users who rated this item
    vector<pair<double, double>> similarities; // {similarity, rating}
    const auto& targetUserRatings = userRatings.at(userId);
    
    if (itemRatings.find(itemId) != itemRatings.end()) {
        for (const auto& [otherUserId, rating] : itemRatings.at(itemId)) {
            if (otherUserId == userId) continue;
            
            // Calculate similarity between users
            vector<double> ratings1, ratings2;
            const auto& otherUserRatings = userRatings.at(otherUserId);
            
            for (const auto& [item, r1] : targetUserRatings) {
                auto it = otherUserRatings.find(item);
                if (it != otherUserRatings.end()) {
                    ratings1.push_back(r1);
                    ratings2.push_back(it->second);
                }
            }
            
            if (ratings1.size() >= 5) {  // Minimum common items threshold
                double sim = pearsonCorr(ratings1, ratings2);
                if (sim > 0.1) {  // Minimum similarity threshold
                    similarities.push_back({sim, rating});
                }
            }
        }
    }
    
    if (similarities.empty()) {
        // Fall back to user's mean rating
        double sum = 0.0;
        for (const auto& [_, rating] : targetUserRatings) {
            sum += rating;
        }
        return sum / targetUserRatings.size();
    }
    
    // Calculate weighted average
    double numerator = 0.0, denominator = 0.0;
    for (const auto& [sim, rating] : similarities) {
        numerator += sim * rating;
        denominator += sim;
    }
    
    double prediction = denominator > 0 ? numerator / denominator : 3.0;
    return std::clamp(prediction, 1.0, 5.0);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    unordered_map<int, unordered_map<int, double>> userRatings;
    unordered_map<int, unordered_map<int, double>> itemRatings;
    vector<pair<int, int>> testCases;
    
    string line;
    bool isTest = false;
    
    while (getline(cin, line)) {
        if (line == "train dataset") continue;
        if (line == "test dataset") {
            isTest = true;
            continue;
        }
        
        stringstream ss(line);
        int userId, itemId;
        
        if (!isTest) {
            double rating;
            if (ss >> userId >> itemId >> rating) {
                userRatings[userId][itemId] = rating;
                itemRatings[itemId][userId] = rating;
            }
        } else {
            if (ss >> userId >> itemId) {
                testCases.emplace_back(userId, itemId);
            }
        }
    }
    
    for (const auto& [userId, itemId] : testCases) {
        double prediction = predictRating(userId, itemId, userRatings, itemRatings);
        cout << fixed << setprecision(1) << prediction << '\n';
    }
    
    return 0;
}