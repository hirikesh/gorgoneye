#include <cv.h>
#include <ml.h>
#include <highgui.h>
#include <QDebug>
#include "mlearningdetector.h"
#include "parameter.h"
#include "store.h"

MLearningDetector::MLearningDetector(Store* st, int ml, bool sd, bool ld, bool uc, int hlc, int hls, double lr) :
    BaseDetector(st, "Machine Learning"),
    mlAlgorithm(ml), saveData(sd), loadData(ld), trained(false),
    useClassification(uc),
    hiddenLayerCount(hlc), hiddenLayerSize(hls), learningRate(lr)
{
    // Expose parameters to UI, debugging images as well
    _images.push_back(new ImageModeParam("Visualise Input Training Data", &visInputsImg, &st->dispImg));
    _images.push_back(new ImageModeParam("Visualise Output Training Data", &visOutputsImg, &st->dispImg));
    _params.push_back(new RangeParam<int>("Decision Tree | Random Trees | MLP ANN", Param::RANGE, &mlAlgorithm, 1, 3, 1));
    _params.push_back(new ModeParam("DTrees treat outputs as categories", &useClassification, false));
    _params.push_back(new RangeParam<int>("MLP ANN Hidden Layers", Param::RANGE, &hiddenLayerCount, 2, 4, 1));
    _params.push_back(new RangeParam<int>("MLP ANN Hidden Layer Size", Param::RANGE, &hiddenLayerSize, 2, 2000, 10));
    _params.push_back(new RangeParam<double>("MLP ANN Learning Rate", Param::RANGE_DBL, &learningRate, 0.001, 1, 0.001));
    _params.push_back(new ModeParam("Save trained ML data", &saveData, true));

    // Initialise machine learning algorithms
    dTree = CvDTree();
    rndTrees = CvRTrees();
    mlpANN = CvANN_MLP();
    svMach = CvSVM();

    if(loadData)
    {
        cv::FileStorage load("gaze_training_data.yml", cv::FileStorage::READ);
        cv::Mat inputs, outputs;
        load["inputs"] >> inputs;
        load["outputs"] >> outputs;
        train(inputs, outputs);
    }
}


void MLearningDetector::train(const cv::Mat& inputs, const cv::Mat& outputs)
{
    // Save image data to file
    if(saveData)
    {
        cv::FileStorage save("gaze_training_data.yml", cv::FileStorage::WRITE);
        save << "inputs" << inputs << "outputs" << outputs;
    }

    // Update dimensions
    inputDim = inputs.cols;
    outputDim = outputs.cols;

    // Decision Tree estimation type
    cv::Mat varType;
//    if(useClassification) // don't try this for now
//        varType = cv::Mat(inputDim + 1, 1, CV_8UC1, cv::Scalar(CV_VAR_CATEGORICAL));
//    else
        varType = cv::Mat(inputDim + 1, 1, CV_8UC1, cv::Scalar(CV_VAR_ORDERED));

    // Train Decision Tree
    dTree.clear(); // we want to retrain
    dTree.train(inputs,
                CV_ROW_SAMPLE, // one input feature vectors per row
                outputs,
                cv::Mat(), // Weights for inputs - treat all as equally important
                cv::Mat(), // Train on all inputs
                varType, // Set classificationt or regression training
                cv::Mat(), // Missing inputs - might use this if we catch a closed eye
                CvDTreeParams());

    // Train Random Trees
    rndTrees.clear(); // we want to retrain
    rndTrees.train(inputs,
                   CV_ROW_SAMPLE, // one input feature vectors per row
                   outputs,
                   cv::Mat(), // Weights for inputs - treat all as equally important
                   cv::Mat(), // Train on all inputs
                   varType, // Set classificationt or regression training
                   cv::Mat(), // Missing inputs - might use this if we catch a closed eye
                   CvRTParams());

    // Train MLP ANN
    mlpANN.clear(); // we want to retrain
    cv::Mat mlpANN_layer_size(1, 2 + hiddenLayerCount, CV_32SC1);
    mlpANN_layer_size.at<int>(0,0) = inputDim;
    for(int i = 1; i <= hiddenLayerCount; i++)
        mlpANN_layer_size.at<int>(0,i) = hiddenLayerSize;
    mlpANN_layer_size.at<int>(0,hiddenLayerCount+1) = outputDim;
    mlpANN.create(mlpANN_layer_size, CvANN_MLP::SIGMOID_SYM);
    qDebug() << mlpANN.train(inputs,
                 outputs,
                 cv::Mat(), // Weights for inputs
                 cv::Mat(), // Train on all inputs given
                 CvANN_MLP_TrainParams(cvTermCriteria(CV_TERMCRIT_EPS, 200, 0.0000001),
                                       CvANN_MLP_TrainParams::BACKPROP, learningRate));

    // Train SVM
    svMach.clear();
    svMach.train(inputs,
                 outputs,
                 cv::Mat(),
                 cv::Mat(),
                 CvSVMParams());

    // Save trained data to file
    if(saveData)
    {
        dTree.save("dtree.xml");
        rndTrees.save("rndtree.xml");
        mlpANN.save("mlpann.xml");
    }

    // Visualise inputs and outputs for debugging
    cv::resize(inputs,visInputsImg, cv::Size(inputs.cols, 640));
    imshow("inputs", visInputsImg);
//    inputs.convertTo(visInputsImg, CV_8UC1, 255);
//    cvtColor(inputs, visInputsImg, CV_GRAY2BGR);
//    outputs.convertTo(visOutputsImg, CV_8UC1, 255./640);
//    cvtColor(visOutputsImg, visOutputsImg, CV_GRAY2BGR);

    // Let detector know it can start detecting now
    trained = true;

    // Check performance
    analyse_perf(inputs, outputs);
}


bool MLearningDetector::locate(const cv::Mat& input, cv::Mat& output)
{
    if(trained)
    {
        switch(mlAlgorithm)
        {
        case 1:
            {
                CvDTreeNode* result = dTree.predict(input);
                output.at<float>(0,0) = (float)result->value;
            }
            break;
        case 2:
            {
                float value = rndTrees.predict(input);
                output.at<float>(0,0) = value;
            }
            break;
        case 3:
            {
                mlpANN.predict(input, output);
            }
            break;
        case 4:
            {
                float value = svMach.predict(input);
                output.at<float>(0,0) = value;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}


void MLearningDetector::analyse_perf(const cv::Mat& inputs, const cv::Mat& outputs)
{
    // PERFORMANCE ANALYSIS ON TRAINING DATA
    // Error per data sample
    cv::Mat errors(inputs.rows, 1, CV_32SC1), estimate(inputs.rows, 1, CV_32FC1);
    for(int i = 0; i < inputs.rows; i++)
    {
        mlpANN.predict(inputs(cv::Rect(0,i,inputs.cols,1)), estimate);
        errors.at<int>(i,0) = abs(outputs.at<float>(i,0) - estimate.at<float>(0,0));
    }
    // Mean and std dev of error per coordinate
    cv::Mat error_stats(inputs.rows/SAMPLES_PER_POINT, 3, CV_32SC1);
    cv::Scalar error_mean, error_stddev;
    for(int i = 0; i < outputs.rows/SAMPLES_PER_POINT; i++)
    {
        meanStdDev(errors(cv::Rect(0,SAMPLES_PER_POINT*i,1,SAMPLES_PER_POINT)), error_mean, error_stddev);
        error_stats.at<int>(i,0) = outputs.at<float>(SAMPLES_PER_POINT*i,0);
        error_stats(cv::Rect(1,i,1,1)) = error_mean;
        error_stats(cv::Rect(2,i,1,1)) = error_stddev;
    }
    // Error mean and std dev of error mean and stddev of all coordinates
    cv::Mat error_total(1, 4, CV_32SC1);
    meanStdDev(error_stats(cv::Rect(1,0,1,inputs.rows/SAMPLES_PER_POINT)), error_mean, error_stddev);
    error_total(cv::Rect(0,0,1,1)) = error_mean;
    error_total(cv::Rect(1,0,1,1)) = error_stddev;
    meanStdDev(error_stats(cv::Rect(2,0,1,inputs.rows/SAMPLES_PER_POINT)), error_mean, error_stddev);
    error_total(cv::Rect(2,0,1,1)) = error_mean;
    error_total(cv::Rect(3,0,1,1)) = error_stddev;
    // Error mean and stddev of all samples
    meanStdDev(errors, error_mean, error_stddev);

    // STUB FOR PROPER ERROR
    // 1. Take absolute euclidean distance of calibration point to estimated point as error
    // 2. Square the error in (1), and sum errrors over all trained points
    // 3. Divide the squared sum of errors (2) by number of training samples less one
    // 4. Take the square-root of (3) as the total mean squared error

    // Save errors to YML file
    cv::FileStorage error("errors.yml", cv::FileStorage::WRITE);
    error << "errors_mean" << error_mean << "errors_stddev" << error_stddev
          << "errors_total" << error_total
          << "errors_per_coord" << error_stats
          << "errors_per_sample" << errors;
    // PERFORMANCE ANALYSIS ON TRAINING DATA
}
