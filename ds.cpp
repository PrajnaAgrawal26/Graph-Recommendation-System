#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iomanip> // For std::setprecision
#include <set> // For storing unique genres
#include <cmath> // For sqrt function
#include <algorithm> // For sort function

using namespace std;

// Define a structure for the bipartite graph
class BipartiteGraph {
public:
    // Adjacency list representation
    unordered_map<int, vector<int>> userToMovies;
    unordered_map<int, vector<int>> movieToUsers;

    // New structures to hold total ratings and counts
    unordered_map<int, double> movieTotalRatings; // Total ratings for each movie
    unordered_map<int, int> movieRatingCounts; // Count of ratings for each movie

    // New structure to hold genres for each movie
    unordered_map<int, unordered_map<string, int>> movieGenres; // Movie ID to genre frequency map

    // New structure to hold movie titles
    unordered_map<int, string> movieTitles; // Movie ID to Title map

    // Function to add an edge and compute ratings
    void addEdge(int userId, int movieId, double rating) {
        userToMovies[userId].push_back(movieId);
        movieToUsers[movieId].push_back(userId);

        // Update total ratings and counts
        movieTotalRatings[movieId] += rating; // Add rating to total
        movieRatingCounts[movieId]++; // Increment count of ratings
    }

    // Function to get the average rating for a specific movie
    double getAverageRating(int movieId) {
        if (movieRatingCounts.find(movieId) != movieRatingCounts.end() && 
            movieRatingCounts[movieId] > 0) {
            return movieTotalRatings[movieId] / movieRatingCounts[movieId];
        }
        return 0.0; // Return 0 if no ratings are found for the movie
    }

    // Function to add genres for a specific movie with frequencies
    void addGenres(int movieId, const unordered_map<string, int>& genres) {
        movieGenres[movieId] = genres; // Store the genre frequencies for the given movie ID
    }

    // Function to add titles for a specific movie
    void addTitle(int movieId, const string& title) {
        movieTitles[movieId] = title; // Store the title for the given movie ID
    }

    // Function to display genres of a specific movie without brackets
    string getGenres(int movieId) {
        string genresList;
        if (movieGenres.find(movieId) != movieGenres.end()) {
            for (const auto& genre : movieGenres[movieId]) {
                genresList += genre.first + ", "; // Just add the genre name without frequency
            }
            if (!genresList.empty()) {
                genresList.pop_back(); // Remove the last space
                genresList.pop_back(); // Remove the last comma
            }
        }
        return genresList; // Return formatted genre list
    }

    // Function to get the title of a movie by ID
    string getTitle(int movieId) {
        if (movieTitles.find(movieId) != movieTitles.end()) {
            return movieTitles[movieId]; // Return the movie title
        }
        return "Unknown Title"; // Return a placeholder if not found
    }
};

// Function to read data from ratings CSV and build the graph
void buildGraphFromCSV(const string& filename, BipartiteGraph& graph) {
    ifstream file(filename);
    string line;

    // Skip header line
    getline(file, line);

    // Read each line from the file
    while (getline(file, line)) {
        stringstream ss(line);
        string userIdStr, movieIdStr, ratingStr;

        getline(ss, userIdStr, ',');
        getline(ss, movieIdStr, ',');
        getline(ss, ratingStr, ',');

        int userId = stoi(userIdStr);
        int movieId = stoi(movieIdStr);
        double rating = stod(ratingStr); // Convert rating to double

        // Add edge for each rating and store it in the graph structure
        graph.addEdge(userId, movieId, rating);
    }
}

// Function to read genre and title data from movies CSV and populate the graph's genre and title map
void buildGenresFromCSV(const string& filename, BipartiteGraph& graph) {
    ifstream file(filename);
    string line;

    // Skip header line
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string movieIdStr, title, genresStr;

        getline(ss, movieIdStr, ',');  // Read the Movie ID
        getline(ss, title, ',');       // Read the Title
        getline(ss, genresStr);        // Read the Genres

        int movieId = stoi(movieIdStr);
        graph.addTitle(movieId, title); // Add the title to the graph

        // Process genres and count their frequencies
        unordered_map<string, int> genres;
        stringstream genreStream(genresStr);
        string genre;

        while (getline(genreStream, genre, '|')) { // Split by '|'
            genres[genre]++; // Increment frequency of the genre
        }

        graph.addGenres(movieId, genres); // Add the map of genres to the graph
    }
}

// Function to get genre frequencies from a user's rated movies
unordered_map<string, int> getUserGenreFrequencies(int userId, BipartiteGraph& graph) {
    unordered_map<string, int> userGenreFreq; // To store genre frequencies
    if (graph.userToMovies.find(userId) != graph.userToMovies.end()) {
        for (int movieId : graph.userToMovies.at(userId)) {
            if (graph.movieGenres.find(movieId) != graph.movieGenres.end()) {
                const auto& genres = graph.movieGenres.at(movieId);
                for (const auto& genre : genres) {
                    userGenreFreq[genre.first] += genre.second; // Accumulate genre frequencies
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

    // Calculate the dot product and magnitudes
    for (const auto& entry : userGenreFreq) {
        const string& genre = entry.first;
        int userFreq = entry.second;

        // Only calculate if the genre exists in the movie's genre frequency map
        if (movieGenreFreq.find(genre) != movieGenreFreq.end()) {
            dotProduct += userFreq * movieGenreFreq.at(genre); // Multiply frequencies
        }
        magnitudeUser += userFreq * userFreq; // Sum of squares of user frequencies
    }

    for (const auto& entry : movieGenreFreq) {
        magnitudeMovie += entry.second * entry.second; // Sum of squares of movie frequencies
    }

    if (magnitudeUser == 0 || magnitudeMovie == 0) return 0; // Avoid division by zero

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

    // Load data from CSV files
    buildGraphFromCSV("ratings.csv", graph);
    buildGenresFromCSV("movies_cleaned.csv", graph);

    int userId;
    cout << "Enter User ID: ";
    cin >> userId;

    // Find top rated similar movies
    vector<pair<int, double>> recommendedMovies = findTopRatedSimilarMovies(userId, graph, 5, 40);

    // Display recommended movies with IDs, titles, genres, and average ratings
    cout << "Top 5 recommended movies:\n";
    cout << "Movie ID | Title | Genres | Average Rating\n"; // Header
    for (const auto& movie : recommendedMovies) {
        cout << movie.first << " | " 
             << graph.getTitle(movie.first) << " | "
             << graph.getGenres(movie.first) << " | "
             << setprecision(2) << fixed << movie.second << endl; // Show movie details
    }

    return 0;
}