#include "filtre.hpp"

using namespace std ;


Filtre::Filtre() : cv::Mat(){}
Filtre::Filtre( const cv::Mat input ) : cv::Mat(input){}
Filtre::Filtre( const cv::Mat input, const std::string n ) : cv::Mat(input)
{
	name = n ;
}


void Filtre::prewitt()
{
	cv::Mat filtre = cv::Mat( 3, 3, CV_32FC1, 1.0f ) ;
	filtre.at<float>(0,0) = -1.0 ;
	filtre.at<float>(1,0) = -1.0 ;
	filtre.at<float>(2,0) = -1.0 ;
	filtre.at<float>(0,1) = 0.0 ;
	filtre.at<float>(1,1) = 0.0 ;
	filtre.at<float>(2,1) = 0.0 ;
	filtre.at<float>(0,2) = 1.0 ;
	filtre.at<float>(1,2) = 1.0 ;
	filtre.at<float>(2,2) = 1.0 ;

	(*this) = Filtre( filtre, "Prewitt" ) ;
}

void Filtre::sobel()
{
	cv::Mat filtre = cv::Mat( 3, 3, CV_32FC1, 1.0f ) ;
	filtre.at<float>(0,0) =  -1 ;
	filtre.at<float>(1,0) =  -2 ;
	filtre.at<float>(2,0) =  -1 ;
	filtre.at<float>(0,1) =  0 ;
	filtre.at<float>(1,1) =  0 ;
	filtre.at<float>(2,1) =  0 ;
	filtre.at<float>(0,2) =  1 ;
	filtre.at<float>(1,2) =  2 ;
	filtre.at<float>(2,2) =  1 ;

	(*this) = Filtre( filtre, "Sobel" ) ;
}

void Filtre::kirsch()
{
	cv::Mat filtre = cv::Mat( 3, 3, CV_32FC1, 1.0f ) ;
	filtre.at<float>(0,0) = -3.0 ;
	filtre.at<float>(1,0) = -3.0 ;
	filtre.at<float>(2,0) = -3.0 ;
	filtre.at<float>(0,1) = -3.0 ;
	filtre.at<float>(1,1) = 0.0 ;
	filtre.at<float>(2,1) = -3.0 ;
	filtre.at<float>(0,2) = 5.0 ;
	filtre.at<float>(1,2) = 5.0 ;
	filtre.at<float>(2,2) = 5.0 ;

	(*this) = Filtre( filtre, "Kirsch" ) ;
}

void Filtre::lapalacien( unsigned int size )
{
	cv::Mat filtre = cv::Mat( 5, 5, CV_32FC1, 1.0f ) ;
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

	(*this) = Filtre( filtre, "Lapalacien" ) ;
}

void Filtre::mean( unsigned int size )
{
	cv::Mat filtre = cv::Mat( size, size, CV_32F, 1.0f ) ;
	filtre = (1.0f / (size*size) ) ;

	(*this) = Filtre( filtre, "Moyenne" ) ;
}


void Filtre::gaussian( unsigned int size, float sigma )
{
	cv::Mat filtre = cv::Mat( size, size, CV_32FC1, 1.0f ) ;
	
	int rayon = size / 2 ;
	
	float factor = 0 ; 
	float e = 0 ; 

	for ( int i = -rayon; i <= rayon; ++i )
	{
		for ( int j = -rayon; j <= rayon; ++j )
		{
			e =  exp( - (i*i+j*j) / (2*sigma*sigma)) ; 
			filtre.at<float>(i+rayon, j+rayon) = e ;
			factor += e ;
		}
	}

	filtre = (filtre / factor ) ;

	(*this) = Filtre( filtre, "Gaussian" ) ;
}


Filtre Filtre::rotate45()
{
    Filtre res = (*this).clone() ;
    Filtre tmp = (*this).clone() ;

    unsigned int size = rows ;

    int taille = size-1;
    unsigned int nbAnneau = (unsigned int)(size/2);
        
        for(unsigned int k = 0 ; k < nbAnneau ; k++)
        {
            unsigned int l = 0 ;
            while( l < nbAnneau-k )
            {
                for (unsigned int i = k+1; i <= taille-k; ++i)
                {
                    res.at<float>(k,i) = tmp.at<float>(k, i-1); //haut
                    res.at<float>(i,taille-k) = tmp.at<float>(i-1,taille-k); //droite
                }
                for (unsigned int i = k; i <= (taille -1)-k; ++i)
                {
                    res.at<float>(taille-k,i) = tmp.at<float>(taille-k,i+1); //bas
                    res.at<float>(i,k) = tmp.at<float>(i+1, k); //gauche
                }
                l++ ;
                res.copyTo(tmp);
            }
        }

    res.at<float>((size/2),(size/2)) = at<float>((size/2),(size/2)); 

    return res ;
}

Filtre Filtre::rotate90()
{
    Filtre res = rotate45() ;
    return res.rotate45() ;
}