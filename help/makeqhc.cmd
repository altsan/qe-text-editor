/* REXX script
 * Generate the QtAssistant collection and compressed help files.
 */
PARSE SOURCE _os .
IF _os == 'OS/2' THEN DO
    uxroot = VALUE('UNIXROOT',,'OS2ENVIRONMENT')
    qcg_cmd = uxroot'\usr\lib\qt4\bin\qcollectiongenerator'
END
ELSE qcg_cmd = 'qcollectiongenerator'

ADDRESS CMD qcg_cmd 'qe.qhcp -o qe.qhc'
EXIT rc
