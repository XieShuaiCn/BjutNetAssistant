#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include <QObject>
#include <QString>
#include <QFile>

namespace bna {
namespace core{

class DebugTool : public QObject{

    Q_OBJECT

public:
    enum DebugStatus{
        STATUS_NONE,
        STATUS_INFO,
        STATUS_SUCCESS,
        STATUS_FAIL,
        STATUS_DATA
    };

    static DebugTool &I();
    static DebugTool &getInstance();
    static DebugTool *getPointer();

    void init(const QString &name);

    void release();

    QString fileName() const;

    const QString &lastErrorString() const;

    void setInfo(const QString &content, bool with_time = true);

    void writeInfo(const QString &content, bool with_time = true, bool end_line = true);
    void writeSuccess(const QString &content, bool with_time = true, bool end_line = true);
    void writeFail(const QString &content, bool with_time = true, bool end_line = true);
    void writeData(const QString &content, bool with_time = true, bool end_line = true);
    void writeData(const QByteArray &content, bool with_time = true, bool end_line = true);
    void write(DebugStatus status, const QString &content, bool with_time = true, bool end_line = true);
    void write(DebugStatus status, const QByteArray &content, bool with_time = true, bool end_line = true);

public slots:

    void writeString(DebugStatus status, const QString &content, bool with_time = true, bool end_line = true);
    void writeBytes(DebugStatus status, const QByteArray &content, bool with_time = true, bool end_line = true);

private:
    QFile m_file;
    QString m_strLastError;
    int m_nLastError;
};

inline DebugTool& DebugTool::getInstance()
{
    return I();
}
inline DebugTool* DebugTool::getPointer()
{
    return &getInstance();
}

inline QString DebugTool::fileName() const {
    return m_file.fileName();
}

inline const QString &DebugTool::lastErrorString() const {
    return m_strLastError;
}
inline void DebugTool::write(DebugStatus status, const QString &content, bool with_time, bool end_line){
    writeBytes(status, content.toUtf8(), with_time, end_line);
}
inline void DebugTool::write(DebugStatus status, const QByteArray &content, bool with_time, bool end_line){
    writeBytes(status, content, with_time, end_line);
}
inline void DebugTool::writeInfo(const QString &content, bool with_time, bool end_line){
    writeString(STATUS_INFO, content, with_time, end_line);
}
inline void DebugTool::writeSuccess(const QString &content, bool with_time, bool end_line){
    writeString(STATUS_SUCCESS, content, with_time, end_line);
}
inline void DebugTool::writeFail(const QString &content, bool with_time, bool end_line){
    writeString(STATUS_FAIL, content, with_time, end_line);
}
inline void DebugTool::writeData(const QString &content, bool with_time, bool end_line){
    writeString(STATUS_DATA, content, with_time, end_line);
}
inline void DebugTool::writeData(const QByteArray &content, bool with_time, bool end_line){
    writeBytes(STATUS_DATA, content, with_time, end_line);
}

}
}

#endif // DEBUGINFO_H
