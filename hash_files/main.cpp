#include <iostream>
#include <tuple>
#include <vector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QDirIterator>
#include <QCryptographicHash>
#include "../BjutNetService/Version.h"

int main(int argc, char *argv[])
{
    if(argc < 2){
        std::cout << "Usage: hash_files [Path]" << std::endl;
        return 0;
    }
    using file_tuple = std::tuple<QString, int, QString, int>;
    std::vector<file_tuple> vecClientFiles;
    QString strDistPath(argv[1]);

    QDir dirBin(strDistPath);
    if(!dirBin.exists()) {
        std::cout << "Path do not exist. =>" << strDistPath.toStdString() << std::endl;
        return 0;
    }
    // for each files, compute md5
    QDirIterator iterDir(dirBin, QDirIterator::Subdirectories);
    while(iterDir.hasNext())
    {
        iterDir.next();
        auto fi = iterDir.fileInfo();
        if(!fi.isFile()){
            continue;
        }
        auto nm = fi.filePath();
        if(nm.endsWith("/..") || nm.endsWith("/.")){
            continue;
        }
        if(nm.startsWith("./")){
            nm = nm.right(nm.size()-2);
        }
        if(nm.startsWith(strDistPath)){
            nm = nm.right(nm.size()-strDistPath.size());
        }
        auto size = fi.size();
        auto p = fi.permissions();
        QCryptographicHash hash(QCryptographicHash::Md5);
        QFile fd(fi.absoluteFilePath());
        if(fd.open(QFile::ReadOnly)){
            hash.addData(fd.readAll());
        }
        auto hash_data = hash.result().toHex();
        std::cout << nm.toStdString() << "  " << size << " " << hash_data.toStdString() << std::endl;
        vecClientFiles.emplace_back(nm, size, QString(hash_data), int(p));
    }
    // convert to json
    QJsonArray jfilesarray;
    for(const auto &f : vecClientFiles){
        QJsonObject jf;
        QJsonValue jfname(std::get<0>(f));
        QJsonValue jfsize(std::get<1>(f));
        QJsonValue jfsum(std::get<2>(f));
        QJsonValue jperm(std::get<3>(f));
        jf.insert("name", jfname);
        jf.insert("size", jfsize);
        jf.insert("sum", jfsum);
        jf.insert("permission", jperm);
        jfilesarray.append(jf);
    }
    QJsonObject jo;
    jo.insert("version", BNA_VERSION);
    jo.insert("inner_ver", BNA_INNER_VERSION);
#ifdef Q_OS_WIN
    jo.insert("platform", "Windows");
#elif defined(Q_OS_LINUX)
    jo.insert("platform", "Linux64");
#elif defined(Q_OS_MAC)
    jo.insert("platform", "Mac");
#else
    jo.insert("platform", "Unix");
#endif
    jo.insert("files", jfilesarray);
    QJsonDocument jdoc;
    jdoc.setObject(jo);
    auto data = jdoc.toJson(QJsonDocument::Compact);
    // write to file
    QFile f("content.json");
    if(f.open(QFile::WriteOnly))
    {
        f.write(data);
        f.close();
    }
    return 0;
}
