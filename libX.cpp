#include "libX.hpp"

///////////////////////////////////////////////////////////
//                      BUILDERS                        //
/////////////////////////////////////////////////////////

String::String()
: std::string()
{}

String::String(const std::string &ref)
: std::string(ref)
{}

String::String(const char *ref)
: std::string(ref)
{}

String::String(const String &ref)
:std::string(ref.c_str())
{}


/////////////////////////////////////////////////////////////
///                     OPERATORZ                          //
/////////////////////////////////////////////////////////////

String    String::operator-(const String &sub)
{
    String  str(*this);
    int     found = 0;

    while (found != -1)
    {
        found = str.find(sub);
        if (found != -1)
            str.erase(found, sub.size());
    }
    return (str);
}

String    String::operator-(const std::string &sub)
{
    String  str(*this);
    int     found = 0;

    while (found != -1)
    {
        found = str.find(sub);
        if (found != -1)
            str.erase(found, sub.size());
    }
    return (str);
}

String    String::operator-(const char *sub)
{
    String  str(*this);
    int     found = 0;
    int     lenght = 0;

    while (sub[lenght])
        lenght++;
    while (found != -1)
    {
        found = str.find(sub);
        if (found != -1)
            str.erase(found, lenght);
    }
    return (str);
}

String  &String::operator-=(const String &sub)
{
    int     found;

    for (found = this->find(sub); found != -1; found = this->find(sub))
        this->erase(found, sub.size());
    return (*this);
}

String  &String::operator-=(const std::string &sub)
{
    int     found;

    for (found = this->find(sub); found != -1; found = this->find(sub))
        this->erase(found, sub.size());
    return (*this);
}

String  &String::operator-=(const char *sub)
{
    int     found;
    int     size = 0;

    while (sub[size])
        size++;
    for (found = this->find(sub); found != -1; found = this->find(sub))
        this->erase(found, size);
    return (*this);
}


///////////////////////////////////////////////////////////////
//                       FONCTIONS                          //
//////////////////////////////////////////////////////////////


void    String::pop_back()
{
    if (!this->empty())    
        erase(this->size() - 1, 1);
}

char    &String::back()
{
    return (at(size() - 1));
}

bool    String::getline(std::ifstream   &fd)
{
    char    ch = 'Q';

    this->clear();
    while(ch != '\n')
    {
        if (!fd.get(ch))
        {
            if (this->size() != 0)
                return (true);
            return (false);
        }
        if (ch != '\n')
            this->push_back(ch);
    }
    return (true);
}

int     String::X_occur(char ch)
{
    int     x = 0;
    int     nb = 0;
    String  tmp(*this);

    while (tmp[x])
    {
        if (tmp[x] == ch)
            nb++;
        x++;
    }
    return (nb);
}

int         String::wordCount()//compte les mots
{
    String  tmp(*this);
    int     count = 0;
    
    for (unsigned int i = 0; i < tmp.size(); i++)
    {
        for (; tmp[i] == ' ' && tmp[i]; i++);

        if (tmp[i] != '\0')
            count++;

        for (; tmp[i] != ' ' && tmp[i]; i++);
    }
    return (count); 
}

String      String::getWord(int wordNb)//renvoi le mot que tu veux
{
    String  word(*this);
    int     s = wordStartPos(wordNb);
    int     e = wordEndPos(wordNb);
    
    if (s != -1 && e != -1)
        word = word.substr(s, (e + 1) - s);
    return (word);
}

void    String::rmWord(int wordNb)//supprime le mot que tu veux bb
{
    String  tmp(*this);
    int     start = tmp.wordStartPos(wordNb);
    int     end = tmp.wordEndPos(wordNb);

    if (start == -1 || end == -1)
        return;

    tmp.erase(start, (end + 1) - start + 1);
    *this = tmp;
}

void    String::rrmFromWord(int wordNb, char target)
{
    String  tmp(*this);
    int     idx = tmp.wordEndPos(wordNb);

    if (wordNb <= 0 || wordNb > this->wordCount() || idx == -1)
        return;

    for (; tmp[idx] != target && tmp[idx] != ' ' && idx >= 0; idx--);

    if (tmp[idx] == target)
        tmp.erase(idx, 1);
    
    *this = tmp;
}

void    String::rmFromWord(int wordNb, char target)
{
    String  tmp(*this);
    int     idx = tmp.wordStartPos(wordNb);

    if (wordNb <= 0 || wordNb > this->wordCount() || idx == -1)
        return;

    for (; tmp[idx] != target && tmp[idx] != ' ' && tmp[idx]; idx++);

    if (tmp[idx] == target)
        tmp.erase(idx, 1);
    
    *this = tmp;
}

void    String::rmAllFromWord(int wordNb, char target)
{
    String  tmp(*this);
    int     idx = tmp.wordStartPos(wordNb);

    if (wordNb <= 0 || wordNb > this->wordCount() || idx == -1 || target == ' ')
        return;

    for (; tmp[idx] != ' ' && tmp[idx]; idx++)
    {    
        if (tmp[idx] == target)
        {    
            tmp.erase(idx, 1);
            idx--;
        }
    }
    *this = tmp;
}

int     String::wordStartPos(int wordNb)
{
    String  tmp(*this);
    int     count = 0;
    int     i = 0;

    if (wordNb <= 0 || wordNb > this->wordCount())
        return (-1);

    while (count != wordNb && tmp[i])
    {
        for (; tmp[i] == ' ' && tmp[i]; i++);

        if (tmp[i] != '\0')
            count++;
        if (count != wordNb)
            for (; tmp[i] != ' ' && tmp[i]; i++);
    }
    if (tmp[i] == '\0')
        return (-1);
    return (i);
}

int     String::wordEndPos(int wordNb)
{
        String  tmp(*this);
        int     e = tmp.wordStartPos(wordNb);

        if (wordNb <= 0 || wordNb > this->wordCount() || e == -1)
            return (-1);

        for (; tmp[e + 1] != '\0' && tmp[e + 1] != ' '; e++);
        
        return (e);
}

char    String::wordStartChar(int wordNb)
{
    String  tmp(*this);
    int     pos = tmp.wordStartPos(wordNb);

    if (pos == -1)
        return ('\0');

    return (tmp[tmp.wordStartPos(wordNb)]);
}

char    String::wordEndChar(int wordNb)
{
    String  tmp(*this);
    int     pos = tmp.wordEndPos(wordNb);

    if (pos == -1)
        return ('\0');

    return (tmp[pos]);
}



void    String::bigJoin(char *str1, char *str2, char *str3, char *str4, char *str5)
{
    String tmp;

    if (str1 != NULL)
        tmp = str1;
    if (str2 != NULL)    
        tmp += str2;
    if (str3 != NULL)
        tmp += str3;
    if (str4 != NULL)
        tmp += str4;
    if (str5 != NULL)
        tmp += str5;
    *this = tmp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         TOOLBOX                                                                   //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Toolbox::skipLine(std::ifstream &fd)
{
    char ch = 'Q';

    while (ch != EOF && ch != '\n')
        fd.get(ch);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        XCEPTION                                                                   //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



Xception::Xception(String error)
: err_msg(error)
{}

Xception::Xception(const char *str, char ch)
{
    err_msg = str;
    err_msg.push_back(ch);
}

Xception::Xception(const char *str1, char *str2)
{
    err_msg = str1;
    err_msg.append(str2);
}

Xception::Xception(const char *str1, char ch, const char *str2)
{
    err_msg = str1;
    err_msg.push_back(ch);
    err_msg += str2;
}

Xception::Xception(const char *str1, char *str2, const char *str3)
{
    err_msg = str1;
    err_msg.append(str2);
    err_msg += str3;
}

Xception::Xception(String str1, const char *str2)
{
    err_msg = str1;
    err_msg.append(str2);
}

Xception::~Xception() throw ()
{}

const char *Xception::what() const throw()
{
    return (err_msg.c_str());
}