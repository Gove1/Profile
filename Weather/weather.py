import os
import requests
import warnings
from dotenv import load_dotenv

warnings.filterwarnings("ignore", category=Warning)

current_dir = os.path.dirname(os.path.abspath(__file__))
env_path = os.path.join(current_dir, "API.env")

def initialize_api_key():
    load_dotenv(dotenv_path=env_path)
    api_key = os.getenv("OPENWEATHER_API_KEY")
    
    # 키가 없거나 빈 값이라면 새로 입력받기
    if not api_key:
        print(" OpenWeatherMap API 키 설정이 필요합니다.")
        user_key = input(" 발급받은 API 키를 입력해주세요: ").strip()
        
        if not user_key:
            print(" 키가 입력되지 않았습니다. 프로그램을 종료합니다.")
            exit()
            
        with open(env_path, "w", encoding="utf-8") as f:
            f.write(f"OPENWEATHER_API_KEY={user_key}\n")
            
        print(f" 성공: API 키가 '{env_path}'에 안전하게 저장되었습니다.\n")
        
        load_dotenv(dotenv_path=env_path, override=True)
        api_key = os.getenv("OPENWEATHER_API_KEY")
        
    return api_key

def get_weather(city_name):
    
    api_key = initialize_api_key()
    
    url = f"http://api.openweathermap.org/data/2.5/weather?q={city_name}&appid={api_key}&units=metric&lang=kr"
    
    try:
        response = requests.get(url)
        data = response.json()
        
        if data["cod"] == 200:
            city = data["name"]
            weather_desc = data["weather"][0]["description"]
            temp = data["main"]["temp"]
            feels_like = data["main"]["feels_like"]
            humidity = data["main"]["humidity"]
            wind_speed = data["wind"]["speed"]
            
            print(f" [{city}] 현재 날씨 정보")
            print(f"─ 상태: {weather_desc}")
            print(f"─ 기온: {temp}°C (체감 온도: {feels_like}°C)")
            print(f"─ 습도: {humidity}%")
            print(f"─ 풍속: {wind_speed} m/s\n")
        else:
            print(f"! 도시를 찾을 수 없습니다. (에러 코드: {data['cod']})")
            print("영어 도시명으로 정확하게 입력해보세요. (예: Seoul, Gyeongju)")
            
    except Exception as e:
        print(f"! 네트워크 연결 중 오류가 발생했습니다: {e}")

if __name__ == "__main__":
    print(" 실시간 터미널 날씨 알림이 (자동 설정 버전)")
    city = input("조회할 도시 이름을 입력하세요 (영문): ")
    get_weather(city)