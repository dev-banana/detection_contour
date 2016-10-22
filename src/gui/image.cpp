#include "image.hpp"

using namespace std ;


/*************************************************************
**************************************************************

                     CONSTRUCTORS

**************************************************************
*************************************************************/

Image::Image() : cv::Mat(){

    dirs.resize(rows) ;
    gradients.resize(rows) ;
    normes.resize(rows) ;

    for ( int i = 0; i < rows; i++)
    {
        dirs[i].resize(cols) ;
        gradients[i].resize(cols) ;
        normes[i].resize(cols) ;

        for (int j = 0; j < cols; ++j)
        {
            dirs[i][j] = 0 ;
            gradients[i][j] = 0 ;
            normes[i][j] = 0 ;   
        }
    }
}
Image::Image( const cv::Mat input ) : cv::Mat(input){

    dirs.resize(rows) ;
    gradients.resize(rows) ;
    normes.resize(rows) ;

    for ( int i = 0; i < rows; i++)
    {
        dirs[i].resize(cols) ;
        gradients[i].resize(cols) ;
        normes[i].resize(cols) ;

        for (int j = 0; j < cols; ++j)
        {
            dirs[i][j] = 0 ;
            gradients[i][j] = 0 ;
            normes[i][j] = 0 ;   
        }
    }
}
Image::Image(const Image& im) : cv::Mat(im){

    dirs.resize(rows) ;
    gradients.resize(rows) ;
    normes.resize(rows) ;

    for ( int i = 0; i < rows; i++)
    {
        dirs[i].resize(cols) ;
        gradients[i].resize(cols) ;
        normes[i].resize(cols) ;

        for (int j = 0; j < cols; ++j)
        {
            dirs[i][j] = im.dirs[i][j] ;
            gradients[i][j] = im.gradients[i][j] ;
            normes[i][j] = im.normes[i][j] ;   
        }
    }
}
Image::~Image(){}


Image Image::clone()
{
    Image res = (*this).cv::Mat::clone() ;

    res.dirs.resize(rows) ;
    res.gradients.resize(rows) ;
    res.normes.resize(rows) ;

    for ( int i = 0; i < rows; i++)
    {
        res.dirs[i].resize(cols) ;
        res.gradients[i].resize(cols) ;
        res.normes[i].resize(cols) ;

        for (int j = 0; j < cols; ++j)
        {
            res.dirs[i][j] = dirs[i][j] ;
            res.gradients[i][j] = gradients[i][j] ;
            res.normes[i][j] = normes[i][j] ;
        }
    }

    return res ;
}



/*************************************************************
**************************************************************

                        UTILS

**************************************************************
*************************************************************/

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

/*
    Passe l'image en Nuances de Gris
    3 modes possibles :
    CLARTE
    LUMINOSITE
    MOYENNE
*/
Image Image::toGray( unsigned int mode)
{
    Image res = clone() ;

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


/*
    Inverse les couleurs de l'image
*/
Image Image::inverse()
{
    Image res = clone() ;

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


Image Image::rotate90()
{
    Image res = cv::Mat( cols, rows, cv::DataType<cv::Vec3b>::type ) ;

    res.dirs.resize(cols) ;
    res.gradients.resize(cols) ;
    res.normes.resize(cols) ;
    for ( int i = 0; i < cols; i++)
    {
        res.dirs[i].resize(rows) ;
        res.gradients[i].resize(rows) ;
        res.normes[i].resize(rows) ;
    }

    for ( int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            
            res.at<cv::Vec3b>(j, cols-1-i) = at<cv::Vec3b>(i, j) ;

            res.dirs[j][cols-1-i] = dirs[i][j] ;
            res.gradients[j][cols-1-i] = gradients[i][j] ;
            res.normes[j][cols-1-i] = normes[i][j] ;
        }
    }

    return res ;
}

Image Image::rotate180()
{
    Image res = cv::Mat( rows, cols, cv::DataType<cv::Vec3b>::type ) ;

    res.dirs.resize(rows) ;
    res.gradients.resize(rows) ;
    res.normes.resize(rows) ;

    for ( int i = 0; i < rows; i++) {

        res.dirs[i].resize(cols) ;
        res.gradients[i].resize(cols) ;
        res.normes[i].resize(cols) ;

        for (int j = 0; j < cols; j++) {
            
            res.at<cv::Vec3b>(rows-1-i, cols-1-j) = at<cv::Vec3b>(i, j) ;

            res.dirs[rows-1-i][cols-1-j] = dirs[i][j] ;
            res.gradients[rows-1-i][cols-1-j] = gradients[i][j] ;
            res.normes[rows-1-i][cols-1-j] = normes[i][j] ;            
        }
    }

    return res ;
}


/*
Applique un noyau de convolution quelconque passé en paramètre sur l'image
*/
Image Image::simple_convolution( const Filtre filtre )
{
    Image res = clone() ;

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

/*
 Lissage par filtre median
*/
Image Image::mediane( int size_kernel )
{
    Image res = clone() ;

    int sizeTab = size_kernel * size_kernel ;

    for( int i = size_kernel/2; i<rows - size_kernel/2; i++ )
    {
        for ( int j=size_kernel/2; j<cols - size_kernel/2; j++ )
        {
            
            std::vector<int> triR(sizeTab) ;
            std::vector<int> triG(sizeTab) ;
            std::vector<int> triB(sizeTab) ;

            int n = 0 ;
            for( int k=-size_kernel/2; k<=size_kernel/2; k++ )
            {
                for( int l=-size_kernel/2; l<=size_kernel/2; l++ )
                {
                    triR[n] = at<cv::Vec3b>(i+k, j+l)[0] ;
                    triG[n] = at<cv::Vec3b>(i+k, j+l)[1] ;
                    triB[n] = at<cv::Vec3b>(i+k, j+l)[2] ;
                    
                    n++ ;
                } 
            }
            sort(triR.begin(), triR.end());
            sort(triG.begin(), triG.end());
            sort(triB.begin(), triB.end());
            res.at<cv::Vec3b>(i,j)[0] = triR[ sizeTab/2 ] ;
            res.at<cv::Vec3b>(i,j)[1] = triG[ sizeTab/2 ] ;
            res.at<cv::Vec3b>(i,j)[2] = triB[ sizeTab/2 ] ;
        }
    }

    return res ;
}

/*************************************************************
**************************************************************

                        FILTRE

**************************************************************
*************************************************************/

Image Image::lissage( const Option option )
{
    Filtre filtre ;
    Image res ;

    switch( option.lissage_type )
    {
        case MOYENNE :
            filtre.mean( option.lissage_size ) ;
            res = simple_convolution( filtre ) ;
            break ;
        case GAUSSIEN :
            filtre.gaussian( option.lissage_size, option.lissage_sigma ) ;
            res = simple_convolution( filtre ) ;
            break ;
        case MEDIANE :
            res = mediane( option.lissage_size ) ;
            break ;
        default :
            break ;
    }

    return res ;
}

Image Image::filtre_differentiel( const Option option )
{   
    (*this) = toGray( 3 ) ;

    Image res = clone() ;
    
    Filtre tabFiltre[8] ;
    switch( option.direction )
    {
        case HORIZONTAL :
            tabFiltre[0] = option.filtre.clone() ;
            tabFiltre[0] = tabFiltre[0].rotate90() ;
            break ;
        case VERTICAL :
            tabFiltre[0] = option.filtre.clone() ;
            break ;
        case BI_DIRECTIONNEL :
            tabFiltre[0] = option.filtre.clone() ;
            tabFiltre[1] = tabFiltre[0].rotate90() ;
            break ;
        case MULTI_DIRECTIONNEL :
            tabFiltre[0] = option.filtre.clone() ;
            tabFiltre[1] = tabFiltre[0].rotate45() ;
            tabFiltre[2] = tabFiltre[1].rotate45() ;
            tabFiltre[3] = tabFiltre[2].rotate45() ;
            tabFiltre[4] = tabFiltre[3].rotate45() ;
            tabFiltre[5] = tabFiltre[4].rotate45() ;
            tabFiltre[6] = tabFiltre[5].rotate45() ;
            tabFiltre[7] = tabFiltre[6].rotate45() ;
            break ;
        default :
            break ;
    }
    
    for( int i = option.filtre.rows/2; i<rows - option.filtre.rows/2; i++ )
    {
        for ( int j=option.filtre.rows/2; j<cols - option.filtre.rows/2; j++ )
        {
            double gradient, gradient1, gradient2, gradient3, gradient4, gradient5, gradient6, gradient7 ;
            gradient = gradient1 = gradient2 = gradient3 = gradient4 = gradient5 = gradient6 = gradient7 = 0 ;

            for( int k = -option.filtre.rows/2; k <= option.filtre.rows/2; k++ )
            {
                for( int l = -option.filtre.rows/2; l <= option.filtre.rows/2; l++ )
                {
                    switch( option.direction )
                    {
                        case HORIZONTAL : case VERTICAL :
                            gradient += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[0].at<float>(k+option.filtre.rows/2,l+option.filtre.rows/2) ; 
                            break ;
                        case BI_DIRECTIONNEL :    
                            gradient += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[0].at<float>(k+option.filtre.rows/2,l+option.filtre.rows/2) ; 
                            gradient1 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[1].at<float>(k+option.filtre.rows/2,l+option.filtre.rows/2) ;  
                            break ;
                        case MULTI_DIRECTIONNEL :
                            gradient += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[0].at<float>(k+option.filtre.rows/2,l+option.filtre.rows/2) ; 
                            gradient1 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[1].at<float>(k+option.filtre.rows/2,l+option.filtre.rows/2) ;  
                            gradient2 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[2].at<float>(k+option.filtre.rows/2,l+option.filtre.rows/2) ; 
                            gradient3 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[3].at<float>(k+option.filtre.rows/2,l+option.filtre.rows/2) ;  
                            gradient4 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[4].at<float>(k+option.filtre.rows/2,l+option.filtre.rows/2) ;  
                            gradient5 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[5].at<float>(k+option.filtre.rows/2,l+option.filtre.rows/2) ;  
                            gradient6 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[6].at<float>(k+option.filtre.rows/2,l+option.filtre.rows/2) ;  
                            gradient7 += at<cv::Vec3b>(i+k,j+l)[0]*tabFiltre[7].at<float>(k+option.filtre.rows/2,l+option.filtre.rows/2) ;  
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
                    
                    switch( option.type_norme )
                    {
                        case EUCLIDIENNE : 
                            norme = sqrt((gradient*gradient)+(gradient1*gradient1)) ;
                            break ;
                        case ABSOLUE : 
                            norme = abs(gradient) + abs(gradient1) ;
                            break ;
                        case MAX : 
                            norme = max( gradient, gradient1 ) ;
                            break ;
                        default : 
                            break ;
                    }

                    angle = atan2( gradient1, gradient ); //return angle radian
                    dir =  angle * (180.0 / M_PI); //degrés
                    dir = (dir > 0.0 ? dir : (360.0 + dir)); // correction
                    break ;
                case MULTI_DIRECTIONNEL :
                    norme = max( gradient, max( gradient1, max( gradient2, max( gradient3, max( gradient4, max( gradient5, max( gradient6, gradient7 ) ) ) ) ) ) ) ;
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

            res.dirs[i][j] = dir ;
            res.gradients[i][j] = gradient ;
            res.normes[i][j] = norme ;
        }
    }

    return res ;
}


Image Image::seuillage( const Option option )
{   
    Image res = clone() ;

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

    return res ;
}


float Image::normes_mediane(int xmin, int ymin, int xmax, int ymax )
{
    if( xmin < 0 ) xmin = 0 ;
    if( ymin < 0 ) ymin = 0 ;
    if( xmax < 0 || xmax >= rows ) xmax = rows ;
    if( ymax < 0 || ymax >= cols ) ymax = cols ;

    int nb = (xmax-xmin)*(ymax-ymin) ;
    std::vector<int> tri(nb) ;

    int n = 0 ;
    
    for (int i = xmin; i < xmax; ++i)
    {
        for (int j = ymin; j < ymax; ++j)
        {
            tri[n] = normes[i][j] ;
            n++ ;
        }
    }   

    sort(tri.begin(), tri.end());
        
    return tri[ nb/2 ] ;
}

void Image::normes_stats(float &moyenne, float &ecart_type, int xmin, int ymin, int xmax, int ymax )
{
    if( xmin < 0 ) xmin = 0 ;
    if( ymin < 0 ) ymin = 0 ;
    if( xmax < 0 || xmax >= rows ) xmax = rows ;
    if( ymax < 0 || ymax >= cols ) ymax = cols ;

    int nb = (xmax-xmin)*(ymax-ymin) ;
    if( nb == 0 ) return ;

    int SV = 0 ; // somme des valeurs
    int SC = 0 ; // somme des carrés

    for (int i = xmin; i < xmax; ++i)
    {
        for (int j = ymin; j < ymax; ++j)
        {
            int val = normes[i][j] ;
            SV = SV + val ;
            SC = SC + val*val ;
        }
    }

    moyenne = SV / nb ;
    float moyenneCarre = SC / nb ;
    float variance = moyenneCarre - moyenne*moyenne ;
    ecart_type = sqrt( variance ) ;
}

void Image::seuil_unique( Option option )
{
    for( int i = 0; i < rows ; i++ )
    {
        for ( int j= 0; j < cols ; j++ )
        {
            if( normes[i][j] < option.seuil_val ) //SEUILLAGE
            {
                at<cv::Vec3b>(i,j)[0] = 0 ;
                at<cv::Vec3b>(i,j)[1] = 0 ;
                at<cv::Vec3b>(i,j)[2] = 0 ;
            }
            else if(!option.keep_norme)
            {
                at<cv::Vec3b>(i,j)[0] = 255 ;
                at<cv::Vec3b>(i,j)[1] = 255 ;
                at<cv::Vec3b>(i,j)[2] = 255 ;
            }
        }
    }    
}

void Image::seuil_global( Option option )
{    
    float seuil ;

    float moyenne, ecart_type ;

    if( option.seuil_calcul == MOYENNE ){
        normes_stats(moyenne, ecart_type, -1,-1,-1,-1) ;
        seuil = moyenne ;
    }
    else if( option.seuil_calcul == ECART_TYPE ){
        normes_stats(moyenne, ecart_type, -1,-1,-1,-1) ;
        seuil = ecart_type ;
    }
    else if( option.seuil_calcul == MEDIANE )
        seuil = normes_mediane(-1,-1,-1,-1) ;
    else{
        normes_stats(moyenne, ecart_type, -1,-1,-1,-1) ;
        seuil = moyenne ;
    }

    for( int i = 0; i < rows ; i++ )
    {
        for ( int j= 0; j < cols ; j++ )
        { 
            if( normes[i][j] < seuil )
            {
                at<cv::Vec3b>(i,j)[0] = 0 ;
                at<cv::Vec3b>(i,j)[1] = 0 ;
                at<cv::Vec3b>(i,j)[2] = 0 ;
            }
            else if(!option.keep_norme)
            {
                at<cv::Vec3b>(i,j)[0] = 255 ;
                at<cv::Vec3b>(i,j)[1] = 255 ;
                at<cv::Vec3b>(i,j)[2] = 255 ;
            }
        }
    } 
}

void Image::seuil_local( Option option )
{
    Image temp = clone() ;
    for( unsigned int i = option.seuil_fenetre/2; i < rows-option.seuil_fenetre/2 ; i++ )
    {
        for ( unsigned int j= option.seuil_fenetre/2; j < cols-option.seuil_fenetre/2 ; j++ )
        { 
            float seuil ;
            float moyenne, ecart_type ;

            if( option.seuil_calcul == MOYENNE ){
                normes_stats(moyenne, ecart_type, i-option.seuil_fenetre/2, j-option.seuil_fenetre/2, i+option.seuil_fenetre/2, j+option.seuil_fenetre/2 ) ;
                seuil = moyenne ;
            }
            else if( option.seuil_calcul == ECART_TYPE ){
                normes_stats(moyenne, ecart_type, i-option.seuil_fenetre/2, j-option.seuil_fenetre/2, i+option.seuil_fenetre/2, j+option.seuil_fenetre/2 ) ;
                seuil = ecart_type ;
            }
            else if( option.seuil_calcul == MEDIANE )
                seuil = normes_mediane( i-option.seuil_fenetre/2, j-option.seuil_fenetre/2, i+option.seuil_fenetre/2, j+option.seuil_fenetre/2 ) ;
            else{
                normes_stats(moyenne, ecart_type, i-option.seuil_fenetre/2, j-option.seuil_fenetre/2, i+option.seuil_fenetre/2, j+option.seuil_fenetre/2 ) ;
                seuil = moyenne ;
            }
            

            if( normes[i][j] < seuil )
            {
                at<cv::Vec3b>(i,j)[0] = 0 ;
                at<cv::Vec3b>(i,j)[1] = 0 ;
                at<cv::Vec3b>(i,j)[2] = 0 ;
            }
            else if(!option.keep_norme)
            {
                at<cv::Vec3b>(i,j)[0] = 255 ;
                at<cv::Vec3b>(i,j)[1] = 255 ;
                at<cv::Vec3b>(i,j)[2] = 255 ;
            }
        }
    } 
}

void Image::seuil_hysteresis( Option option)
{
    Image temp = clone() ;

    int bas ;
    int haut ;

    if( option.seuil == HYSTERESIS_AUTO )
    {
        float moyenne, ecart_type ;
        normes_stats(moyenne, ecart_type, -1,-1,-1,-1) ;
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
            if( normes[i][j] > haut )
            {
                if(!option.keep_norme){
                    at<cv::Vec3b>(i,j)[0] = 255 ;
                    at<cv::Vec3b>(i,j)[1] = 255 ;
                    at<cv::Vec3b>(i,j)[2] = 255 ;
                }
            }
            else if( normes[i][j] < bas )
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
                        if( at<cv::Vec3b>(k,l)[0] > haut ) //si un voisin a deja été validé comme contour
                        {
                            if(option.keep_norme)
                            {
                                at<cv::Vec3b>(i,j)[0] = normes[i][j] ;
                                at<cv::Vec3b>(i,j)[1] = normes[i][j] ;
                                at<cv::Vec3b>(i,j)[2] = normes[i][j] ;
                            }
                            else
                            {
                                at<cv::Vec3b>(i,j)[0] = 255 ;
                                at<cv::Vec3b>(i,j)[1] = 255 ;
                                at<cv::Vec3b>(i,j)[2] = 255 ;
                            }

                            k = i+option.seuil_fenetre/2 ; //exit for loop
                            break ;
                        }
                    }
                }
            }
        }
    }     
}


Image Image::affinage( const Option option )
{   
    Image res = clone() ;

    for (int i = 1; i < rows-1; ++i)
    {
        for (int j = 1; j < cols-1; ++j)
        {
            if( res.at<cv::Vec3b>(i,j)[0] != 0 )
            {                
                float dir = dirs[i][j] ;
                int aI, aJ;
                int bI, bJ;

                if( dir <= 1*45 )
                {
                    aI = 0 ;
                    aJ = 1 ;
                    bI = 0 ;
                    bJ = -1 ;
                }
                else if( dir <= 2*45 )
                {
                    aI = -1;
                    aJ = 1 ;
                    bI = 1 ;
                    bJ = -1 ;

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
                    aI = -1 ;
                    aJ = -1 ;
                    bI = 1 ;
                    bJ = 1 ;
                }
                else if( dir <= 5*45 )
                {
                    aI = 0 ;
                    aJ = -1 ;
                    bI = 0 ;
                    bJ = 1 ;
                }
                else if( dir <= 6*45 )
                {
                    aI = 1 ;
                    aJ = -1 ;
                    bI = -1 ;
                    bJ = 1 ;
                }
                else if( dir <= 7*45 )
                {
                    aI = 1 ;
                    aJ = 0 ;
                    bI = -1 ;
                    bJ = 0 ;
                }
                else
                {
                    aI = 1 ;
                    aJ = 1 ;
                    bI = -1 ;
                    bJ = -1 ;
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

    return res ;
}



Image Image::fermeture( const Option option )
{   
    Image res = clone() ;

    int compteur_voisin = 0 ;

    for (int i = 1; i < rows-1; ++i)
    {
        for (int j = 1; j < cols-1; ++j)
        {
            if( res.at<cv::Vec3b>(i,j)[0] != 0 )
            {       
                int x = i ;
                int y = j ;

                do
                {
                    compteur_voisin = 0 ;

                    int voisinI, voisinJ ;
                    for (int k = x-1; k <= x+1; ++k)
                    {
                        for (int l = y-1; l <= y+1; ++l)
                        {
                            if( res.at<cv::Vec3b>(k,l)[0] != 0 && ( k!=x || l!=y ) )
                            { 
                                compteur_voisin++ ;
                                voisinI = k ;
                                voisinJ = l ;
                            }
                        }
                    }

                    if(compteur_voisin == 1)
                    {
                        int dirI, dirJ ;

                        if( dirs[x][y] <= 1*45 )
                        {
                            if( voisinJ > y ){
                                dirI = 0 ;
                                dirJ = -1 ;
                            }else{
                                dirI = 0 ;
                                dirJ = 1 ;
                            }
                        }
                        else if( dirs[x][y] <= 2*45 )
                        {
                            if( voisinI > x ){
                                dirI = 1 ;
                                dirJ = -1 ;
                            }else{
                                dirI = -1 ;
                                dirJ = 1 ;
                            }
                        }
                        else if( dirs[x][y] <= 3*45 )
                        {
                            if( voisinI > x ){
                                dirI = -1 ;
                                dirJ = 0 ;
                            }else{
                                dirI = 1 ;
                                dirJ = 0 ;
                            }
                        }
                        else if( dirs[x][y] <= 4*45 )
                        {
                            if( voisinI > x ){
                                dirI = -1 ;
                                dirJ = -1 ;
                            }else{
                                dirI = 1 ;
                                dirJ = 1 ;
                            }
                        }
                        else if( dirs[x][y] <= 5*45 )
                        {
                            if( voisinJ > y ){
                                dirI = 0 ;
                                dirJ = -1 ;
                            }else{
                                dirI = 0 ;
                                dirJ = 1 ;
                            }
                        }
                        else if( dirs[x][y] <= 6*45 )
                        {
                            if( voisinI < x ){
                                dirI = -1 ;
                                dirJ = 1 ;
                            }else{
                                dirI = 1 ;
                                dirJ = -1 ;
                            } 
                        }
                        else if( dirs[x][y] <= 7*45 )
                        {
                            if( voisinI > x ){
                                dirI = -1 ;
                                dirJ = 0 ;
                            }else{
                                dirI = 1 ;
                                dirJ = 0 ;
                            }
                        }
                        else
                        {
                            if( voisinI > x ){
                                dirI = -1 ;
                                dirJ = -1 ;
                            }else{
                                dirI = 1 ;
                                dirJ = 1 ;
                            }
                        } 

                           
                        bool trouve = false ;
                        int oldX = x ;
                        int oldY = y ;
                        for( unsigned int k =1 ; k<= option.fermeture_size; k++)
                        {         
                            int pI = oldX + k*dirI ;
                            int pJ = oldY + k*dirJ ;
                            if( pI >=0 && pI < rows && pJ >=0 && pJ<cols)
                            {    
                                if( res.at<cv::Vec3b>(pI,pJ)[0] == 0 && normes[pI][pJ] > option.fermeture_seuil && dirs[pI][pJ] == dirs[oldX][oldY] )
                                {

                                    for( unsigned int m = 1 ; m <= k ; m++)
                                    {
                                        int mI = oldX + m*dirI ;
                                        int mJ = oldY + m*dirJ ; 
                                        if( option.keep_norme )
                                        {
                                            res.at<cv::Vec3b>(mI,mJ)[0] = normes[mI][mJ] ;
                                            res.at<cv::Vec3b>(mI,mJ)[1] = normes[mI][mJ] ;
                                            res.at<cv::Vec3b>(mI,mJ)[2] = normes[mI][mJ] ;
                                        }
                                        else
                                        {
                                            res.at<cv::Vec3b>(mI,mJ)[0] = 255 ;
                                            res.at<cv::Vec3b>(mI,mJ)[1] = 255 ;
                                            res.at<cv::Vec3b>(mI,mJ)[2] = 255 ;
                                        }
                                    }

                                    trouve = true ;
                                    
                                    x = pI ;
                                    y = pJ ;
                                }
                            }
                        }

                        if( !trouve )
                            compteur_voisin = 0 ;
                    }

                } while( compteur_voisin == 1 ) ;
            }
        }
    }

    return res ;
}

Image Image::detection_contour( const Option option )
{   
    Image res = clone() ;
    Image res2 = res.lissage( option ) ;
    Image res3 = res2.filtre_differentiel( option ) ;
    Image res4 = res3.seuillage( option ) ;
    Image res5 = res4.affinage( option ) ;
    Image res6 = res5.fermeture( option ) ;

    //COLOR DIRECTION
    if(option.show_color) // faire varier les couleurs
    {
        res6 = res6.color_direction( option ) ;
    }

    return res6 ;
}

Image Image::color_direction( const Option option )
{
    Image res = clone() ;

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if( at<cv::Vec3b>(i,j)[0] != 0 )
            {                
                float dir = dirs[i][j] ;

                if( dir <= 1*45 ) //violet
                {
                    if( !option.keep_norme )
                    {
                        res.at<cv::Vec3b>(i,j)[2] = 255 ;
                        res.at<cv::Vec3b>(i,j)[0] = 255 ;
                    }                    
                    res.at<cv::Vec3b>(i,j)[1] = 0 ;
                }
                else if( dir <= 2*45 ) //bleu
                {
                    res.at<cv::Vec3b>(i,j)[2] = 0 ;
                    res.at<cv::Vec3b>(i,j)[1] = 0 ;
                    if( !option.keep_norme )
                    {
                        res.at<cv::Vec3b>(i,j)[0] = 255 ;
                    } 
                }
                else if( dir <= 3*45 ) //vert
                {
                    res.at<cv::Vec3b>(i,j)[2] = 0 ;
                    res.at<cv::Vec3b>(i,j)[0] = 0 ;
                    if( !option.keep_norme )
                    {
                        res.at<cv::Vec3b>(i,j)[1] = 255 ;
                    } 
                }
                else if( dir <= 4*45 ) //jaune
                {
                    if( !option.keep_norme )
                    {
                        res.at<cv::Vec3b>(i,j)[2] = 255 ;
                        res.at<cv::Vec3b>(i,j)[1] = 255 ;
                    }
                    res.at<cv::Vec3b>(i,j)[0] = 0 ;
                }
                else if( dir <= 5*45 ) //orange
                {
                    if( !option.keep_norme )
                    {
                        res.at<cv::Vec3b>(i,j)[2] = 255 ;
                        res.at<cv::Vec3b>(i,j)[1] = 128 ;
                    }
                    else
                    {
                        res.at<cv::Vec3b>(i,j)[2] = res.at<cv::Vec3b>(i,j)[2] ;
                        res.at<cv::Vec3b>(i,j)[1] = res.at<cv::Vec3b>(i,j)[1]/2 ;
                    }
                    
                    res.at<cv::Vec3b>(i,j)[0] = 0 ;
                }
                else if( dir <= 6*45 ) //rose
                {
                    if( !option.keep_norme )
                    {
                        res.at<cv::Vec3b>(i,j)[2] = 255 ;
                        res.at<cv::Vec3b>(i,j)[0] = 128 ;
                    }
                    else
                    {
                        res.at<cv::Vec3b>(i,j)[2] = res.at<cv::Vec3b>(i,j)[2] ;
                        res.at<cv::Vec3b>(i,j)[0] = res.at<cv::Vec3b>(i,j)[0]/2 ;
                    }
                    res.at<cv::Vec3b>(i,j)[1] = 0 ;
                    
                }
                else if( dir <= 7*45 ) //rouge
                {
                    if( !option.keep_norme )
                    {
                        res.at<cv::Vec3b>(i,j)[2] = 255 ;
                    }
                    res.at<cv::Vec3b>(i,j)[1] = 0 ;
                    res.at<cv::Vec3b>(i,j)[0] = 0 ;
                }
                else //marron
                {
                    if( !option.keep_norme )
                    {
                        res.at<cv::Vec3b>(i,j)[2] = 128 ;
                        res.at<cv::Vec3b>(i,j)[1] = 64 ;
                    }
                    else
                    {
                        res.at<cv::Vec3b>(i,j)[2] = res.at<cv::Vec3b>(i,j)[2]/2 ;
                        res.at<cv::Vec3b>(i,j)[1] = res.at<cv::Vec3b>(i,j)[1]/4 ;
                    }
                    res.at<cv::Vec3b>(i,j)[0] = 0 ;
                }     
            }
        }
    }

    return res ;
}