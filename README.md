QE Text Editor
==============

  QE is a simple text editor loosely modelled after IBM's E Editor and 
  Aaron Lawrence's AE.  It is written using Qt4 and is therefore
  reasonably cross-platform.

  The philosophy behind QE is to provide a simple tool for quickly
  reading and/or editing text files.  It is not a programmer's editor,
  word processor, or document manager.  As such, it doesn't provide
  developer features like syntax highlighting, code completion, folding,
  compiler integration, or anything else that could get in the user's
  way, clutter up the interface, or cause lengthy load times.

  What it does aim to provide is the essentials for viewing and editing
  plain text.  This includes straightforward yet flexible search and
  replace, a go-to-line feature, basic printing support, the ability to
  read text files in different encodings, and full support for displaying
  Unicode text.

  More information is available at:
  https://github.com/altsan/qe-text-editor/wiki


Installation
------------

Manual installation for various platforms is described below.  RPM or
other installable packages may be available elsewhere.

### OS/2

  Make sure you have the following runtime packages installed. Most of 
  these are available in zip, WarpIN, and/or yum/rpm distributions:
  - Qt4 runtime version 4.7.3 or later
  - klibc (1:0.1.4) and libgcc (9.2.0)

  Copy QE.EXE to a directory in your PATH, and QE.HLP to a directory in your
  HELP path.  Create a program object for QE.EXE if you wish.

### Windows

  Create a directory for QE and copy QE.EXE into it. Create a shortcut for
  QE.EXE.

  If you do not have the Qt4 and GCC/MingW 4.8.2 runtime libraries installed,
  you must also extract the contents of qe_win32-mingw-qt4_libs.zip either
  into the same directory, or to a directory on your PATH.

  To enable online help support, copy QE.QCH and QE.QHC into the same
  directory as QE.EXE. If you do not have Qt installed system-wide, you must
  also extract the contents of qe_win32-mingw-qt4_assistant.zip either into
  the same directory, or to a directory on your PATH.

### Linux

  Make sure the Qt4 and gcc runtime libraries are installed (using your
  distribution's package manager), along with QtAssistant if you want online
  help support.

  Copy `qe` into a pathed directory (such as /usr/local/bin), along with
  `qe.qch` and `qe.qhc` if you want online help support.


Building from Source
--------------------

GCC, GNU Make, and the Qt4 development libraries for your platform are 
required.  As long as these are all installed correctly, building the
executable should be as simple as running `qmake qe.pro` followed by `make`
(or `make release` under Windows, if you are building the non-debug version).

Building the help is somewhat more complicated.  The help file source is in
IBM IPF format.  Under OS/2 this is compiled to HLP using the IPFC compiler:
run `ipfc qe.ipf` from inside the `help` subdirectory.  

However, on Windows and Linux the help must be converted to QtAssistant
format, which involves converting the IPF file to HTML and then running
`qcollectiongenerator`.  The script `makehtml.cmd` performs the first part of
this task; it is written in REXX and has been confirmed to work with classic
REXX under OS/2, and Regina REXX under Windows.  It also requires the SED 
utility (GNU sed or the equivalent) to run.  This script runs `qe.ipf` 
through the SED script `ipfhtml.sed`, splits the resulting output into 
single-section HTML files, and ensures that any IPF links are converted to 
the proper URIs.  Once this is done, you will have a series of `qe.*.html` 
files; running `qcollectiongenerator qe.qhcp -o qe.qhc` should then generate 
the final QtAssistant help files.


Translation
-----------

Message file sources for various languages are in the language/ subdirectory.
These can be translated using QtLinguist or a text editor. To make sure the
files are in sync with the latest English sources before translating, run
`lupdate qe.pro`. To compile the message files, run `lrelease qe.pro'.  (On 
OS/2, both of these may require the %UNIXROOT%/usr/lib/qt4/bin directory to
either be be prefixed to the command, else be in your PATH.  On Linux, they
are normally available on the path.)

Help file source in help/ should also be translated. Copy qe.ipf to either
qe_xx.ipf or qe_xx_yy.ipf (where 'xx' is the two-letter ISO code for your 
language, and 'yy' is the two-letter code for a particular country or region)
and use your preferred text or INF editor to translate the contents.  See
'Building from Source' (above) for build instructions.


Notices
-------

QE Text Editor  
Copyright (C) 2018-2021 Alexander Taylor

A few routines were derived from examples in "C++ GUI Programming with 
Qt4" (Second Edition) by Jasmin Blanchette & Mark Summerfield.

OS/2 version includes the public domain "EAString" module by Roger Orr.

OS/2 native file dialog code is derived in part from the Mozilla for OS/2
project.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
