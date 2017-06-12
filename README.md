# SoftwareEngineering
Simple translator as a part of Software Engineering course

This program translate simple program on SIGNAL language to Assembly language.
Complexity of the program have been defined by my grammar - subset of full SIGNAL's grammar(read carefully, it's very specific stuff):

1.	\<signal-program> --> \<program> 
2.	\<program> --> PROCEDURE \<procedure-identifier>\<parameters-list>; \<block> . 
3.	\<block> --> \<declarations> BEGIN \<statements- list> END  
4.	\<declarations> --> \<label-declarations> \<procedure-declarations> 
5.  \<label-declarations> --> LABEL \<unsigned-integer> \<labels-list>
6.  \<labels-list> --> ,\<unsigned-integer>\<labels-list> | \<empty>
7.	\<procedure-declarations> --> \<procedure> \<procedure-declarations> | \<empty> 
8.	\<procedure> --> PROCEDURE \<procedure-identifier>\<parameters-list> ;
9.	\<parameters-list> -->( \<variable-identifier>\<identifiers-list> ) | \<empty> 
10.	\<identifiers-list> -->  , \<variable-identifier>\<identifiers-list> | \<empty> 
11.	\<statements-list> --> \<label-value>\<statement>\<statements-list> | \<empty>
12.	\<statement> --> \<procedure-identifier>\<actual-arguments> ; | RETURN :
13.	\<actual-arguments> --> ( \<unsigned-integer>\<actual-arguments-list> )  | \<empty> 
14.	\<actual-arguments-list> --> , \<unsigned-integer>\<actual-arguments-list> | \<empty>
15.	\<variable-identifier> --> \<identifier>
16.	\<procedure-identifier> --> \<identifier> 
17.	\<unsigned-integer> --> \<digit>\<digits-string> 
18.	\<identifier> --> \<letter>\<string>
19. \<label-value> --> \<label-name>: | \<empty>
20. \<label-name> --> @ \<unsigned-integer>

