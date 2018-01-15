#ifndef QEOS2CODEC_H
#define QEOS2CODEC_H

#include <QTextCodec>

template <typename T> class QAtomicPointer;

class QeOS2Codec : public QTextCodec
{
    public:
        enum { numCodecs = 8 };
        enum { IBM437, IBM858, IBM859, IBM862, IBM863, IBM864, IBM865, IBM869 };
        QeOS2Codec( int );
        ~QeOS2Codec();
        QList<QByteArray> aliases() const;
        int mibEnum() const;
        QByteArray name() const;
        QByteArray convertFromUnicode( const QChar *, int, ConverterState * ) const;
        QString convertToUnicode( const char *, int, ConverterState * ) const;

    private:
        int forwardIndex;
        mutable QAtomicPointer<QByteArray> reverseMap;
};

#endif
