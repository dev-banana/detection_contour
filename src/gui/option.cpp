
#include "option.hpp"

Option::Option()
{  
  lissage_size = 3 ;
  lissage_type = MOYENNE ;

  filtre_size = 3 ;
  type_moyenne = EUCLIDIENNE ;
	direction = BI_DIRECTIONNEL ;

	seuil = HYSTERESIS ;
  seuil_type = MOYENNE ;
  seuil_val = 50 ;
  seuil_fenetre = 15 ;
	seuil_bas = 44 ;
  seuil_haut = 60 ;

	show_color = true ;
  keep_norme = true ;
  affine = true ;

	reset_filtre( 3 );
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
    case 4:
      seuil = e_seuil::HYSTERESIS_AUTO ;
      break ;
    case 5:
      seuil = e_seuil::HYSTERESIS ;
      break ;
    default:
      seuil = e_seuil::HYSTERESIS ;
      break ;
  }
}

void Option::set_seuil_type( unsigned int id )
{
  switch(id)
  {
    case 0:
      seuil_type = e_type_calcul::MOYENNE ;
      break ;
    case 1:
      seuil_type = e_type_calcul::ECART_TYPE ;
      break ;
    case 2:
      seuil_type = e_type_calcul::MEDIANE ;
      break ;
    case 3 :
      seuil_type = e_type_calcul::PASSE_BAS_3 ;
      break ;
    case 4 :
      seuil_type = e_type_calcul::PASSE_BAS_5 ;
      break ;
    default:
      seuil_type = e_type_calcul::MOYENNE ;
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
      lissage_type = e_type_calcul::ECART_TYPE ;
      break ;
    case 2:
      lissage_type = e_type_calcul::MEDIANE ;
      break ;
    case 3 :
      lissage_type = e_type_calcul::PASSE_BAS_3 ;
      break ;
    case 4 :
      lissage_type = e_type_calcul::PASSE_BAS_5 ;
      break ;
    default:
      lissage_type = e_type_calcul::MOYENNE ;
      break ;
  }
}

void Option::set_moyenne( unsigned int id )
{
  switch(id)
  {
    case 0:
      type_moyenne = e_moyenne::EUCLIDIENNE ;
      break ;
    case 1:
      type_moyenne = e_moyenne::MANHATTAN ;
      break ;
    case 2:
      type_moyenne = e_moyenne::ABSOLU ;
      break ;
    default:
      type_moyenne = e_moyenne::EUCLIDIENNE ;
      break ;
  }
}

void Option::reset_filtre( int size )
{
	filtre_size = size ;
	filtre = cv::Mat( size, size, CV_32FC1, 1.0f ) ;
}

void Option::prewitt()
{
  filtre.at<float>(0,0) = -1.0 ;
  filtre.at<float>(1,0) = -1.0 ;
  filtre.at<float>(2,0) = -1.0 ;
  filtre.at<float>(0,1) = 0.0 ;
  filtre.at<float>(1,1) = 0.0 ;
  filtre.at<float>(2,1) = 0.0 ;
  filtre.at<float>(0,2) = 1.0 ;
  filtre.at<float>(1,2) = 1.0 ;
  filtre.at<float>(2,2) = 1.0 ;
}
void Option::sobel()
{
  filtre.at<float>(0,0) =  -1 ;
  filtre.at<float>(1,0) =  -2 ;
  filtre.at<float>(2,0) =  -1 ;
  filtre.at<float>(0,1) =  0 ;
  filtre.at<float>(1,1) =  0 ;
  filtre.at<float>(2,1) =  0 ;
  filtre.at<float>(0,2) =  1 ;
  filtre.at<float>(1,2) =  2 ;
  filtre.at<float>(2,2) =  1 ;
}
void Option::kirsch()
{
  filtre.at<float>(0,0) = -3.0 ;
  filtre.at<float>(1,0) = -3.0 ;
  filtre.at<float>(2,0) = -3.0 ;
  filtre.at<float>(0,1) = -3.0 ;
  filtre.at<float>(1,1) = 0.0 ;
  filtre.at<float>(2,1) = -3.0 ;
  filtre.at<float>(0,2) = 5.0 ;
  filtre.at<float>(1,2) = 5.0 ;
  filtre.at<float>(2,2) = 5.0 ;
}
void Option::filtre_5()
{
  filtre.at<float>(0,0) = 0 ;
  filtre.at<float>(1,0) = 0 ;
  filtre.at<float>(2,0) = -1 ;
  filtre.at<float>(3,0) = 0 ;
  filtre.at<float>(4,0) = 0 ;
  filtre.at<float>(0,1) = 0 ;
  filtre.at<float>(1,1) = -1 ;
  filtre.at<float>(2,1) = -2 ;
  filtre.at<float>(3,1) = -1 ;
  filtre.at<float>(4,1) = 0 ;
  filtre.at<float>(0,2) = -1 ;
  filtre.at<float>(1,2) = -2 ;
  filtre.at<float>(2,2) = 17 ;
  filtre.at<float>(3,2) = -2 ;
  filtre.at<float>(4,2) = -1 ;
  filtre.at<float>(0,3) = 0 ;
  filtre.at<float>(1,3) = -1 ;
  filtre.at<float>(2,3) = -2 ;
  filtre.at<float>(3,3) = -1 ;
  filtre.at<float>(4,3) = 0 ;
  filtre.at<float>(0,4) = 0 ;
  filtre.at<float>(1,4) = 0 ;
  filtre.at<float>(2,4) = -1 ;
  filtre.at<float>(3,4) = 0 ;
  filtre.at<float>(4,4) = 0 ;
}