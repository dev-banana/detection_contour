#ifndef FILTRE_HPP
#define FILTRE_HPP

#include <cstdlib>
#include <cstring>
#include <opencv2/opencv.hpp>

#include "bases.hpp"

class Filtre : public cv::Mat
{
        private : 
        public :

                std::string name ;
                
                Filtre() ;
                Filtre( const cv::Mat ) ;
                Filtre( const cv::Mat, const std::string ) ;
        	~Filtre(){}

                Filtre rotate45() ;
        	Filtre rotate90() ;

        	void prewitt() ;
        	void sobel() ;
        	void kirsch() ;
        	void lapalacien( unsigned int ) ;

        	void mean( unsigned int ) ;
        	void median( unsigned int ) ;
        	void gaussian( unsigned int, float ) ;
} ;

#endif // FILTRE_HPP