#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iomanip> 
#include <set> 
#include <cmath> 
#include <algorithm> 

using namespace std;


class BipartiteGraph {
public:
    // Adjacency list representation
    unordered_map<int, vector<int>> userToMovies;
    unordered_map<int, vector<int>> movieToUsers;

    
    unordered_map<int, double> movieTotalRatings; 
    unordered_map<int, int> movieRatingCounts; 

    
    unordered_map<int, unordered_map<string, int>> movieGenres; 

    
    unordered_map<int, string> movieTitles; 

    // Function to add an edge and compute ratings
    void addEdge(int userId, int movieId, double rating) {
        userToMovies[userId].push_back(movieId);
        movieToUsers[movieId].push_back(userId);

        // Update total ratings and counts
        movieTotalRatings[movieId] += rating; 
        movieRatingCounts[movieId]++; 
    }

    // Function to get the average rating for a specific movie
    double getAverageRating(int movieId) {
        if (movieRatingCounts.find(movieId) != movieRatingCounts.end() && 
            movieRatingCounts[movieId] > 0) {
            return movieTotalRatings[movieId] / movieRatingCounts[movieId];
        }
        return 0; 
    }

    // Function to add genres for a specific movie with frequencies
    void addGenres(int movieId, const unordered_map<string, int>& genres) {
        movieGenres[movieId] = genres; 
    }

    // Function to add titles for a specific movie
    void addTitle(int movieId, const string& title) {
        movieTitles[movieId] = title; 
    }

    // Function to display genres of a specific movie without brackets
    string getGenres(int movieId) {
        string genresList;
        if (movieGenres.find(movieId) != movieGenres.end()) {
            for (const auto& genre : movieGenres[movieId]) {
                genresList += genre.first + ", "; 
            }
            if (!genresList.empty()) {
                genresList.pop_back(); 
                genresList.pop_back(); 
            }
        }
        return genresList;
    }

    // Function to get the title of a movie by ID
    string getTitle(int movieId) {
        if (movieTitles.find(movieId) != movieTitles.end()) {
            return movieTitles[movieId]; 
        }
        return "Unknown Title"; 
    }
};

// Function to read data from ratings CSV and build the graph
void buildGraphFromCSV(const string& filename, BipartiteGraph& graph) {
    ifstream file(filename);
    string line;

    
    getline(file, line);

    
    while (getline(file, line)) {
        stringstream ss(line);
        string userIdStr, movieIdStr, ratingStr;

        getline(ss, userIdStr, ',');
        getline(ss, movieIdStr, ',');
        getline(ss, ratingStr, ',');

        int userId = stoi(userIdStr);
        int movieId = stoi(movieIdStr);
        double rating = stod(ratingStr); 

        
        graph.addEdge(userId, movieId, rating);
    }
}

// Function to read genre and title data from movies CSV and populate the graph's genre and title map
void buildGenresFromCSV(const string& filename, BipartiteGraph& graph) {
    ifstream file(filename);
    string line;

    
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string movieIdStr, title, genresStr;

        getline(ss, movieIdStr, ','); 
        getline(ss, title, ',');      
        getline(ss, genresStr);        

        int movieId = stoi(movieIdStr);
        graph.addTitle(movieId, title); 

        
        unordered_map<string, int> genres;
        stringstream genreStream(genresStr);
        string genre;

        while (getline(genreStream, genre, '|')) { 
            genres[genre]++; 
        }

        graph.addGenres(movieId, genres); 
    }
}

// Function to get genre frequencies from a user's rated movies
unordered_map<string, int> getUserGenreFrequencies(int userId, BipartiteGraph& graph) {
    unordered_map<string, int> userGenreFreq; 
    if (graph.userToMovies.find(userId) != graph.userToMovies.end()) {
        for (int movieId : graph.userToMovies.at(userId)) {
            if (graph.movieGenres.find(movieId) != graph.movieGenres.end()) {
                const auto& genres = graph.movieGenres.at(movieId);
                for (const auto& genre : genres) {
                    userGenreFreq[genre.first] = userGenreFreq[genre.first] +genre.second; 
                }
            }
        }
    }
    return userGenreFreq;
}

// Function to calculate cosine similarity based on genre frequencies
double cosineSimilarity(const unordered_map<string, int>& userGenreFreq, const unordered_map<string, int>& movieGenreFreq) {
    double dotProduct = 0.0;
    double magnitudeUser = 0.0;
    double magnitudeMovie = 0.0;

    
    for (const auto& entry : userGenreFreq) {
        const string& genre = entry.first;
        int userFreq = entry.second;

        
        if (movieGenreFreq.find(genre) != movieGenreFreq.end()) {
            dotProduct += userFreq * movieGenreFreq.at(genre); 
        }
        magnitudeUser = magnitudeUser +userFreq * userFreq; 
    }

    for (const auto& entry : movieGenreFreq) {
        magnitudeMovie =magnitudeMovie + entry.second * entry.second; 
    }

    if (magnitudeUser == 0 || magnitudeMovie == 0){
        return 0; 
    }

    return dotProduct / (sqrt(magnitudeUser) * sqrt(magnitudeMovie));
}

// Function to find top similar movies based on user's rated genres
vector<pair<int, double>> findTopSimilarMovies(int userId, BipartiteGraph& graph, int topN = 40) {
    unordered_map<string, int> userGenreFreq = getUserGenreFrequencies(userId, graph);

    vector<pair<int, double>> similarMovies;

    for (const auto& entry : graph.movieGenres) { 
        int movieId = entry.first;
        const unordered_map<string, int>& movieGenreFreq = entry.second;

        double similarity = cosineSimilarity(userGenreFreq, movieGenreFreq);

        if (similarity > 0) { 
            similarMovies.push_back({movieId, similarity});
        }
    }

    sort(similarMovies.begin(), similarMovies.end(), [](const pair<int,double>& a, const pair<int,double>& b) {
        return a.second > b.second;
    });

    if (similarMovies.size() > topN) {
        similarMovies.resize(topN); 
    }

    return similarMovies;
}

// Function to find top rated movies among the similar movies
vector<pair<int, double>> findTopRatedSimilarMovies(int userId, BipartiteGraph& graph, int topN = 5, int similarN = 40) {
    vector<pair<int, double>> similarMovies = findTopSimilarMovies(userId, graph, similarN);
    
    // Sort by average rating
    vector<pair<int, double>> topRatedMovies;

    for (const auto& movie : similarMovies) {
        double averageRating = graph.getAverageRating(movie.first);
        topRatedMovies.push_back({movie.first, averageRating}); // Add movie ID and average rating
    }

    sort(topRatedMovies.begin(), topRatedMovies.end(), [](const pair<int,double>& a, const pair<int,double>& b) {
        return a.second > b.second;
    });

    if (topRatedMovies.size() > topN) {
        topRatedMovies.resize(topN); 
    }

    return topRatedMovies;
}

int main() {
    BipartiteGraph graph;

    
    buildGraphFromCSV("ratings.csv", graph);
    buildGenresFromCSV("movies_cleaned.csv", graph);

    int userId;
    cout << "Enter User ID: ";
    cin >> userId;

    // To find top rated similar movies
    vector<pair<int, double>> recommendedMovies = findTopRatedSimilarMovies(userId, graph, 5, 40);

    // Display recommended movies with IDs, titles, genres, and average ratings
    cout << "Top 5 recommended movies:\n";

    for (const auto& movie : recommendedMovies) {
        cout << movie.first << " | " 
             << graph.getTitle(movie.first) << " | "
             << graph.getGenres(movie.first) << " | "
             << setprecision(2) << fixed << movie.second << endl; 
    }
   return 0;
}