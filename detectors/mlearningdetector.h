#ifndef MLEARNINGDETECTOR_H
#define MLEARNINGDETECTOR_H

#include <cv.h>
#include <ml.h>
#include "basedetector.h"

class MLearningDetector : public BaseDetector
{
public:
    MLearningDetector(Store* st, int ml = 3,
                      bool sd = true, bool ld = false,
                      bool uc = false,
                      int hlc = 2, int hls = 50, double lr = 0.001);

    void train(const cv::Mat& inputs, const cv::Mat& outputs);
    bool locate(const cv::Mat& input, cv::Mat& ouput);

protected:
    void analyse_perf(const char* descrip, const cv::Mat& inputs, const cv::Mat& outputs, const int iter, const int spp);

private:
    // User parameters
    int mlAlgorithm;
    bool saveData;
    bool loadData;

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
    double learningRate;

    CvSVM svMach;

    Store* store;
};

#endif // MLEARNINGDETECTOR_H
