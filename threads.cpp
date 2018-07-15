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
}


// ----------------------------------------------------------------------------
void QeOpenThread::run()
{
    fullText = "";
    progress = 0;
    stop     = false;

    if ( inputFile != NULL ) {
        QTextStream in( inputFile );
        qint64 total = inputFile->size();
        if ( total )
            fullText.reserve( total );
        if ( inputEncoding != NULL )
            in.setCodec( inputEncoding );

        // Read the file in max-size chunks
        if ( total > FILE_READ_SIZE ) {
            while ( !stop && !in.atEnd() ) {
                fullText.append( in.read( FILE_READ_SIZE ));
                if (( progress + FILE_READ_SIZE ) > total )
                    progress = total;
                else
                    progress += FILE_READ_SIZE;
            }
        }
        else {
            fullText = in.readAll();
            progress = total;
        }

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
int QeOpenThread::getProgress()
{
    int percent = 0;
    if ( progress && inputFile ) {
        qint16 total = inputFile->size();
        if ( total ) percent = ( progress * 100 ) / total;
    }
    return percent;
}


// ----------------------------------------------------------------------------
void QeOpenThread::cancel()
{
    stop     = true;
    fullText = "";
    progress = 0;
}

