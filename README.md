# Graph-Recommendation-System
This project is a movie recommendation system implemented in C++ that uses content-based filtering on a bipartite graph. It recommends movies to a user based on their previous ratings and the genres of movies they liked. The system leverages cosine similarity to compare genre frequencies, identifies similar movies, and ranks them based on average ratings.

# Features
**Bipartite Graph Structure**: Utilizes a graph structure to connect users and movies via ratings, enabling fast, efficient recommendation processing.

**Content-Based Filtering**: Recommends movies by matching user-rated genres to similar movies, creating tailored suggestions based on established user interests.

**Cosine Similarity Calculation**: Calculates similarity between user and movie genre vectors, ensuring recommendations closely align with the user's taste.

**Top Recommendations**: Selects the top 40 similar movies, ranks them by average rating, and presents the top 5 to ensure quality recommendations.
