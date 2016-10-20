-----------------------
# 		DETECTION DE CONTOURS

ANALYSE D'IMAGE - octobre 2016

TP1

ANNA BENNETON - FABIEN MAROT

-----------------------


------
 GUI
------
Pour compiler [et lancer] en mode Graphique (par défaut) :

	make [run]


------
 CLI
------
Pour compiler [et lancer] en mode Console :
( si probleme de compatibilité avec glib/gtk )

	make MODE="CLI" [run]


!! En mode console, les options ne sont pas dynamiques mais peuvent être modifiées facilement dans le fichier :
	src/cli/option.cpp  
--> choisir les nouvelles valeurs parmis celles en commentaires dans le constructeur de la classe Option.

______________________

LIBS nécessaires :

	opencv2

//Pour l'interface graphique :

	glibmm-2.4
	glib-2.0
	gtkmm-3.0
	gtk+-3.0
	gdkmm-3.0