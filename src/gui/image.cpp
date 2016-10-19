#include "image.hpp"

using namespace std ;


Image::Image() : cv::Mat(){
    unsigned int size = cols*rows ;

    dirs = (float *)malloc( size * sizeof(float) ) ;
    gradients = (float *)malloc( size * sizeof(float) ) ;
    normes = (float *)malloc( size * sizeof(float) ) ;

    for ( unsigned int i = 0; i < size; i++)
    {
        dirs[i] = 0 ;
        gradients[i] = 0 ;
        normes[i] = 0 ;
    }
}
Image::Image( const cv::Mat input ) : cv::Mat(input){
    unsigned int size = cols*rows ;

    dirs = (float *)malloc( size * sizeof(float) ) ;
    gradients = (float *)malloc( size * sizeof(float) ) ;
    normes = (float *)malloc( size * sizeof(float) ) ;

    for ( unsigned int i = 0; i < size; i++)
    {
        dirs[i] = 0 ;
        gradients[i] = 0 ;
        normes[i] = 0 ;
    }
}
Image::Image(const Image& im) : cv::Mat(im){
    unsigned int size = cols*rows ;

    dirs = (float *)malloc( size * sizeof(float) ) ;
    gradients = (float *)malloc( size * sizeof(float) ) ;
    normes = (float *)malloc( size * sizeof(float) ) ;

    for ( unsigned int i = 0; i < size; i++)
    {
        dirs[i] = 0 ;
        gradients[i] = 0 ;
        normes[i] = 0 ;
    }
}
Image::~Image(){}

Image Image::copy()
{
    Image res = (*this).clone() ;

    unsigned int size = cols*rows ;

    res.dirs = (float *)malloc( size * sizeof(float) ) ;
    res.gradients = (float *)malloc( size * sizeof(float) ) ;
    res.normes = (float *)malloc( size * sizeof(float) ) ;

    for ( unsigned int i = 0; i < size; i++)
    {
        res.dirs[i] = dirs[i] ;
        res.gradients[i] = gradients[i] ;
        res.normes[i] = normes[i] ;
    }

    return res ;
}


Glib::RefPtr<Gdk::Pixbuf> Image::toGtk( int sizeMax )
{
    if( !imgGtk || imgGtk->Gdk::Pixbuf::get_width () != sizeMax )
    {
        IplImage* ipl_img;
        ipl_img = cvCreateImage(cvSize(cols,rows),8,3);
        IplImage ipltemp=*this;
        cvCopy(&ipltemp,ipl_img);


        cvCvtColor(ipl_img, ipl_img, CV_BGR2RGB);

        Glib::RefPtr<Gdk::Pixbuf> px = Gdk::Pixbuf::create_from_data(
                        (guint8*)ipl_img->imageData,
                        Gdk::COLORSPACE_RGB,
                        false,
                        ipl_img->depth,
                        ipl_img->width,
                        ipl_img->height,
                        ipl_img->widthStep) ;


        int height = sizeMax * ipl_img->height / ipl_img->width ;
        imgGtk = px->scale_simple (
            sizeMax,
            height,
            Gdk::INTERP_BILINEAR
        );
    }
    return imgGtk ;
}

bool Image::isRGB()
{
    float threshold = 0.006 ;

    uint8_t r,g ;

    uint8_t* pixelPtr = (uint8_t*)data ;

    int cn = channels() ;

    for(int i = 0 ; i < rows ; i++ )
    {
        for (int j = 0; j < cols; ++j)
        {
            // b = pixelPtr[i*cols*cn + j*cn + 0] ;         // B
            g = pixelPtr[i*cols*cn + j*cn + 1] ;         // G
            r = pixelPtr[i*cols*cn + j*cn + 2] ;         // R

            if( abs( r-g ) >= threshold )
            {
                return true ;
            }
        } 
    }
    return false ;
}

Image Image::toGray( unsigned int mode)
{
    Image res = copy() ;

    for(int i = 0 ; i < rows ; i++ )
    {
        for (int j = 0; j < cols; ++j)
        {
            double intensite = 0 ;
            double b = res.at<cv::Vec3b>(i,j)[0] ;         // B
            double g = res.at<cv::Vec3b>(i,j)[1] ;         // G
            double r = res.at<cv::Vec3b>(i,j)[2] ;         // R
            switch( mode )
            {
                case 1 : //clarté
                    intensite = 0.5*( std::max(r,std::max(g,b)) + std::min(r,std::min(g,b)) ) ;
                    break ;
                case 2 : //luminosité
                    intensite = 0.21 * r + 0.71 * g + 0.07 * b ;
                    break ;
                case 3 : //moyenne
                    intensite = (r+g+b)/3 ;
                    break ;
                default :
                    intensite = (r+g+b)/3 ;
                    break ;
            }

            res.at<cv::Vec3b>(i,j)[0] = intensite ;
            res.at<cv::Vec3b>(i,j)[1] = intensite ;
            res.at<cv::Vec3b>(i,j)[2] = intensite ;
        } 
    }

    return res ;
}


Image Image::inverse()
{
    Image res = copy() ;

    for(int i = 0 ; i < rows ; i++ )
    {
        for (int j = 0; j < cols; ++j)
        {
            res.at<cv::Vec3b>(i,j)[0] = 255-at<cv::Vec3b>(i,j)[0];
            res.at<cv::Vec3b>(i,j)[1] = 255-at<cv::Vec3b>(i,j)[1];
            res.at<cv::Vec3b>(i,j)[2] = 255-at<cv::Vec3b>(i,j)[2];
        } 
    }

    return res ;
}

Image Image::normalize()
{
    Image res = copy() ;

    float min = 255 ;
    float max = 0 ;

    for(int i = 0 ; i < rows ; i++ )
    {
        for (int j = 0; j < cols; ++j)
        {
            if( at<cv::Vec3b>(i,j)[0] < min )
                min = at<cv::Vec3b>(i,j)[0] ;

            if( at<cv::Vec3b>(i,j)[0] > max )
                max = at<cv::Vec3b>(i,j)[0] ;
        } 
    }

    for(int i = 0 ; i < rows ; i++ )
    {
        for (int j = 0; j < cols; ++j)
        {
            float y = (( at<cv::Vec3b>(i,j)[0] -min)*255/(max-min)) ;
            res.at<cv::Vec3b>(i,j)[0] = y ;
            res.at<cv::Vec3b>(i,j)[1] = y ;
            res.at<cv::Vec3b>(i,j)[2] = y ;
        } 
    }

    return res ;    
}

cv::Mat Image::rotate45( const cv::Mat& mat ) //mat 3*3
{
    cv::Mat res = mat.clone() ;
    cv::Mat tmp = mat.clone() ;

    unsigned int size = mat.rows ;

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

    res.at<float>((size/2),(size/2)) = mat.at<float>((size/2),(size/2)); 
    return res ;
}

Image Image::rotate90()
{
    Image res = cv::Mat( cols, rows, cv::DataType<cv::Vec3b>::type ) ;

    int size = cols*rows ;
    res.dirs = (float *)malloc( size * sizeof(float) ) ;
    res.gradients = (float *)malloc( size * sizeof(float) ) ;
    res.normes = (float *)malloc( size * sizeof(float) ) ;

    for ( int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            
            res.at<cv::Vec3b>(j, cols-1-i) = at<cv::Vec3b>(i, j) ;

            int index = (i*cols) + j ;
            res.dirs[index] = 0 ;
            res.gradients[index] = 0 ;
            res.normes[index] = 0 ;
        }
    }

    return res ;
}

Image Image::rotate180()
{
    Image res = cv::Mat( rows, cols, cv::DataType<cv::Vec3b>::type ) ;

    int size = cols*rows ;
    res.dirs = (float *)malloc( size * sizeof(float) ) ;
    res.gradients = (float *)malloc( size * sizeof(float) ) ;

    for ( int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            
            res.at<cv::Vec3b>(rows-1-i, cols-1-j) = at<cv::Vec3b>(i, j) ;

            int index = (i*cols) + j ;
            res.dirs[index] = 0 ;
            res.gradients[index] = 0 ;
        }
    }

    return res ;
}

Image Image::filtre_lissage( const Option option )
{
    Mat filtre ;

    if( option.lissage_type == PASSE_BAS_3 ){
        filtre = cv::Mat( option.lissage_size, option.lissage_size, CV_32F, 1.0f ) ;
        filtre.at<float>(0,0) =  1 ;
        filtre.at<float>(1,0) =  2 ;
        filtre.at<float>(2,0) =  1 ;
        filtre.at<float>(0,1) =  2 ;
        filtre.at<float>(1,1) =  4 ;
        filtre.at<float>(2,1) =  2 ;
        filtre.at<float>(0,2) =  1 ;
        filtre.at<float>(1,2) =  2 ;
        filtre.at<float>(2,2) =  1 ;
    }
    else if( option.lissage_type == PASSE_BAS_5 ){
        filtre = cv::Mat( 5, 5, CV_32F, 1.0f ) ;
        filtre.at<float>(0,0) = 1 ;
        filtre.at<float>(1,0) = 1 ;
        filtre.at<float>(2,0) = 1 ;
        filtre.at<float>(3,0) = 1 ;
        filtre.at<float>(4,0) = 1 ;
        filtre.at<float>(0,1) = 1 ;
        filtre.at<float>(1,1) = 4 ;
        filtre.at<float>(2,1) = 4 ;
        filtre.at<float>(3,1) = 4 ;
        filtre.at<float>(4,1) = 1 ;
        filtre.at<float>(0,2) = 1 ;
        filtre.at<float>(1,2) = 4 ;
        filtre.at<float>(2,2) = 12 ;
        filtre.at<float>(3,2) = 4 ;
        filtre.at<float>(4,2) = 1 ;
        filtre.at<float>(0,3) = 1 ;
        filtre.at<float>(1,3) = 4 ;
        filtre.at<float>(2,3) = 4 ;
        filtre.at<float>(3,3) = 4 ;
        filtre.at<float>(4,3) = 1 ;
        filtre.at<float>(0,4) = 1 ;
        filtre.at<float>(1,4) = 1 ;
        filtre.at<float>(2,4) = 1 ;
        filtre.at<float>(3,4) = 1 ;
        filtre.at<float>(4,4) = 1 ;
    }
    else{
        filtre = cv::Mat( option.lissage_size, option.lissage_size, CV_32F, 1.0f ) ;
        filtre = (1.0f / (option.lissage_size*option.lissage_size) ) ;
    }

    Image res = copy() ;

    // #pragma omp parallel for
    for( int i = filtre.rows/2; i<rows - filtre.rows/2; i++ )
    {
        for ( int j=filtre.cols/2; j<cols - filtre.cols/2; j++ )
        {
            double sommeB = 0 ;
            double sommeG = 0 ;
            double sommeR = 0 ;
            for( int k=-filtre.rows/2; k<=filtre.rows/2; k++ )
            {
                for( int l=-filtre.cols/2; l<=filtre.cols/2; l++ )
                {

                    sommeR += at<cv::Vec3b>(i+k,j+l)[0]*filtre.at<float>(k+filtre.rows/2,l+filtre.cols/2) ; 
                    sommeG += at<cv::Vec3b>(i+k,j+l)[1]*filtre.at<float>(k+filtre.rows/2,l+filtre.cols/2) ; 
                    sommeB += at<cv::Vec3b>(i+k,j+l)[2]*filtre.at<float>(k+filtre.rows/2,l+filtre.cols/2) ; 
                } 
            }
            res.at<cv::Vec3b>(i,j)[0] = sommeR ;
            res.at<cv::Vec3b>(i,j)[1] = sommeG ;
            res.at<cv::Vec3b>(i,j)[2] = sommeB ;
        }
    }

    return res ;
}

void Image::Vfiltre_lissage( const Option option )
{
    Mat filtre ;

    if( option.lissage_type == PASSE_BAS_3 ){
        filtre = cv::Mat( option.lissage_size, option.lissage_size, CV_32F, 1.0f ) ;
        filtre.at<float>(0,0) =  1 ;
        filtre.at<float>(1,0) =  2 ;
        filtre.at<float>(2,0) =  1 ;
        filtre.at<float>(0,1) =  2 ;
        filtre.at<float>(1,1) =  4 ;
        filtre.at<float>(2,1) =  2 ;
        filtre.at<float>(0,2) =  1 ;
        filtre.at<float>(1,2) =  2 ;
        filtre.at<float>(2,2) =  1 ;
    }
    else if( option.lissage_type == PASSE_BAS_5 ){
        filtre = cv::Mat( 5, 5, CV_32F, 1.0f ) ;
        filtre.at<float>(0,0) = 1 ;
        filtre.at<float>(1,0) = 1 ;
        filtre.at<float>(2,0) = 1 ;
        filtre.at<float>(3,0) = 1 ;
        filtre.at<float>(4,0) = 1 ;
        filtre.at<float>(0,1) = 1 ;
        filtre.at<float>(1,1) = 4 ;
        filtre.at<float>(2,1) = 4 ;
        filtre.at<float>(3,1) = 4 ;
        filtre.at<float>(4,1) = 1 ;
        filtre.at<float>(0,2) = 1 ;
        filtre.at<float>(1,2) = 4 ;
        filtre.at<float>(2,2) = 12 ;
        filtre.at<float>(3,2) = 4 ;
        filtre.at<float>(4,2) = 1 ;
        filtre.at<float>(0,3) = 1 ;
        filtre.at<float>(1,3) = 4 ;
        filtre.at<float>(2,3) = 4 ;
        filtre.at<float>(3,3) = 4 ;
        filtre.at<float>(4,3) = 1 ;
        filtre.at<float>(0,4) = 1 ;
        filtre.at<float>(1,4) = 1 ;
        filtre.at<float>(2,4) = 1 ;
        filtre.at<float>(3,4) = 1 ;
        filtre.at<float>(4,4) = 1 ;
    }
    else{
        filtre = cv::Mat( option.lissage_size, option.lissage_size, CV_32F, 1.0f ) ;
        filtre = (1.0f / (option.lissage_size*option.lissage_size) ) ;
    }

    Image tmp = copy() ;

    // #pragma omp parallel for
    for( int i = filtre.rows/2; i<rows - filtre.rows/2; i++ )
    {
        for ( int j=filtre.cols/2; j<cols - filtre.cols/2; j++ )
        {
            double sommeB = 0 ;
            double sommeG = 0 ;
            double sommeR = 0 ;
            for( int k=-filtre.rows/2; k<=filtre.rows/2; k++ )
            {
                for( int l=-filtre.cols/2; l<=filtre.cols/2; l++ )
                {

                    sommeR += tmp.at<cv::Vec3b>(i+k,j+l)[0]*filtre.at<float>(k+filtre.rows/2,l+filtre.cols/2) ; 
                    sommeG += tmp.at<cv::Vec3b>(i+k,j+l)[1]*filtre.at<float>(k+filtre.rows/2,l+filtre.cols/2) ; 
                    sommeB += tmp.at<cv::Vec3b>(i+k,j+l)[2]*filtre.at<float>(k+filtre.rows/2,l+filtre.cols/2) ; 
                } 
            }
            at<cv::Vec3b>(i,j)[0] = sommeR ;
            at<cv::Vec3b>(i,j)[1] = sommeG ;
            at<cv::Vec3b>(i,j)[2] = sommeB ;
        }
    }
}

Image Image::filtre_differentiel( const Option option )
{   
    (*this) = toGray( 3 ) ;

    Image res = copy() ;

    res.dirs = (float*)malloc((rows*cols)*sizeof(float));
    res.gradients = (float*)malloc((rows*cols)*sizeof(float));
    
    cv::Mat tabFiltre[8] ;
    switch( option.direction )
    {
        case HORIZONTAL :
            tabFiltre[0] = rotate45( option.filtre ) ;
            tabFiltre[0] = rotate45( tabFiltre[0] ) ;
            break ;
        case VERTICAL :
            tabFiltre[0] = option.filtre.clone() ;
            break ;
        case BI_DIRECTIONNEL :
            tabFiltre[0] = option.filtre.clone() ;
            tabFiltre[1] = rotate45( option.filtre ) ;
            tabFiltre[1] = rotate45( tabFiltre[1] ) ;
            break ;
        case MULTI_DIRECTIONNEL :
            tabFiltre[0] = option.filtre.clone() ;
            tabFiltre[1] = rotate45( option.filtre ) ;
            tabFiltre[2] = rotate45( tabFiltre[1] ) ;
            tabFiltre[3] = rotate45( tabFiltre[2] ) ;
            tabFiltre[4] = rotate45( tabFiltre[3] ) ;
            tabFiltre[5] = rotate45( tabFiltre[4] ) ;
            tabFiltre[6] = rotate45( tabFiltre[5] ) ;
            tabFiltre[7] = rotate45( tabFiltre[6] ) ;
            break ;
        default :
            break ;
    }
    
    // #pragma omp parallel for
    for( int i = option.filtre_size/2; i<rows - option.filtre_size/2; i++ )
    {
        for ( int j=option.filtre_size/2; j<cols - option.filtre_size/2; j++ )
        {
            double gradient, gradient1, gradient2, gradient3, gradient4, gradient5, gradient6, gradient7 ;
            gradient = gradient1 = gradient2 = gradient3 = gradient4 = gradient5 = gradient6 = gradient7 = 0 ;

            for( int k = -option.filtre_size/2; k <= option.filtre_size/2; k++ )
            {
                for( int l = -option.filtre_size/2; l <= option.filtre_size/2; l++ )
                {
                    switch( option.direction )
                    {
                        case HORIZONTAL : case VERTICAL :
                            gradient += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[0].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ; 
                            break ;
                        case BI_DIRECTIONNEL :    
                            gradient += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[0].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ; 
                            gradient1 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[1].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ;  
                            break ;
                        case MULTI_DIRECTIONNEL :
                            gradient += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[0].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ; 
                            gradient1 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[1].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ;  
                            gradient2 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[2].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ; 
                            gradient3 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[3].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ;  
                            gradient4 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[4].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ;  
                            gradient5 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[5].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ;  
                            gradient6 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[6].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ;  
                            gradient7 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[7].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ;  
                            break ;
                        default :
                            break ;
                    }
                } 
            }

            float norme = 0 ;
            float dir = 0 ;
            float angle = 0 ;

            switch( option.direction )
            {
                case HORIZONTAL : 
                    norme = gradient ;
                    dir = 90 ;
                    break ;
                case VERTICAL :
                    norme = gradient ;
                    dir = 0 ;
                    break ;
                case BI_DIRECTIONNEL :
                    norme = sqrt((gradient*gradient)+(gradient1*gradient1)) ;
                    angle = atan2( gradient1, gradient ); //return angle radian
                    dir =  angle * (180.0 / M_PI); //degrés
                    dir = (dir > 0.0 ? dir : (360.0 + dir)); // correction
                    break ;
                case MULTI_DIRECTIONNEL :
                    norme = std::max( gradient, std::max( gradient1, std::max( gradient2, std::max( gradient3, std::max( gradient4, std::max( gradient5, std::max( gradient6, gradient7 ) ) ) ) ) ) ) ;
                    if( norme == gradient ) dir = 0*45 ;
                    else if( norme == gradient1 ) dir = 1*45 ;
                    else if( norme == gradient2 ) dir = 2*45 ;
                    else if( norme == gradient3 ) dir = 3*45 ;
                    else if( norme == gradient4 ) dir = 4*45 ;
                    else if( norme == gradient5 ) dir = 5*45 ;
                    else if( norme == gradient6 ) dir = 6*45 ;
                    else if( norme == gradient7 ) dir = 7*45 ;
                    break ;
                default :
                    break ;
            }

            res.at<cv::Vec3b>(i,j)[0] = norme ;
            res.at<cv::Vec3b>(i,j)[1] = norme ;
            res.at<cv::Vec3b>(i,j)[2] = norme ;

            int index = (i*cols) + j ;
            res.dirs[ index ] = dir ;
            res.gradients[ index ] = gradient ;
            res.normes[ index ] = norme ;
        }
    }
      
    
    //COLOR DIRECTION
    if(option.show_color) // faire varier les couleurs
    {
        res.color( option ) ;
    }

    return res ;
}

void Image::Vfiltre_differentiel( const Option option )
{   
    (*this) = toGray( 3 ) ;

    Image tmp = copy() ;
    
    cv::Mat tabFiltre[8] ;
    switch( option.direction )
    {
        case HORIZONTAL :
            tabFiltre[0] = rotate45( option.filtre ) ;
            tabFiltre[0] = rotate45( tabFiltre[0] ) ;
            break ;
        case VERTICAL :
            tabFiltre[0] = option.filtre.clone() ;
            break ;
        case BI_DIRECTIONNEL :
            tabFiltre[0] = option.filtre.clone() ;
            tabFiltre[1] = rotate45( option.filtre ) ;
            tabFiltre[1] = rotate45( tabFiltre[1] ) ;
            break ;
        case MULTI_DIRECTIONNEL :
            tabFiltre[0] = option.filtre.clone() ;
            tabFiltre[1] = rotate45( option.filtre ) ;
            tabFiltre[2] = rotate45( tabFiltre[1] ) ;
            tabFiltre[3] = rotate45( tabFiltre[2] ) ;
            tabFiltre[4] = rotate45( tabFiltre[3] ) ;
            tabFiltre[5] = rotate45( tabFiltre[4] ) ;
            tabFiltre[6] = rotate45( tabFiltre[5] ) ;
            tabFiltre[7] = rotate45( tabFiltre[6] ) ;
            break ;
        default :
            break ;
    }
    
    // #pragma omp parallel for
    for( int i = option.filtre_size/2; i<rows - option.filtre_size/2; i++ )
    {
        for ( int j=option.filtre_size/2; j<cols - option.filtre_size/2; j++ )
        {
            double gradient, gradient1, gradient2, gradient3, gradient4, gradient5, gradient6, gradient7 ;
            gradient = gradient1 = gradient2 = gradient3 = gradient4 = gradient5 = gradient6 = gradient7 = 0 ;

            for( int k = -option.filtre_size/2; k <= option.filtre_size/2; k++ )
            {
                for( int l = -option.filtre_size/2; l <= option.filtre_size/2; l++ )
                {
                    switch( option.direction )
                    {
                        case HORIZONTAL : case VERTICAL :
                            gradient += tmp.at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[0].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ; 
                            break ;
                        case BI_DIRECTIONNEL :    
                            gradient += tmp.at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[0].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ; 
                            gradient1 += tmp.at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[1].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ;  
                            break ;
                        case MULTI_DIRECTIONNEL :
                            gradient += tmp.at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[0].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ; 
                            gradient1 += tmp.at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[1].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ;  
                            gradient2 += tmp.at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[2].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ; 
                            gradient3 += tmp.at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[3].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ;  
                            gradient4 += tmp.at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[4].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ;  
                            gradient5 += tmp.at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[5].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ;  
                            gradient6 += tmp.at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[6].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ;  
                            gradient7 += tmp.at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[7].at<float>(k+option.filtre_size/2,l+option.filtre_size/2) ;  
                            break ;
                        default :
                            break ;
                    }
                } 
            }

            float norme = 0 ;
            float dir = 0 ;
            float angle = 0 ;

            switch( option.direction )
            {
                case HORIZONTAL : 
                    norme = gradient ;
                    dir = 90 ;
                    break ;
                case VERTICAL :
                    norme = gradient ;
                    dir = 0 ;
                    break ;
                case BI_DIRECTIONNEL :
                    norme = sqrt((gradient*gradient)+(gradient1*gradient1)) ;
                    angle = atan2( gradient1, gradient ); //return angle radian
                    dir =  angle * (180.0 / M_PI); //degrés
                    dir = (dir > 0.0 ? dir : (360.0 + dir)); // correction
                    break ;
                case MULTI_DIRECTIONNEL :
                    norme = std::max( gradient, std::max( gradient1, std::max( gradient2, std::max( gradient3, std::max( gradient4, std::max( gradient5, std::max( gradient6, gradient7 ) ) ) ) ) ) ) ;
                    if( norme == gradient ) dir = 0*45 ;
                    else if( norme == gradient1 ) dir = 1*45 ;
                    else if( norme == gradient2 ) dir = 2*45 ;
                    else if( norme == gradient3 ) dir = 3*45 ;
                    else if( norme == gradient4 ) dir = 4*45 ;
                    else if( norme == gradient5 ) dir = 5*45 ;
                    else if( norme == gradient6 ) dir = 6*45 ;
                    else if( norme == gradient7 ) dir = 7*45 ;
                    break ;
                default :
                    break ;
            }

            at<cv::Vec3b>(i,j)[0] = norme ;
            at<cv::Vec3b>(i,j)[1] = norme ;
            at<cv::Vec3b>(i,j)[2] = norme ;

            int index = (i*cols) + j ;
            dirs[ index ] = dir ;
            gradients[ index ] = gradient ;
            normes[ index ] = norme ;
        }
    }
}

Image Image::filtre_seuillage( const Option option )
{   
    Image res = (*this).clone() ;

    //SEUILLAGE
    switch( option.seuil )
    {
        case UNIQUE : 
            res.seuil_unique( option ) ;
            break ;
        case GLOBAL :
            res.seuil_global( option ) ;
            break ;
        case LOCAL :
            res.seuil_local( option ) ;
            break ;
        case HYSTERESIS_AUTO : case HYSTERESIS :
            res.seuil_hysteresis( option ) ;
            break ;
        default :
            res.seuil_unique( option ) ;
            break ;
    }

    //COLOR DIRECTION
    if(option.show_color) // faire varier les couleurs
    {
        res.color( option ) ;
    }    

    return res ;
}

void Image::Vfiltre_seuillage( const Option option )
{   
    //SEUILLAGE
    switch( option.seuil )
    {
        case UNIQUE : 
            seuil_unique( option ) ;
            break ;
        case GLOBAL :
            seuil_global( option ) ;
            break ;
        case LOCAL :
            seuil_local( option ) ;
            break ;
        case HYSTERESIS_AUTO : case HYSTERESIS :
            seuil_hysteresis( option ) ;
            break ;
        default :
            seuil_unique( option ) ;
            break ;
    } 
}


void Image::seuil_unique( Option option )
{
    for( int i = 0; i < rows ; i++ )
    {
        for ( int j= 0; j < cols ; j++ )
        {
            int index = (i*cols) + j ;
            if( normes[index] < option.seuil_val ) //SEUILLAGE
            {
                at<cv::Vec3b>(i,j)[0] = 0 ;
                at<cv::Vec3b>(i,j)[1] = 0 ;
                at<cv::Vec3b>(i,j)[2] = 0 ;
            }
        }
    }    
}

void Image::seuil_global( Option option )
{
    cv::Vec<double,4> tempMoyenne ;
    cv::Vec<double,4> tempStd ;
    meanStdDev( *(this), tempMoyenne, tempStd );
    float seuil ;

    if( option.seuil_type == MOYENNE )
        seuil = tempMoyenne.val[0] ;
    else if( option.seuil_type == ECART_TYPE )
        seuil = tempStd.val[0] ;
    else
        seuil = tempMoyenne.val[0] ;

    for( int i = 0; i < rows ; i++ )
    {
        for ( int j= 0; j < cols ; j++ )
        { 
            int index = (i*cols) + j ;
            if( normes[index] < seuil ) //SEUILLAGE
            {
                at<cv::Vec3b>(i,j)[0] = 0 ;
                at<cv::Vec3b>(i,j)[1] = 0 ;
                at<cv::Vec3b>(i,j)[2] = 0 ;
            }
        }
    } 
}

void Image::seuil_local( Option option )
{
    Image temp = (*this).clone() ;
    for( unsigned int i = option.seuil_fenetre/2; i < rows-option.seuil_fenetre/2 ; i++ )
    {
        for ( unsigned int j= option.seuil_fenetre/2; j < cols-option.seuil_fenetre/2 ; j++ )
        { 
            int somme = 0 ;
            double moyenne = 0 ;
            for( unsigned int k = i-option.seuil_fenetre/2; k < i+option.seuil_fenetre/2 ; k++ )
            {
                for ( unsigned int l= j-option.seuil_fenetre/2; l < j+option.seuil_fenetre/2 ; l++ )
                {
                    somme += temp.at<cv::Vec3b>(k,l)[0] ;
                }
            }
            moyenne = somme / (option.seuil_fenetre*option.seuil_fenetre) ;
            
            int index = (i*cols) + j ;
            if( normes[index] < moyenne ) //SEUILLAGE
            {
                at<cv::Vec3b>(i,j)[0] = 0 ;
                at<cv::Vec3b>(i,j)[1] = 0 ;
                at<cv::Vec3b>(i,j)[2] = 0 ;
            }
        }
    } 
}

void Image::seuil_hysteresis( Option option)
{
    Image temp = (*this).clone() ;
    cv::Vec<double,4> tempMoyenne ;
    cv::Vec<double,4> tempStd ;

    meanStdDev( *(this), tempMoyenne, tempStd );

    float moyenne = tempMoyenne.val[0] ;
    float ecart_type = tempStd.val[0] ;

    int bas ;
    int haut ;

    if( option.seuil == HYSTERESIS_AUTO )
    {
        bas = moyenne - ecart_type ;
        haut = moyenne + ecart_type ;
    }
    else
    {
        bas = option.seuil_bas ;
        haut = option.seuil_haut ;
    }

    if( haut > 255 )
        haut = 255 ;
    if( bas < 0 )
        bas = 255 ;

    for( unsigned int i = option.seuil_fenetre/2; i < rows-option.seuil_fenetre/2 ; i++ )
    {
        for ( unsigned int j= option.seuil_fenetre/2; j < cols-option.seuil_fenetre/2 ; j++ )
        { 
            int index = (i*cols) + j ;

            if( normes[index] > haut )
            {
                // if(!option.keep_norme){
                //     at<cv::Vec3b>(i,j)[0] = 255 ;
                //     at<cv::Vec3b>(i,j)[1] = 255 ;
                //     at<cv::Vec3b>(i,j)[2] = 255 ;
                // }
            }
            else if( normes[index] < bas )
            {
                at<cv::Vec3b>(i,j)[0] = 0 ;
                at<cv::Vec3b>(i,j)[1] = 0 ;
                at<cv::Vec3b>(i,j)[2] = 0 ;
            }
            else
            {
                at<cv::Vec3b>(i,j)[0] = 0 ;
                at<cv::Vec3b>(i,j)[1] = 0 ;
                at<cv::Vec3b>(i,j)[2] = 0 ;
                for( unsigned int k = i-option.seuil_fenetre/2; k < i+option.seuil_fenetre/2 ; k++ )
                {
                    for ( unsigned int l= j-option.seuil_fenetre/2; l < j+option.seuil_fenetre/2 ; l++ )
                    {
                        int index2 = (k*cols) + l ;
                        if( normes[index2] > haut ) //SEUILLAGE
                        {
                            // if(option.keep_norme)
                            // {
                                at<cv::Vec3b>(i,j)[0] = temp.at<cv::Vec3b>(i,j)[0] ;
                                at<cv::Vec3b>(i,j)[1] = temp.at<cv::Vec3b>(i,j)[0] ;
                                at<cv::Vec3b>(i,j)[2] = temp.at<cv::Vec3b>(i,j)[0] ;
                            // }
                            // else
                            // {
                            //     at<cv::Vec3b>(i,j)[0] = 255 ;
                            //     at<cv::Vec3b>(i,j)[1] = 255 ;
                            //     at<cv::Vec3b>(i,j)[2] = 255 ;
                            // }
                            k = i+option.seuil_fenetre/2 ;
                            break ;
                        }
                    }
                }
            }
        }
    }     
}


Image Image::filtre_affinage( const Option option )
{   
    Image res = (*this).clone() ;

    //AFFINAGE
    for (int i = 1; i < rows-1; ++i)
    {
        for (int j = 1; j < cols-1; ++j)
        {
            if( res.at<cv::Vec3b>(i,j)[0] != 0 )
            {                
                float dir = dirs[ (i*cols) + j ] ;
                int aI, aJ;
                int bI, bJ;

                if( dir <= 1*45 )
                {
                    aI = 0 ;
                    aJ = -1 ;
                    bI = 0 ;
                    bJ = 1 ;
                }
                else if( dir <= 2*45 )
                {
                    aI = -1;
                    aJ = -1 ;
                    bI = 1 ;
                    bJ = 1 ;

                }
                else if( dir <= 3*45 )
                {
                    aI = -1 ;
                    aJ = 0 ;
                    bI = 1 ;
                    bJ = 0 ;
                }
                else if( dir <= 4*45 )
                {
                    aI = 1 ;
                    aJ = -1 ;
                    bI = -1 ;
                    bJ = 1 ;
                }
                else if( dir <= 5*45 )
                {
                    aI = 0 ;
                    aJ = 1 ;
                    bI = 0 ;
                    bJ = -1 ;
                }
                else if( dir <= 6*45 )
                {
                    aI = 1 ;
                    aJ = 1 ;
                    bI = -1 ;
                    bJ = -1 ;
                }
                else if( dir <= 7*45 )
                {
                    aI = 0 ;
                    aJ = 1 ;
                    bI = 0 ;
                    bJ = -1 ;
                }
                else
                {
                    aI = 1 ;
                    aJ = -1 ;
                    bI = -1 ;
                    bJ = 1 ;
                } 

                if(res.at<cv::Vec3b>(i+aI,j+aJ)[0] != 0 && res.at<cv::Vec3b>(i+bI,j+bJ)[0] != 0)
                {
                    if(res.at<cv::Vec3b>(i,j)[0] >= res.at<cv::Vec3b>(i+aI,j+aJ)[0] && res.at<cv::Vec3b>(i,j)[0] > res.at<cv::Vec3b>(i+bI,j+bJ)[0])
                    {
                        res.at<cv::Vec3b>(i+aI,j+aJ)[0] = 0;
                        res.at<cv::Vec3b>(i+aI,j+aJ)[1] = 0;
                        res.at<cv::Vec3b>(i+aI,j+aJ)[2] = 0;
                        res.at<cv::Vec3b>(i+bI,j+bJ)[0] = 0;
                        res.at<cv::Vec3b>(i+bI,j+bJ)[1] = 0;
                        res.at<cv::Vec3b>(i+bI,j+bJ)[2] = 0;
                    }
                    else
                    {
                        res.at<cv::Vec3b>(i,j)[0] = 0;
                        res.at<cv::Vec3b>(i,j)[1] = 0;
                        res.at<cv::Vec3b>(i,j)[2] = 0;
                    }
                }
                else if(res.at<cv::Vec3b>(i+aI,j+aJ)[0] != 0)
                {
                    if(res.at<cv::Vec3b>(i,j)[0] > res.at<cv::Vec3b>(i+aI,j+aJ)[0])
                    {
                        res.at<cv::Vec3b>(i+aI,j+aJ)[0] = 0;
                        res.at<cv::Vec3b>(i+aI,j+aJ)[1] = 0;
                        res.at<cv::Vec3b>(i+aI,j+aJ)[2] = 0;
                    }
                    else
                    {
                        res.at<cv::Vec3b>(i,j)[0] = 0;
                        res.at<cv::Vec3b>(i,j)[1] = 0;
                        res.at<cv::Vec3b>(i,j)[2] = 0;
                    }
                }
                else if(res.at<cv::Vec3b>(i+bI,j+bJ)[0] != 0)
                {
                    if(res.at<cv::Vec3b>(i,j)[0] > res.at<cv::Vec3b>(i+bI,j+bJ)[0])
                    {
                        res.at<cv::Vec3b>(i+bI,j+bJ)[0] = 0;
                        res.at<cv::Vec3b>(i+bI,j+bJ)[1] = 0;
                        res.at<cv::Vec3b>(i+bI,j+bJ)[2] = 0;
                    }
                    else
                    {
                        res.at<cv::Vec3b>(i,j)[0] = 0;
                        res.at<cv::Vec3b>(i,j)[1] = 0;
                        res.at<cv::Vec3b>(i,j)[2] = 0;
                    }
                }

            }
        }
    }

    if(option.show_color) // faire varier les couleurs
    {
        res.color( option ) ;
    }

    return res ;
}

void Image::Vfiltre_affinage( const Option option )
{   
    Image tmp = (*this).clone() ;

    //AFFINAGE
    for (int i = 1; i < rows-1; ++i)
    {
        for (int j = 1; j < cols-1; ++j)
        {
            if( tmp.at<cv::Vec3b>(i,j)[0] != 0 )
            {                
                float dir = dirs[ (i*cols) + j ] ;
                int aI, aJ;
                int bI, bJ;

                if( dir <= 1*45 )
                {
                    aI = 0 ;
                    aJ = -1 ;
                    bI = 0 ;
                    bJ = 1 ;
                }
                else if( dir <= 2*45 )
                {
                    aI = -1;
                    aJ = -1 ;
                    bI = 1 ;
                    bJ = 1 ;

                }
                else if( dir <= 3*45 )
                {
                    aI = -1 ;
                    aJ = 0 ;
                    bI = 1 ;
                    bJ = 0 ;
                }
                else if( dir <= 4*45 )
                {
                    aI = 1 ;
                    aJ = -1 ;
                    bI = -1 ;
                    bJ = 1 ;
                }
                else if( dir <= 5*45 )
                {
                    aI = 0 ;
                    aJ = 1 ;
                    bI = 0 ;
                    bJ = -1 ;
                }
                else if( dir <= 6*45 )
                {
                    aI = 1 ;
                    aJ = 1 ;
                    bI = -1 ;
                    bJ = -1 ;
                }
                else if( dir <= 7*45 )
                {
                    aI = 0 ;
                    aJ = 1 ;
                    bI = 0 ;
                    bJ = -1 ;
                }
                else
                {
                    aI = 1 ;
                    aJ = -1 ;
                    bI = -1 ;
                    bJ = 1 ;
                } 

                if(tmp.at<cv::Vec3b>(i+aI,j+aJ)[0] != 0 && tmp.at<cv::Vec3b>(i+bI,j+bJ)[0] != 0)
                {
                    if(tmp.at<cv::Vec3b>(i,j)[0] >= tmp.at<cv::Vec3b>(i+aI,j+aJ)[0] && tmp.at<cv::Vec3b>(i,j)[0] > tmp.at<cv::Vec3b>(i+bI,j+bJ)[0])
                    {
                        at<cv::Vec3b>(i+aI,j+aJ)[0] = 0;
                        at<cv::Vec3b>(i+aI,j+aJ)[1] = 0;
                        at<cv::Vec3b>(i+aI,j+aJ)[2] = 0;
                        at<cv::Vec3b>(i+bI,j+bJ)[0] = 0;
                        at<cv::Vec3b>(i+bI,j+bJ)[1] = 0;
                        at<cv::Vec3b>(i+bI,j+bJ)[2] = 0;
                    }
                    else
                    {
                        at<cv::Vec3b>(i,j)[0] = 0;
                        at<cv::Vec3b>(i,j)[1] = 0;
                        at<cv::Vec3b>(i,j)[2] = 0;
                    }
                }
                else if(tmp.at<cv::Vec3b>(i+aI,j+aJ)[0] != 0)
                {
                    if(tmp.at<cv::Vec3b>(i,j)[0] > tmp.at<cv::Vec3b>(i+aI,j+aJ)[0])
                    {
                        at<cv::Vec3b>(i+aI,j+aJ)[0] = 0;
                        at<cv::Vec3b>(i+aI,j+aJ)[1] = 0;
                        at<cv::Vec3b>(i+aI,j+aJ)[2] = 0;
                    }
                    else
                    {
                        at<cv::Vec3b>(i,j)[0] = 0;
                        at<cv::Vec3b>(i,j)[1] = 0;
                        at<cv::Vec3b>(i,j)[2] = 0;
                    }
                }
                else if(tmp.at<cv::Vec3b>(i+bI,j+bJ)[0] != 0)
                {
                    if(tmp.at<cv::Vec3b>(i,j)[0] > tmp.at<cv::Vec3b>(i+bI,j+bJ)[0])
                    {
                        at<cv::Vec3b>(i+bI,j+bJ)[0] = 0;
                        at<cv::Vec3b>(i+bI,j+bJ)[1] = 0;
                        at<cv::Vec3b>(i+bI,j+bJ)[2] = 0;
                    }
                    else
                    {
                        at<cv::Vec3b>(i,j)[0] = 0;
                        at<cv::Vec3b>(i,j)[1] = 0;
                        at<cv::Vec3b>(i,j)[2] = 0;
                    }
                }

            }
        }
    }
}


Image Image::fermeture( const Option option )
{   
    Image res = (*this).clone() ;

    return res ;
}


void Image::Vfermeture( const Option option )
{   

    int compteur = 0 ;
    // unsigned int size_ext = 0 ;
    for (int i = 1; i < rows-1; ++i)
    {
        for (int j = 1; j < cols-1; ++j)
        {
            // int index = (i*cols) + j ;

            if( at<cv::Vec3b>(i,j)[0] != 0 )
            {                
                compteur = 0 ;
                for (int k = i-1; k <= i+1; ++k)
                {
                    for (int l = j-1; l <= j+1; ++l)
                    {
                        if( at<cv::Vec3b>(k,l)[0] != 0 )
                        { 
                            compteur++ ;
                        }
                    }
                }

                if(compteur == 1)
                {
                    at<cv::Vec3b>(i,j)[0] = 0 ;
                    at<cv::Vec3b>(i,j)[1] = 0 ;
                    at<cv::Vec3b>(i,j)[2] = 255 ;
                }
                // float dir = dirs[ index ] ;
            }
        }
    }
}

Image Image::detection_contour( const Option option )
{   
    Image res = (*this).clone() ;

    res.Vfiltre_lissage( option ) ;
    res.Vfiltre_differentiel( option ) ;
    res.Vfiltre_seuillage( option ) ;
    res.Vfiltre_affinage( option ) ;
    // res.Vfermeture( option ) ;

    //COLOR DIRECTION
    if(option.show_color) // faire varier les couleurs
    {
        res.color( option ) ;
    }

    return res ;
}

void Image::color( const Option option )
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            int index = (i*cols) + j ;
            if( at<cv::Vec3b>(i,j)[0] != 0 )
            {                
                float dir = dirs[ index ] ;

                if( dir <= 90 )
                {
                    if(!option.keep_norme)
                        at<cv::Vec3b>(i,j)[0] = 255 ;
                    at<cv::Vec3b>(i,j)[1] = 0 ;
                    at<cv::Vec3b>(i,j)[2] = 0 ;
                }
                else if( dir <= 180 )
                {
                    if(!option.keep_norme)
                        at<cv::Vec3b>(i,j)[1] = 255 ;
                    at<cv::Vec3b>(i,j)[0] = 0 ;
                    at<cv::Vec3b>(i,j)[2] = 0 ;
                }
                else if( dir <= 270 )
                {
                    if(!option.keep_norme)
                        at<cv::Vec3b>(i,j)[2] = 255 ;
                    at<cv::Vec3b>(i,j)[0] = 0 ;
                    at<cv::Vec3b>(i,j)[1] = 0 ;
                }
                else
                {
                    at<cv::Vec3b>(i,j)[1] = 0 ;
                    if(!option.keep_norme){
                        at<cv::Vec3b>(i,j)[0] = 255 ;
                        at<cv::Vec3b>(i,j)[2] = 255 ;
                    }
                }       

            }
        }
    }
}