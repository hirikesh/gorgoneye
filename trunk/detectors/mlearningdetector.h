#ifndef MLEARNINGDETECTOR_H
#define MLEARNINGDETECTOR_H

#include <cv.h>
#include <ml.h>
#include "basedetector.h"

class MLearningDetector : public BaseDetector
{
public:
    MLearningDetector(Store* st, int ml = 1, bool sx = false,
                      bool uc = false,
                      int hlc = 2, int hls = 100, double fp = 0.1);

    void train(const cv::Mat& inputs, const cv::Mat& outputs);
    bool locate(const cv::Mat& input, cv::Mat& ouput);

private:
    // User parameters
    int mlAlgorithm;
    bool saveXml;

    // Common parameters
    int inputDim;
    int outputDim;

    // State
    bool trained;
    cv::Mat visInputsImg;
    cv::Mat visOutputsImg;

    // Algorithms + specific parameters
    CvDTree dTree;
    CvRTrees rndTrees;
    bool useClassification;

    CvANN_MLP mlpANN;
    int hiddenLayerCount;
    int hiddenLayerSize;
    double freeParam;
};

#endif // MLEARNINGDETECTOR_H
