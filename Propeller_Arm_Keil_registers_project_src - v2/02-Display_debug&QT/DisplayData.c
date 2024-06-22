/*
 * STM32 Projects
 *
 * Copyright (c) 2017 - Terence M. Darwen - tmdarwen.com
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "DisplayData.h"



void FormatString(char *buffer, size_t bufferSize, const char *format, va_list args) {
    // Use vsnprintf to format the string into the buffer
    vsnprintf(buffer, bufferSize, format, args);
}

void ApplyLevel(char *formattedString, SeverityLevel severity, char *outputBuffer, size_t outputBufferSize) {
    switch (severity) {
		 case DEFAULT:
            snprintf(outputBuffer, outputBufferSize, "%s\n\r", formattedString);
            break;
        case INFO:
            snprintf(outputBuffer, outputBufferSize, "\033[32m%s\033[0m\n\r", formattedString); // Green text for INFO
            break;
        case WARNING:
            snprintf(outputBuffer, outputBufferSize, "\033[33m%s\033[0m\n\r", formattedString); // Yellow text for WARNING
            break;
        case ERROR:
            snprintf(outputBuffer, outputBufferSize, "\033[31m%s\033[0m\n\r", formattedString); // Red text for ERROR
            break;
        default:
            snprintf(outputBuffer, outputBufferSize, "%s\n\r", formattedString); // Default format
            break;
    }
}

void PrintConsole(SeverityLevel severity, const char *format, ...) {
    char buffer[256]; // Create a character buffer to store the formatted string
    char coloredBuffer[256]; // Create a character buffer to store the colorized string

    va_list args;
    va_start(args, format);

    // Step 1: Format the string with placeholders
    FormatString(buffer, sizeof(buffer), format, args);

    va_end(args);

    // Step 2: Apply color to the formatted string based on severity
    ApplyLevel(buffer, severity, coloredBuffer, sizeof(coloredBuffer));

    // Print or send the colored message
	 // SendString("\r");
    SendString(coloredBuffer);
}

void SendValuesToQT(float Time,float theta_acc, float theta_estimated, float pwmg, float pwmd) {
    char values[50];
    sprintf(values, "%.4f;%.4f;%.4f;%.4f;%.4f \r\n", Time/1000, theta_acc, theta_estimated, pwmg, pwmd);
    QTSendFloat(values);
}

void DisplayFloatValue(float value)
{
    // Convert the float number to string
    char valueAsString[12]; // Adjust the size as needed
    snprintf(valueAsString, sizeof(valueAsString), "%lf", value); // "%.5f" formats the float with 5 decimal places
    // Right justify display the string
    int strlen = 0;
    while (valueAsString[strlen])
    {
        strlen++;
    }

    while (strlen < 7)
    {
        SendString(" ");
        ++strlen;
    }

    SendString(valueAsString);
}


void DisplayIntegerValue(short value)
{
	// Convert the number to string
	char valueAsString[12];
	IntegerToString(value, valueAsString, BASE_10);

	// Right justify display the string
	short strlen = 0;
	while(valueAsString[strlen])
	{
		strlen++;
	}

	while(strlen < 7)
	{
		SendString(" ");
		++strlen;
	}

	SendString(valueAsString);
}

void DisplayAxisValues()
{
	
	float accX, accY, accZ;
	GetAccelerometerValuesInMS2(&accX, &accY, &accZ);
	
	short gyroX, gyroY, gyroZ;
	GetGyroValues(&gyroX, &gyroY, &gyroZ);
	
	float display_gyro_X, display_gyro_Y, display_gyro_Z;
	
	display_gyro_X = gyroX;
	display_gyro_Y = gyroY;
	display_gyro_Z = gyroZ;
	
	
	SendString("\r");
	PrintConsole(INFO,"    Accel       Gyro");
	SendString("\r");
	PrintConsole(INFO, "X: %lf   %lf",accX, display_gyro_X);
	SendString("\r");
	PrintConsole(INFO, "Y: %lf   %lf",accY, display_gyro_Y);
	SendString("\r");
	PrintConsole(INFO, "Z: %lf   %lf",accZ, display_gyro_Z);
	SendString("\r");
	
//	lcd_clear();
//	lcd_put_cur (0,0);
//	lcd_send_string("Theta in degree");
//	lcd_put_cur (1,0);
	
//	char display_theta[256];
//	snprintf(display_theta, sizeof(display_theta),"%lf",theta_data.theta);
	//lcd_send_string(display_theta);
	
	GetGyroData_read_fast();
	GetAccData_read_fast();
	
	SendString("\r\n");
	PrintConsole(INFO,"Theta_acc = %lf", acc_data.theta_acc*180/3.1415926535);
	

	SendString("\033[6A");  // Makes cursor got up to lines
	SendString("\r");       // Return cursor to the beginning of the line
}
