// weather.c
#include "weather_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include <unistd.h>
#include "email_sender.h" 


int main() {
    const char *api_key = "ee5de1217e2254d61409fffa4abad982";
    char city[100];

    // Hardcode the city name to "California"
    strcpy(city, "California");
    get_weather(api_key, city);

    // Print a message indicating that the API response has been obtained
    //printf("API response obtained for %s\n", city);
    const char *api_response = api_return_data;

    // Print the obtained API response
    //printf("API response before save_raw_data:\n%s\n", api_response);

    // Save API response to raw_data.txt
    save_raw_data(api_response, "raw_data.txt");
    
    // Example usage of send_email_with_attachment
    const char *recipient_email = "jamil4599991@cloud.neduet.edu.pk";
    const char *cc_email ="jamil4599991@cloud.neduet.edu.pk";
    const char *attachment_path = "report.txt";

    // ... (Previous code remains unchanged)



// ... (Continue with the rest of your code)

    // ... (Previous code remains unchanged)

    int result = send_email_with_attachment(recipient_email, cc_email, attachment_path);

    if (result == 0) {
    printf("Email sent successfully!\n");
    }  else {
        fprintf(stderr, "Failed to send email. Error code: %d\n", result);
    }

// ... (Continue with the rest of your code)


    

    // Print a message after attempting to save
    //printf("After save_raw_data\n");
    CURL *curl;
    CURLcode res;

    // Rest of your code remains unchanged
    // ...


    

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Create a curl handle
    curl = curl_easy_init();
    if (curl) {
        // Set the API endpoint and parameters
        char url[256];
        sprintf(url, "https://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric", city, api_key);

        // Create a buffer to store the received data
        char data[4096] = {0};

        // Set the URL to fetch
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the write callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);

        // Perform the HTTP request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Parse and display the weather information
            json_t *root;
            json_error_t error;
            root = json_loads(data, 0, &error);

            if (!root) {
                fprintf(stderr, "JSON parsing error: %s\n", error.text);
            } else {
                // Extract relevant information from the JSON response
                //const char *api_response = "{\"coord\":{\"lon\":67.0822,\"lat\":24.9056},\"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear sky\",\"icon\":\"01n\"}],\"base\":\"stations\",\"main\":{\"temp\":288.05,\"feels_like\":286.67,\"temp_min\":288.05,\"temp_max\":288.05,\"pressure\":1019,\"humidity\":41},\"visibility\":6000,\"wind\":{\"speed\":2.06,\"deg\":340},\"clouds\":{\"all\":0},\"dt\":1704912033,\"sys\":{\"type\":1,\"id\":7576,\"country\":\"PK\",\"sunrise\":1704853080,\"sunset\":1704891555},\"timezone\":18000,\"id\":1174872,\"name\":\"Karachi\",\"cod\":200}";

    // Save API response to raw_data.txt
                //save_raw_data(api_response, "raw_data.txt");
                double temperature = json_number_value(json_object_get(json_object_get(root, "main"), "temp"));
                //double humidity = json_number_value(json_object_get(json_object_get(root, "main"), "humidity"));
                //double wind_speed = json_number_value(json_object_get(json_object_get(root, "wind"), "speed"));
                //double air_pressure = json_number_value(json_object_get(json_object_get(root, "main"), "pressure"));
                //const char *country = json_string_value(json_object_get(json_object_get(root, "sys"), "country"));

                // Display the weather information
                //printf("Temperature: %.2f°C\n", temperature);
                //printf("Humidity: %.2f%%\n", humidity);
               // printf("Wind Speed: %.2f m/s\n", wind_speed);
                //printf("Air Pressure: %.2f hPa\n", air_pressure);
                //printf("Country: %s\n", country);

                // Additional variables for humidity, wind speed, and air pressure
                double *humidity_values, *wind_speed_values, *air_pressure_values;
                int count;

                // Calculate average humidity
                humidity_values = calculate_average_humidity(root, &count);
                if (humidity_values == NULL) {
                    // Handle memory allocation error
                    return 1;
                }
                double average_humidity = calculate_average_value(humidity_values, count);

                // Calculate average wind speed
                wind_speed_values = calculate_average_wind_speed(root, &count);
                if (wind_speed_values == NULL) {
                    // Handle memory allocation error
                    return 1;
                }
                double average_wind_speed = calculate_average_value(wind_speed_values, count);

                // Calculate average air pressure
                air_pressure_values = calculate_average_air_pressure(root, &count);
                if (air_pressure_values == NULL) {
                    // Handle memory allocation error
                    return 1;
                }
                double average_air_pressure = calculate_average_value(air_pressure_values, count);

                // Generate report
                generate_process_data(temperature, average_humidity, average_wind_speed, average_air_pressure);
                

                // Execute the shell script
                execute_shell_script();

                // Display real-time alert
                display_alert(THRESHOLD);

                // Free allocated memory for humidity, wind speed, and air pressure
                free(humidity_values);
                free(wind_speed_values);
                free(air_pressure_values);

                // Cleanup JSON object
                json_decref(root);
            }
        }

        // Cleanup curl handle
        curl_easy_cleanup(curl);
    }

    // Cleanup global state
    curl_global_cleanup();

    return 0;
}
