#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "bases.hpp"
#include "filtre.hpp"
#include "option.hpp"

#include <cstdlib>
#include <cstring>
#include <opencv2/opencv.hpp>

class Image : public cv::Mat
{
        private :    
                               
                void seuil_unique( const Option ) ;
                void seuil_global( const Option ) ;
                void seuil_local( const Option ) ;
                void seuil_ecart_type( const Option ) ;
                void seuil_hysteresis( const Option ) ;

                Image simple_convolution( const Filtre ) ;
                Image mediane( int ) ;

                float normes_mediane( int, int, int, int ) ;
                void normes_stats( float &, float &, int, int, int, int ) ;
        public :

                std::vector<std::vector<float>> dirs ;
                std::vector<std::vector<float>> normes ;
                std::vector<std::vector<float>> gradients ;
                
                Image() ;
                Image( const cv::Mat ) ;
                Image( const Image& ) ;
                ~Image() ;
                Image clone() ;

                bool isRGB() ;
                Image toGray( unsigned int ) ;
                Image inverse() ;
                Image normalize() ;
                Image rotate90() ;
                Image rotate180() ;


                // FILTRES
                Image lissage( const Option ) ;
                Image filtre_differentiel( const Option ) ;
                Image seuillage( const Option ) ;
                Image affinage( const Option ) ;
                Image fermeture( const Option ) ;

                Image detection_contour( const Option ) ;

                Image color_direction( const Option ) ;
} ;

#endif // IMAGE_HPP