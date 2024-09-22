#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"
#include <regex>

// for convenience
using json = nlohmann::json;

// Function to handle the data received by cURL
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp)
{
    size_t totalSize = size * nmemb;
    // Append the received data to the string
    userp->append((char *)contents, totalSize);
    return totalSize;
}

// Function to make an HTTP GET request
std::string httpGet(const std::string &url)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl)
    {
        // TODO: Set the URL for the request
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // TODO: Set the write callback to WriteCallback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        // TODO: Pass the readBuffer to the callback
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        // TODO: Perform the request and check for errors
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

int main()
{
    std::string apiKey = "4dbb09ae7690fe13510dd7030ce4f1ca"; // TODO: Replace with your actual API key
    std::string city;

    std::cout << "Enter city name: ";
    std::getline(std::cin >> std::ws, city);

    // Strip city of any leading or trailing spaces
    city.erase(0, city.find_first_not_of(" "));
    city.erase(city.find_last_not_of(" ") + 1);

    // Join city spaces by %20
    std::string joinedCity = std::regex_replace(city, std::regex(" "), "%20");

    // TODO: Construct the API URL with the city and API key
    std::string url = "http://api.openweathermap.org/data/2.5/weather?q=" + joinedCity + "&appid=" + apiKey + "&units=metric";

    std::string response = httpGet(url);

    if (response.empty())
    {
        std::cerr << "Failed to get weather data." << std::endl;
        return 1;
    }

    // TODO: Parse the JSON response
    json weatherData;
    try
    {
        weatherData = json::parse(response);
    }
    catch (json::parse_error &e)
    {
        std::cerr << "JSON Parse Error: " << e.what() << std::endl;
        return 1;
    }

    // TODO: Extract and display relevant weather information
    std::cout << "Weather in " << city << ":" << std::endl;
    std::cout << "Temperature: " << weatherData["main"]["temp"] << " C" << std::endl;
    std::cout << "Humidity: " << weatherData["main"]["humidity"] << "%" << std::endl;
    std::cout << "Description: " << weatherData["weather"][0]["description"] << std::endl;

    return 0;
}
