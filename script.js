document.addEventListener('DOMContentLoaded', () => {
    const searchButton = document.getElementById('searchButton');
    const userIdInput = document.getElementById('userIdInput');
    const loader = document.getElementById('loader');
    const resultsContainer = document.getElementById('results');
  
    // Function to fetch and display recommendations
    async function fetchRecommendations() {
      const userId = userIdInput.value.trim();
      if (userId === '' || isNaN(userId) || userId < 1 || userId > 671) {
        alert('Please enter a valid User ID (1-671).');
        return;
      }
  
      loader.classList.remove('hidden');
      resultsContainer.innerHTML = '';
  
      try {
        const response = await fetch(`http://127.0.0.1:5000/recommendations?userId=${userId}`);
        if (!response.ok) throw new Error('Failed to fetch recommendations.');
  
        const recommendations = await response.json();
        displayRecommendations(recommendations);
  
      } catch (error) {
        alert('Error fetching recommendations: ' + error.message);
      } finally {
        loader.classList.add('hidden');
      }
    }
  
    // Function to display the recommendations
    function displayRecommendations(recommendations) {
      resultsContainer.innerHTML = recommendations.length
        ? recommendations.map(rec => `
            <div class="result-card">
              <h3>${rec.title} (ID: ${rec.movie_id})</h3>
              <p>Genres: ${rec.genres}</p>
              <p class="rating">Average Rating: ${rec.rating.toFixed(2)}</p>
            </div>
          `).join('')
        : '<p>No recommendations found for this User ID.</p>';
    }
  
    // Attach event listener
    searchButton.addEventListener('click', fetchRecommendations);
  });
  