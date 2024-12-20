﻿#include <windows.h>
#include <iostream>
#include <winhttp.h>
#include <string>
#include <map>
#include <set>

// Підключення winhttp.lib
#pragma comment(lib, "winhttp.lib")


void printHeaderTop() {
    for (int i = 0; i < 45; i++) {
        std::cout << "-";
    }
    std::cout << "Received information to server";
    for (int i = 0; i < 45; i++) {
        std::cout << "-";
    }
}

void printHeaderDown() {
    for (int i = 0; i < 120; i++) {
        std::cout << "-";
    }
    std::cout << "\n\n";
}

// Конвертація вузького рядка у широкий
std::wstring stringToWideString(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
    return wstr;
}

// Сервіси
std::set<std::string> subscriptions;

void subscribe(const std::string& service) {
    subscriptions.insert(service);
}

void unsubscribe(const std::string& service) {
    subscriptions.erase(service);
}

std::string get_subscriptions() {
    if (subscriptions.empty()) {
        return "No subscriptions found.";
    }
    std::string result = "Subscribed services:";
    for (const auto& service : subscriptions) {
        result += " " + service;
    }
    return result;
}

// Функція HTTP GET-запиту
std::string HttpGetRequest(const std::string& server, const std::string& url) {
    HINTERNET hSession = nullptr, hConnect = nullptr, hRequest = nullptr;
    std::string response;

    // Відкрити сесію
    hSession = WinHttpOpen(L"WeatherApp/1.0",
        WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (hSession) {
        hConnect = WinHttpConnect(hSession, stringToWideString(server).c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
    }
    else {
        std::cerr << "Failed to open session. Error: " << GetLastError() << std::endl;
        return "";
    }

    if (hConnect) {
        hRequest = WinHttpOpenRequest(hConnect, L"GET", stringToWideString(url).c_str(), nullptr, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    }
    else {
        std::cerr << "Failed to connect to server. Error: " << GetLastError() << std::endl;
        return "";
    }

    if (hRequest) {
        BOOL sent = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
            WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
        if (sent && WinHttpReceiveResponse(hRequest, nullptr)) {
            DWORD bytesRead = 0;
            char buffer[4096];
            do {
                if (WinHttpReadData(hRequest, buffer, sizeof(buffer), &bytesRead)) {
                    response.append(buffer, bytesRead);
                }
            } while (bytesRead > 0);
        }
        else {
            std::cerr << "Failed to send or receive request. Error: " << GetLastError() << std::endl;
        }
    }
    else {
        std::cerr << "Failed to open request. Error: " << GetLastError() << std::endl;
    }

    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);

    return response;
}

// Отримати погоду
std::string get_weather() {
    std::string server = "api.openweathermap.org";
    std::string url = "/data/2.5/weather?q=Lviv&appid=6e40e88884e9158103f7fead74988026"; // Ваш API ключ

    std::string response = HttpGetRequest(server, url);

    // Вивід отриманої відповіді для діагностики
    std::cout << "Weather request processed successfully\n\n";
    //printHeaderTop();
    //std::cout << "Raw API Response: " << response << "\n";
    //printHeaderDown();

    // Отримання температури
    size_t tempPos = response.find("\"temp\":");
    size_t humidityPos = response.find("\"humidity\":");
    size_t weatherPos = response.find("\"description\":");
    size_t windPos = response.find("\"speed\":");

    if (tempPos != std::string::npos && humidityPos != std::string::npos &&
        weatherPos != std::string::npos && windPos != std::string::npos) {

        // Температура
        size_t tempEnd = response.find(',', tempPos);
        std::string temperatureK = response.substr(tempPos + 7, tempEnd - tempPos - 7);
        float temperatureC = std::stof(temperatureK) - 273.15f;  // Переведення з Кельсії в Цельсія

        // Вологість
        size_t humidityEnd = response.find(',', humidityPos);
        std::string humidity = response.substr(humidityPos + 11, humidityEnd - humidityPos - 11);

        // Опис погоди
        size_t weatherStart = response.find('"', weatherPos + 14);
        size_t weatherEnd = response.find('"', weatherStart + 1);
        std::string weatherDescription = response.substr(weatherStart + 1, weatherEnd - weatherStart - 1);

        // Швидкість вітру
        size_t windEnd = response.find(',', windPos);
        std::string windSpeed = response.substr(windPos + 8, windEnd - windPos - 8);

        return "Weather in Lviv:\n"
            "Temperature: " + std::to_string(temperatureC) + "°C\n"
            "Humidity: " + humidity + "%\n"
            "Description: " + weatherDescription + "\n"
            "Wind Speed: " + windSpeed + " m/s";
    }

    return "Unable to retrieve weather data.";
}

std::string get_currency_rates() {
    std::string server = "bank.gov.ua";
    std::string url = "/NBUStatService/v1/statdirectory/exchange?json";

    std::string response = HttpGetRequest(server, url);

    // Вивід отриманої відповіді для діагностики
    std::cout << "Currency request processed successfully\n\n";
    //printHeaderTop();
    //std::cout << "Raw Currency API Response: " << response << "\n";
    //printHeaderDown();

    // Пошук даних для долара, євро та фунта стерлінгів
    size_t usdPos = response.find("\"r030\":840"); // Код долара США
    size_t eurPos = response.find("\"r030\":978"); // Код євро
    size_t gbpPos = response.find("\"r030\":826"); // Код фунта стерлінгів

    if (usdPos != std::string::npos && eurPos != std::string::npos && gbpPos != std::string::npos) {
        // Курс долара
        size_t usdRatePos = response.find("\"rate\":", usdPos);
        size_t usdRateEnd = response.find(',', usdRatePos);
        std::string usdRate = response.substr(usdRatePos + 7, usdRateEnd - usdRatePos - 7);

        // Курс євро
        size_t eurRatePos = response.find("\"rate\":", eurPos);
        size_t eurRateEnd = response.find(',', eurRatePos);
        std::string eurRate = response.substr(eurRatePos + 7, eurRateEnd - eurRatePos - 7);

        // Курс фунта стерлінгів
        size_t gbpRatePos = response.find("\"rate\":", gbpPos);
        size_t gbpRateEnd = response.find(',', gbpRatePos);
        std::string gbpRate = response.substr(gbpRatePos + 7, gbpRateEnd - gbpRatePos - 7);

        return "Currency Rates:\n"
            "USD: " + usdRate + " UAH\n"
            "EUR: " + eurRate + " UAH\n"
            "GBP: " + gbpRate + " UAH";
    }

    return "Unable to retrieve currency rates.";
}

std::string get_stock_price(const std::string& symbol) {
    std::string server = "finnhub.io";
    std::string apiKey = "c34391qad3i8edlcgrgg"; // Замість ВАШ_API_КЛЮЧ вставте ваш реальний ключ
    std::string url = "/api/v1/quote?symbol=" + symbol + "&token=" + apiKey;

    std::string response = HttpGetRequest(server, url);

    // Вивід отриманої відповіді для діагностики
    //std::cout << "Raw Stock API Response for " << symbol << ": " << response << "\n";

    // Пошук поточної ціни акції (поле "c")
    size_t currentPricePos = response.find("\"c\":");

    if (currentPricePos != std::string::npos) {
        size_t priceEnd = response.find(',', currentPricePos);
        std::string currentPrice = response.substr(currentPricePos + 4, priceEnd - currentPricePos - 4);

        return "Current price of " + symbol + ": " + currentPrice + " USD";
    }

    return "Unable to retrieve stock price for " + symbol + ".";
}

// Приклад використання
std::string get_stocks() {
    std::string result;
    std::cout << "Stock request processed successfully\n\n";
    //printHeaderTop();
    result += get_stock_price("MSFT") + "\n"; // Microsoft
    result += get_stock_price("PYPL") + "\n"; // PayPal
    result += get_stock_price("AAPL") + "\n"; // Apple
    //printHeaderDown();
    return result;
}

// Основний цикл сервера
int main() {
    std::string PIPE_NAME = R"(\\.\pipe\server_pipe)";
    std::cout << "Server started. Waiting for clients..." << std::endl;

    bool clientConnected = false;

    do {
        HANDLE hPipe = CreateNamedPipe(
            stringToWideString(PIPE_NAME).c_str(),
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES, 512, 512, 0, nullptr);

        if (hPipe == INVALID_HANDLE_VALUE) {
            std::cerr << "Failed to create pipe. Error: " << GetLastError() << std::endl;
            return 1;
        }

        if (!ConnectNamedPipe(hPipe, nullptr) && GetLastError() != ERROR_PIPE_CONNECTED) {
            std::cerr << "Failed to connect client. Error: " << GetLastError() << std::endl;
            CloseHandle(hPipe);
            continue;
        }
        
        if (!clientConnected) {
            std::cout << "Client connected successfully!\n";
            clientConnected = true; // Встановлюємо прапорець
        }

        char buffer[256];
        DWORD bytesRead;
        if (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr)) {
            buffer[bytesRead] = '\0';
            std::string request(buffer);

            std::cout << "(Received data from client: " << request << ")" << std::endl;

            size_t pos = request.find(':');
            if (pos == std::string::npos) {
                std::cerr << "Invalid request format." << std::endl;
                CloseHandle(hPipe);
                continue;
            }

            std::string command = request.substr(0, pos);
            std::string service = request.substr(pos + 1);

            std::string response;
            
            if (command == "subscribe") {
                subscribe(service);
                response = "Subscribed to " + service;
                std::cout << "Client " << response << "\n";
            }
            else if (command == "unsubscribe") {
                unsubscribe(service);
                response = "Unsubscribed from " + service;
                std::cout << "Client " << response << "\n\n";
            }
            else if (command == "get") {
                response = get_subscriptions();
            }
            else if (command == "weather" && subscriptions.count("weather")) {
                response = get_weather();
            }
            else if (command == "currency" && subscriptions.count("currency")) {
                response = get_currency_rates();
            }
            else if (command == "stocks" && subscriptions.count("stocks")) {
                response = get_stocks();
            }
            else {
                response = "Unknown command or no subscription to the service.";
            }


            DWORD bytesWritten;
            WriteFile(hPipe, response.c_str(), response.size(), &bytesWritten, nullptr);
        }
        CloseHandle(hPipe);
    } while (true);

    return 0;
}
