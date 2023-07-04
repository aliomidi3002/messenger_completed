#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QUrlQuery>
#include <QtNetwork>
#include <iostream>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QByteArray>
QString getAndRemoveFirst30000Characters(QString& inputString)
{
    int maxLength = qMin(inputString.length(), 30000);
    QString first30000Chars = inputString.left(maxLength);
    inputString.remove(0, maxLength);
    return first30000Chars;
}

QString readFileToString(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Error: Unable to open the file
        return QString();
    }

    QTextStream in(&file);
    QString fileContent = in.readAll();

    file.close();

    return fileContent;
}

QString convertToUrlAccessibleString(const QString& inputString)
{
    QByteArray byteArray = inputString.toUtf8();
    QByteArray percentEncoded = QUrl::toPercentEncoding(byteArray);
    return QString(percentEncoded);
}
QString getFileFormat(const QString& filePath)
{
    QFileInfo fileInfo(filePath);
    QString fileSuffix = fileInfo.suffix();
    return fileSuffix.toLower();
}

QString response_code(QString Server_Response);
struct MessageBlock {
    QString src;
    QString dst;
    QString body;
    QString date;
};
QString findSubstringAndReturnRest(const QString& originalString, const QString& substring)
{
    int index = originalString.indexOf(substring);

    if (index != -1) {
        // Found the substring, return the portion of the string starting from the occurrence
        return originalString.mid(index + substring.length());
    }

    // Substring not found, return an empty string
    return "";
}

QString getStringBetweenLastTwoStrings(const QString& first, const QString& second, const QString& third)
{
    int secondIndex = first.lastIndexOf(second);
    int thirdIndex = first.lastIndexOf(third);

    if (secondIndex == -1 || thirdIndex == -1 || secondIndex >= thirdIndex)
    {
        return QString(); // Return an empty string if either of the substrings is not found or the order is incorrect.
    }

    int startIndex = secondIndex + second.length();
    int length = thirdIndex - startIndex;
    return first.mid(startIndex, length);
}
MessageBlock* getuserchats_server_to_chat_display(QString token,QString dst) {
    QString url1= "http://api.barafardayebehtar.ml:8080/getuserchats?token=";
    QString url2= "&dst=";
    url1=url1+token+url2+dst;
    QUrl url(url1);
    /*
    QUrl url2("&password=");
    url = url.resolved(user1);
    url = url.resolved(url2);
    url = url.resolved(pass1);*/
    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        // Reading the response data
        QByteArray responseData = reply->readAll();
        response = QString(responseData);
    } else {
        // Handle error cases
        qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();

    QString first="There Are -";
    QString second= "- Messages\"";
    QString temp=response;
    int num=getStringBetweenLastTwoStrings(temp,first,second).toInt(),i=0;
    //qDebug()<<num<<"\n\n";
    MessageBlock* answer_to_the_chat_info = new MessageBlock[num];
    //qDebug() << response<<"\n\n\n";
    QString serverResponse = response;

    // Parse the JSON response
    QJsonDocument jsonResponse = QJsonDocument::fromJson(serverResponse.toUtf8());
    if (!jsonResponse.isNull() && jsonResponse.isObject()) {
        QJsonObject rootObject = jsonResponse.object();

        // Iterate over each block
        for (const QString& key : rootObject.keys()) {
            if (key.startsWith("block ")) {
                QJsonObject blockObject = rootObject.value(key).toObject();

                // Extract values and store in struct

                answer_to_the_chat_info[i].src = blockObject.value("src").toString();
                answer_to_the_chat_info[i].dst = blockObject.value("dst").toString();
                answer_to_the_chat_info[i].body = blockObject.value("body").toString();
                answer_to_the_chat_info[i].date = blockObject.value("date").toString();

                // Print the values for verification
                //qDebug() << "Block" << key;
                //qDebug() << "Source:" << answer_to_the_chat_info[i].src;
                //qDebug() << "Destination:" << answer_to_the_chat_info[i].dst;
                //qDebug() << "Body:" << answer_to_the_chat_info[i].body;
                //qDebug() << "Date:" << answer_to_the_chat_info[i].date;
                i++;
            }
        }
    }

    //qDebug() <<"\n\n\n"<< response;
    //qDebug() <<"238476874682374\tefwwwe\twr";
    return answer_to_the_chat_info;



    //extractJsonValue(responseData,"bo")

}
MessageBlock* getchannelchats_server_to_chat_display(QString token,QString dst) {
    QString url1= "http://api.barafardayebehtar.ml:8080/getchannelchats?token=";
    QString url2= "&dst=";
    url1=url1+token+url2+dst;
    QUrl url(url1);
    /*
    QUrl url2("&password=");
    url = url.resolved(user1);
    url = url.resolved(url2);
    url = url.resolved(pass1);*/
    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        // Reading the response data
        QByteArray responseData = reply->readAll();
        response = QString(responseData);
    } else {
        // Handle error cases
        qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();

    QString first="There Are -";
    QString second= "- Messages\"";
    QString temp=response;
    int num=getStringBetweenLastTwoStrings(temp,first,second).toInt(),i=0;
    //qDebug()<<num<<"\n\n";
    MessageBlock* answer_to_the_chat_info = new MessageBlock[num];
    //qDebug() << response<<"\n\n\n";
    QString serverResponse = response;

    // Parse the JSON response
    QJsonDocument jsonResponse = QJsonDocument::fromJson(serverResponse.toUtf8());
    if (!jsonResponse.isNull() && jsonResponse.isObject()) {
        QJsonObject rootObject = jsonResponse.object();

        // Iterate over each block
        for (const QString& key : rootObject.keys()) {
            if (key.startsWith("block ")) {
                QJsonObject blockObject = rootObject.value(key).toObject();

                // Extract values and store in struct

                answer_to_the_chat_info[i].src = blockObject.value("src").toString();
                answer_to_the_chat_info[i].dst = blockObject.value("dst").toString();
                answer_to_the_chat_info[i].body = blockObject.value("body").toString();
                answer_to_the_chat_info[i].date = blockObject.value("date").toString();

                // Print the values for verification
                //qDebug() << "Block" << key;
                //qDebug() << "Source:" << answer_to_the_chat_info[i].src;
                //qDebug() << "Destination:" << answer_to_the_chat_info[i].dst;
                //qDebug() << "Body:" << answer_to_the_chat_info[i].body;
                //qDebug() << "Date:" << answer_to_the_chat_info[i].date;
                i++;
            }
        }
    }

    //qDebug() <<"\n\n\n"<< response;
    //qDebug() <<"238476874682374\tefwwwe\twr";
    return answer_to_the_chat_info;



    //extractJsonValue(responseData,"bo")

}
MessageBlock* getgroupchats_server_to_chat_display(QString token,QString dst) {
    QString url1= "http://api.barafardayebehtar.ml:8080/getgroupchats?token=";
    QString url2= "&dst=";
    url1=url1+token+url2+dst;
    QUrl url(url1);
    /*
    QUrl url2("&password=");
    url = url.resolved(user1);
    url = url.resolved(url2);
    url = url.resolved(pass1);*/
    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        // Reading the response data
        QByteArray responseData = reply->readAll();
        response = QString(responseData);
    } else {
        // Handle error cases
        qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();

    QString first="There Are -";
    QString second= "- Messages\"";
    QString temp=response;
    int num=getStringBetweenLastTwoStrings(temp,first,second).toInt(),i=0;
    //qDebug()<<num<<"\n\n";
    MessageBlock* answer_to_the_chat_info = new MessageBlock[num];
    //qDebug() << response<<"\n\n\n";
    QString serverResponse = response;

    // Parse the JSON response
    QJsonDocument jsonResponse = QJsonDocument::fromJson(serverResponse.toUtf8());
    if (!jsonResponse.isNull() && jsonResponse.isObject()) {
        QJsonObject rootObject = jsonResponse.object();

        // Iterate over each block
        for (const QString& key : rootObject.keys()) {
            if (key.startsWith("block ")) {
                QJsonObject blockObject = rootObject.value(key).toObject();

                // Extract values and store in struct

                answer_to_the_chat_info[i].src = blockObject.value("src").toString();
                answer_to_the_chat_info[i].dst = blockObject.value("dst").toString();
                answer_to_the_chat_info[i].body = blockObject.value("body").toString();
                answer_to_the_chat_info[i].date = blockObject.value("date").toString();

                // Print the values for verification
                //qDebug() << "Block" << key;
                //qDebug() << "Source:" << answer_to_the_chat_info[i].src;
                //qDebug() << "Destination:" << answer_to_the_chat_info[i].dst;
                //qDebug() << "Body:" << answer_to_the_chat_info[i].body;
                //qDebug() << "Date:" << answer_to_the_chat_info[i].date;
                i++;
            }
        }
    }

    //qDebug() <<"\n\n\n"<< response;
    //qDebug() <<"238476874682374\tefwwwe\twr";
    return answer_to_the_chat_info;



    //extractJsonValue(responseData,"bo")

}

QVector<QString> getuserlist(QString token){
    QString url1= "http://api.barafardayebehtar.ml:8080/getuserlist?token=";
    url1=url1+token;
    QUrl url(url1);

    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        // Reading the response data
        QByteArray responseData = reply->readAll();
        response = QString(responseData);
    } else {
        // Handle error cases
        qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();

    QString first="There Are -";
    QString second= "- Messages\"";
    QString temp=response;
    //qDebug()<<num<<"\n\n";
    //qDebug() << response<<"\n\n\n";
    QString serverResponse = response;

    QVector<QString> names;

    // Parse the JSON response
    QJsonDocument jsonDoc = QJsonDocument::fromJson(serverResponse.toUtf8());
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();

        // Find and extract the "block" objects
        QStringList blockKeys = jsonObj.keys().filter("block ");
        for (const QString& blockKey : blockKeys) {
            QJsonObject blockObj = jsonObj[blockKey].toObject();
            if (blockObj.contains("src")) {
                names.append(blockObj["src"].toString());
            }
        }
    }

    return names;

}
QVector<QString> getchanellist(QString token){
    QString url1= "http://api.barafardayebehtar.ml:8080/getchannellist?token=";
    url1=url1+token;
    QUrl url(url1);

    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        // Reading the response data
        QByteArray responseData = reply->readAll();
        response = QString(responseData);
    } else {
        // Handle error cases
        qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();

    QString first="There Are -";
    QString second= "- Messages\"";
    QString temp=response;
    //qDebug()<<num<<"\n\n";
    //qDebug() << response<<"\n\n\n";
    QString serverResponse = response;

    QVector<QString> names;

    // Parse the JSON response
    QJsonDocument jsonDoc = QJsonDocument::fromJson(serverResponse.toUtf8());
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();

        // Find and extract the "block" objects
        QStringList blockKeys = jsonObj.keys().filter("block ");
        for (const QString& blockKey : blockKeys) {
            QJsonObject blockObj = jsonObj[blockKey].toObject();
            if (blockObj.contains("src")) {
                names.append(blockObj["src"].toString());
            }
        }
    }

    return names;

}
QVector<QString> getgrouplist(QString token){
    QString url1= "http://api.barafardayebehtar.ml:8080/getgrouplist?token=";
    url1=url1+token;
    QUrl url(url1);

    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        // Reading the response data
        QByteArray responseData = reply->readAll();
        response = QString(responseData);
    } else {
        // Handle error cases
        qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();

    QString first="There Are -";
    QString second= "- Messages\"";
    QString temp=response;
    //qDebug()<<num<<"\n\n";
    //qDebug() << response<<"\n\n\n";
    QString serverResponse = response;

    QVector<QString> names;

    // Parse the JSON response
    QJsonDocument jsonDoc = QJsonDocument::fromJson(serverResponse.toUtf8());
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();

        // Find and extract the "block" objects
        QStringList blockKeys = jsonObj.keys().filter("block ");
        for (const QString& blockKey : blockKeys) {
            QJsonObject blockObj = jsonObj[blockKey].toObject();
            if (blockObj.contains("src")) {
                names.append(blockObj["src"].toString());
            }
        }
    }

    return names;

}
QString creategroup (QString token,QString group_name ,QString group_title) {

    QString url1= "http://api.barafardayebehtar.ml:8080/creategroup?token=";
    QString url2= "&group_name=";
    QString url3= "&group_title=";
    url1=url1+token+url2+group_name + url3 + group_title;
    QUrl url(url1);
    /*
    QUrl url2("&password=");
    url = url.resolved(user1);
    url = url.resolved(url2);
    url = url.resolved(pass1);*/
    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        // Reading the response data
        QByteArray responseData = reply->readAll();
        response = QString(responseData);
    } else {
        // Handle error cases
        //qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();

    return response_code(response);
}
QString createchannel (QString token,QString group_name ,QString group_title) {

    QString url1= "http://api.barafardayebehtar.ml:8080/createchannel?token=";
    QString url2= "&channel_name=";
    QString url3= "&channel_title=";
    url1=url1+token+url2+group_name + url3 + group_title;
    QUrl url(url1);
    /*
    QUrl url2("&password=");
    url = url.resolved(user1);
    url = url.resolved(url2);
    url = url.resolved(pass1);*/
    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        // Reading the response data
        QByteArray responseData = reply->readAll();
        response = QString(responseData);
    } else {
        // Handle error cases
        //qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();

    return response_code(response);
}

QString response_code(QString Server_Response){
    //seperating the code out of the respose of the server
    QString searchString1 = "\"204\"";
    QString searchString2 = "\"404\"";
    QString searchString3 = "\"401\"";
    QString searchString4 = "\"200\"";
    if(Server_Response.contains(searchString1)){
        return "204";
    }
    if(Server_Response.contains(searchString2)){
        return "404";
    }
    if(Server_Response.contains(searchString3)){
        return "401";
    }
    if(Server_Response.contains(searchString4)){
        return "200";
    }
    else{
        return "Error";
    }

}
QString signup(QString user,QString pass) {

    QString url1= "http://api.barafardayebehtar.ml:8080/signup?username=";
    QString url2= "&password=";

    url1=url1+user+url2+pass;
    QUrl url(url1);
    /*
    QUrl url2("&password=");
    url = url.resolved(user1);
    url = url.resolved(url2);
    url = url.resolved(pass1);*/
    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        // Reading the response data
        QByteArray responseData = reply->readAll();
        response = QString(responseData);
    } else {
        // Handle error cases
        //qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();

    return response_code(response);
}
QString sendmessagegroup_chat_to_server(QString token,QString dst , QString body) {
    QString url1= "http://api.barafardayebehtar.ml:8080/sendmessagegroup?token=";
    QString url2= "&dst=";
    QString url3= "&body=";
    url1=url1+token+url2+dst+url3+body;
    QUrl url(url1);
    /*
    QUrl url2("&password=");
    url = url.resolved(user1);
    url = url.resolved(url2);
    url = url.resolved(pass1);*/
    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        // Reading the response data
        QByteArray responseData = reply->readAll();
        response = QString(responseData);
    } else {
        // Handle error cases
        qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();
    QString response_code_login_server = response_code(response);

        return response_code_login_server;

}

QString sendmessageuser_chat_to_server(QString token,QString dst , QString body) {
    QString url1= "http://api.barafardayebehtar.ml:8080/sendmessageuser?token=";
    QString url2= "&dst=";
    QString url3= "&body=";
    url1=url1+token+url2+dst+url3+body;
    QUrl url(url1);
    /*
    QUrl url2("&password=");
    url = url.resolved(user1);
    url = url.resolved(url2);
    url = url.resolved(pass1);*/
    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        // Reading the response data
        QByteArray responseData = reply->readAll();
        response = QString(responseData);
    } else {
        // Handle error cases
        qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();
    QString response_code_login_server = response_code(response);

    return response_code_login_server;
    }

QString sendmessagechannel_chat_to_server(QString token,QString dst , QString body) {
    QString url1= "http://api.barafardayebehtar.ml:8080/sendmessagechannel?token=";
    QString url2= "&dst=";
    QString url3= "&body=";
    url1=url1+token+url2+dst+url3+body;
    QUrl url(url1);
    /*
    QUrl url2("&password=");
    url = url.resolved(user1);
    url = url.resolved(url2);
    url = url.resolved(pass1);*/
    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        // Reading the response data
        QByteArray responseData = reply->readAll();
        response = QString(responseData);
    } else {
        // Handle error cases
        qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();
    QString response_code_login_server = response_code(response);

    return response_code_login_server;
    }

QString login(QString user,QString pass) {
    QString url1= "http://api.barafardayebehtar.ml:8080/login?username=";
    QString url2= "&password=";

    url1=url1+user+url2+pass;
    QUrl url(url1);
    /*
    QUrl url2("&password=");
    url = url.resolved(user1);
    url = url.resolved(url2);
    url = url.resolved(pass1);*/
    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        // Reading the response data
        QByteArray responseData = reply->readAll();
        response = QString(responseData);
    } else {
        // Handle error cases
        qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();
    QString response_code_login_server = response_code(response);
    QString searchString = "\"token\": \"";
    QString targetString = "\"}";

    // Find the positions of the search and target substrings
    size_t searchPos = response.toStdString().find(searchString.toStdString());
    size_t targetPos = response.toStdString().find(targetString.toStdString());

    // Extract the desired portion of the original string
    if(response_code_login_server=="200" && searchPos != std::string::npos){

    std::string token = response.toStdString().substr(searchPos + searchString.length(), targetPos - (searchPos + searchString.length()));
    //response = QString::fromStdString(token);
    QString the_token_that_we_return= QString::fromStdString(token);
    return the_token_that_we_return ;
    }
    else if(response_code_login_server=="200"){
    return "300";
    }

    else {
    return response_code_login_server;
    }
}
QString logout(QString user,QString pass) {
    QString url1= "http://api.barafardayebehtar.ml:8080/logout?username=";
    QString url2= "&password=";

    url1=url1+user+url2+pass;
    QUrl url(url1);
    /*
    QUrl url2("&password=");
    url = url.resolved(user1);
    url = url.resolved(url2);
    url = url.resolved(pass1);*/
    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
    // Reading the response data
    QByteArray responseData = reply->readAll();
    response = QString(responseData);
    } else {
    // Handle error cases
    qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();
    QString response_code_login_server = response_code(response);
    return response_code_login_server;

}
//------------------------------------------------------------------------------------------------------

QString send_file_user_chat_to_server(QString token,QString dst , QString filePath) {
    //path need two // check
    int error_while=0;
    QString kham_tabdil_shodeh = readFileToString(filePath),what_we_send;
    QString tabdil_shodeh = convertToUrlAccessibleString(kham_tabdil_shodeh);
    int tedad = tabdil_shodeh.length();
    QString first_message = "**/begin/** "+getFileFormat(filePath) +  QString::number(tedad/30000);
    sendmessageuser_chat_to_server(token,dst,first_message);
    while(tedad>0){
    what_we_send=getAndRemoveFirst30000Characters(tabdil_shodeh);
    while(sendmessageuser_chat_to_server(token,dst,what_we_send) != "200"){
            error_while++;
            if(error_while>10){
                return "error";
            }
    }
    tedad = tabdil_shodeh.length();
    }
    QString last_message = "**/end/**";
    sendmessageuser_chat_to_server(token,dst,last_message);
    return "200";
}
//------------------------------------------------------------------------------------------------------

QString joingroup(QString token,QString group_name) {

    QString url1= "http://api.barafardayebehtar.ml:8080/joingroup?token=";
    QString url2= "&group_name=";

    url1=url1+token+url2+group_name;
    QUrl url(url1);
    /*
    QUrl url2("&password=");
    url = url.resolved(user1);
    url = url.resolved(url2);
    url = url.resolved(pass1);*/
    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
    // Reading the response data
    QByteArray responseData = reply->readAll();
    response = QString(responseData);
    } else {
    // Handle error cases
    //qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();

    return response_code(response);
}

QString joinchannel(QString token,QString group_name) {

    QString url1= "http://api.barafardayebehtar.ml:8080/joinchannel?token=";
    QString url2= "&channel_name=";

    url1=url1+token+url2+group_name;
    QUrl url(url1);
    /*
    QUrl url2("&password=");
    url = url.resolved(user1);
    url = url.resolved(url2);
    url = url.resolved(pass1);*/
    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
    // Reading the response data
    QByteArray responseData = reply->readAll();
    response = QString(responseData);
    } else {
    // Handle error cases
    //qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();

    return response_code(response);
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString username = ui->lineEdit->text();
    QString pass = ui->lineEdit_2->text();
    QString dst = ui->lineEdit_3->text();
    QString body = ui->lineEdit_4->text();
    /*
    MessageBlock* chatArray = getuserchats_server_to_chat_display(login(username,pass),dst);
    for (int i = 0; i < 5; ++i) {
    qDebug() << "Chat Text" << i;
    qDebug() << "sender_ID" << chatArray[i].src;
    qDebug() << "reciver_ID" << chatArray[i].dst;
    qDebug() << "textContent" << chatArray[i].body;
    qDebug() << "time_of_text" << chatArray[i].date;
    qDebug() << "\n\n\n";
    }*/
send_file_user_chat_to_server(login(username,pass),dst,"C:/Users/Im'z/Desktop/desk/cool_space");
    //delete[] chatArray;
    //ui->label->setText();
}

