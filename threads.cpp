#include "threads.h"

// ============================================================================
// QeOpenThread
//

// ----------------------------------------------------------------------------
QeOpenThread::QeOpenThread()
{
    inputFile     = NULL;
    inputEncoding = NULL;
    inputFileName = "";

    fullText = "";
    stop     = false;
}


// ----------------------------------------------------------------------------
void QeOpenThread::run()
{
    fullText = "";
    if ( inputFile != NULL ) {
        QTextStream in( inputFile );

        if ( inputEncoding != NULL )
            in.setCodec( inputEncoding );

        QString text = in.readAll();
        fullText = text;

        inputFile->close();
        delete inputFile;
    }
}


// ----------------------------------------------------------------------------
void QeOpenThread::setFile( QFile *file, QTextCodec *codec, QString fileName )
{
    inputFile     = file;
    inputEncoding = codec;
    inputFileName = fileName;
}


// ----------------------------------------------------------------------------
QString QeOpenThread::getText()
{
    return fullText;
}


// ----------------------------------------------------------------------------
void QeOpenThread::cancel()
{
    stop = true;
}

