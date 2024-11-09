from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import subprocess
import re

app = FastAPI()

from fastapi.middleware.cors import CORSMiddleware

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

class MovieRecommendation(BaseModel):
    movieId: int
    title: str
    genres: str
    averageRating: float

class RecommendationResponse(BaseModel):
    recommendations: list[MovieRecommendation]

@app.get("/recommend/{user_id}", response_model=RecommendationResponse)
async def get_recommendations(user_id: int):
    if user_id < 1 or user_id > 671:
        raise HTTPException(status_code=400, detail="User ID must be between 1 and 671")

    try:
    
        result = subprocess.run(["./ds"], input=str(user_id), capture_output=True, text=True, check=True)
   
        
        recommendations = []
        lines = result.stdout.strip().split("\n")
        
    
    
        if lines[0].startswith("Movie ID | Title | Genres | Average Rating"):
            lines = lines[1:]  

        for line in lines:
            parts = re.split(r'\s*\|\s*', line.strip())
            if len(parts) == 4:
                try:
                    movie_id = int(parts[0])
                    title = parts[1]
                    genres = parts[2]
                    avg_rating = float(parts[3])
                    recommendations.append({
                        "movieId": movie_id,
                        "title": title,
                        "genres": genres,
                        "averageRating": avg_rating
                    })
                except ValueError as e:
                    pass
            else:
                pass
                
        return {"recommendations": recommendations}
    
    except subprocess.CalledProcessError as e:
        print("Error running C++ backend:", e.stderr)
        raise HTTPException(status_code=500, detail="Error processing the request in C++ backend.")
    except Exception as ex:
        print("Unexpected error:", ex)
        raise HTTPException(status_code=500, detail="An unexpected error occurred.")
