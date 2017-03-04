#ifndef __AI_DETECT_ANALYZERS_HOMOG_UPDATE_KEYPOINTS_HPP__
#define __AI_DETECT_ANALYZERS_HOMOG_UPDATE_KEYPOINTS_HPP__

#include <boost/gil/gil_all.hpp>

#include <vector>
#include <array>
#include <map>

namespace cv {
    struct Point2f {
        Point2f(float xx, float yy) : x(xx), y(yy) {}
        ~Point2f() {}
        float x;
        float y;
    };

    struct Keypoint {
        float angle;
        int class_id;
        int octave;
        Point2f pt;
        float response;
        float size;
    };
}

namespace ai { 
    namespace image {

        template <typename SViewT, typename SViewT2>
        float covarianceOfViews(const SViewT &view1, const SViewT2 &view2)
        {
            float covariance 1.0;

            return covariance;
        }

    }
}



namespace ai {
    namespace detect {
        namespace homography {
            /**
            *   remove this when in use!
            **/
            using MatchedKeypointsT = std::vector<std::array<cv::Keypoint, 2>>;


            namespace details {

                enum EShiftTestType
                {
                    eCovariance = 0
                };
                
                struct WiggleKPFunct {
                    WiggleKPFunct(std::size_t radius, std::size_t nIter = 1, float covarTh = 0.7) 
                        : _searchRadius(radius)
                        , _searchWinSize(2*radius + 1)
                        , _numIterations(nIter) 
                        , _covarThreshold(covarTh)
                        , _stepSize(radius)
                    {}
                    ~WiggleKPFunct() {}


                    /**
                    *   @brief raster the refView's pixels over different sections of the Warped view's pixels until check covariance
                    *
                    **/
                    template <typename SViewT>
                    cv::Point2f operator()(const SViewT &subViewRef, const SViewT &subViewSearchArea, const std::array<cv::Keypoint, 2> kpPair)
                    {                        
                        //gather covariances at 
                        boost::gil::point2<ptrdiff_t> regionTL(0, 0);           // location of top-left pixel i'm on
                        auto searchAreaDims = subViewSearchArea.dimensions();
                        auto refAreaDims = subViewRef.dimensions();
                        std::map<boost::gil::point2<ptrdiff_t>, float> costFunctResults;
                        while (regionTL.y + refAreaDims.y < searchAreadims.y)
                        {
                            //update what subview i'm looking at
                            while (while (regionTL.y + refAreaDims.y < searchAreadims.y))
                            {
                                SViewT covarCheckView = boost::gil::subimage_view(subViewSearchArea, regionTL, refAreaDims);
                                // check covar
                                costFunctResults[regionTL] = ai::image::covarianceOfViews(subViewRef, covarCheckView);
                                regionTL.x += _stepSize;
                            };
                            regionTL.x = 0;
                            regionTL.y += _stepSize;
                        };
                        //determine best covariance. if numIterations == currentIteration || _stepsize == 1, calc the keypoint location (center)
                        // otherwise, calc new step size and starting/ending boundary conditions for rastering                     
                        {

                        }


                        return shift;
                    }

                private:
                    std::size_t _searchRadius; // max pixel shift in x or y to wiggle the warpedView to compare variance with the reference view.
                    std::ptrdiff_t _searchWinSize;
                    std::size_t _numIterations;
                    float _covarThreshold;
                    std::ptrdiff_t _stepSize;
                };



            }
            




            /**
            *
            *   @brief iterates through the inOutKeypoints function attempts to wiggle them within a certain search radius to maximize the covariance between the two input views until the overlap can be maximized.
            *   @note   reference keypoint is first in MatchedKeypointsT.
            **/
            template <typename SViewT>
            void shiftKeypoint(
                const SViewT &refView, 
                const SViewT &warpedView, // view whose 
                MatchedKeypointsT &inOutKeypoints,
                const std::size_t searchRadius, // max pixel shift in x or y to wiggle the warpedView to compare variance with the reference view.
                const std::size_t numIterations = 1, 
                const details::EShiftTestType shiftType = details::EShiftTestType::eCovariance)
            {
                const std::size_t kpNum = inOutKeypoints.size();
                const std::size_t searchWinRef = 2 * searchRadius + 1;
                const std::size_t searchWinWarp = 3 * searchWinRef;   // search window for warp needs to be twice as large as reference window size b/c it needs to be able to slide up to the search radius distance away
                details::WiggleKPFunct functor(searchRadius, 1);
                for (auto kpIndex = 0; kpIndex < kpNum; ++kpIndex)
                {
                    boost::gil::point2<std::size_t> kpRefLoc(std::size_t(inOutKeypoints[kpIndex][0].pt.x), std::size_t(inOutKeypoints[kpIndex][0].pt.y));           // keypoing location
                    boost::gil::point2<std::size_t> kpWarpLoc(std::size_t(inOutKeypoints[kpIndex][1].pt.x), std::size_t(inOutKeypoints[kpIndex][1].pt.y));          // keypoing location    
                    //get subviews around keypoint 
                    boost::gil::point2<std::size_t> refLocation(std::max(0, kpRefLoc.x - searchRadius), std::max(0, kpRefLoc.y - searchRadius));
                    std::size_t maxWinSizeRefTempX = (kpRefLoc + searchWinRef) > refView.width() ? refView.width() - refLocation.x : searchWinRef;
                    std::size_t maxWinSizeRefTempY = (kpRefLoc + searchWinRef) > refView.height() ? refView.height() - refLocation.y : searchWinRef;
                    SViewT windowedViewRef = boost::gil::subimage_view(refView, refLocation.x, refLocation.y, maxWinSizeRefTempX, maxWinSizeRefTempY);

                    boost::gil::point2<std::size_t> warpLocation(std::max(0, refLocation.x - searchWinRef), std::max(0, refLocation.y - searchWinRef)); // topleft corner
                    std::size_t maxWinSizeWarpTempX = (kpWarpLoc + searchWinWarp) > warpedView.width() ? warpedView.width() - warpLocation.x : searchWinWarp;   // width and height
                    std::size_t maxWinSizeWarpTempY = (kpWarpLoc + searchWinWarp) > warpedView.height() ? warpedView.height() - warpLocation.y : searchWinWarp; //
                    SViewT windowedViewWarp = boost::gil::subimage_view(warpedView, warpLocation.x, warpLocation.y, maxWinSizeWarpTempX, maxWinSizeWarpTempY);  // 

                    // update keypoint after maximizing covariance
                    cv::Point2f inOutKeypoints[kpIndex][1] = functor(windowsViewRef, windowedViewWarp, inOutKeypoints[kpIndex]);

                }


            }
            
            
            
            
        }
    }
}




#endif