#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "option.hpp"

#include <cstdlib>
#include <cstring>
#include <opencv2/opencv.hpp>

#define ICI fprintf(stderr,"%s:%d\n",__FILE__,__LINE__)

class Image : public cv::Mat
{
        private :    

                cv::Mat rotate45( const cv::Mat& ) ;
                void seuil_unique( const Option ) ;
                void seuil_global( const Option ) ;
                void seuil_local( const Option ) ;
                void seuil_ecart_type( const Option ) ;
                void seuil_hysteresis( const Option ) ;

        public :

                Image() ;
                Image( const cv::Mat ) ;
                Image( const Image& ) ;
                ~Image() ;

                bool isRGB() ;
                Image toGray( unsigned int ) ;
                Image inverse() ;
                Image normalize() ;
                Image rotate90() ;
                Image rotate180() ;

                // FILTRES
                Image filtre_lissage( const Option ) ;
                Image filtre_differentiel( const Option ) ;
                Image fermeture( const Option ) ;
} ;

#endif // IMAGE_HPP