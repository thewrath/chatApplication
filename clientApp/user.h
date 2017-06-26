#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();

    //getters and setters
    QString getName(){ return this->name; }
    QString getEmail(){ return this->email; }
    QString getPassword(){ return this->password; }
    void setName(QString name){ this->name = name; }
    void setEmail(QString email){ this->email = email; }
    void setPassword(QString password){ this->password = password; }

private:
    QString name;
    QString email;
    QString password;
};

#endif // USER_H
