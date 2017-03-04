#ifndef _AI_IMAGE_RESCALE_BICUBIC_HPP_
#define _AI_IMAGE_RESCALE_BICUBIC_HPP_

#include <boost/numeric/ublas/matrix_vector.hpp>
#include <boost/numeric/ublas/matrix.hpp> // for matrix math
#include <boost/gil/gil_all.hpp>

#include <array>


namespace ai {
    namespace image {
        namespace details {

            template<typename PixT>
            struct BicubicKernelFunct {
            private:
                static const int _numChannels = boost::gil::num_channels<typename PixT>::value;
            public:
                BicubicKernelFunct() : _kSize(4),  _nextPop(-1), _Ainverse(16,16) {
                    float aInvInit[16][16] = {
                         1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                         0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                        -3.0, 3.0, 0.0, 0.0,-2.0,-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                         2.0,-2.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
                         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,-3.0, 3.0, 0.0, 0.0,-2.0,-1.0, 0.0, 0.0,
                         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0,-2.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0,
                        -3.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0,-2.0, 0.0,-1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                         0.0, 0.0, 0.0, 0.0,-3.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0,-2.0, 0.0,-1.0, 0.0,
                         9.0,-9.0,-9.0, 9.0, 6.0, 3.0,-6.0,-3.0, 6.0,-6.0, 3.0,-3.0, 4.0, 2.0, 2.0, 4.0,
                        -6.0, 6.0, 6.0,-6.0,-3.0,-3.0, 3.0, 3.0,-4.0, 4.0,-2.0, 2.0,-2.0,-2.0,-1.0,-1.0,
                         2.0, 0.0,-2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                         0.0, 0.0, 0.0, 0.0, 2.0, 0.0,-2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0,
                        -6.0, 6.0, 6.0,-6.0,-4.0,-2.0, 4.0, 2.0,-3.0, 3.0,-3.0, 3.0,-2.0,-1.0,-2.0,-1.0,
                         4.0,-4.0,-4.0, 4.0, 2.0, 2.0,-2.0,-2.0, 2.0,-2.0, 2.0,-2.0, 1.0, 1.0, 1.0, 1.0
                    };
                    _Ainverse boost::numeric::ublas::make_matrix_from_pointer(aInvInit);


           //         for (auto ch = _numChannels; ch < _numChannels; ++ch)
           //         {
           //             _coeffA[ch](16, 16);
           //         }
                }
                ~BicubicKernelFunct() {}
            public:
                void operator()()
                {

                }



            private:     
                void updateMatrix()
                {

                }

                inline int nextPop()
                {
                    return ++_nextPop % _kSize;
                }

            private:
                boost::numeric::ublas::matrix<float> _Ainverse;
                std::array<boost::numeric::ublas::matrix<double>, _numChannels>  _coeffA;
                const std::size_t _kSize;
                int _nextPop;
            };
            

        }

        template<typename SourceViewT, typename DestImageT>
        void rescaleBicubic(const SourceViewT &sourceView, DestImageT &destImage, int newWidth, int newHeight)
        {
            namespace bg = boost::gil;
            using SPixT = typename SourceViewT::value_type;
            using SBitT = typename bg::channel_type<SPixT>::type;
            using WPixT = bg::pixel<bg::bits32f, SPixT::layout_t>;
            using WImgT = bg::image<WPixT, bg::is_planar<typename SourceViewT>::value>;
            using WViewT = typename WImgT::view_t;
            WImgT workImg(sourceView.dimensions());
            boost::gil::copy_and_convert_pixels(sourceView, boost::gil::view(workImg));
            WImgT derivImg(sourceView.dimensions());
         //   ai::image::sobel<XY>(sourceView, boost::gil::view(derivImg));
         //   std::size_t sHeight = sourceView.height();
         //   std::size_t sWidth = sourceView.width();
         //   details::BicubicKernelFunct<WPixT> pixFunct;
         //   for (std::size_t yInd = 0; yInd < sHeight; ++yInd)
         //   {
         //       for (std::size_t xInd = 0; xInd < sWidth; ++xInd)
         //       {
         //
         //
         //
         //       }
         //
         //   }


        }

    }
}

#endif