#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <limits>
#include <exception>

#define NPOS std::string::npos

class   String: public std::string
{
    public:
            String();
            String(const std::string &ref);
            String(const char *ref);
            String(const String &ref);

            String  operator-(const String &sub);
            String  operator-(const std::string &sub);
            String  operator-(const char *sub);
            String  &operator-=(const String &sub);
            String  &operator-=(const std::string &sub);
            String  &operator-=(const char *sub);

            void                pop_back();
            char                &back();
            bool                getline(std::ifstream   &fd);
            int                 X_occur(char ch);
            int                 wordCount();
            String              getWord(int wordNb);
            String              extractWord(int wordNb);
            void                rmWord(int wordNb);
            void                rmFromWord(int wordNb, char target);
            void                rrmFromWord(int wordNb, char target);
            void                rmAllFromWord(int wordNb, char target);
            int                 wordStartPos(int wordNb);
            int                 wordEndPos(int wordNb);
            char                wordStartChar(int wordNb);
            char                wordEndChar(int wordNb);
            String              lastWord();
            void                bigJoin(char *str1, char *str2, char *str3, char *str4, char *str5);
};

class   Toolbox
{
    public:
            static void skipLine(std::ifstream &fd);
    
    protected:
            virtual int abstract_Status() = 0;
};

class Xception: public std::exception
{
    public:
            Xception(String error);
            Xception(const char *str, char ch);
            Xception(const char *str1, char *str2);
            Xception(const char *str1, char ch, const char *str2);
            Xception(const char *str1, char *str2, const char *str3);
            Xception(String str1, const char *str2);
            ~Xception() throw();
            const char  *what() const throw();

    private:
            String  err_msg;
};