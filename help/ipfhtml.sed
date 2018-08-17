s/</\&lt;/g
s/>/\&gt;/g
s/:userdoc\./<html>/g
s/:euserdoc\./<\/html>/g
s/^\.\*(.*)$/<!-- \1 -->/g
s/:p\./<p>/g
s/\&amp\./\&amp;/g
s/&lt./\&lt;/g
s/&gt./\&gt;/g
s/&colon\./:/g
s/&osq\./\&#x2018;/g
s/&csq\./\&#x2019;/g
s/&mdash\./\&emdash;/g
s/&ndash\./\&endash;/g
s/&bxh\./\&#x2014;/g
s/:ol.*\./<ol>/g
s/:eol.*\./<\/ol>/g
s/:ul.*\./<ul>/g
s/:eul.*\./<\/ul>/g
s/:li\./<li>/g
s/:dl.*\./<dl>/g
s/:edl\./<\/dl>/g
s/:dt\./<dt>/g
s/:dd\./<dd>/g
s/\.br[:blank:]*$/<br>/g
s/\.ce (.*)$/<p align="center">\1<\/p>/g
s/\.im (.*)$/<!-- include file: \1 -->/g
s/:h1.* id=(.*) .*\.(.*)/<!-- begin section l1 -->\n<hr><a name="\1"><\/a><h1>\2<\/h1>/g
s/:h1.*\.(.*)/<!-- begin section l1 -->\n<hr><h1>\1<\/h1>/g
s/:h2.* id=(.*) .*\.(.*)/<!-- begin section l2 -->\n<hr><a name="\1"><\/a><h2>\2<\/h2>/g
s/:h2.*\.(.*)/<!-- begin section l2 -->\n<hr><h2>\1<\/h2>/g
s/:h3.* id=(.*) .*\.(.*)/<!-- begin section l3 -->\n<hr><a name="\1"><\/a><h3>\2<\/h3>/g
s/:h3.*\.(.*)/<!-- begin section l3 -->\n<hr><h3>\1<\/h3>/g
s/:h4.* id=(.*) .*\.(.*)/<hr><a name="\1"><\/a><h4>\2<\/h4>/g
s/:h4.*\.(.*)/<hr><h4>\1<\/h4>/g
s/:fn.* id=(.*)\./<!-- begin footnote -->\n<a name="\1"><div style="border: thin solid black; padding: 1em; background: #e0e0e0;">/g
s/:efn\./<\/div>\n<!-- end footnote -->/g
s/:xmp\./<pre>/g
s/:exmp\./<\/pre>/g
s/:hp1\./<em>/g
s/:ehp1\./<\/em>/g
s/:hp2\./<strong>/g
s/:ehp2\./<\/strong>/g
s/:hp4\./<font color="blue">/g
s/:ehp4\./<\/font>/g
s/:hp5\./<u>/g
s/:ehp5\./<\/u>/g
s/:hp6\./<i><u>/g
s/:ehp6\./<\/u><\/i>/g
s/:hp7\./<b><i>/g
s/:ehp7\./<\/i><\/b>/g
s/:hp8\./<font color="red">/g
s/:ehp8\./<\/font>/g
s/:hp9\./<font color="pink">/g
s/:ehp9\./<\/font>/g
s/:link [a-zA-Z0-9]*refid=([^.]*)\.(.*):elink\./<a href="#\1">\2<\/a>/g
s/:link [a-zA-Z0-9]*refid=([^.]*)\./<a href="#\1">/g
s/:elink\./<\/a>/g
s/:link [^.]*\.//g
s/:elink\.//g
s/:note.*\./<b>Note:<\/b> /g
s/:nt.* text='(.*)'\./<b>\1:<\/b> /g
s/:nt.*\./<b>Note:<\/b> /g
s/:ent\./<\/p>/g
s/:artwork runin name='([^.]*)\.bmp'\./<img src="\1.png">/g
s/:artwork runin name='([^']*)'\./<img src="\1">/g
s/:artwork align=([a-zA-Z]*) name='([^.]*)\.bmp'\./<p align="\1"><img src="\2.png"><\/p>/g
s/:artwork align=([a-zA-Z]*) name='([^']*)'\./<p align="\1"><img src="\2"><\/p>/g
s/:lm margin=[2-9]*\./<blockquote>/g
s/:lm margin=1\./<\/blockquote>/g
