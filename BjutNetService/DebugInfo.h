#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include <QObject>
#include <QString>
#include <QFile>

namespace bna {

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

    void init(const QString &name);

    void release();

    QString fileName() const;

    const QString &lastErrorString() const;

    void setInfo(const QString &content, bool with_time = true);

    //void writeInfo(const QString &content, bool with_time = true, bool end_line = true);

public slots:

    void writeInfo(DebugStatus status, const QString &content, bool with_time = true, bool end_line = true);

private:
    QFile m_file;
    QString m_strLastError;
    int m_nLastError;
};

inline QString DebugTool::fileName() const {
    return m_file.fileName();
}

inline const QString &DebugTool::lastErrorString() const {
    return m_strLastError;
}

}


#endif // DEBUGINFO_H
