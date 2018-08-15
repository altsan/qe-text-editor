:userdoc.

.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=001.QE text editor
:p.QE has been designed as a simple and efficient text editor with modern
features such as Unicode support and anti-aliased text.

:p.The philosophy behind QE is to provide a simple tool for quickly reading
and/or editing text files. It is not a programmer's editor, and therefore
does not provide developer features like syntax highlighting, code completion,
or compiler integration. Rather, it aims to provide the essentials for
viewing and editing plain text.

:p.Refer to the following sections for information about using QE&colon.
:ul compact.
:li.:link res=100 reftype=hd.User interface:elink.
:li.:link res=200 reftype=hd.Keyboard commands:elink.
:eul.

:note.The name &osq.QE&csq. was chosen for continuity with other basic OS/2 text
editors (like &osq.AE&csq. and &osq.EE&csq.). It did not originate as an
acronym for anything in particular (although if one is desired,
&osq.quintessential editor&csq. might be a convenient description.)


.*----------------------------------------------------------------------------
:h2 x=left y=bottom width=100% height=100% res=002.Command-line options
:p.QE accepts the following command-line options when run.

:dl tsize=20.
:dt./?  or  /h
:dd.Show usage information
:dt./read
:dd.Start QE in read-only mode
:dt./cp&colon.:hp1.encoding:ehp1.
:dd.Select the specified :link reftype=fn refid=codepages.encoding:elink.
:edl.


:fn id=codepages.
:p.The encoding may be specified by codepage number or by name. The following
encodings are supported&colon.
:xmp.
 Number  Name
 &bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.
  437    IBM-437
  813    ISO 8859-7
  819    Windows-1252
  850    IBM-850
  858    IBM-858
  859    IBM-859
  862    IBM-867
  863    IBM-863
  864    IBM-864
  865    IBM-865
  866    IBM-866
  867    IBM-867
  869    IBM-869
  874    IBM-874
  878    KOI8-R
  912    ISO 8859-2
  913    ISO 8859-3
  914    ISO 8859-4
  915    ISO 8859-5
  916    ISO 8859-8
  919    ISO 8859-10
  921    ISO 8859-13
  923    ISO 8859-15
  932    Shift-JIS
  943    Shift-JIS
  950    Big5-HKSCS
  954    EUC-JP
  970    EUC-KR
 1089    ISO 8859-6
 1090    ISO 8859-14
 1091    ISO 8859-16"
 1092    TSCII
 1168    KOI8-U
 1200    UTF-16BE
 1202    UTF-16LE
 1208    UTF-8
 1250    Windows-1250
 1251    Windows-1251
 1252    Windows-1252
 1253    Windows-1253
 1254    Windows-1254
 1255    Windows-1255
 1256    Windows-1256
 1257    Windows-1257
 1258    Windows-1258
 1275    Apple Roman
 1363    cp949
 1381    GB2312
 1386    GB18030-0
 4992    ISO-2022-JP
:exmp.
:efn.


.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=100.User interface
:p.The main window of QE consists of the top menu bar, the edit window proper,
and the status bar at the bottom.

:p.:hp7.Menu Bar:ehp7.
:p.:artwork runin name='menu.bmp'.
.br

:p.The menu bar consists of the :hp2.File:ehp2., :hp2.Edit:ehp2., :hp2.Options:ehp2.
and :hp2.Help:ehp2. menus.
:ul.
:li.The :hp2.File:ehp2. menu contains the commands for creating, opening, saving
and printing files; it also features a list of the five most-recently-opened
files.  In addition, this menu includes the :hp2.Encoding:ehp2. sub-menu, which
allows you to set the text encoding used for the current text (see the section on
:link reftype=hd res=300.using encodings:elink. for more information).
:li.The :hp2.Edit:ehp2. menu contains clipboard actions, undo/redo controls, and
options for navigating, :link reftype=hd res=110.searching and replacing:elink.
within the current text.
:li.The :hp2.Options:ehp2. menu contains several options for configuring QE's
behaviour.
:li.The :hp2.Help:ehp2. menu allows you to access program help and product
information.
:eul.

:p.
:p.:hp7.Edit Window:ehp7.
:p.The edit window contains the current text. Certain aspects of its appearance
and behaviour can be configured using the items under the :hp2.Options:ehp2.
menu.

:p.The edit window uses a white background to indicate existing lines of text,
and a pale grey background in areas where the current text does not occupy the
full height of the window.

:p.:artwork runin name='edit_linebreak.bmp'.  :artwork runin name='edit_nolinebreak.bmp'.
.br

:p.Note that the white background will extend :hp1.past:ehp1. the final line
break in the text. This means that if the text ends with a line break, a blank
line will be shown underneath it (as illustrated above).

:p.
:p.:hp7.Status Bar:ehp7.
:p.:artwork runin name='status.bmp'.
.br

:p.At the bottom of the window is the status bar. This contains five sections,
from left to right as follows&colon.
:ul compact.
:li.Status messages
:li.Current text encoding
:li.Edit mode (insert or overwrite)
:li.Current cursor position (line and column)
:li.File-modified status
:eul.


.*----------------------------------------------------------------------------
:h2 x=left y=bottom width=100% height=100% res=110.Finding and replacing text
:p.QE provides two interactive dialogs for finding text within the current
file.
:ul.
:li.The :link reftype=hd res=111.Find:elink. dialog allows quick but flexible
searches.
:li.The :link reftype=hd res=112.Find/Replace:elink. dialog provides the same
search capability, but also allows text which is so found to be replaced with
different text.
:eul.

:p.In addition, QE provides a :hp2.find again:ehp2. command (available via
the :hp2.Edit:ehp2. menu) which repeats the last find operation (if any)
without requiring either dialog to be opened again.


.* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
:h3 x=left y=bottom width=100% height=100% res=111.Find dialog
:p.:artwork align=left name='find_dialog.bmp'.
.br
:p.In the &osq.Find&colon.&csq.entry field, enter the text (or regular expression)
to search for.
:p.The remaining controls on this dialog are described below.
:dl break=fit tsize=24.
:dt.Regular expression
:dd.Normally, QE will search for the text as entered, verbatim, in the entry field.
Checking this box will cause QE to interpret that text as a
:link reftype=fn refid=regex.regular expression:elink. instead.
:dt.Match case
:dd.By default, searches are not case-sensitive. Check this box to perform a
case-sensitive search.
:dt.Match whole words
:dd.Check this box to search only for whole words which match the specified text.
:dt.Search backwards
:dd.Normally, searches are carried out from the starting position moving forwards
in the text. Check this box to search backwards from the starting position instead.
:dt.From start of file / From end of file
:dd.The starting position for searching is normally the current cursor position.
Check this box to start from the beginning of the file instead (or from the end
of the file, if the &osq.Search backwards&csq. box is checked).
:dt.Keep this dialog open
:dd.By default, the dialog will close when the search is initiated. Check this
box to keep the dialog open. This can be useful if you need to perform several
consecutive searches using different search terms.
:dt.Find (button)
:dd.This button will begin the search operation.
:dt.Cancel (button)
:dd.This button will close the dialog without doing a search.
:edl.


.* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
:h3 x=left y=bottom width=100% height=100% res=112.Find/Replace dialog
:p.:artwork align=left name='replace_dialog.bmp'.
.br


:fn id=regex.
:p.A :hp2.regular expression:ehp2. (or &osq.regex(p)&csq.) is an advanced search
pattern which is specified using a special syntax. QE uses the default Qt4
implementation of regular expressions, which is compatible with Perl.
:p.A description of regular expression syntax is outside the scope of this help
file. More information on regular expressions in general can be found at&colon.
:p.https&colon.//en.wikipedia.org/wiki/Regular_expression#Syntax
:p.and information on the Qt4 implemention in particular at&colon.
http&colon.//doc.qt.io/archives/qt-4.7/qregexp.html#details
:efn.


.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=200.Keyboard commands
:p.QE supports the following keyboard commands and shortcuts.

:p.:hp5.File and program commands:ehp5.
:dl compact tsize=30 break=fit.

:dt.F2
:dd.Save file

:dt.F3
:dd.Exit QE

:dt.Ctrl+N
:dd.New file

:dt.Ctrl+O
:dd.Open file

:dt.Ctrl+P
:dd.Print file

:edl.

:p.:hp5.Navigation commands:ehp5.
:dl compact tsize=30 break=fit.
:dt.Ctrl+End
:dd.Go to end of file

:dt.Ctrl+Home
:dd.Go to start of file

:dt.Ctrl+L
:dd.Go to line

:dt.End
:dd.Go to end of line

:dt.Home
:dd.Go to start of line

:dt.PgDn
:dd.Page the visible text down by one screen

:dt.PgUp
:dd.Page the visible text up by one screen

:edl.


:p.:hp5.Editing commands:ehp5.
:dl compact tsize=30 break=fit.

:dt.Ctrl+A  or  Ctrl+/
:dd.Select all

:dt.Ctrl+F
:dd.Find text

:dt.Ctrl+G
:dd.Find again

:dt.Ctrl+Ins  or  Ctrl+C
:dd.Copy selected text

:dt.Ctrl+R
:dd.Find and replace text

:dt.Ctrl+Y  or  Shift+Alt+Bksp
:dd.Redo edit

:dt.Ctrl+Z  or  Alt+Bksp
:dd.Undo edit

:dt.Shift+Del  or  Ctrl+X
:dd.Cut selected text

:dt.Shift+Ins  or  Ctrl+V
:dd.Paste selected text

:edl.


:p.:hp5.Option commands:ehp5.
:dl compact tsize=30 break=fit.

:dt.Alt+R
:dd.Toggle read-only mode on/off

:dt.Alt+W
:dd.Toggle word wrap on/off

:dt.Ins
:dd.Toggle edit mode (insert/overwrite)

:edl.


:p.:hp5.Other commands:ehp5.
:dl compact tsize=30 break=fit.
:dt.F1
:dd.Show help
:edl.


.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=300.Using encodings
:p.


.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=800.Limitations
:p.This section lists some of QE's limitations.
:ul.
:li.OS/2 bitmap fonts are not supported.
:li.The OS/2 version does not currently support printing to non-CUPS printers.
:li.The Windows and Linux versions will not remember the character encoding
used for a given file, except in the case where the file begins with a Unicode
byte-order mark (BOM).
:li.If you save a file containing characters not supported by the current
encoding, those characters will replaced by substitution markers in the saved
file. Consequently, if you have a need to use characters not supported by the
system codepage, you should be sure to set the current file's encoding to an
appropriate selection. (If unsure which encoding is best, UTF-8 is generally
a good default choice.)
:li.Although QE supports editing files of arbitrary size, opening or saving
:link reftype=fn refid=large_file.extremely large files:elink. may exhibit the
following issues&colon.
:ul.
:li.Depending on the speed of the disk and/or processor, large files may take a
long time to open or save.
:li.File I/O is performed on a separate thread, and therefore will not block the
input queue. However, once loading is complete, the edit window will be populated
with the complete file contents; by necessity this must be done on the user
interface thread, which may cause QE to become briefly unresponsive if the file
is very large.
:eul.
:eul.


:fn id=large_file.
:p.Precisely what constitutes an &osq.extremely large file&csq. in the context
described here may depend greatly on your computer. When using a solid state
drive (SSD) or flash drive on a system with a Core-series processor and multiple
GB of RAM, files of 100 MB or more may load and save within a second or two.
On older hardware using a mechanical hard disk, files of only a few megabytes
may cause significant delays.
:efn.


.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=900.Notices
:p.QE is written in C++ using the Qt toolkit. It has been built using Qt4 and
the GNU Compiler Collection (GCC).

:p.Project home page&colon. https&colon.//github.com/altsan/quick-text-editor/
:p.

:lm margin=4.
:p.:hp2.QE Text Editor:ehp2.
.br
Copyright (C) 2018 Alexander Taylor

:p.Contains some code derived from examples in :hp1.C++ GUI Programming
with Qt4 (Second Edition):ehp1. by Jasmin Blanchette &amp. Mark Summerfield.

:p.OS/2 version includes the public domain "EAString" module by Roger Orr.

:p.OS/2 native file dialog code is derived in part from the Mozilla for OS/2
project.

:p.This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

:p.This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

:p.You should have received a copy of the GNU General Public License
along with this program.  If not, see &lt.http&colon.//www.gnu.org/licenses/&gt..
:lm margin=1.

:euserdoc.
