# Graph-Recommendation-System
This project is a movie recommendation system implemented in C++ that uses content-based filtering on a bipartite graph. It recommends movies to a user based on their previous ratings and the genres of movies they liked. The system leverages cosine similarity to compare genre frequencies, identifies similar movies, and ranks them based on average ratings.

# Features
**Bipartite Graph Structure**: Utilizes a graph structure to connect users and movies via ratings, enabling fast, efficient recommendation processing.

**Content-Based Filtering**: Recommends movies by matching user-rated genres to similar movies, creating tailored suggestions based on established user interests.

**Cosine Similarity Calculation**: Calculates similarity between user and movie genre vectors, ensuring recommendations closely align with the user's taste.

**Top Recommendations**: Selects the top 40 similar movies, ranks them by average rating, and presents the top 5 to ensure quality recommendations.

# File Structure
**main.cpp**: Contains the implementation of the bipartite graph, data handling, and recommendation logic.

**ratings.csv**: Input file containing user ratings in the format: userId,movieId,rating.

**movies_cleaned.csv**: Input file containing movie details in the format: movieId,title,genres.

# Requirements
Python 3.x

FastAPI

Uvicorn (for running FastAPI)

C++ compiler

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/PrajnaAgrawal26/Graph-Recommendation-System.git
   cd Graph-Recommendation-System
   
2. Ensure you have a C++ compiler installed (e.g., g++, clang++).
   
3. Place ratings.csv and movies_cleaned.csv in the same directory as main.cpp.


# Usage
**Compile the program**:
\g++ main.cpp -o MovieRecommendation

**Run the executable**:
\./MovieRecommendation

Enter a User ID when prompted to receive movie recommendations.
   
