#include "threads.h"
#include "eastring.h"


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
    stop     = false;

    if ( inputFile != NULL ) {
        QTextStream in( inputFile );
        qint64 progress = 0;
        qint64 total    = inputFile->size();
        if ( total )
            fullText.reserve( total );
        if ( inputEncoding != NULL )
            in.setCodec( inputEncoding );

        // Read the file in max-size chunks
        if ( total > FILE_CHUNK_SIZE ) {
            while ( !stop && !in.atEnd() ) {
                fullText.append( in.read( FILE_CHUNK_SIZE ));
                if (( progress + FILE_CHUNK_SIZE ) > total )
                    progress = total;
                else
                    progress += FILE_CHUNK_SIZE;
                setProgress( progress, total );
            }
        }
        else {
            fullText = in.readAll();
            progress = total;
        }

        inputFile->close();
        delete inputFile;
    }

    if ( stop ) {
        fullText = "";
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
void QeOpenThread::setProgress( qint64 progress, qint64 total )
{
    int percent = 0;
    if ( progress && total )
        percent = ( progress * 100 ) / total;
    emit updateProgress( percent );
}


// ----------------------------------------------------------------------------
void QeOpenThread::cancel()
{
    stop = true;
}




// ============================================================================
// QeSaveThread
//

// ----------------------------------------------------------------------------
QeSaveThread::QeSaveThread()
{
    outputFile     = NULL;
    outputEncoding = NULL;
    outputFileName = "";
    bExists        = FALSE;
}


// ----------------------------------------------------------------------------
void QeSaveThread::run()
{
    stop = false;
    if ( outputFile != NULL ) {
        QTextStream out( outputFile );

        /* TODO
        if platform_newline == DOS and requested_newline == UNIX:
            fulltext.replace("\r\n", "\n");
        else if platform_newline == UNIX and requested_newline == DOS:
            fulltext.replace("\n", "\r\n");
        */

        qint64 total = fullText.size();
        qint64 written = 0;
        if ( outputEncoding != NULL )
            out.setCodec( outputEncoding );

        if ( total > FILE_CHUNK_SIZE ) {
            qint64 offset = 0;
            while ( !stop && ( offset < total )) {
                out << fullText.mid( offset, FILE_CHUNK_SIZE );
                out.flush();
                written = out.pos();
                offset += FILE_CHUNK_SIZE;
                setProgress( written, total );
            }
        }
        else {
            out << fullText;
            out.flush();
            written = out.pos();
        }

        // In case an existing file is being shrunk, make sure it's resized to the new contents
        if ( written != -1 ) outputFile->resize( written );

        outputFile->flush();
        outputFile->close();
        delete outputFile;

        if ( !bExists ) {
#ifdef __OS2__
printf("Clearing default EAs\n");
            // If this is a new file, get rid of the useless default EAs added by klibc
            EASetString( (PSZ) outputFileName.toLocal8Bit().data(), (PSZ) "UID",   (PSZ) "");
            EASetString( (PSZ) outputFileName.toLocal8Bit().data(), (PSZ) "GID",   (PSZ) "");
            EASetString( (PSZ) outputFileName.toLocal8Bit().data(), (PSZ) "MODE",  (PSZ) "");
            EASetString( (PSZ) outputFileName.toLocal8Bit().data(), (PSZ) "INO",   (PSZ) "");
            EASetString( (PSZ) outputFileName.toLocal8Bit().data(), (PSZ) "RDEV",  (PSZ) "");
            EASetString( (PSZ) outputFileName.toLocal8Bit().data(), (PSZ) "GEN",   (PSZ) "");
            EASetString( (PSZ) outputFileName.toLocal8Bit().data(), (PSZ) "FLAGS", (PSZ) "");
#endif
        }

        emit saveComplete( written );
    }
}


// ----------------------------------------------------------------------------
void QeSaveThread::setFile( QFile *file, QTextCodec *codec, QString fileName, bool bExisting )
{
    outputFile     = file;
    outputEncoding = codec;
    outputFileName = fileName;
    bExists        = bExisting;
}


// ----------------------------------------------------------------------------
void QeSaveThread::setText( const QString &text )
{
    fullText = text;
}


// ----------------------------------------------------------------------------
void QeSaveThread::setProgress( qint64 progress, qint64 total )
{
    int percent = 0;
    if ( progress && total )
        percent = ( progress * 100 ) / total;
    emit updateProgress( percent );
}


// ----------------------------------------------------------------------------
void QeSaveThread::cancel()
{
    stop = true;
}



