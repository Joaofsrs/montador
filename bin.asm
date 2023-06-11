
SECTION TEXT
INPUT OLD_DATA ;                0   12  29
LOAD OLD_DATA ;                 2   10  29
L1:  DIV DOIS ;                 4   04  28
STORE  NEW_DATA ;               6   11  30
MUL DOIS ;                      8   03  28
STORE TMP_DATA ;                10  11  31
LOAD OLD_DATA ;                 12  10  29
SUB TMP_DATA ;                  14  02  31
STORE TMP_DATA ;                16  11  31
OUTPUT TmP_DATA ;               18  13  32
COPY NEW_DATA , OLD_DATA ;      20  09  30  29
LOAD OLD_DATA ;                 23  10  29
JMPP L1 ;                       25  07  04
STOP ;                          27  14  
SECTION DATA                
DOIS: CONST 2 ;                 28  02
OLD_DATA: SPACE ;               29  00
NEW_DATA: SPACE ;               30  00
TMP_DATA: SPACE ;               31  00