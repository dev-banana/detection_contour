
#include "option.hpp"

Option::Option()
{ 
  lissage_type = GAUSSIEN ; //MOYENNE ; MEDIANE ; GAUSSIEN
  lissage_size = 7 ; //nb impair
  lissage_sigma = 5 ;

  filtre.prewitt(); //prewitt() ; sobel() ; kirsch() ; lapalacien( size ) 
  direction = MULTI_DIRECTIONNEL ; //HORIZONTAL ; VERTICAL ; BI_DIRECTIONNEL ; MULTI_DIRECTIONNEL

  seuil = HYSTERESIS_AUTO ; //UNIQUE ; GLOBAL ; LOCAL ; HYSTERESIS_AUTO ; HYSTERESIS
  seuil_calcul = MOYENNE ; // MOYENNE ; MEDIANE ; ECART_TYPE
  seuil_val = 50 ;
  seuil_fenetre = 15 ; // nb impair
  seuil_bas = 44 ; // bas < haut
  seuil_haut = 60 ; // haut > bas
  
  show_color = true ;
  keep_norme = true ;
}



void Option::set_direction( unsigned int id )
{
  switch(id)
  {
    case 0:
      direction = e_direction::HORIZONTAL ;
      break ;
    case 1:
      direction = e_direction::VERTICAL ;
      break ;
    case 2:
      direction = e_direction::BI_DIRECTIONNEL ;
      break ;
    case 3:
      direction = e_direction::MULTI_DIRECTIONNEL ;
      break ;
    default:
      direction = e_direction::BI_DIRECTIONNEL ;
      break ;
  }
}

void Option::set_seuil( unsigned int id )
{
  switch(id)
  {
    case 0:
      seuil = e_seuil::UNIQUE ;
      break ;
    case 1:
      seuil = e_seuil::GLOBAL ;
      break ;
    case 2:
      seuil = e_seuil::LOCAL ;
      break ;
    case 3:
      seuil = e_seuil::HYSTERESIS_AUTO ;
      break ;
    case 4:
      seuil = e_seuil::HYSTERESIS ;
      break ;
    default:
      seuil = e_seuil::HYSTERESIS ;
      break ;
  }
}

void Option::set_seuil_calcul( unsigned int id )
{
  switch(id)
  {
    case 0:
      seuil_calcul = e_type_calcul::MOYENNE ;
      break ;
    case 1:
      seuil_calcul = e_type_calcul::MEDIANE ;
      break ;
    case 2:
      seuil_calcul = e_type_calcul::ECART_TYPE ;
      break ;
    default:
      seuil_calcul = e_type_calcul::MOYENNE ;
      break ;
  }
}

void Option::set_lissage_type( unsigned int id )
{
  switch(id)
  {
    case 0:
      lissage_type = e_type_calcul::MOYENNE ;
      break ;
    case 1:
      lissage_type = e_type_calcul::MEDIANE ;
      break ;
    case 3:
      lissage_type = e_type_calcul::GAUSSIEN ;
      break ;
    default:
      lissage_type = e_type_calcul::MOYENNE ;
      break ;
  }
}