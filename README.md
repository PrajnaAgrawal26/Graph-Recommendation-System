# Graph-Recommendation-System
This project is a movie recommendation system implemented in C++ that uses content-based filtering on a bipartite graph. It recommends movies to a user based on their previous ratings and the genres of movies they liked. The system leverages cosine similarity to compare genre frequencies, identifies similar movies, and ranks them based on average ratings.

# 📋Overview

This program reads movie ratings and genre data from CSV files and constructs a bipartite graph structure:

1. Users and Movies are represented as nodes.
   
2. An edge between a user and a movie represents that the user rated the movie, along with the rating value.
   
The program then recommends movies to a specified user by:

1. Calculating genre-based similarity between movies the user has rated and all other movies.

2. Finding movies with the highest similarity scores.
   
3. Sorting these movies by their average ratings and displaying the top recommendations.

# 🚀Features

**Bipartite Graph Structure**: Utilizes a graph structure to connect users and movies via ratings, enabling fast, efficient recommendation processing.

**Content-Based Filtering**: Recommends movies by matching user-rated genres to similar movies, creating tailored suggestions based on established user interests.

**Cosine Similarity Calculation**: Calculates similarity between user and movie genre vectors, ensuring recommendations closely align with the user's taste.

**Top Recommendations**: Selects the top 40 similar movies, ranks them by average rating, and presents the top 5 to ensure quality recommendations.

# File Structure
**main.cpp**: Contains the implementation of the bipartite graph, data handling, and recommendation logic.

**ratings.csv**: Input file containing user ratings in the format: userId,movieId,rating.

**movies_cleaned.csv**: Input file containing movie details in the format: movieId,title,genres.

# 🛠️ Data Structures Used
1. unordered_map<int, vector<int>> userToMovies: Maps a user ID to a list of movie IDs they have rated.
   
2. unordered_map<int, vector<int>> movieToUsers: Maps a movie ID to a list of user IDs who have rated it.
   
3. unordered_map<int, double> movieTotalRatings: Stores the sum of ratings for each movie.
   
4. unordered_map<int, int> movieRatingCounts: Stores the count of ratings for each movie.
   
5. unordered_map<int, unordered_map<string, int>> movieGenres: Stores genre data for each movie, where each genre is mapped to its frequency.
   
6. unordered_map<int, string> movieTitles: Maps a movie ID to its title.

# 📚Algorithms used
**1. Cosine Similarity for Genre Matching**

**Algorithm:**

The cosineSimilarity function calculates the cosine similarity between two genre frequency vectors: one for the user and one for each movie.
It computes the dot product of the two vectors and normalizes it by the magnitudes of each vector.
The result is a similarity score between 0 and 1.

**Purpose**: Cosine similarity quantifies how similar a movie’s genre profile is to the user’s preferences.

# Requirements
Python 3.x

FastAPI

Uvicorn (for running FastAPI)

C++ compiler

# 💻Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/PrajnaAgrawal26/Graph-Recommendation-System.git
   cd Graph-Recommendation-System
   
2. Ensure you have a C++ compiler installed (e.g., g++, clang++).
   
3. Place ratings.csv and movies_cleaned.csv in the same directory as main.cpp.


# 🧩 Usage

CSV Data Files:
Ensure that ratings.csv contains the following columns: userId, movieId, rating.

movies_cleaned.csv should contain movieId, title, genres, where genres are separated by |.

**Running the Program**:

After compiling the code, run the program and input a User ID when prompted.

The program will display the top 5 recommended movies based on your movie preferences and the system’s collaborative filtering and genre-based similarity.

**Output**:

The output will include the following for each movie:

Movie ID

Movie Title

Movie Genres

Average Rating

# 👨‍💻 Author

Polimetla Eshikha

Penmetsa Navyasri 

PrajnaAgrawal




   
