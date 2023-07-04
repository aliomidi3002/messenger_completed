#include "chatpage.h"
#include "ui_chatpage.h"
#include "ID.h"
#include <QDebug>
#include <QUrlQuery>
#include <QtNetwork>
#include <iostream>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QByteArray>
#include <QJsonArray>
#include <QString>
#include <QWidget>
#include <QVector>
#include <QFileDialog>
#include <QStandardPaths>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include "logout.h"
int sending_file_count = 1;
QString get_Desktop_Path()
{
    QStringList desktopLocations = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
    if (!desktopLocations.isEmpty())
    {
        QString desktopPath = desktopLocations.first();
        if (!desktopPath.endsWith('/'))
        {
            desktopPath.append('/');
        }
        desktopPath.append("AP_project");
        desktopPath.append('/');
        if(QDir(desktopPath).exists())
            return desktopPath;
        else {
            QDir().mkdir(desktopPath);
            return desktopPath;
        }
    }

    return QString(); // Return an empty string if the desktop path cannot be determined
}
QString get_login_inf_Path()
{
    QStringList desktopLocations = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
    if (!desktopLocations.isEmpty())
    {
        QString desktopPath = desktopLocations.first();
        if (!desktopPath.endsWith('/'))
        {
            desktopPath.append('/');
        }
        desktopPath.append("AP_project");
        desktopPath.append('/');
        desktopPath.append("user_info/");
        if(QDir(desktopPath).exists())
            return desktopPath;
        else {
            QDir().mkdir(desktopPath);
            return desktopPath;
        }
    }

    return QString(); // Return an empty string if the desktop path cannot be determined
}

//-----------------------------------------------------------------------
bool removeDir(const QString dirName = get_Desktop_Path())
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists()) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = QDir().rmdir(dirName);
    }
    return result;
}

bool if_is_not_logged_in( const QString& username, const QString& password, const QString& token)
{
    QString path = get_login_inf_Path();
    path.append("login_info.txt/");
    QFile file(path);
    if (!file.exists()) {
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return false; // Failed to create or open the file
        }
        file.close();
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false; // Failed to open the file
    }

    QTextStream out(&file);
    out << "Username: " << username << "\n";
    out << "Password: " << password << "\n";
    out << "Token: " << token << "\n";

    file.close();
    return true;
}

bool is_it_loged_in( QString& username, QString& password, QString& token)
{
    QString path = get_Desktop_Path();
    path.append("user_info/login_info.txt/");
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false; // Failed to open the file
    }

    QTextStream in(&file);
    QString line;
    while (!in.atEnd()) {
        line = in.readLine();
        if (line.startsWith("Username: ")) {
            username = line.mid(10); // Extract the username value
        } else if (line.startsWith("Password: ")) {
            password = line.mid(10); // Extract the password value
        } else if (line.startsWith("Token: ")) {
            token = line.mid(8); // Extract the token value
        }
    }

    file.close();
    return true;
}
//-------------------------------------------------------------------------------------------------------------------------------

bool fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}
QString saveFileToDesktop(const QString& base64Data, const QString& format,QString name);
QString decodeString(const QString& encodedString);

struct MessageBlock {
    QString src;
    QString dst;
    QString body;
    QDateTime dateTime; // Updated to use QDateTime for date and time
    QString dateString; // Added QString member for date
};

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
QString extractSubstring_for_extracting_the_chat_info(const QString& original, QString& first, QString& second) {
    int firstPos = original.indexOf(first);
    if (firstPos == -1) {
        // First string not found
        return QString();
    }

    int secondPos = original.indexOf(second, firstPos + first.length());
    if (secondPos == -1) {
        // Second string not found
        return QString();
    }

    // Extract the substring between the first and second strings
    first = original.mid(firstPos + first.length(), secondPos - firstPos - first.length());

    // Return the remaining part of the original string
    QString remaining = original.mid(secondPos + second.length());

    return remaining;
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
            if (blockObj.contains("group_name")) {
                names.append(blockObj["group_name"].toString());
            }
        }
    }

    return names;

}
QVector<QString> getchannellist(QString token){
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
            if (blockObj.contains("channel_name")) {
                names.append(blockObj["channel_name"].toString());
            }
        }
    }

    return names;

}

QString logout(QString user,QString pass);


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



// Custom sorting function for MessageBlock objects based on the date
bool sortMessageBlocks(const MessageBlock& block1, const MessageBlock& block2) {
    return block1.dateTime < block2.dateTime;
}

void processMessageBlocks(QVector<MessageBlock>& blocks ,QString name) {
    int startIndex = -1;
    int endIndex = -1;
    QString accumulatedBodies;

    // Find start and end indices
    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i].body == "**start**") {
            startIndex = i;
            accumulatedBodies.clear();
        }
        else if (blocks[i].body == "**end**") {
            endIndex = i;
            break;
        }
        else if (startIndex != -1) {
            accumulatedBodies += blocks[i].body;

        }
    }
    if(startIndex == -1 || endIndex == -1){
        return ;
    }
    // Delete blocks between start and end indices
    QString format = blocks[endIndex+1].body;
    QString decode = decodeString(accumulatedBodies);

    QString final_path = saveFileToDesktop(decode,format,name);
    // Replace deleted blocks with new block
    if (!accumulatedBodies.isEmpty()) {
        MessageBlock newBlock;
        newBlock.src = blocks[startIndex].src;
        newBlock.dst = blocks[startIndex].dst;
        newBlock.body = final_path;
        newBlock.dateTime = blocks[startIndex].dateTime;
        newBlock.dateString = blocks[startIndex].dateString;
        if (startIndex != -1 && endIndex != -1 && endIndex > startIndex) {
            blocks.erase(blocks.begin() + startIndex, blocks.begin() + endIndex + 2);
        }
        blocks.insert(blocks.begin() + startIndex, newBlock);
    }
    return ;
}
void deleteAfterFirstStart(QVector<MessageBlock>& messageBlocks) {
    bool foundStart = false;
    auto startIt = messageBlocks.begin();

    for (auto it = messageBlocks.begin(); it != messageBlocks.end(); ++it) {
        if (it->body == "**start**") {
            if (foundStart) {
                messageBlocks.erase(it, messageBlocks.end());
                break;
            } else {
                foundStart = true;
                startIt = it;
            }
        }
    }
}

void processMessageBlocks2(QVector<MessageBlock>& blocks,QString path_of_the_previous ) {
    int startIndex = -1;
    int endIndex = -1;
    QString accumulatedBodies;

    // Find start and end indices
    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i].body == "**start**") {
            startIndex = i;
            accumulatedBodies.clear();
        }
        else if (blocks[i].body == "**end**") {
            endIndex = i;
            break;
        }
        else if (startIndex != -1) {
            accumulatedBodies += blocks[i].body;

        }
    }
    if(startIndex == -1 || endIndex == -1){
        return ;
    }
    // Delete blocks between start and end indices

    // Replace deleted blocks with new block
    if (!accumulatedBodies.isEmpty()) {
        MessageBlock newBlock;
        newBlock.src = blocks[startIndex].src;
        newBlock.dst = blocks[startIndex].dst;
        newBlock.body = path_of_the_previous;
        newBlock.dateTime = blocks[startIndex].dateTime;
        newBlock.dateString = blocks[startIndex].dateString;
        if (startIndex != -1 && endIndex != -1 && endIndex > startIndex) {
            blocks.erase(blocks.begin() + startIndex, blocks.begin() + endIndex + 1);
        }
        blocks.insert(blocks.begin() + startIndex, newBlock);
    }
    return ;
}


bool hasEqualStartAndEnd(const QVector<MessageBlock>& messageBlocks) {
    int startCount = 0;
    int endCount = 0;

    for (const auto& message : messageBlocks) {
        if (message.body == "**start**") {
            startCount++;
        } else if (message.body == "**end**") {
            endCount++;
        }
    }

    return startCount == endCount;
}
int Start_count_exist(const QVector<MessageBlock>& messageBlocks){
    int startCount = 0;

    for (const auto& message : messageBlocks) {
        if (message.body.contains("*start*")) {
            startCount++;
        }
    }

    return startCount;
}
int end_count_exist(const QVector<MessageBlock>& messageBlocks){
    int startCount = 0;

    for (const auto& message : messageBlocks) {
        if (message.body.contains("*end*")) {
            startCount++;
        }
    }

    return startCount;
}
int pic_user_display_count=0;
QVector<QString> night;
QVector<MessageBlock> getgroupchats_server_to_chat_display(QString token, QString dst) {
    QString url1 = "http://api.barafardayebehtar.ml:8080/getgroupchats?token=";
    QString url2 = "&dst=";

    url1 = url1 + token + url2 + dst;
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
        //qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();


    QVector<MessageBlock> messageBlocks; // Array to store the MessageBlocks

    // Parse the JSON response
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());

    if (!jsonResponse.isNull()) {
        if (jsonResponse.isObject()) {
            QJsonObject jsonObject = jsonResponse.object();

            // Extract the "message" and "code" values
            QString message = jsonObject["message"].toString();
            QString code = jsonObject["code"].toString();

            qDebug() << "Message: " << message;
            qDebug() << "Code: " << code;

            // Iterate over the blocks
            QJsonObject::iterator it;
            for (it = jsonObject.begin(); it != jsonObject.end(); ++it) {
                QString key = it.key();
                if (key.startsWith("block")) {
                    QJsonObject blockObject = it.value().toObject();

                    // Extract values from each block
                    QString body = blockObject["body"].toString();
                    QString src = blockObject["src"].toString();
                    QString dst = blockObject["dst"].toString();
                    QString date = blockObject["date"].toString();

                    // Create a MessageBlock struct and store it in the array
                    MessageBlock block;
                    block.src = src;
                    block.dst = dst;
                    block.body = body;
                    block.dateTime = QDateTime::fromString(date, "yyyy-MM-dd hh:mm:ss");
                    block.dateString = block.dateTime.toString("yyyy-MM-dd hh:mm:ss");

                    messageBlocks.append(block);
                }
            }

            // Sort the messageBlocks by dateTime in ascending order
            std::sort(messageBlocks.begin(), messageBlocks.end(), [](const MessageBlock& a, const MessageBlock& b) {
                return a.dateTime < b.dateTime;
            });
        } else {
            qDebug() << "Invalid JSON response: Not an object";
        }
    } else {
        qDebug() << "Invalid JSON response: Failed to parse";
    }
    QString temp;
    int a = Start_count_exist(messageBlocks);
    int b = end_count_exist(messageBlocks);
    if(a==b){
        qDebug()<<a<<"     "<<a;
        for(int i=0;i<a;i++){
            temp = QString::number(i) + "_group_" + dst;
            processMessageBlocks(messageBlocks,temp);
        }

    }
    else{
        pic_user_display_count = -20;
    }
    return messageBlocks;
}
QVector<MessageBlock> getuserchats_server_to_chat_display(QString token, QString dst) {
    QString url1 = "http://api.barafardayebehtar.ml:8080/getuserchats?token=";
    QString url2 = "&dst=";

    url1 = url1 + token + url2 + dst;
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
        //qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();

    QVector<MessageBlock> messageBlocks; // Array to store the MessageBlocks

    // Parse the JSON response
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());

    if (!jsonResponse.isNull()) {
        if (jsonResponse.isObject()) {
            QJsonObject jsonObject = jsonResponse.object();

            // Extract the "message" and "code" values
            QString message = jsonObject["message"].toString();
            QString code = jsonObject["code"].toString();

            qDebug() << "Message: " << message;
            qDebug() << "Code: " << code;

            // Iterate over the blocks
            QJsonObject::iterator it;
            for (it = jsonObject.begin(); it != jsonObject.end(); ++it) {
                QString key = it.key();
                if (key.startsWith("block")) {
                    QJsonObject blockObject = it.value().toObject();

                    // Extract values from each block
                    QString body = blockObject["body"].toString();
                    QString src = blockObject["src"].toString();
                    QString dst = blockObject["dst"].toString();
                    QString date = blockObject["date"].toString();

                    // Create a MessageBlock struct and store it in the array
                    MessageBlock block;
                    block.src = src;
                    block.dst = dst;
                    block.body = body;
                    block.dateTime = QDateTime::fromString(date, "yyyy-MM-dd hh:mm:ss");
                    block.dateString = block.dateTime.toString("yyyy-MM-dd hh:mm:ss");

                    messageBlocks.append(block);
                }
            }

            // Sort the messageBlocks by dateTime in ascending order
            std::sort(messageBlocks.begin(), messageBlocks.end(), [](const MessageBlock& a, const MessageBlock& b) {
                return a.dateTime < b.dateTime;
            });
        } else {
            qDebug() << "Invalid JSON response: Not an object";
        }
    } else {
        qDebug() << "Invalid JSON response: Failed to parse";
    }
    QString temp;
    int a = Start_count_exist(messageBlocks);
    int b = end_count_exist(messageBlocks);
    if(a==b){
        qDebug()<<a<<"     "<<a;
        for(int i=0;i<a;i++){
            temp = QString::number(i) + "_user_" + dst;
            processMessageBlocks(messageBlocks,temp);
        }

    }
    else{
        pic_user_display_count = -20;
    }
    return messageBlocks;
}
QVector<MessageBlock> getchannelchats_server_to_chat_display(QString token, QString dst) {
    QString url1 = "http://api.barafardayebehtar.ml:8080/getchannelchats?token=";
    QString url2 = "&dst=";

    url1 = url1 + token + url2 + dst;
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
        //qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();


    QVector<MessageBlock> messageBlocks; // Array to store the MessageBlocks

    // Parse the JSON response
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());

    if (!jsonResponse.isNull()) {
        if (jsonResponse.isObject()) {
            QJsonObject jsonObject = jsonResponse.object();

            // Extract the "message" and "code" values
            QString message = jsonObject["message"].toString();
            QString code = jsonObject["code"].toString();

            qDebug() << "Message: " << message;
            qDebug() << "Code: " << code;

            // Iterate over the blocks
            QJsonObject::iterator it;
            for (it = jsonObject.begin(); it != jsonObject.end(); ++it) {
                QString key = it.key();
                if (key.startsWith("block")) {
                    QJsonObject blockObject = it.value().toObject();

                    // Extract values from each block
                    QString body = blockObject["body"].toString();
                    QString src = blockObject["src"].toString();
                    QString dst = blockObject["dst"].toString();
                    QString date = blockObject["date"].toString();

                    // Create a MessageBlock struct and store it in the array
                    MessageBlock block;
                    block.src = src;
                    block.dst = dst;
                    block.body = body;
                    block.dateTime = QDateTime::fromString(date, "yyyy-MM-dd hh:mm:ss");
                    block.dateString = block.dateTime.toString("yyyy-MM-dd hh:mm:ss");

                    messageBlocks.append(block);
                }
            }

            // Sort the messageBlocks by dateTime in ascending order
            std::sort(messageBlocks.begin(), messageBlocks.end(), [](const MessageBlock& a, const MessageBlock& b) {
                return a.dateTime < b.dateTime;
            });
        } else {
            qDebug() << "Invalid JSON response: Not an object";
        }
    } else {
        qDebug() << "Invalid JSON response: Failed to parse";
    }
    QString temp;
    int a = Start_count_exist(messageBlocks);
    int b = end_count_exist(messageBlocks);
    if(a==b){
        qDebug()<<a<<"     "<<a;
        for(int i=0;i<a;i++){
            temp = QString::number(i) + "_channel_" + dst;
            processMessageBlocks(messageBlocks,temp);
        }

    }
    else{
        pic_user_display_count = -20;
    }

    return messageBlocks;
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

QString readFileAsBinary(const QString& filePath)
{
    QString binaryString;

    // Open the file for reading
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        // Read the file contents
        QByteArray fileData = file.readAll();

        // Convert to binary string
        binaryString = QString::fromLatin1(fileData.toBase64());
    }

    return binaryString;
}

QVector<QString> splitStringIntoChunks(const QString& str, int chunkSize)
{
    QVector<QString> chunks;
    int length = str.length();
    int numChunks = (length + chunkSize - 1) / chunkSize;

    for (int i = 0; i < numChunks; ++i) {
        int start = i * chunkSize;
        int chunkLength = qMin(chunkSize, length - start);
        QString chunk = str.mid(start, chunkLength);
        chunks.append(chunk);
    }

    return chunks;
}

QString getFileFormat(const QString filePath)
{
    QFileInfo fileInfo(filePath);
    QString fileSuffix = fileInfo.suffix();
    return fileSuffix.toLower();
}

QString replaceBackslashWithForwardSlash(const QString& str) {
    QString result = str;
    result.replace("\\", "/");
    return result;
}
QString select_File_Path_to_send() {
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Select File", "", "All Files (*.*)");
    if (!filePath.isEmpty()) {
        QString replacedPath = replaceBackslashWithForwardSlash(filePath);
        return replacedPath;
    } else {
        qDebug() << "Failed to open the file.";
        return "";
    }
}
QString convert_file_To_String(const QString& filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray imageData = file.readAll();
        return QString(imageData.toBase64());
    }

    return QString(); // Return an empty string if the file cannot be opened or read
}

QString saveFileToDesktop(const QString& base64Data, const QString& format, QString name)
{
    // Decode the base64-encoded data
    QByteArray decodedData = QByteArray::fromBase64(base64Data.toLatin1());

    // Get the desktop path
    QString desktopPath = get_Desktop_Path();

    // Generate the file path using the current date and time
    //QDateTime currentDateTime = QDateTime::currentDateTime();
    QString filePath = desktopPath + "/" + name + "." + format;
    if(fileExists(filePath)){
        return filePath;
    }
    // Save the file to the desktop
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(decodedData);
        file.close();
        qDebug() << "File saved to:" << filePath;
        return filePath;
    }
    else
    {
        qDebug() << "Failed to save file to:" << filePath;
        return QString();
    }
}


QString encodeString(const QString& originalString) {
    QString encodedString = QUrl::toPercentEncoding(originalString);
    return encodedString;
}

QString decodeString(const QString& encodedString) {
    QString decodedString = QUrl::fromPercentEncoding(encodedString.toUtf8());
    return decodedString;
}





//uiCode////////////////////////////////////////////////////////////////////////////////////////////////////
QString CurrentUsername;
QString CurrentPassword;
QString UserToken;


Chatpage::Chatpage(QWidget *parent, const userID& currentUser) :
    QDialog(parent),
    ui(new Ui::Chatpage),
    mCurrentUser(currentUser)
{
    ui->setupUi(this);
    QString username = currentUser.getUsername();
    QString password = currentUser.getPassword();
    QString token = currentUser.getToken();
    CurrentUsername = username;
    CurrentPassword = password;
    UserToken = token;

    ui->listWidget_5->setVisible(false);
    ui->label->setText(username);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Chatpage::on_pushButton_5_clicked);
    timer->start(500);
}

Chatpage::~Chatpage()
{
    delete ui;
}

// Thread Button


// find user
void Chatpage::on_toolButton_5_clicked()
{
    QString username = ui->lineEdit->text();
    ui->lineEdit->clear();
    sendmessageuser_chat_to_server(UserToken, username, "hi");
}

// create group
void Chatpage::on_toolButton_3_clicked()
{
    QString groupName = ui->lineEdit_3->text();
    ui->lineEdit_3->clear();
    creategroup(UserToken,groupName,"title");
}

// create channel
void Chatpage::on_toolButton_4_clicked()
{
    QString ChannelName = ui->lineEdit_2->text();
    ui->lineEdit_2->clear();
    createchannel(UserToken,ChannelName,"title");;
}

// sticker
bool Chatpage::sticker(QString body) {
    QLabel* iconLabel = new QLabel();
    QPixmap iconPixmap;

    if (body == "*UTF_laght*") {
        iconPixmap.load(":/new/prefix1/image/5-2-face-with-tears-of-joy-emoji-png-thumb.png");
        iconLabel->setPixmap(iconPixmap.scaled(30, 30, Qt::KeepAspectRatio));

        QListWidgetItem* iconItem = new QListWidgetItem();
        iconItem->setSizeHint(QSize(0, 40));
        ui->listWidget->addItem(iconItem);
        ui->listWidget->setItemWidget(iconItem, iconLabel);
        return true;
    }
    else if (body == "*UTF_cry*") {
        iconPixmap.load(":/new/prefix1/image/11-2-loudly-crying-emoji-png-thumb.png");
        iconLabel->setPixmap(iconPixmap.scaled(30, 30, Qt::KeepAspectRatio));

        QListWidgetItem* iconItem = new QListWidgetItem();
        iconItem->setSizeHint(QSize(0, 40));
        ui->listWidget->addItem(iconItem);
        ui->listWidget->setItemWidget(iconItem, iconLabel);
        return true;
    }
    else if (body == "*UTF_blink*") {
        iconPixmap.load(":/new/prefix1/image/1-2-wink-emoji-png-thumb.png");
        iconLabel->setPixmap(iconPixmap.scaled(30, 30, Qt::KeepAspectRatio));

        QListWidgetItem* iconItem = new QListWidgetItem();
        iconItem->setSizeHint(QSize(0, 40));
        ui->listWidget->addItem(iconItem);
        ui->listWidget->setItemWidget(iconItem, iconLabel);
        return true;
    }
    else if (body == "*UTF_hearteye*") {
        iconPixmap.load(":/new/prefix1/image/3-2-love-hearts-eyes-emoji-png-thumb.png");
        iconLabel->setPixmap(iconPixmap.scaled(30, 30, Qt::KeepAspectRatio));

        QListWidgetItem* iconItem = new QListWidgetItem();
        iconItem->setSizeHint(QSize(0, 40));
        ui->listWidget->addItem(iconItem);
        ui->listWidget->setItemWidget(iconItem, iconLabel);
        return true;
    }
    else if (body == "*UTF_sunglass*") {
        iconPixmap.load(":/new/prefix1/image/4-2-smiling-face-with-sunglasses-cool-emoji-png-thumb.png");
        iconLabel->setPixmap(iconPixmap.scaled(30, 30, Qt::KeepAspectRatio));

        QListWidgetItem* iconItem = new QListWidgetItem();
        iconItem->setSizeHint(QSize(0, 40));
        ui->listWidget->addItem(iconItem);
        ui->listWidget->setItemWidget(iconItem, iconLabel);
        return true;
    }
    else if (body == "*UTF_angle*") {
        iconPixmap.load(":/new/prefix1/image/6-2-angel-blushing-smile-emoji-png-thumb.png");
        iconLabel->setPixmap(iconPixmap.scaled(30, 30, Qt::KeepAspectRatio));

        QListWidgetItem* iconItem = new QListWidgetItem();
        iconItem->setSizeHint(QSize(0, 40));
        ui->listWidget->addItem(iconItem);
        ui->listWidget->setItemWidget(iconItem, iconLabel);
        return true;
    }
    else if (body == "*UTF_fear*") {
        iconPixmap.load(":/new/prefix1/image/8-2-fearful-emoji-png-thumb.png");
        iconLabel->setPixmap(iconPixmap.scaled(30, 30, Qt::KeepAspectRatio));

        QListWidgetItem* iconItem = new QListWidgetItem();
        iconItem->setSizeHint(QSize(0, 40));
        ui->listWidget->addItem(iconItem);
        ui->listWidget->setItemWidget(iconItem, iconLabel);
        return true;
    }
    else if (body == "*UTF_fire*") {
        iconPixmap.load(":/new/prefix1/image/58685-apple-color-symbol-fire-shape-iphone-emoji-thumb.png");
        iconLabel->setPixmap(iconPixmap.scaled(30, 30, Qt::KeepAspectRatio));

        QListWidgetItem* iconItem = new QListWidgetItem();
        iconItem->setSizeHint(QSize(0, 40));
        ui->listWidget->addItem(iconItem);
        ui->listWidget->setItemWidget(iconItem, iconLabel);
        return true;
    }

    return false;
}

// show users chat
void Chatpage::show_users_chat(QString user)
{
    QVector<MessageBlock> chats = getuserchats_server_to_chat_display(UserToken,user);
    ui->listWidget->clear();
    for (int i = 0 ; i < chats.size(); i++) {
        // user name
        if(chats.at(i).src == ui->label->text()){
            QString text = chats.at(i).body;
            QString name = chats.at(i).src;
            QString date = chats.at(i).dateString;
            QString formattedText;
            bool emoji = sticker(text);

            if(!emoji){
                int charCount = 0;
                const int maxCharsPerLine = 50;
                const int marginForLineBreak = 30;

                int labelHeight = 40;  // Initialize the label height with the base height

                QString message = name + ":\n" + text + "\n" + date;

                for (const QChar& character : message) {
                    formattedText.append(character);
                    charCount++;

                    if (charCount == maxCharsPerLine) {
                        formattedText.append('\n');
                        charCount = 0;
                        labelHeight += marginForLineBreak; // Add margin for the line break
                    }
                }

                QLabel* label = new QLabel(formattedText);
                label->setStyleSheet("QLabel { color: white; background-color: rgb(0, 170, 255); font: 12pt 'Segoe UI'; border-radius: 5px; margin-right: 400px; }"); // Set the label's style with right margin
                label->setAlignment(Qt::AlignLeft);

                labelHeight += (text.count('\n') * marginForLineBreak); // Adjust the label height for existing line breaks

                QListWidgetItem* newItem = new QListWidgetItem();
                newItem->setSizeHint(QSize(0, labelHeight+25));
                ui->listWidget->addItem(newItem);
                ui->listWidget->setItemWidget(newItem, label);
                ui->listWidget->setSpacing(10);
                ui->listWidget->scrollToBottom();
            }

        }
        else if(chats.at(i).src == ui->label_2->text()){
            QString text = chats.at(i).body;
            QString name = chats.at(i).src;
            QString date = chats.at(i).dateString;
            QString formattedText;
            bool emoji = sticker(text);

            if(!emoji){
                int charCount = 0;
                const int maxCharsPerLine = 50;
                const int marginForLineBreak = 30;

                int labelHeight = 40;  // Initialize the label height with the base height

                QString message = name + ":\n" + text + "\n" + date;

                for (const QChar& character : message) {
                    formattedText.append(character);
                    charCount++;

                    if (charCount == maxCharsPerLine) {
                        formattedText.append('\n');
                        charCount = 0;
                        labelHeight += marginForLineBreak; // Add margin for the line break
                    }
                }

                QLabel* label = new QLabel(formattedText);
                label->setStyleSheet("QLabel { color: white; background-color: rgb(0, 85, 127); font: 12pt 'Segoe UI'; border-radius: 5px; margin-right: 400px; }"); // Set the label's style with right margin
                label->setAlignment(Qt::AlignLeft);

                labelHeight += (text.count('\n') * marginForLineBreak); // Adjust the label height for existing line breaks

                QListWidgetItem* newItem = new QListWidgetItem();
                newItem->setSizeHint(QSize(0, labelHeight+25));
                ui->listWidget->addItem(newItem);
                ui->listWidget->setItemWidget(newItem, label);
                ui->listWidget->setSpacing(10);
                ui->listWidget->scrollToBottom();
            }
        }
    }
}

// show groups chats
void Chatpage::show_groups_chats(QString name){
    QVector<MessageBlock> chats = getgroupchats_server_to_chat_display(UserToken,name);
    ui->listWidget->clear();
    for (int i = 0 ; i < chats.size(); i++) {
        // user name
        if(chats.at(i).src == ui->label->text()){
            QString text = chats.at(i).body;
            QString name = chats.at(i).src;
            QString date = chats.at(i).dateString;
            QString formattedText;
            bool emoji = sticker(text);

            if(!emoji){
                int charCount = 0;
                const int maxCharsPerLine = 50;
                const int marginForLineBreak = 30;

                int labelHeight = 40;  // Initialize the label height with the base height

                QString message = name + ":\n" + text + "\n" + date;

                for (const QChar& character : message) {
                    formattedText.append(character);
                    charCount++;

                    if (charCount == maxCharsPerLine) {
                        formattedText.append('\n');
                        charCount = 0;
                        labelHeight += marginForLineBreak; // Add margin for the line break
                    }
                }

                QLabel* label = new QLabel(formattedText);
                label->setStyleSheet("QLabel { color: white; background-color: rgb(0, 170, 255); font: 12pt 'Segoe UI'; border-radius: 5px; margin-right: 400px; }"); // Set the label's style with right margin
                label->setAlignment(Qt::AlignLeft);

                labelHeight += (text.count('\n') * marginForLineBreak); // Adjust the label height for existing line breaks

                QListWidgetItem* newItem = new QListWidgetItem();
                newItem->setSizeHint(QSize(0, labelHeight+25));
                ui->listWidget->addItem(newItem);
                ui->listWidget->setItemWidget(newItem, label);
                ui->listWidget->setSpacing(10);
                ui->listWidget->scrollToBottom();
            }
        }
        else{
            QString text = chats.at(i).body;
            QString name = chats.at(i).src;
            QString date = chats.at(i).dateString;
            QString formattedText;
            bool emoji = sticker(text);

            if(!emoji){
                int charCount = 0;
                const int maxCharsPerLine = 50;
                const int marginForLineBreak = 30;

                int labelHeight = 40;  // Initialize the label height with the base height

                QString message = name + ":\n" + text + "\n" + date;

                for (const QChar& character : message) {
                    formattedText.append(character);
                    charCount++;

                    if (charCount == maxCharsPerLine) {
                        formattedText.append('\n');
                        charCount = 0;
                        labelHeight += marginForLineBreak; // Add margin for the line break
                    }
                }

                QLabel* label = new QLabel(formattedText);
                label->setStyleSheet("QLabel { color: white; background-color: rgb(0, 85, 127); font: 12pt 'Segoe UI'; border-radius: 5px; margin-right: 400px; }"); // Set the label's style with right margin
                label->setAlignment(Qt::AlignLeft);

                labelHeight += (text.count('\n') * marginForLineBreak); // Adjust the label height for existing line breaks

                QListWidgetItem* newItem = new QListWidgetItem();
                newItem->setSizeHint(QSize(0, labelHeight+25));
                ui->listWidget->addItem(newItem);
                ui->listWidget->setItemWidget(newItem, label);
                ui->listWidget->setSpacing(10);
                ui->listWidget->scrollToBottom();
            }
        }
    }
}

// show Channel chats
void Chatpage::show_channel_chats(QString name){
    QVector<MessageBlock> chats = getchannelchats_server_to_chat_display(UserToken,name);
    ui->listWidget->clear();
    for (int i = 0 ; i < chats.size(); i++) {
        QString text = chats.at(i).body;
        QString name = chats.at(i).src;
        QString date = chats.at(i).dateString;
        QString formattedText;
        bool emoji = sticker(text);

        if(!emoji){
            int charCount = 0;
            const int maxCharsPerLine = 50;
            const int marginForLineBreak = 30;

            int labelHeight = 40;  // Initialize the label height with the base height

            QString message = name + ":\n" + text + "\n" + date;

            for (const QChar& character : message) {
                formattedText.append(character);
                charCount++;

                if (charCount == maxCharsPerLine) {
                    formattedText.append('\n');
                    charCount = 0;
                    labelHeight += marginForLineBreak; // Add margin for the line break
                }
            }

            QLabel* label = new QLabel(formattedText);
            label->setStyleSheet("QLabel { color: white; background-color: rgb(0, 170, 255); font: 12pt 'Segoe UI'; border-radius: 5px; margin-right: 400px; }"); // Set the label's style with right margin
            label->setAlignment(Qt::AlignLeft);

            labelHeight += (text.count('\n') * marginForLineBreak); // Adjust the label height for existing line breaks

            QListWidgetItem* newItem = new QListWidgetItem();
            newItem->setSizeHint(QSize(0, labelHeight+25));
            ui->listWidget->addItem(newItem);
            ui->listWidget->setItemWidget(newItem, label);
            ui->listWidget->setSpacing(10);
            ui->listWidget->scrollToBottom();
        }
    }
}

// send message
void Chatpage::on_pushButton_2_clicked() {
    QString name = ui->label_2->text();
    QString text = ui->textEdit->toPlainText();

    // Get the index of the currently selected tab
    int currentIndex = ui->tabWidget->currentIndex();

    if (text.isEmpty()) {
        return;
    }

    if (currentIndex == 0) {
        sendmessageuser_chat_to_server(UserToken, name, text);
        ui->textEdit->clear();
        show_users_chat(name);

    } else if (currentIndex == 1) {
        sendmessagegroup_chat_to_server(UserToken,name,text);
        ui->textEdit->clear();
        show_groups_chats(name);

    } else if (currentIndex == 2) {
        sendmessagechannel_chat_to_server(UserToken,name,text);
        ui->textEdit->clear();
        show_channel_chats(name);
    }
}

// Quit Button
void Chatpage::on_pushButton_clicked()
{
    while(logout(CurrentUsername,CurrentPassword)!="200"){
        ;
    };
    QCoreApplication::quit();
}

// personal chat display on screen
void Chatpage::on_listWidget_2_itemClicked(QListWidgetItem *item)
{
    QString name = item->text();
    ui->label_2->setText(name);
    show_users_chat(name);
}

// group chat display on screen
void Chatpage::on_listWidget_4_itemClicked(QListWidgetItem *item)
{
    QString name = item->text();
    ui->label_2->setText(name);
    show_groups_chats(name);
}

// Channel Chat display on Screen
void Chatpage::on_listWidget_3_itemClicked(QListWidgetItem *item)
{
    QString name = item->text();
    ui->label_2->setText(name);
    show_channel_chats(name);
}

// join for group
void Chatpage::on_pushButton_7_clicked()
{
    QString groupName = ui->lineEdit_3->text();
    ui->lineEdit_3->clear();
    joingroup(UserToken,groupName);
}

// join for channel
void Chatpage::on_pushButton_6_clicked()
{
    QString ChannelName = ui->lineEdit_2->text();
    ui->lineEdit_2->clear();
    joinchannel(UserToken,ChannelName);
}


void Chatpage::on_pushButton_3_clicked()
{
    int iconWidth = 50;
    int iconHeight = 50;
    ui->listWidget_5->setIconSize(QSize(iconWidth, iconHeight));
    bool isVisible = ui->listWidget_5->isVisible();
    ui->listWidget_5->setVisible(!isVisible);
}

// logout
void Chatpage::on_pushButton_8_clicked()
{
    LogOut *out = new LogOut(this);
    out->show();
}
//////remember------------------------------------------------------------

//void Chatpage::on_pushButton_4_clicked()
//{
//    QString dst = ui->label_2->text();
//    function();
//    QApplication::processEvents();

//    // Show the chat page again
//    show();
//}
void Chatpage::on_pushButton_4_clicked()
{

    sending_file_count = 0;
    QString dis = ui->label_2->text();
    QString path =  QFileDialog::getOpenFileName(this, "Open File", QDir::homePath());   //"C:/Users/Im'z/Desktop/New folder (8)/download/jaja1.jpg";
    if(path != ""){
    QString format = getFileFormat(path);
    qDebug()<<format;
    QString converted_string = convert_file_To_String(path);
    qDebug()<<converted_string;
    QString coded = encodeString(converted_string);
    qDebug()<<coded;
    //--------------------------------------------------------------
    while(sendmessageuser_chat_to_server(UserToken,dis,"**start**")!="200"){};
    QThread::msleep(1000);
    const int chunkSize = 1000;
    int totalLength = coded.length();
    int currentIndex = 0;

    while (currentIndex < totalLength) {
        int chunkLength = qMin(chunkSize, totalLength - currentIndex);
        QString chunk = coded.mid(currentIndex, chunkLength);
        while(sendmessageuser_chat_to_server(UserToken,dis,chunk)!="200"){};
        QThread::msleep(1000);
        currentIndex += chunkLength;
    }
    QThread::msleep(1000);
    while(sendmessageuser_chat_to_server(UserToken,dis,"**end**")!="200"){};
    QThread::msleep(1000);
    while(sendmessageuser_chat_to_server(UserToken,dis,format)!="200"){};
    //--------------------------------------------------------------

    QString decode = decodeString(coded);
    qDebug()<<decode;
}
    sending_file_count=1;
    //QString final_path = saveFileToDesktop(decode,format);
    //qDebug()<<final_path;

}

void Chatpage::on_pushButton_5_clicked()
{
    if(pic_user_display_count >= 0){
    if(sending_file_count==1){
    int currentindex =  ui->tabWidget->currentIndex();


    if(currentindex == 0){
        QVector<QString> updatedUsers = getuserlist(UserToken);
            ui->listWidget_2->clear();
            for (int i = updatedUsers.size() - 1; i >= 0; --i) {
                ui->listWidget_2->addItem(updatedUsers[i]);
            }
            show_users_chat(ui->label_2->text());
        }

        else if(currentindex == 1){
            QVector<QString> groupList = getgrouplist(UserToken);
            ui->listWidget_4->clear();
            for(int i = groupList.size() -1 ; i >= 0; --i){
                ui->listWidget_4->addItem(groupList[i]);
            }
            show_groups_chats(ui->label_2->text());
        }

        else if(currentindex == 2){
            QVector<QString> channelList = getchannellist(UserToken);
            ui->listWidget_3->clear();
            for(int i = channelList.size() -1 ; i >= 0 ; --i){
                ui->listWidget_3->addItem(channelList[i]);
            }
            show_channel_chats(ui->label_2->text());
        }
    }
    }
    else {
    pic_user_display_count++;
    }
}

void Chatpage::on_listWidget_5_itemClicked(QListWidgetItem *item)
{
    int current = ui->listWidget_5->row(item);
    QString name = ui->label_2->text();

    if(current == 0){
        int currentIndex = ui->tabWidget->currentIndex();

        if (currentIndex == 0) {
            sendmessageuser_chat_to_server(UserToken, name, "*UTF_laght*");
            return;

        } else if (currentIndex == 1) {
            sendmessagegroup_chat_to_server(UserToken,name,"*UTF_laght*");
            return;

        } else if (currentIndex == 2) {
            sendmessagechannel_chat_to_server(UserToken,name,"*UTF_laght*");
            return;
        }
    }

    else if(current == 1){
        int currentIndex = ui->tabWidget->currentIndex();

        if (currentIndex == 0) {
            sendmessageuser_chat_to_server(UserToken, name, "*UTF_cry*");
            return;

        } else if (currentIndex == 1) {
            sendmessagegroup_chat_to_server(UserToken,name,"*UTF_cry*");
            return;

        } else if (currentIndex == 2) {
            sendmessagechannel_chat_to_server(UserToken,name,"*UTF_cry*");
            return;
        }
    }

    else if(current == 2){
        int currentIndex = ui->tabWidget->currentIndex();


        if (currentIndex == 0) {
            sendmessageuser_chat_to_server(UserToken, name, "*UTF_blink*");
            return;

        } else if (currentIndex == 1) {
            sendmessagegroup_chat_to_server(UserToken,name,"*UTF_blink*");
            return;

        } else if (currentIndex == 2) {
            sendmessagechannel_chat_to_server(UserToken,name,"*UTF_blink*");
            return;
        }
    }

    else if(current == 3){
        int currentIndex = ui->tabWidget->currentIndex();

        if (currentIndex == 0) {
            sendmessageuser_chat_to_server(UserToken, name, "*UTF_hearteye*");
            return;

        } else if (currentIndex == 1) {
            sendmessagegroup_chat_to_server(UserToken,name,"*UTF_hearteye*");
            return;

        } else if (currentIndex == 2) {
            sendmessagechannel_chat_to_server(UserToken,name,"*UTF_hearteye*");
            return;

        }
    }

    else if(current == 4){
        int currentIndex = ui->tabWidget->currentIndex();

        if (currentIndex == 0) {
            sendmessageuser_chat_to_server(UserToken, name, "*UTF_sunglass*");
            return;


        } else if (currentIndex == 1) {
            sendmessagegroup_chat_to_server(UserToken,name,"*UTF_sunglass*");
            return;


        } else if (currentIndex == 2) {
            sendmessagechannel_chat_to_server(UserToken,name,"*UTF_sunglass*");
            return;

        }
    }

    else if(current == 5){
        int currentIndex = ui->tabWidget->currentIndex();

        if (currentIndex == 0) {
            sendmessageuser_chat_to_server(UserToken, name,"*UTF_angle*");
            return;

        } else if (currentIndex == 1) {
            sendmessagegroup_chat_to_server(UserToken,name,"*UTF_angle*");
            return;

        } else if (currentIndex == 2) {
            sendmessagechannel_chat_to_server(UserToken,name,"*UTF_angle*");
            return;
        }
    }

    else if(current == 6){
        int currentIndex = ui->tabWidget->currentIndex();

        if (currentIndex == 0) {
            sendmessageuser_chat_to_server(UserToken, name, "*UTF_fear*");
            return;

        } else if (currentIndex == 1) {
            sendmessagegroup_chat_to_server(UserToken,name,"*UTF_fear*");
            return;

        } else if (currentIndex == 2) {
            sendmessagechannel_chat_to_server(UserToken,name,"*UTF_fear*");
            return;
        }
    }

    else if(current == 7){
        int currentIndex = ui->tabWidget->currentIndex();

        if (currentIndex == 0) {
            sendmessageuser_chat_to_server(UserToken, name, "*UTF_fire*");
            return;

        } else if (currentIndex == 1) {
            sendmessagegroup_chat_to_server(UserToken,name,"*UTF_fire*");
            return;

        } else if (currentIndex == 2) {
            sendmessagechannel_chat_to_server(UserToken,name,"*UTF_fire*");
            return;

        }
    }
}


