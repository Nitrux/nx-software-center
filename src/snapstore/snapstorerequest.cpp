#include "snapstorerequest.h"
#include "snapstore.h"

SnapStoreRequest::SnapStoreRequest(SnapStore *store) : QObject(store)
{
    m_snapStore = store;
    m_error = 0;
}


QString SnapStoreRequest::errorString() const
{
    return m_errorString;
}

bool SnapStoreRequest::isFinished() const
{
    return m_isFinished;
}

int SnapStoreRequest::error() const
{
    return m_error;
}

void SnapStoreRequest::onProgress(qint64 progress, qint64 total)
{
    qDebug() <<  "progress: " << progress << " of " << total;
}

void SnapStoreRequest::onSslErrors(QList<QSslError> sslErrors)
{
    qDebug() << sslErrors;
    m_error = 1;
    m_errorString = sslErrors.first().errorString();
    emit(complete());
}

void SnapStoreRequest::onNetworkErrorResponse(QNetworkReply::NetworkError error)
{
    qDebug() << "QNetworkReply::NetworkError" << error;

    m_error = 1;
    m_errorString = error;
    emit(complete());
}
