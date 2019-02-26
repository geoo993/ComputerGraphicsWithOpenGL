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

#include "Common.h"

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

    /*
     00 01 02 03
     
     04 05 06 07
     
     08 09 10 11
     
     12 13 14 15
     
     or
     
     | 00 04 08 12 |
     | 01 05 09 13 |
     | 02 06 10 14 |
     | 03 07 11 15 |
     
     or
     
     [ 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 ]
     */
     
     /*
     m0 m4 m8  m12
     m1 m5 m9  m13
     m2 m6 m10 m14
     m3 m7 m11 m15
     
         or
     
     int mat[16] = {
         0, 1, 2, 3,
         4, 5, 6, 7,
         8, 9, 10, 11,
         12, 13, 14, 15,
     }
      
      float right[4]    = { 1, 0, 0, 0 };
      float up[4]       = { 0, 1, 0, 0 };
      float forward[4]  = { 0, 0, 1, 0 };
      float position[4] = { 0, 0, 0, 1 };
      
      float matrix[4][4] = {
      {   right[0],    right[1],    right[2],    right[3] }, // First column
      {      up[0],       up[1],       up[2],       up[3] }, // Second column
      { forward[0],  forward[1],  forward[2],  forward[3] }, // Third column
      {position[0], position[1], position[2], position[3] }  // Forth column
      };
      
     
     */
     
    static glm::mat4 convertToMat4( const GLfloat arr[16]){
        return glm::mat4(
                         arr[0],
                         arr[1],
                         arr[2],
                         arr[3],
                         arr[4],
                         arr[5],
                         arr[6],
                         arr[7],
                         arr[8],
                         arr[9],
                         arr[10],
                         arr[11],
                         arr[12],
                         arr[13],
                         arr[14],
                         arr[15]
                         );
    }

    
    static GLfloat * mat4ToArray(const glm::mat4 &mat) {
        return new GLfloat[16]{
            mat[0][0],  mat[0][1],  mat[0][2],  mat[0][3], 
            mat[1][0],  mat[1][1],  mat[1][2],  mat[1][3], 
            mat[2][0],  mat[2][1],  mat[2][2],  mat[2][3], 
            mat[3][0],  mat[3][1],  mat[3][2],  mat[3][3], 
        };
        
        /* or 
         
         a[0] = mat[0][0];
         a[1] = mat[0][1];
         a[2] = mat[0][2];
         a[3] = mat[0][3];
         a[4] = mat[1][0];
         a[5] = mat[1][1];
         a[6] = mat[1][2];
         a[7] = mat[1][3];
         a[8] = mat[2][0];
         a[9] = mat[2][1];
         a[10] = mat[2][2];
         a[11] = mat[2][3];
         a[12] = mat[3][0];
         a[13] = mat[3][1];
         a[14] = mat[3][2];
         a[15] = mat[3][3];
         */
    };
    static std::vector<GLfloat> mat4ToVector(const glm::mat4 &mat) {
        return {
            mat[0][0],  mat[0][1],  mat[0][2],  mat[0][3], 
            mat[1][0],  mat[1][1],  mat[1][2],  mat[1][3], 
            mat[2][0],  mat[2][1],  mat[2][2],  mat[2][3], 
            mat[3][0],  mat[3][1],  mat[3][2],  mat[3][3], 
        };
    };
    
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
    
    bool isPowerOfTwo(int n) {
        return n > 0 && (n & (n - 1)) == 0;
    }
    
    int padToPowerOfTwo(int n) {
        int r = 1;
        while (r < n)
            r <<= 1;
        return r;
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
    
    static float uniformRandomInRange(float min, float max) {
        assert(min < max);
        double n = (double) rand() / (double) RAND_MAX;
        double v = min + n * (max - min);
        return v;
    }
    
    /*
         float lerp(float a, float b, float f)
         {
             return a + f * (b - a);
         }
     */
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
        return float( (double)rand() / (RAND_MAX + 1.0f));
        //return  static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    }
    
    //Random between 2 floats 
    static float  randomFloatBetween(float min, float max)    
    {    
        return (min + 1) + (((float) rand()) / (float) RAND_MAX) * (max - (min + 1));    
    }
    
    //Random between 2 int 
    static int    randomInt(int min, int max)    
    {    
        return rand() % (max - min) + min + 1;     
    }
    
    static glm::vec3 randRGB(){
        return glm::vec3 (randFloat(), randFloat(), randFloat());
    }
    
    static float computeAngleBetween(const glm::vec3 &a, const glm::vec3 &b)
    {
        ////this is the dot product of a/b divided by the magnitude of a mulitplied by magnitude of b
        float ABdotProduct = computeDotProduct(a,b);
        float AabsoluteValue = computeLength(a);
        float BabsoluteValue = computeLength(b);
        
        float AB = AabsoluteValue * BabsoluteValue;
        
        ////then compute the angle with cosine data
        float cosineData = ABdotProduct / AB;
        float angle = acosf(cosineData) * 180.0 / M_PI;
        return angle;
    }
    
    static glm::vec3 computeVectorBetween(const glm::vec3 &a, const glm::vec3 &b){
        ////get the vector between a and b
        return glm::vec3( (b.x - a.x), (b.y - a.y), (b.z - a.z) ) ;
    }
    
    static float computeDotProduct(const glm::vec3 &a, const glm::vec3 &b){
        ////the dot product which is the sum of all the entries multiplied by each other
        return ( (a.x * b.x) + (a.y * b.y) + (a.z * b.z) ) ;
    }
    
    static float computeLength(const glm::vec3 &a){
        ////also known as the magnitude, size or absolute value
        ////can also be known as the hypotenuse
        ////the square root of the sum of all the (entries squared) in the vector 
        return sqrt( ( pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2) ) );
    }
    
    static glm::vec3 computeNormalBetween(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3) 
    { 
        glm::vec3 a = glm::vec3( (p2.x - p1.x), (p2.y - p1.y), (p2.z - p1.z) ) ;
        glm::vec3 b = glm::vec3( (p3.x - p1.x), (p3.y - p1.y), (p3.z - p1.z) ) ;
        return glm::normalize(glm::cross(a, b));
    }
    
    static glm::vec3 computeUnitVector(const glm::vec3 &a){
        //also known as normalised vector, which is the vector with 1 unit length
        float llvll = computeLength(a);
        return (1/llvll) * a; 
    }
    
    static glm::vec3 computeCrossProduct(const glm::vec3 &a, const glm::vec3 &b){
        
//        Meaning cross product of a x b  where a [a1, a2, a3 ] and b [b1, b2, b3]:
//        i =  | a2  a3 |      j = | a1  a3 |    k = | a1  a2 |
//             | b2  b3 |          | b1  b3 |        | b1   b2 |
//        
//        a x b = [ ( (a2 * b3) - (a3 * b2) ), ( (a3 * b1) - (a1 * b3) ),  ( (a1 * b2) - (a2 * b1) )  ]
        glm::vec3 ab = glm::vec3( 
                                 ( (a.y * b.z) - (a.z * b.y) ), 
                                 ( (a.z * b.x) - (a.x * b.z) ),  
                                 ( (a.x * b.y) - (a.y * b.x) )  
                                 );
        
        return a * b;
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
    
    // https://stackoverflow.com/questions/662918/how-do-i-concatenate-multiple-c-strings-on-one-line
    static std::string join(const std::string& separator,
                            const std::vector<std::string>& strings)
    {
        if (strings.empty())
            return "";
        
        if (strings.size() == 1)
            return strings[0];
        
        std::stringstream ss;
        ss << strings[0];
        
        auto aggregate = [&ss, &separator](const std::string& s) { ss << separator << s; };
        for_each(begin(strings) + 1, end(strings), aggregate);
        
        return ss.str();
    }

    // https://stackoverflow.com/questions/15319859/how-to-concatenate-two-strings-in-c
    static char * concat(const char * first, const char * second) {
        int l1 = 0, l2 = 0;
        const char * f = first, * l = second;
        
        // step 1 - find lengths (you can also use strlen)
        while (*f++) ++l1;
        while (*l++) ++l2;
        
        char *result = new char[l1 + l2];
        
        // then concatenate
        for (int i = 0; i < l1; i++) result[i] = first[i];
        for (int i = l1; i < l1 + l2; i++) result[i] = second[i - l1];
        
        // finally, "cap" result with terminating null char
        result[l1+l2] = '\0';
        return result;
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
 
    static rgb converter2rgb(int hexValue)
    {
        rgb rgbColor;
        rgbColor.r = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
        rgbColor.g = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
        rgbColor.b = ((hexValue) & 0xFF) / 255.0;        // Extract the BB byte
        
        return rgbColor; 
    }
    
    static rgb converter2rgb(const char * hexValue)
    {
        int r, g, b;
        sscanf(hexValue, "%02x%02x%02x", &r, &g, &b);
        
        rgb rgbColor;
        rgbColor.r = (double)r;
        rgbColor.g = (double)g;
        rgbColor.b = (double)b;
        
        return rgbColor; 
    }
    ///https://seblagarde.wordpress.com/2011/08/17/feeding-a-physical-based-lighting-mode/
    static rgb silver()
    {
        rgb rgbColor; rgbColor.r = 0.971519; rgbColor.g = 0.959915; rgbColor.b = 0.915324;
        return rgbColor;
    }
    
    static rgb aluminium()
    {
        rgb rgbColor; rgbColor.r = 0.913183; rgbColor.g = 0.921494; rgbColor.b = 0.924524;
        return rgbColor;
    }
    
    static rgb gold()
    {
        rgb rgbColor; rgbColor.r = 1.0; rgbColor.g = 0.765557; rgbColor.b = 0.336057;
        return rgbColor;
    }
    
    static rgb copper()
    {
        rgb rgbColor; rgbColor.r = 0.955008; rgbColor.g = 0.637427; rgbColor.b = 0.538163;
        return rgbColor;
    }
    
    static rgb chromium()
    {
        rgb rgbColor; rgbColor.r = 0.549585; rgbColor.g = 0.556114; rgbColor.b = 0.554256;
        return rgbColor;
    }
    
    static rgb nickel()
    {
        rgb rgbColor; rgbColor.r = 0.659777; rgbColor.g = 0.608679; rgbColor.b = 0.525649;
        return rgbColor;
    }
    
    static rgb titanium()
    {
        rgb rgbColor; rgbColor.r = 0.541931; rgbColor.g = 0.496791; rgbColor.b = 0.449419;
        return rgbColor;
    }
    
    static rgb cobalt()
    {
        rgb rgbColor; rgbColor.r = 0.662124; rgbColor.g = 0.654864; rgbColor.b = 0.633732;
        return rgbColor;
    }
    
    static rgb platinum()
    {
        rgb rgbColor; rgbColor.r = 0.672411; rgbColor.g = 0.637331; rgbColor.b = 0.585456;
        return rgbColor;
    }
    
    static std::vector<std::string> indexColors() {
        std::vector<std::string> c = {
        "#000000", "#FFFF00", "#1CE6FF", "#FF34FF", "#FF4A46", "#008941", "#006FA6", "#A30059",
        "#FFDBE5", "#7A4900", "#0000A6", "#63FFAC", "#B79762", "#004D43", "#8FB0FF", "#997D87",
        "#5A0007", "#809693", "#FEFFE6", "#1B4400", "#4FC601", "#3B5DFF", "#4A3B53", "#FF2F80",
        "#61615A", "#BA0900", "#6B7900", "#00C2A0", "#FFAA92", "#FF90C9", "#B903AA", "#D16100",
        "#DDEFFF", "#000035", "#7B4F4B", "#A1C299", "#300018", "#0AA6D8", "#013349", "#00846F",
        "#372101", "#FFB500", "#C2FFED", "#A079BF", "#CC0744", "#C0B9B2", "#C2FF99", "#001E09",
        "#00489C", "#6F0062", "#0CBD66", "#EEC3FF", "#456D75", "#B77B68", "#7A87A1", "#788D66",
        "#885578", "#FAD09F", "#FF8A9A", "#D157A0", "#BEC459", "#456648", "#0086ED", "#886F4C",
        
        "#34362D", "#B4A8BD", "#00A6AA", "#452C2C", "#636375", "#A3C8C9", "#FF913F", "#938A81",
        "#575329", "#00FECF", "#B05B6F", "#8CD0FF", "#3B9700", "#04F757", "#C8A1A1", "#1E6E00",
        "#7900D7", "#A77500", "#6367A9", "#A05837", "#6B002C", "#772600", "#D790FF", "#9B9700",
        "#549E79", "#FFF69F", "#201625", "#72418F", "#BC23FF", "#99ADC0", "#3A2465", "#922329",
        "#5B4534", "#FDE8DC", "#404E55", "#0089A3", "#CB7E98", "#A4E804", "#324E72", "#6A3A4C",
        "#83AB58", "#001C1E", "#D1F7CE", "#004B28", "#C8D0F6", "#A3A489", "#806C66", "#222800",
        "#BF5650", "#E83000", "#66796D", "#DA007C", "#FF1A59", "#8ADBB4", "#1E0200", "#5B4E51",
        "#C895C5", "#320033", "#FF6832", "#66E1D3", "#CFCDAC", "#D0AC94", "#7ED379", "#012C58"
        };
        return c;
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
    
    
    static long double sciToDouble(const std::string& str) {
        std::stringstream ss(str);
        long double d = 0;
        ss.precision(10);
        ss.showpoint;
        ss.fixed;
        ss >> d;
        
        if (ss.fail()) {
            std::string s = "Unable to format ";
            s += str;
            s += " as a number!";
            throw (s);
        }
        
        return (d);
    }
    
    // boolean to string conversions
    static const char * const BoolToString(bool b){ return b ? "true" : "false"; }
};


#endif /* Extensions_h */
