/* REXX script
 * Convert IPF help source into single-section HTML files.
 */
CALL RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
CALL SysLoadFuncs

CALL SysFileTree '*.ipf', 'ipfs.', 'FO'
h = 0
DO i = 1 TO ipfs.0
    lp = LASTPOS('.', ipfs.i )
    outfile = SUBSTR( ipfs.i, 1, lp ) || 'html'
    'sed -r -f ipfhtml.sed' ipfs.i '>' outfile
    IF STREAM( outfile, 'C', 'QUERY EXISTS') <> '' THEN DO
        h = h + 1
        htms.h = outfile
    END
END
htms.0 = h

DO i = 1 TO htms.0
    CALL LINEIN htms.i, 1, 0
    secs = 1
    lp = LASTPOS('.', htms.i )
    outfile = SUBSTR( htms.i, 1, lp ) || secs'.html'
    DO WHILE LINES( htms.i )
        l = LINEIN( htms.i )
        IF TRANSLATE( STRIP( LEFT( l, 10 ))) == '<!-- BEGIN' THEN DO
            IF secs > 1 THEN DO
                CALL LINEOUT outfile, '</html>'
                CALL LINEOUT outfile
                outfile = SUBSTR( htms.i, 1, lp ) || secs'.html'
                CALL LINEOUT outfile, '<html>'
            END
            secs = secs + 1
        END
        CALL LINEOUT outfile, l
    END
    CALL LINEOUT outfile
    CALL STREAM htms.i, 'C', 'CLOSE'
    CALL SysFileDelete htms.i
END


RETURN 0
