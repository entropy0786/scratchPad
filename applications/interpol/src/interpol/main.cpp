

#include <libImage/matrix/getType.hpp>
//#include <boost/log/core.hpp>

#include <iostream>

int main(int argc, char** argv)
{
    cv::Mat cvMat(CV_16S);
    boost::numeric::ublas::matrix<signed short> ublas16s(2, 3);
    std::cout << "hello, cvMat is type: " << cvMat.type() << std::endl;
    ai::math::matrix::CvUblasConverter converter;
    converter.convCvToUblas(cvMat, ublas16s);
    std::cout << "converted!" << std::endl;
    std::cin.get();
    return 0;
}