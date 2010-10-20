#include <cv.h>
#include <ml.h>
#include <QDebug>
#include "mlearningdetector.h"
#include "parameter.h"
#include "store.h"

MLearningDetector::MLearningDetector(Store* st, int ml, bool sx, bool uc, int hlc, int hls, double fp) :
    BaseDetector(st, "Machine Learning"),
    mlAlgorithm(ml), saveXml(sx), trained(false),
    useClassification(uc),
    hiddenLayerCount(hlc), hiddenLayerSize(hls), freeParam(fp)
{
    // Expose parameters to UI, debugging images as well
    _images.push_back(new ImageModeParam("Visualise Input Training Data", &visInputsImg, &st->dispImg));
    _images.push_back(new ImageModeParam("Visualise Output Training Data", &visOutputsImg, &st->dispImg));
    _params.push_back(new RangeParam<int>("Decision Tree | Random Trees | MLP ANN", Param::RANGE, &mlAlgorithm, 1, 3, 1));
    _params.push_back(new ModeParam("DTrees treat outputs as categories", &useClassification, false));
    _params.push_back(new RangeParam<int>("MLP ANN Hidden Layers", Param::RANGE, &hiddenLayerCount, 2, 4, 1));
    _params.push_back(new RangeParam<int>("MLP ANN Hidden Layer Size", Param::RANGE, &hiddenLayerSize, 2, 2000, 10));
    _params.push_back(new RangeParam<double>("MLP ANN free parameter", Param::RANGE_DBL, &freeParam, 0.001, 1, 0.01));
    _params.push_back(new ModeParam("Save trained ML data", &saveXml, true));

    // Initialise machine learning algorithms
    dTree = CvDTree();
    rndTrees = CvRTrees();
    mlpANN = CvANN_MLP();
}


void MLearningDetector::train(const cv::Mat& inputs, const cv::Mat& outputs)
{
    // Update dimensions and
    inputDim = inputs.cols;
    outputDim = outputs.cols;

    // Decision Tree estimation type.
    cv::Mat varType;
    if(useClassification)
        varType = cv::Mat(inputDim + 1, 1, CV_8UC1, cv::Scalar(CV_VAR_CATEGORICAL));
    else
        varType = cv::Mat(inputDim + 1, 1, CV_8UC1, cv::Scalar(CV_VAR_ORDERED));

    // Train Decision Tree
    dTree.clear(); // we want to retrain
    dTree.train(inputs,
                CV_ROW_SAMPLE, // one input feature vectors per row
                outputs,
                cv::Mat(), // Weights for inputs - treat all as equally important
                cv::Mat(), // Train on all inputs
                varType, // Set classificationt or regression training
                cv::Mat(), // Missin inputs - might use this if we catch a closed eye
                CvDTreeParams());

    // Train Random Trees
    rndTrees.clear(); // we want to retrain
    rndTrees.train(inputs,
                   CV_ROW_SAMPLE, // one input feature vectors per row
                   outputs,
                   cv::Mat(), // Weights for inputs - treat all as equally important
                   cv::Mat(), // Train on all inputs
                   varType, // Set classificationt or regression training
                   cv::Mat(), // Missin inputs - might use this if we catch a closed eye
                   CvRTParams());

    // Train MLP ANN
    mlpANN.clear(); // we want to retrain
    cv::Mat mlpANN_layer_size(1, 2 + hiddenLayerCount, CV_32SC1);
    mlpANN_layer_size.ptr<int>(0)[0] = inputDim;
    for(int i = 1; i <= hiddenLayerCount; i++)
        mlpANN_layer_size.ptr<int>(0)[i] = hiddenLayerSize;
    mlpANN_layer_size.ptr<int>(0)[hiddenLayerCount+1] = outputDim;
    mlpANN.create(mlpANN_layer_size, CvANN_MLP::SIGMOID_SYM);
    mlpANN.train(inputs,
                 outputs,
                 cv::Mat(inputs.rows,1,CV_32FC1,cv::Scalar(1.0)), // Weights for inputs - treat all as equally important
                 cv::Mat(), // Train on all inputs given
                 CvANN_MLP_TrainParams(cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 300, 0.01),
                                       CvANN_MLP_TrainParams::BACKPROP, freeParam));

    // Save trained data to file
    if(saveXml)
    {
        dTree.save("dtree.xml");
        rndTrees.save("rndtree.xml");
        mlpANN.save("mlpann.xml");
    }

    // Visualise inputs and outputs for debugging
    cv::Mat tmpVisImg;
    inputs.convertTo(tmpVisImg, CV_8UC1, 255);
    cvtColor(tmpVisImg, visInputsImg, CV_GRAY2BGR);
    outputs.convertTo(tmpVisImg, CV_8UC1, 255);
    cvtColor(tmpVisImg, visOutputsImg, CV_GRAY2BGR);

    // Let detector know it can start detecting now
    trained = true;
}


bool MLearningDetector::locate(const cv::Mat& input, cv::Mat& output)
{
    if(trained)
    {
        output = cv::Mat(1, outputDim, CV_32FC1, cv::Scalar(-1));
        switch(mlAlgorithm)
        {
        case 1:
            {
                CvDTreeNode* result = dTree.predict(input);
                output.ptr<float>(0)[0] = result->value;
            }
            break;
        case 2:
            {
                float value = rndTrees.predict(input);
                output.ptr<float>(0)[0] = value;
            }
            break;
        case 3:
            {
                mlpANN.predict(input, output);
            }
            break;
        }
        return true;
    }
    else
    {
        return false;
    }
}
