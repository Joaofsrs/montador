BIN1: BEGIN
EXTERN: BIN2
EXTERN: OLD_DATA
PUBLIC L1
PUBLIC BIN1
SECTION TEXT
JMP BIN2 ;                      00 05 00
L1: DIV DOIS ;                  02 04 26
STORE  NEW_DATA ;               04 11 27
MUL DOIS ;                      06 03 26
STORE TMP_DATA ;                08 11 28
LOAD OLD_DATA ;                 10 10 00
SUB TMP_DATA ;                  12 02 28
STORE TMP_DATA ;                14 11 28
OUTPUT TMP_DATA ;               16 13 28
COPY NEW_DATA , OLD_DATA ;      18 09 27 00
LOAD OLD_DATA ;                 21 10 00
JMPP L1 ;                       23 07 02
STOP ;                          25 14 
SECTION DATA                
DOIS: CONST 2 ;                 26 02
NEW_DATA: SPACE ;               27 00
TMP_DATA: SPACE ;               28 00
