# detection_contour

-----------------------
		README
-----------------------

ANALYSE D'IMAGE - 2016

TP1

ANNA BENNETON - FABIEN MAROT
______________________


Pour compiler en mode Graphique (par défaut) :

make && ./bin/tp1


------

Pour compiler en mode Console ( si probleme de compatibilité avec glib/gtk ) :

make MODE="CLI" && ./bin/tp1


______________________

LIB nécessaires :

opencv2

//Pour l'interface graphique :
glibmm-2.4
glib-2.0
gtkmm-3.0
gtk+-3.0
gdkmm-3.0


a faire :
 modifier les filtres local, global (% en plus/moins, moyenne/medianne/ecart-type)
 affinage : mieux + réduction du bruit
 fermeture

 --> detection de formes ?

 calcul sur les bords ?
 