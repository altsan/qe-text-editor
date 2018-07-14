/******************************************************************************
** threads.h
**
** Contains all our worker threads.
**
**  Copyright (C) 2018 Alexander Taylor
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
******************************************************************************/

#ifndef QE_THREADS_H
#define QE_THREADS_H

#include <QThread>
#include <QFile>
#include <QTextStream>

class QeOpenThread : public QThread
{
    Q_OBJECT

public:
    QeOpenThread();
    void    setFile( QFile *file, QTextCodec *codec, QString fileName );
    QString getText();

    QString inputFileName;

protected:
    void run();

private:
    QString     fullText;
    QFile      *inputFile;
    QTextCodec *inputEncoding;

};

#endif

