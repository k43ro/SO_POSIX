#ifndef Functions_H
#define Functions_H

class Functions{
    private:
    static constexpr int MAX = 256;

    public:
    static void read(char * in);
    static int strsplit(char * str, const char * delim, char ** & parts);
    static char * strtrim(char * str);
    static void getredir(char* str, char* & in, char* & out);
};


#endif