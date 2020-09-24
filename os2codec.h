/******************************************************************************
** (C) 2018 Alexander Taylor
** (C) 2015 The Qt Company Ltd.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 or version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv21 and LICENSE.LGPLv3
** included in the packaging of this file. Please review the following
** information to ensure the GNU Lesser General Public License requirements
** will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
*******************************************************************************/

#ifndef QEOS2CODEC_H
#define QEOS2CODEC_H

#include <QTextCodec>

template <typename T> class QAtomicPointer;

class QeOS2Codec : public QTextCodec
{
    public:
        enum { numCodecs = 17 };
        enum { IBM437, IBM852, IBM855, IBM857, IBM858, IBM859, IBM860, IBM861, IBM863, IBM864, IBM865, IBM867, IBM869, IBM922, IBM1125, IBM1131, MEMJA };
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
