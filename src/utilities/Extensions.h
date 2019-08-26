//
//  Extensions.hpp
//  OpenGL_Course_Work
//
//  Created by GEORGE QUENTIN on 11/02/2017.
//  Copyright © 2017 GEORGE QUENTIN. All rights reserved.
//

#pragma once

#ifndef Extensions_h
#define Extensions_h

#include "../UtilitiesBase.h"
#include "Colours.h"

template<typename T>
glm::tvec4<T> tvec4_from_t(const T *arr) {
    return glm::tvec4<T>(arr[0], arr[1], arr[2], arr[3]);
}

template<typename T>
glm::tmat4x4<T> tmat4_from_t(const T *arr) {
    return glm::tmat4x4<T>(tvec4_from_t(arr), tvec4_from_t(arr + 4), tvec4_from_t(arr + 8), tvec4_from_t(arr + 12));
}

/*  example  GLfloat biasMatrixf[16] 
 //glm::mat4 biasMatrix = glm::make_mat4(biasMatrixf); // or //tmat4_from_t(biasMatrixf);
*/     

class Extensions {
    
public:

    static std::vector<glm::vec3> sortByNearestToCameraFirst(std::vector<glm::vec3> & positions, glm::vec3 &cameraPosition) {
        
        
        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < positions.size(); i++)
        {
            float distance = glm::length(cameraPosition - positions[i]);
            sorted[distance] = positions[i];
        }
        
        std::vector<glm::vec3> result;
        for(std::map<float,glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            result.push_back(it->second) ;
            
        }
        
        return result;
    }
    
    static float clamp(float min, float value, float max) {
        if (value < min)
            return min;
        else if (value > max)
            return max;
        else
            return value;
    }
    
    std::string format(const std::string &fmt, ...) {
        va_list args;
        
        va_start(args, fmt);
        char *buffer = NULL;
        int size = vsnprintf(buffer, 0, fmt.c_str(), args);
        va_end(args);
        buffer = new char[size + 1];
        
        va_start(args, fmt);
        vsnprintf(buffer, size + 1, fmt.c_str(), args);
        va_end(args);
        std::string str(buffer);
        delete [] buffer;
        
        va_end(args);
        return str;
    }
    
    double getTime() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        double time = (double) tv.tv_sec + 1e-6 * (double) tv.tv_usec;
        return time;
    }
    
    /// https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
    // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
    const std::string currentDateTime() {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        
        return buf;
    }
    
    std::string readTextFromFile(const std::string &filename) {
        std::ifstream fin(filename.c_str(), std::ios::binary);
        fin.seekg(0, std::ios::end);
        int size = fin.tellg();
        fin.seekg(0, std::ios::beg);
        char *buffer = new char[size + 1];
        fin.read(buffer, size);
        fin.close();
        buffer[size] = '\0';
        std::string str(buffer);
        delete [] buffer;
        return str;
    }

    static float interpolate(float a, float b, float f) {
        return a + f * (b - a);
    }
    
    static int linearInterpolation(int a, int b, float t)
    {
        return a * (1 - t) + b * t;
    }
   
    static float blend(float a, float b, float alpha)
    {
        return (1.0f - alpha) * a + alpha * b;
    }
    
    static glm::vec3 interpolateColor(const glm::vec3 &startColor, const glm::vec3 &endColor, const float &progress) {
        
        return glm::vec3(
                         interpolate(startColor.x, endColor.x, progress),
                         interpolate(startColor.y, endColor.y, progress),
                         interpolate(startColor.z, endColor.z, progress)
                         );
    }
    
    static glm::vec3 interpolateColorWith(glm::vec3 &startColor, glm::vec3 &endColor, float &interval, std::string &colorSwitch) {
        float colorProgress = interval/255.0f;
        if(colorSwitch == "goingUp")
        {
            interval += 1.0f;
        }
        if(colorSwitch == "goingDown")
        {
            interval -= 1.0f;
        }
        
        if(colorProgress <= 0.0f)
        {
            colorSwitch = "goingUp";
            endColor = randRGB();
        }
        if(colorProgress >= 1.0f)
        {
            startColor = randRGB();
            colorSwitch = "goingDown";
        }
        
        return interpolateColor(startColor, endColor, colorProgress);
    }
    
    static float randFloat(){
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> distBetween(0, RAND_MAX);
        return (float)distBetween(rng);
    }
    
    static float randUnitFloat() {
        return (float)randFloat() / ((float)RAND_MAX);
    }
    
    static float  randomFloat(float min, float max)
    {
        float minimum = min < max ? min : max;
        return randUnitFloat() * abs(float(min - max)) + minimum;
    }
    
    static glm::vec3 randRGB(){
        return glm::vec3 (randUnitFloat(), randUnitFloat(), randUnitFloat());
    }
    
  
    static float percentageWithf(const float &value, const float &maxValue, const float &minValue) 
    {
        float difference = (minValue < 0) ? maxValue : maxValue - minValue;
        return (float(100) * ((value - minValue) / difference));
    }
    
    static float getPercentangeValuef( const float &percent, const float &maxValue, const float &minValue){
        float max = (maxValue > minValue) ? maxValue : minValue;
        float min = (maxValue > minValue) ? minValue : maxValue;
        return ( ( (max - min) * percent) / float(100) ) + min;
    }
    
  
    // https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
    static hsv rgb2hsv(rgb in)
    {
        hsv         out;
        double      min, max, delta;
        
        min = in.r < in.g ? in.r : in.g;
        min = min  < in.b ? min  : in.b;
        
        max = in.r > in.g ? in.r : in.g;
        max = max  > in.b ? max  : in.b;
        
        out.v = max;                                // v
        delta = max - min;
        if (delta < 0.00001)
        {
            out.s = 0;
            out.h = 0; // undefined, maybe nan?
            return out;
        }
        if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
            out.s = (delta / max);                  // s
        } else {
            // if max is 0, then r = g = b = 0              
            // s = 0, h is undefined
            out.s = 0.0;
            out.h = NAN;                            // its now undefined
            return out;
        }
        if( in.r >= max )                           // > is bogus, just keeps compilor happy
            out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
        else
            if( in.g >= max )
                out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
            else
                out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan
        
        out.h *= 60.0;                              // degrees
        
        if( out.h < 0.0 )
            out.h += 360.0;
        
        return out;
    }
    
    
    static rgb hsv2rgb(hsv in)
    {
        double      hh, p, q, t, ff;
        long        i;
        rgb         out;
        
        if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
            out.r = in.v;
            out.g = in.v;
            out.b = in.v;
            return out;
        }
        hh = in.h;
        if(hh >= 360.0) hh = 0.0;
        hh /= 60.0;
        i = (long)hh;
        ff = hh - i;
        p = in.v * (1.0 - in.s);
        q = in.v * (1.0 - (in.s * ff));
        t = in.v * (1.0 - (in.s * (1.0 - ff)));
        
        switch(i) {
            case 0:
                out.r = in.v;
                out.g = t;
                out.b = p;
                break;
            case 1:
                out.r = q;
                out.g = in.v;
                out.b = p;
                break;
            case 2:
                out.r = p;
                out.g = in.v;
                out.b = t;
                break;
                
            case 3:
                out.r = p;
                out.g = q;
                out.b = in.v;
                break;
            case 4:
                out.r = t;
                out.g = p;
                out.b = in.v;
                break;
            case 5:
            default:
                out.r = in.v;
                out.g = p;
                out.b = q;
                break;
        }
        return out;     
    }

    // https://stackoverflow.com/questions/13488957/interpolate-from-one-color-to-another
    static rgb interpolateColors(rgb a, rgb b, float t)
    {
        // 0.0 <= t <= 1.0
        hsv ca = rgb2hsv(a);
        hsv cb = rgb2hsv(b);
        hsv finalColor;
        
        finalColor.h = linearInterpolation(ca.h, cb.h, t);
        finalColor.s = linearInterpolation(ca.s, cb.s, t);
        finalColor.v = linearInterpolation(ca.v, cb.v, t);
        
        return hsv2rgb(finalColor);
    }

    // https://stackoverflow.com/questions/14375156/how-to-convert-a-rgb-color-value-to-an-hexadecimal-value-in-c
    static GLuint createRGB(int r, int g, int b)
    {   
        return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
    }
    //Now instead of:     unsigned long rgb = 0xFA09CA;
    //you can do:         unsigned long rgb = createRGB(0xFA, 0x09, 0xCA);
        
    static GLuint createRGBA(int r, int g, int b, int a)
    {   
        return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
        + (a & 0xff);
    }
    
  
    // boolean to string conversions
    static const char * const BoolToString(bool b){ return b ? "true" : "false"; }
};


#endif /* Extensions_h */
