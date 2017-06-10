# SoftwareEngineering
Simple translator as a part of Software Engineering course

This program translate simple program on SIGNAL language to Assembly language.
Complexity of the program have been defined by my grammar - part of full SIGNAL's grammar(read carefully, it's very specific stuff):

1.	<signal-program> --> <program> 
2.	<program> --> PROCEDURE <procedure-identifier><parameters-list>; <block> . 
3.	<block> --> <declarations> BEGIN <statements- list> END  
4.	<declarations> --> <procedure-declarations> 
5.	<procedure-declarations> --> <procedure> <procedure-declarations> | <empty> 
6.	<procedure> --> PROCEDURE <procedure-identifier><parameters-list> ;
7.	<parameters-list> -->( <variable-identifier><identifiers-list> ) | <empty> 
8.	<identifiers-list> -->  , <variable-identifier><identifiers-list> | <empty> 
9.	<statements-list> --> <statement><statements-list> | <empty>
10.	< statement> --> <procedure-identifier><actual-arguments> ; | RETURN :
11.	<actual-arguments> --> ( <unsigned-integer><actual-arguments-list> )  | <empty> 
12.	<actual-arguments-list> --> , <unsigned-integer><actual-arguments-list> | <empty>
13.	<variable-identifier> --> <identifier>
14.	<procedure-identifier> --> <identifier> 
15.	<unsigned-integer> --> <digit><digits-string> 
16.	<identifier> --> <letter><string>

