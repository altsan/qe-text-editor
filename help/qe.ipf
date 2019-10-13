:userdoc.

.nameit symbol='argprefix' text='/'

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
:li.:link reftype=hd res=100.User interface:elink.
:li.:link reftype=hd res=200.Keyboard commands:elink.
:eul.

:note.The name &osq.QE&csq. was chosen for continuity with other basic OS/2 text
editors (like &osq.AE&csq. and &osq.EE&csq.). It did not originate as an
acronym for anything in particular (although if one is desired,
&osq.quintessential editor&csq. might be a convenient description.)


.*----------------------------------------------------------------------------
:h2 x=left y=bottom width=100% height=100% res=002.Command-line options
:p.QE accepts the following command-line options when run.

:dl tsize=16 break=fit.
:dt.&argprefix.?
.br
&argprefix.h
:dd.Show usage information
:dt.&argprefix.read
:dd.Start QE in read-only mode
:dt.&argprefix.enc&colon.:hp1.encoding:ehp1.
.br
&argprefix.cp&colon.:hp1.encoding:ehp1.
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
 1091    ISO 8859-16
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
 1386    GBK
 4992    ISO-2022-JP
 54936   GB18030
:exmp.
:p.Note that if the encoding name contains spaces, you may need to wrap it
in double quotes (unless you are using a shell which supports other
workarounds such as escape sequences).  On OS/2, for example, to specify
Apple Roman on the command line, any of the following should work:
:xmp.
qe &argprefix.enc&colon."Apple Roman"
qe "&argprefix.enc&colon.Apple Roman"
qe &argprefix.enc&colon.1275
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
files.
:p.This menu also includes the :hp2.Encoding:ehp2. sub-menu, which allows you to
set the text encoding used for the current text.  See the section on
:link reftype=hd res=300.using encodings:elink. for more information.
:li.The :hp2.Edit:ehp2. menu contains clipboard actions, undo/redo controls, and
the :link reftype=hd res=110.Go to Line:elink. dialog.
:p.This menu also provides facilities for finding and/or replacing
text within the current file&colon.
:ul compact.
:li.The :link reftype=hd res=120.Find:elink. dialog allows quick but flexible
text searches.
:li.The :link reftype=hd res=130.Find/Replace:elink. dialog also provides text
search capability, but in addition allows found text to be replaced automatically.
:li.Finally, the :hp2.Find again:ehp2. command is a quick way to repeat the last
find operation without having to open either of the above dialogs.  (This option
is only available if a previous find was performed since QE was started.)
:eul.

:li.The :hp2.Options:ehp2. menu contains several options for configuring QE's
behaviour. These include toggles for word wrap, read-only mode, and input
(insert or overwrite) mode, as well selection of the editor font.

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

:p.:artwork runin name='edit_linebreak.bmp'. :artwork runin name='edit_nolinebreak.bmp'.
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
:h2 x=left y=bottom width=100% height=100% res=110.Go to Line dialog
:p.:artwork align=left name='goto_dialog.bmp'.
.br
:p.Enter the line number within the current text to which you want to
navigate. When you select :hp2.OK:ehp2., the cursor will be moved to the
beginning of the specified line.
:note.You cannot enter a line number which does not currently exist.
In other words, the highest number which may be entered is the number of the
last line in the current text.


.*----------------------------------------------------------------------------
:h2 x=left y=bottom width=100% height=100% res=120.Find dialog
:p.:artwork align=left name='find_dialog.bmp'.
.br
:p.The Find dialog contains the following controls.
:dl break=fit tsize=30.
:dt.Find (combo-box)
:dd.Enter the text (or regular expression &mdash. see below) that you want to
search for. The drop-down list portion contains a history of previous search
terms that you can select from.
:dt.Regular expression
:dd.Normally, QE will search for the text as entered, verbatim. Checking this box
will cause QE to interpret that text as a :link reftype=fn refid=regex.regular
expression:elink. instead.
:dt.Match case
:dd.By default, searching is not case-sensitive. Check this box to perform a
case-sensitive search.
:dt.Match whole words
:dd.Check this box to find only whole words which match the specified text. This
option is disabled when searching for a regular expression.
:dt.Search backwards
:dd.Normally, searching is carried out in the forwards direction. Check this
box to search backwards from the starting position instead.
:dt.From start of file / From end of file
:dd.Normally, searching starts from the current cursor position. Check this box
to start from the beginning or end of the file instead (depending on whether a
forwards or backwards search is being performed).
:dt.Keep this dialog open
:dd.By default, the dialog will close when the search is initiated. Check this
box to keep the dialog open. This can be useful if you need to perform several
consecutive searches using different search terms.
:dt.Find (pushbutton)
:dd.This button will begin the search operation.
:dt.Cancel (pushbutton)
:dd.This button will close the dialog without doing a search.
:edl.


.*----------------------------------------------------------------------------
:h2 x=left y=bottom width=100% height=100% res=130.Find/Replace dialog
:p.:artwork align=left name='replace_dialog.bmp'.
.br
:p.The Find/Replace dialog contains the following controls.
:dl break=fit tsize=30.
:dt.Find (combo-box)
:dd.Enter the text (or regular expression &mdash. see below) that you want
to search for. The drop-down list portion contains a history of previous search terms
that you can select from.
:dt.Replace with (combo-box)
:dd.Enter the text with which the find results will be replaced. This only applies
when using :hp2.Replace:ehp2. or :hp2.Replace all:ehp2.. The drop-down list portion
contains a history of previously-entered replacement text.
:dt.Regular expression
:dd.Normally, QE will search for the text as entered, verbatim. Checking this box
will cause QE to interpret that text as a :link reftype=fn refid=regex.regular
expression:elink. instead.
:p.Note that the text in the &osq.Replace with&csq. field will :hp1.not:ehp1. be
treated as a regular expression, except that regex-style substitution variables
(\1, \2, etc.) may be used.
:dt.Match case
:dd.By default, searching is not case-sensitive. Check this box to perform a
case-sensitive search.
:dt.Match whole words
:dd.Check this box to find only whole words which match the specified text. This
option is disabled when searching for a regular expression.
:dt.Search backwards
:dd.Normally, searching is carried out in the forwards direction. Check this
box to search backwards from the starting position instead.
:dt.From start of file / From end of file
:dd.Normally, searching starts from the current cursor position. Check this box
to start from the beginning or end of the file instead (depending on whether a
forwards or backwards search is being performed).
:dt.Confirm replacement
:dd.Check this box to be prompted before replacing each occurance of the found
text. If not checked, the found text will be replaced automatically. This only
applies using :hp2.Replace:ehp2. or :hp2.Replace all:ehp2..
:dt.Find (pushbutton)
:dd.This button will perform a find operation only; no replacement will be done.
The dialog will remain open.
:dt.Replace (pushbutton)
:dd.This button will initiate a find-and-replace operation. Only the first
match found (following the current start position) will be replaced. The dialog
will remain open.
:dt.Replace all (pushbutton)
:dd.This button will find and replace all instances of the find text (following
the current start position), and close the dialog.
:dt.Close (pushbutton)
:dd.This button will close the dialog without taking any further action.
:edl.


:fn id=regex.
:p.A :hp2.regular expression:ehp2. (or &osq.regex(p)&csq.) is an advanced search
pattern which is specified using a special syntax. QE uses the default Qt4
implementation of regular expressions, which is compatible with Perl.
:p.A description of regular expression syntax is outside the scope of this help
file. More information on regular expressions in general can be found at&colon.
:p.https&colon.//en.wikipedia.org/wiki/Regular_expression#Syntax
:p.and information on the Qt4 implemention in particular at&colon.
:p.http&colon.//doc.qt.io/archives/qt-4.7/qregexp.html#details
:efn.


.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=200.Keyboard commands
:p.QE uses platform-defined keyboard shortcuts for several common commands.
Therefore, certain specific shortcuts may vary depending on the operating
system.

:p.The following keyboard commands and shortcuts are common to all platforms.

.* :p.:hp5.File and program commands:ehp5.
.* :dl compact tsize=30 break=fit.

.* :dt.F2
.* :dd.Save file

.* :dt.Ctrl+N
.* :dd.New file

.* :dt.Ctrl+O
.* :dd.Open file

.* :dt.Ctrl+P
.* :dd.Print file

.* :dt.F3
.* :dd.Exit QE

.* :edl.

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

:dt.Ctrl+/
:dd.Select all (this is in addition to any platform-specific shortcut)

.* :dt.Ctrl+F
.* :dd.Find text

:dt.Ctrl+G
:dd.Find again (repeat last search)

:dt.Ctrl+R
:dd.Find and replace text

.* :dt.Ctrl+Ins  or  Ctrl+C
.* :dd.Copy selected text

.* :dt.Ctrl+Y  or  Shift+Alt+Bksp
.* :dd.Redo edit

.* :dt.Ctrl+Z  or  Alt+Bksp
.* :dd.Undo edit

.* :dt.Shift+Del  or  Ctrl+X
.* :dd.Cut selected text

.* :dt.Shift+Ins  or  Ctrl+V
.* :dd.Paste selected text

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

:dt.F3
:dd.Exit QE

:edl.


.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=300.Using encodings
:p.:hp1.Text encoding:ehp1. refers to the encoding scheme used for representing
text in digital data. In other words, it determines how bytes are translated
into meaningful text. It is therefore essential that&colon.
:ol compact.
:li.The appropriate encoding is used when opening and reading a text file.
:li.Each text file is saved using an encoding that supports the characters it
contains.
:eol.
:p.The encoding features in QE have been designed to facilitate these
objectives. Note that failing in either of these objectives leads to problems
which are described below (under :hp2.Potential Pitfalls:ehp2.).

:p.
:p.:hp7.QE's Behaviour:ehp7.
:p.QE defaults to using the current system encoding (codepage) when started.
This is done to maximize compatibility with other text editors, which generally
do the same. As long as the files you work with use the system encoding (as
most text files generally do), this should not be a problem.

:p.You can select a new encoding at any time using the :hp2.File &bxh.&rahead.
Encoding:ehp2. menu.

:p.If you change the current encoding after opening an existing file, but
:hp1.before:ehp1. any changes have been made, you will be asked if you want to
reload that file using the new encoding. If you know that the file in question
uses that encoding, then you should do so.  Note that you will not be offered
this choice if you have made any changes to the file since opening it.

:p.If, after making changes to a file, you save it using a different encoding
from the one with which the file was originally opened, you will be shown a
message warning you to make sure that you are indeed using the correct encoding
for the file.

:p.
:p.:hp7.Potential Pitfalls:ehp7.

:p.:hp5.Opening a file with the wrong encoding:ehp5.
:p.If you open a file with the wrong encoding, certain characters in the
file may not display correctly. This can generally be rectified by selecting
the correct encoding, if you know (or can determine) what that is.
:p.This problem is not a destructive one unless you subsequently save the
file using the wrong encoding (see below).

:p.:hp5.Saving a file with the wrong encoding:ehp5.
:p.If you save a file using an encoding that does not support all of the
characters in the text, those unsupported characters :hp8.will be lost:ehp8.
&mdash. they will be replaced by substitution symbols in the saved file. It
is therefore very important that you always use an appropriate encoding when
saving a file.
:p.If you are unsure of what the proper encoding should be, you are advised
to select UTF-8, which supports all known characters.


.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=800.Limitations
:p.This section lists some of QE's limitations.
:ul.
:li.OS/2 bitmap fonts are not supported.
:li.The OS/2 version does not currently support printing to non-CUPS printers.
:li.The Windows and Linux versions will not remember the character encoding
used for a given file after that file is closed, except in the case where the
file begins with a Unicode byte-order mark (BOM).
:li.If you save a file containing characters not supported by the current
encoding, those characters will replaced by substitution markers in the saved
file. See the section on :link reftype=hd res=300.encodings:elink. for more
information.
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

:p.Project home page&colon. https&colon.//github.com/altsan/qe-text-editor/
:p.

:lm margin=4.
:p.:hp2.QE Text Editor:ehp2.
.br
Copyright (C) 2019 Alexander Taylor

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
:p.

:p.See the :link reftype=hd res=990.following section:elink. for the complete
text of the GPL.

.*----------------------------------------------------------------------------
:h2 x=left y=bottom width=100% height=100% res=990.COPYING
.ce GNU GENERAL PUBLIC LICENSE
.ce Version 3, 29 June 2007

:p.Copyright (C) 2007 Free Software Foundation, Inc.
<:hp2.http&colon.//fsf.org/:ehp2.>.
.br
:p.Everyone is permitted to copy and distribute verbatim copies of this license
document, but changing it is not allowed.
:p.
.ce Preamble

:p.The GNU General Public License is a free, copyleft license for software and
other kinds of works.

:p.The licenses for most software and other practical works are designed to
take away your freedom to share and change the works. By contrast, the GNU
General Public License is intended to guarantee your freedom to share and
change all versions of a program--to make sure it remains free software for all
its users. We, the Free Software Foundation, use the GNU General Public License
for most of our software; it applies also to any other work released this way
by its authors. You can apply it to your programs, too.

:p.When we speak of free software, we are referring to freedom, not price. Our
General Public Licenses are designed to make sure that you have the freedom to
distribute copies of free software (and charge for them if you wish), that you
receive source code or can get it if you want it, that you can change the
software or use pieces of it in new free programs, and that you know you can do
these things.

:p.To protect your rights, we need to prevent others from denying you these
rights or asking you to surrender the rights. Therefore, you have certain
responsibilities if you distribute copies of the software, or if you modify
it&colon. responsibilities to respect the freedom of others.

:p.For example, if you distribute copies of such a program, whether gratis or
for a fee, you must pass on to the recipients the same freedoms that you
received. You must make sure that they, too, receive or can get the source
code. And you must show them these terms so they know their rights.

:p.Developers that use the GNU GPL protect your rights with two steps&colon.
(1) assert copyright on the software, and (2) offer you this License giving you
legal permission to copy, distribute and/or modify it.

:p.For the developers' and authors' protection, the GPL clearly explains that
there is no warranty for this free software. For both users' and authors' sake,
the GPL requires that modified versions be marked as changed, so that their
problems will not be attributed erroneously to authors of previous versions.

:p.Some devices are designed to deny users access to install or run modified
versions of the software inside them, although the manufacturer can do so. This
is fundamentally incompatible with the aim of protecting users' freedom to
change the software. The systematic pattern of such abuse occurs in the area of
products for individuals to use, which is precisely where it is most
unacceptable. Therefore, we have designed this version of the GPL to prohibit
the practice for those products. If such problems arise substantially in other
domains, we stand ready to extend this provision to those domains in future
versions of the GPL, as needed to protect the freedom of users.

:p.Finally, every program is threatened constantly by software patents. States
should not allow patents to restrict development and use of software on
general-purpose computers, but in those that do, we wish to avoid the special
danger that patents applied to a free program could make it effectively
proprietary. To prevent this, the GPL assures that patents cannot be used to
render the program non-free.

:p.The precise terms and conditions for copying, distribution and modification
follow.
:p.
.ce TERMS AND CONDITIONS

:p.0. Definitions.

:p."This License" refers to version 3 of the GNU General Public License.

:p."Copyright" also means copyright-like laws that apply to other kinds of
works, such as semiconductor masks.

:p."The Program" refers to any copyrightable work licensed under this License.
Each licensee is addressed as "you". "Licensees" and recipients may be
individuals or organizations.

:p.To "modify" a work means to copy from or adapt all or part of the work in a
fashion requiring copyright permission, other than the making of an exact copy.
The resulting work is called a ?modified version? of the earlier work or a work
"based on" the earlier work.

:p.A "covered work" means either the unmodified Program or a work based on the
Program.

:p.To "propagate" a work means to do anything with it that, without permission,
would make you directly or secondarily liable for infringement under applicable
copyright law, except executing it on a computer or modifying a private copy.
Propagation includes copying, distribution (with or without modification),
making available to the public, and in some countries other activities as well.

:p.To "convey" a work means any kind of propagation that enables other parties
to make or receive copies. Mere interaction with a user through a computer
network, with no transfer of a copy, is not conveying.

:p.An interactive user interface displays "Appropriate Legal Notices" to the
extent that it includes a convenient and prominently visible feature that (1)
displays an appropriate copyright notice, and (2) tells the user that there is
no warranty for the work (except to the extent that warranties are provided),
that licensees may convey the work under this License, and how to view a copy
of this License. If the interface presents a list of user commands or options,
such as a menu, a prominent item in the list meets this criterion.

:p.1. Source Code.

:p.The "source code" for a work means the preferred form of the work for
making modifications to it. "Object code" means any non-source form of a work.

:p.A "Standard Interface" means an interface that either is an official
standard defined by a recognized standards body, or, in the case of interfaces
specified for a particular programming language, one that is widely used among
developers working in that language.

:p.The "System Libraries" of an executable work include anything, other than
the work as a whole, that (a) is included in the normal form of packaging a
Major Component, but which is not part of that Major Component, and (b) serves
only to enable use of the work with that Major Component, or to implement a
Standard Interface for which an implementation is available to the public in
source code form. A "Major Component", in this context, means a major essential
component (kernel, window system, and so on) of the specific operating system
(if any) on which the executable work runs, or a compiler used to produce the
work, or an object code interpreter used to run it.

:p.The "Corresponding Source" for a work in object code form means all the
source code needed to generate, install, and (for an executable work) run the
object code and to modify the work, including scripts to control those
activities. However, it does not include the work's System Libraries, or
general-purpose tools or generally available free programs which are used
unmodified in performing those activities but which are not part of the work.
For example, Corresponding Source includes interface definition files
associated with source files for the work, and the source code for shared
libraries and dynamically linked subprograms that the work is specifically
designed to require, such as by intimate data communication or control flow
between those subprograms and other parts of the work.

:p.The Corresponding Source need not include anything that users can regenerate
automatically from other parts of the Corresponding Source.

:p.The Corresponding Source for a work in source code form is that same work.

:p.2. Basic Permissions.

:p.All rights granted under this License are granted for the term of copyright
on the Program, and are irrevocable provided the stated conditions are met.
This License explicitly affirms your unlimited permission to run the unmodified
Program. The output from running a covered work is covered by this License only
if the output, given its content, constitutes a covered work. This License
acknowledges your rights of fair use or other equivalent, as provided by
copyright law.

:p.You may make, run and propagate covered works that you do not convey,
without conditions so long as your license otherwise remains in force. You may
convey covered works to others for the sole purpose of having them make
modifications exclusively for you, or provide you with facilities for running
those works, provided that you comply with the terms of this License in
conveying all material for which you do not control copyright. Those thus
making or running the covered works for you must do so exclusively on your
behalf, under your direction and control, on terms that prohibit them from
making any copies of your copyrighted material outside their relationship with
you.

:p.Conveying under any other circumstances is permitted solely under the
conditions stated below. Sublicensing is not allowed; section 10 makes it
unnecessary.

:p.3. Protecting Users' Legal Rights From Anti-Circumvention Law.

:p.No covered work shall be deemed part of an effective technological measure
under any applicable law fulfilling obligations under article 11 of the WIPO
copyright treaty adopted on 20 December 1996, or similar laws prohibiting or
restricting circumvention of such measures.

:p.When you convey a covered work, you waive any legal power to forbid
circumvention of technological measures to the extent such circumvention is
effected by exercising rights under this License with respect to the covered
work, and you disclaim any intention to limit operation or modification of the
work as a means of enforcing, against the work's users, your or third parties'
legal rights to forbid circumvention of technological measures.

:p.4. Conveying Verbatim Copies.

:p.You may convey verbatim copies of the Program's source code as you receive
it, in any medium, provided that you conspicuously and appropriately publish on
each copy an appropriate copyright notice; keep intact all notices stating that
this License and any non-permissive terms added in accord with section 7 apply
to the code; keep intact all notices of the absence of any warranty; and give
all recipients a copy of this License along with the Program.

:p.You may charge any price or no price for each copy that you convey, and you
may offer support or warranty protection for a fee.

:p.5. Conveying Modified Source Versions.

:p.You may convey a work based on the Program, or the modifications to produce
it from the Program, in the form of source code under the terms of section 4,
provided that you also meet all of these conditions&colon.
:dl tsize=3 break=none.
:dt.a)
:dd.The work must carry prominent notices stating that you modified it, and
giving a relevant date.
:dt.b)
:dd.The work must carry prominent notices stating that it is released under
this License and any conditions added under section 7. This requirement
modifies the requirement in section 4 to "keep intact all notices".
:dt.c)
:dd.You must license the entire work, as a whole, under this License to anyone
who comes into possession of a copy. This License will therefore apply, along
with any applicable section 7 additional terms, to the whole of the work, and
all its parts, regardless of how they are packaged. This License gives no
permission to license the work in any other way, but it does not invalidate
such permission if you have separately received it.
:dt.d)
:dd.If the work has interactive user interfaces, each must display Appropriate
Legal Notices; however, if the Program has interactive interfaces that do not
display Appropriate Legal Notices, your work need not make them do so.
:edl.

:p.A compilation of a covered work with other separate and independent works,
which are not by their nature extensions of the covered work, and which are not
combined with it such as to form a larger program, in or on a volume of a
storage or distribution medium, is called an "aggregate" if the compilation and
its resulting copyright are not used to limit the access or legal rights of the
compilation's users beyond what the individual works permit. Inclusion of a
covered work in an aggregate does not cause this License to apply to the other
parts of the aggregate.

:p.6. Conveying Non-Source Forms.

:p.You may convey a covered work in object code form under the terms of
sections 4 and 5, provided that you also convey the machine-readable
Corresponding Source under the terms of this License, in one of these ways:
:dl tsize=3 break=none.
:dt.a)
:dd.Convey the object code in, or embodied in, a physical product (including a
physical distribution medium), accompanied by the Corresponding Source fixed on
a durable physical medium customarily used for software interchange.
:dt.b)
:dd.Convey the object code in, or embodied in, a physical product (including a
physical distribution medium), accompanied by a written offer, valid for at
least three years and valid for as long as you offer spare parts or customer
support for that product model, to give anyone who possesses the object code
either (1) a copy of the Corresponding Source for all the software in the
product that is covered by this License, on a durable physical medium
customarily used for software interchange, for a price no more than your
reasonable cost of physically performing this conveying of source, or (2)
access to copy the Corresponding Source from a network server at no charge.
:dt.c)
:dd.Convey individual copies of the object code with a copy of the written
offer to provide the Corresponding Source. This alternative is allowed only
occasionally and noncommercially, and only if you received the object code with
such an offer, in accord with subsection 6b.
:dt.d)
:dd.Convey the object code by offering access from a designated place (gratis
or for a charge), and offer equivalent access to the Corresponding Source in
the same way through the same place at no further charge. You need not require
recipients to copy the Corresponding Source along with the object code. If the
place to copy the object code is a network server, the Corresponding Source may
be on a different server (operated by you or a third party) that supports
equivalent copying facilities, provided you maintain clear directions next to
the object code saying where to find the Corresponding Source. Regardless of
what server hosts the Corresponding Source, you remain obligated to ensure that
it is available for as long as needed to satisfy these requirements.
:dt.e)
:dd.Convey the object code using peer-to-peer transmission, provided you inform
other peers where the object code and Corresponding Source of the work are
being offered to the general public at no charge under subsection 6d.
:edl.

:p.A separable portion of the object code, whose source code is excluded from
the Corresponding Source as a System Library, need not be included in conveying
the object code work.

:p.A "User Product" is either (1) a "consumer product", which means any
tangible personal property which is normally used for personal, family, or
household purposes, or (2) anything designed or sold for incorporation into a
dwelling. In determining whether a product is a consumer product, doubtful
cases shall be resolved in favor of coverage. For a particular product received
by a particular user, "normally used" refers to a typical or common use of that
class of product, regardless of the status of the particular user or of the way
in which the particular user actually uses, or expects or is expected to use,
the product. A product is a consumer product regardless of whether the product
has substantial commercial, industrial or non-consumer uses, unless such uses
represent the only significant mode of use of the product.

:p."Installation Information" for a User Product means any methods, procedures,
authorization keys, or other information required to install and execute
modified versions of a covered work in that User Product from a modified
version of its Corresponding Source. The information must suffice to ensure
that the continued functioning of the modified object code is in no case
prevented or interfered with solely because modification has been made.

:p.If you convey an object code work under this section in, or with, or
specifically for use in, a User Product, and the conveying occurs as part of a
transaction in which the right of possession and use of the User Product is
transferred to the recipient in perpetuity or for a fixed term (regardless of
how the transaction is characterized), the Corresponding Source conveyed under
this section must be accompanied by the Installation Information. But this
requirement does not apply if neither you nor any third party retains the
ability to install modified object code on the User Product (for example, the
work has been installed in ROM).

:p.The requirement to provide Installation Information does not include a
requirement to continue to provide support service, warranty, or updates for a
work that has been modified or installed by the recipient, or for the User
Product in which it has been modified or installed. Access to a network may be
denied when the modification itself materially and adversely affects the
operation of the network or violates the rules and protocols for communication
across the network.

:p.Corresponding Source conveyed, and Installation Information provided, in
accord with this section must be in a format that is publicly documented (and
with an implementation available to the public in source code form), and must
require no special password or key for unpacking, reading or copying.

:p.7. Additional Terms.

:p."Additional permissions" are terms that supplement the terms of this
License by making exceptions from one or more of its conditions. Additional
permissions that are applicable to the entire Program shall be treated as
though they were included in this License, to the extent that they are valid
under applicable law. If additional permissions apply only to part of the
Program, that part may be used separately under those permissions, but the
entire Program remains governed by this License without regard to the
additional permissions.

:p.When you convey a copy of a covered work, you may at your option remove any
additional permissions from that copy, or from any part of it. (Additional
permissions may be written to require their own removal in certain cases when
you modify the work.) You may place additional permissions on material, added
by you to a covered work, for which you have or can give appropriate copyright
permission.

:p.Notwithstanding any other provision of this License, for material you add to
a covered work, you may (if authorized by the copyright holders of that
material) supplement the terms of this License with terms:
:dl tsize=3 break=none.
:dt.a)
:dd.Disclaiming warranty or limiting liability differently from the terms of
sections 15 and 16 of this License; or
:dt.b)
:dd.Requiring preservation of specified reasonable legal notices or author
attributions in that material or in the Appropriate Legal Notices displayed by
works containing it; or
:dt.c)
:dd.Prohibiting misrepresentation of the origin of that material, or requiring
that modified versions of such material be marked in reasonable ways as
different from the original version; or
:dt.d)
:dd.Limiting the use for publicity purposes of names of licensors or authors of
the material; or
:dt.e)
:dd.Declining to grant rights under trademark law for use of some trade names,
trademarks, or service marks; or
:dt.f)
:dd.Requiring indemnification of licensors and authors of that material by
anyone who conveys the material (or modified versions of it) with contractual
assumptions of liability to the recipient, for any liability that these
contractual assumptions directly impose on those licensors and authors.
:edl.

:p.All other non-permissive additional terms are considered "further
restrictions" within the meaning of section 10. If the Program as you received
it, or any part of it, contains a notice stating that it is governed by this
License along with a term that is a further restriction, you may remove that
term. If a license document contains a further restriction but permits
relicensing or conveying under this License, you may add to a covered work
material governed by the terms of that license document, provided that the
further restriction does not survive such relicensing or conveying.

:p.If you add terms to a covered work in accord with this section, you must
place, in the relevant source files, a statement of the additional terms that
apply to those files, or a notice indicating where to find the applicable terms.

:p.Additional terms, permissive or non-permissive, may be stated in the form of
a separately written license, or stated as exceptions; the above requirements
apply either way.

:p.8. Termination.

:p.You may not propagate or modify a covered work except as expressly provided
under this License. Any attempt otherwise to propagate or modify it is void,
and will automatically terminate your rights under this License (including any
patent licenses granted under the third paragraph of section 11).

:p.However, if you cease all violation of this License, then your license from
a particular copyright holder is reinstated (a) provisionally, unless and until
the copyright holder explicitly and finally terminates your license, and (b)
permanently, if the copyright holder fails to notify you of the violation by
some reasonable means prior to 60 days after the cessation.

:p.Moreover, your license from a particular copyright holder is reinstated
permanently if the copyright holder notifies you of the violation by some
reasonable means, this is the first time you have received notice of violation
of this License (for any work) from that copyright holder, and you cure the
violation prior to 30 days after your receipt of the notice.

:p.Termination of your rights under this section does not terminate the
licenses of parties who have received copies or rights from you under this
License. If your rights have been terminated and not permanently reinstated,
you do not qualify to receive new licenses for the same material under section
10.

:p.9. Acceptance Not Required for Having Copies.

:p.You are not required to accept this License in order to receive or run a
copy of the Program. Ancillary propagation of a covered work occurring solely
as a consequence of using peer-to-peer transmission to receive a copy likewise
does not require acceptance. However, nothing other than this License grants
you permission to propagate or modify any covered work. These actions infringe
copyright if you do not accept this License. Therefore, by modifying or
propagating a covered work, you indicate your acceptance of this License to do
so.

:p.10. Automatic Licensing of Downstream Recipients.

:p.Each time you convey a covered work, the recipient automatically receives a
license from the original licensors, to run, modify and propagate that work,
subject to this License. You are not responsible for enforcing compliance by
third parties with this License.

:p.An "entity transaction" is a transaction transferring control of an
organization, or substantially all assets of one, or subdividing an
organization, or merging organizations. If propagation of a covered work
results from an entity transaction, each party to that transaction who receives
a copy of the work also receives whatever licenses to the work the party's
predecessor in interest had or could give under the previous paragraph, plus a
right to possession of the Corresponding Source of the work from the
predecessor in interest, if the predecessor has it or can get it with
reasonable efforts.

:p.You may not impose any further restrictions on the exercise of the rights
granted or affirmed under this License. For example, you may not impose a
license fee, royalty, or other charge for exercise of rights granted under this
License, and you may not initiate litigation (including a cross-claim or
counterclaim in a lawsuit) alleging that any patent claim is infringed by
making, using, selling, offering for sale, or importing the Program or any
portion of it.

:p.11. Patents.

:p.A "contributor" is a copyright holder who authorizes use under this License
of the Program or a work on which the Program is based. The work thus licensed
is called the contributor's "contributor version".

:p.A contributor's "essential patent claims" are all patent claims owned or
controlled by the contributor, whether already acquired or hereafter acquired,
that would be infringed by some manner, permitted by this License, of making,
using, or selling its contributor version, but do not include claims that would
be infringed only as a consequence of further modification of the contributor
version. For purposes of this definition, "control" includes the right to grant
patent sublicenses in a manner consistent with the requirements of this License.

:p.Each contributor grants you a non-exclusive, worldwide, royalty-free patent
license under the contributor's essential patent claims, to make, use, sell,
offer for sale, import and otherwise run, modify and propagate the contents of
its contributor version.

:p.In the following three paragraphs, a "patent license" is any express
agreement or commitment, however denominated, not to enforce a patent (such as
an express permission to practice a patent or covenant not to sue for patent
infringement). To "grant" such a patent license to a party means to make such
an agreement or commitment not to enforce a patent against the party.

:p.If you convey a covered work, knowingly relying on a patent license, and the
Corresponding Source of the work is not available for anyone to copy, free of
charge and under the terms of this License, through a publicly available
network server or other readily accessible means, then you must either (1)
cause the Corresponding Source to be so available, or (2) arrange to deprive
yourself of the benefit of the patent license for this particular work, or (3)
arrange, in a manner consistent with the requirements of this License, to
extend the patent license to downstream recipients. "Knowingly relying" means
you have actual knowledge that, but for the patent license, your conveying the
covered work in a country, or your recipient's use of the covered work in a
country, would infringe one or more identifiable patents in that country that
you have reason to believe are valid.

:p.If, pursuant to or in connection with a single transaction or arrangement,
you convey, or propagate by procuring conveyance of, a covered work, and grant
a patent license to some of the parties receiving the covered work authorizing
them to use, propagate, modify or convey a specific copy of the covered work,
then the patent license you grant is automatically extended to all recipients
of the covered work and works based on it.

:p.A patent license is "discriminatory" if it does not include within the scope
of its coverage, prohibits the exercise of, or is conditioned on the
non-exercise of one or more of the rights that are specifically granted under
this License. You may not convey a covered work if you are a party to an
arrangement with a third party that is in the business of distributing
software, under which you make payment to the third party based on the extent
of your activity of conveying the work, and under which the third party grants,
to any of the parties who would receive the covered work from you, a
discriminatory patent license (a) in connection with copies of the covered work
conveyed by you (or copies made from those copies), or (b) primarily for and in
connection with specific products or compilations that contain the covered
work, unless you entered into that arrangement, or that patent license was
granted, prior to 28 March 2007.

:p.Nothing in this License shall be construed as excluding or limiting any
implied license or other defenses to infringement that may otherwise be
available to you under applicable patent law.

:p.12. No Surrender of Others' Freedom.

:p.If conditions are imposed on you (whether by court order, agreement or
otherwise) that contradict the conditions of this License, they do not excuse
you from the conditions of this License. If you cannot convey a covered work so
as to satisfy simultaneously your obligations under this License and any other
pertinent obligations, then as a consequence you may not convey it at all. For
example, if you agree to terms that obligate you to collect a royalty for
further conveying from those to whom you convey the Program, the only way you
could satisfy both those terms and this License would be to refrain entirely
from conveying the Program.

:p.13. Use with the GNU Affero General Public License.

:p.Notwithstanding any other provision of this License, you have permission to
link or combine any covered work with a work licensed under version 3 of the
GNU Affero General Public License into a single combined work, and to convey
the resulting work. The terms of this License will continue to apply to the
part which is the covered work, but the special requirements of the GNU Affero
General Public License, section 13, concerning interaction through a network
will apply to the combination as such.

:p.14. Revised Versions of this License.

:p.The Free Software Foundation may publish revised and/or new versions of the
GNU General Public License from time to time. Such new versions will be similar
in spirit to the present version, but may differ in detail to address new
problems or concerns.

:p.Each version is given a distinguishing version number. If the Program
specifies that a certain numbered version of the GNU General Public License "or
any later version" applies to it, you have the option of following the terms
and conditions either of that numbered version or of any later version
published by the Free Software Foundation. If the Program does not specify a
version number of the GNU General Public License, you may choose any version
ever published by the Free Software Foundation.

:p.If the Program specifies that a proxy can decide which future versions of
the GNU General Public License can be used, that proxy's public statement of
acceptance of a version permanently authorizes you to choose that version for
the Program.

:p.Later license versions may give you additional or different permissions.
However, no additional obligations are imposed on any author or copyright
holder as a result of your choosing to follow a later version.

:p.15. Disclaimer of Warranty.

:p.THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE
LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER
PARTIES PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER
EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE
QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE
DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.

:p.16. Limitation of Liability.

:p.IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL
ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS THE
PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY
GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR
INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA
BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A
FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS), EVEN IF SUCH HOLDER
OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

:p.17. Interpretation of Sections 15 and 16.

:p.If the disclaimer of warranty and limitation of liability provided above
cannot be given local legal effect according to their terms, reviewing courts
shall apply local law that most closely approximates an absolute waiver of all
civil liability in connection with the Program, unless a warranty or assumption
of liability accompanies a copy of the Program in return for a fee.
:p.
.ce END OF TERMS AND CONDITIONS


.ce How to Apply These Terms to Your New Programs

:p.If you develop a new program, and you want it to be of the greatest
possible use to the public, the best way to achieve this is to make it
free software which everyone can redistribute and change under these terms.

:p.To do so, attach the following notices to the program.  It is safest
to attach them to the start of each source file to most effectively
state the exclusion of warranty; and each file should have at least
the "copyright" line and a pointer to where the full notice is found.

:xmp.
    &lt.one line to give the program's name and a brief idea of what it does.&gt.
    Copyright (C) &lt.year&gt.  &lt.name of author&gt.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see &lt.http&colon.//www.gnu.org/licenses/&gt..
:exmp.

:p.Also add information on how to contact you by electronic and paper mail.

:p.If the program does terminal interaction, make it output a short
notice like this when it starts in an interactive mode&colon.

:xmp.
    &lt.program&gt.  Copyright (C) &lt.year&gt.  &lt.name of author&gt.
    This program comes with ABSOLUTELY NO WARRANTY; for details type &osq.show w&csq..
    This is free software, and you are welcome to redistribute it
    under certain conditions; type &osq.show c&csq. for details.
:exmp.

:p.The hypothetical commands &osq.show w&csq. and &osq.show c&csq. should show the appropriate
parts of the General Public License.  Of course, your program's commands
might be different; for a GUI interface, you would use an "about box".

:p.You should also get your employer (if you work as a programmer) or school,
if any, to sign a "copyright disclaimer" for the program, if necessary.
For more information on this, and how to apply and follow the GNU GPL, see
&lt.http&colon.//www.gnu.org/licenses/&gt..

:p.The GNU General Public License does not permit incorporating your program
into proprietary programs.  If your program is a subroutine library, you
may consider it more useful to permit linking proprietary applications with
the library.  If this is what you want to do, use the GNU Lesser General
Public License instead of this License.  But first, please read
&lt.http&colon.//www.gnu.org/philosophy/why-not-lgpl.html&gt..

:euserdoc.
