# ANALYSE D'IMAGE
*octobre 2016*

## DETECTION DE CONTOURS - TP1
## TRANSFORMÉE DE HOUGH - TP2

###### ANNA BENNETON - FABIEN MAROT

https://github.com/dev-banana/detection_contour.git


______________________
______________________


GUI - *mode graphique*
------
Pour compiler (par défaut) :

	make

Pour compiler et lancer le programme :

	make run

______________________


CLI - *mode console*
------
*( si probleme de compatibilité avec glib/gtk )*


Pour compiler :

	make MODE="CLI"

Pour compiler et lancer le programme :

	make MODE="CLI" run


*En mode console, les options ne sont pas dynamiques mais peuvent être modifiées facilement en choisissant les nouvelles valeurs parmis celles en commentaires dans le constructeur de la classe Option, dans le fichier :*

	src/cli/option.cpp  

______________________


LIBS
------

	opencv2

Pour l'interface graphique :

	glibmm-2.4
	glib-2.0
	gtkmm-3.0
	gtk+-3.0
	gdkmm-3.0

______________________


WIKI
------

Vous pouvez trouvez plus de détails sur l'utisation de ce programme dans le [wiki](https://github.com/dev-banana/detection_contour/wiki).