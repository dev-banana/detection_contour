
#include "option.hpp"

Option::Option()
{ 
  lissage_type = GAUSSIEN ;
  lissage_size = 7 ;
  lissage_sigma = 5 ;

  filtre.prewitt();
  direction = MULTI_DIRECTIONNEL ;
  type_norme = EUCLIDIENNE ;

  seuil = HYSTERESIS ;
  seuil_calcul = MOYENNE ;
  seuil_val = 50 ;
  seuil_fenetre = 15 ;
  seuil_bas = 44 ;
  seuil_haut = 60 ;

  fermeture_size = 2 ;
  fermeture_seuil = 15 ;
  
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

void Option::set_type_norme( unsigned int id )
{
  switch(id)
  {
    case 0:
      type_norme = e_type_norme::EUCLIDIENNE ;
      break ;
    case 1:
      type_norme = e_type_norme::ABSOLUE ;
      break ;
    case 2:
      type_norme = e_type_norme::MAX ;
      break ;
    default:
      type_norme = e_type_norme::EUCLIDIENNE ;
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