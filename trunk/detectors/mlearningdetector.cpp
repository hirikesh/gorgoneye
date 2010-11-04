#include <sstream>
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
    hiddenLayerCount(hlc), hiddenLayerSize(hls), learningRate(lr),
    store(st)
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
        cv::FileStorage load("gtd.yml", cv::FileStorage::READ);
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
        cv::FileStorage save("gtd.yml", cv::FileStorage::WRITE);
        save << "inputs" << inputs << "outputs" << outputs;
    }

    // Update dimensions
    inputDim = inputs.cols;
    outputDim = outputs.cols;

    // Break inputs/outputs into training set & test set
    int trainingSamplesMulti = SAMPLES_PER_POINT - XVALID_SAMPLES_PER_POINT;
    int testSamplesMulti = XVALID_SAMPLES_PER_POINT;
    cv::Mat trainInputs(trainingSamplesMulti*inputs.rows/SAMPLES_PER_POINT, inputDim, CV_32FC1);
    cv::Mat trainOutputs(trainingSamplesMulti*outputs.rows/SAMPLES_PER_POINT, outputDim, CV_32FC1);
    cv::Mat testInputs(testSamplesMulti*inputs.rows/SAMPLES_PER_POINT, inputDim, CV_32FC1);
    cv::Mat testOutputs(testSamplesMulti*outputs.rows/SAMPLES_PER_POINT, outputDim, CV_32FC1);
    cv::Mat temp;
    for(int i = 0; i < inputs.rows/SAMPLES_PER_POINT; i++)
    {
        temp = trainInputs(cv::Rect(0,trainingSamplesMulti*i,inputDim,trainingSamplesMulti));
        inputs(cv::Rect(0,SAMPLES_PER_POINT*i,inputDim,trainingSamplesMulti)).copyTo(temp);
        temp = trainOutputs(cv::Rect(0,trainingSamplesMulti*i,outputDim,trainingSamplesMulti));
        outputs(cv::Rect(0,SAMPLES_PER_POINT*i,outputDim,trainingSamplesMulti)).copyTo(temp);

        temp = testInputs(cv::Rect(0,testSamplesMulti*i,inputDim,testSamplesMulti));
        inputs(cv::Rect(0,SAMPLES_PER_POINT*i+trainingSamplesMulti,inputDim,testSamplesMulti)).copyTo(temp);
        temp = testOutputs(cv::Rect(0,testSamplesMulti*i,outputDim,testSamplesMulti));
        outputs(cv::Rect(0,SAMPLES_PER_POINT*i+trainingSamplesMulti,outputDim,testSamplesMulti)).copyTo(temp);
    }

    // Decision Tree estimation type
//    cv::Mat varType;
//    if(useClassification) // don't try this for now
//        varType = cv::Mat(inputDim + 1, 1, CV_8UC1, cv::Scalar(CV_VAR_CATEGORICAL));
//    else
//        varType = cv::Mat(inputDim + 1, 1, CV_8UC1, cv::Scalar(CV_VAR_ORDERED));

    // Train Decision Tree
//    dTree.clear(); // we want to retrain
//    dTree.train(trainingInputs,
//                CV_ROW_SAMPLE, // one input feature vectors per row
//                trainingOutputs,
//                cv::Mat(), // Weights for inputs - treat all as equally important
//                cv::Mat(), // Train on all inputs
//                varType, // Set classificationt or regression training
//                cv::Mat(), // Missing inputs - might use this if we catch a closed eye
//                CvDTreeParams());

    // Train Random Trees
//    rndTrees.clear(); // we want to retrain
//    rndTrees.train(trainingInputs,
//                   CV_ROW_SAMPLE, // one input feature vectors per row
//                   trainingOutputs,
//                   cv::Mat(), // Weights for inputs - treat all as equally important
//                   cv::Mat(), // Train on all inputs
//                   varType, // Set classificationt or regression training
//                   cv::Mat(), // Missing inputs - might use this if we catch a closed eye
//                   CvRTParams());

    // Train MLP ANN
    mlpANN.clear(); // we want to retrain
    cv::Mat mlpANN_layer_size(1, 2 + hiddenLayerCount, CV_32SC1);
    mlpANN_layer_size.at<int>(0,0) = inputDim;
    for(int i = 1; i <= hiddenLayerCount; i++)
        mlpANN_layer_size.at<int>(0,i) = hiddenLayerSize;
    mlpANN_layer_size.at<int>(0,hiddenLayerCount+1) = outputDim;

    mlpANN.create(mlpANN_layer_size, CvANN_MLP::SIGMOID_SYM);
    int iter = mlpANN.train(trainInputs,
                            trainOutputs,
                            cv::Mat(), // Weights for inputs
                            cv::Mat(), // Train on all inputs given
                            CvANN_MLP_TrainParams(cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 500, 0.000001),
                                                  CvANN_MLP_TrainParams::BACKPROP, learningRate));

    // Train SVM
//    svMach.clear();
//    svMach.train(trainingInputs,
//                 trainingOutputs,
//                 cv::Mat(),
//                 cv::Mat(),
//                 CvSVMParams());

    // Save trained data to file
    std::stringstream fname;
    fname << "mlpann-" << hiddenLayerCount << "hl-" << hiddenLayerSize << "n-f" << learningRate << "-e.xml";
    mlpANN.save(fname.str().c_str());
//    if(saveData)
//    {
//        dTree.save("dtree.xml");
//        rndTrees.save("rndtree.xml");
//        mlpANN.save("mlpann.xml");
//        svMach.save("svm.xml");
//    }

    // Visualise inputs and outputs for debugging
//    inputs.convertTo(visInputsImg, CV_8UC1, 255);
//    cvtColor(inputs, visInputsImg, CV_GRAY2BGR);
//    outputs.convertTo(visOutputsImg, CV_8UC1, 255./640);
//    cvtColor(visOutputsImg, visOutputsImg, CV_GRAY2BGR);

    // Let detector know it can start detecting now
    trained = true;

    // Check performance
    analyse_perf("train", trainInputs, trainOutputs, iter, trainingSamplesMulti);
    analyse_perf("test", testInputs, testOutputs, iter, testSamplesMulti);
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


void MLearningDetector::analyse_perf(const char* descrip, const cv::Mat& inputs, const cv::Mat& outputs, const int iter, const int spp)
{
    // PERFORMANCE ANALYSIS ON TRAINING DATA
    // Enumerations of error matrice indexes
    enum { X = 0, Y = 1, TOTAL = 2,
           EPS_DIST = 0, EPS_X = 1, EPS_Y = 2, EPS_TOTAL = 3,

           EPC_DIST_MEAN = 2, EPC_DIST_STDDEV = 3,
           EPC_X_MEAN = 4, EPC_X_STDDEV = 5,
           EPC_Y_MEAN = 6, EPC_Y_STDDEV = 7, EPC_TOTAL = 8,

           EEPC_DIST_MEAN = 0, EEPC_DIST_STDDEV = 1,
           EEPC_X_MEAN = 2, EEPC_X_STDDEV = 3,
           EEPC_Y_MEAN = 4, EEPC_Y_STDDEV = 5, EEPC_TOTAL = 6,

           ET_DIST_MEAN = 0, ET_DIST_STDDEV = 1, ET_DIST_MSE = 2,
           ET_X_MEAN = 3, ET_X_STDDEV = 4, ET_X_MSE = 5,
           ET_Y_MEAN = 6, ET_Y_STDDEV = 7, ET_Y_MSE = 8, ET_TOTAL = 9 };

    // Error per data sample
    cv::Mat error_per_sample(inputs.rows, EPS_TOTAL, CV_32SC1);
    cv::Mat estimate(inputs.rows, TOTAL, CV_32FC1);

    for(int i = 0; i < inputs.rows; i++)
    {
        mlpANN.predict(inputs.row(i), estimate); // estimate gaze on trained sample
//        int errx = abs(outputs.at<float>(i,X) - estimate.at<float>(0,X)); // abs error in x
//        int erry = outputDim == 2 ? abs(outputs.at<float>(i,Y) - estimate.at<float>(0,Y)) : 0; // abs error in y

        int errx = outputs.at<float>(i,X) - estimate.at<float>(0,X); // abs error in x
        int erry = outputDim == 2 ? outputs.at<float>(i,Y) - estimate.at<float>(0,Y) : 0; // abs error in y

        error_per_sample.at<int>(i,EPS_DIST) = sqrt(pow(errx,2) + pow(erry,2)); // euclidean error per coordinate
        error_per_sample.at<int>(i,EPS_X) = errx; // abs error in x direction
        error_per_sample.at<int>(i,EPS_Y) = erry; // abs error in y direction
        store->estPoints.push_back(std::pair<int,int>(estimate.at<float>(0,X), estimate.at<float>(0,Y)));
    }

    // Mean and std dev of error per coordinate
    cv::Mat error_per_coord(inputs.rows/spp, EPC_TOTAL, CV_32SC1);
    cv::Rect samples; cv::Scalar error_mean, error_stddev;

    for(int i = 0; i < outputs.rows/spp; i++)
    {
        error_per_coord.at<int>(i,X) = outputs.at<float>(spp*i,X); // x coord
        error_per_coord.at<int>(i,Y) = outputs.at<float>(spp*i,Y); // y coord

        samples = cv::Rect(EPS_DIST,spp*i,1,spp); // mean & stddev of euc. dist. errors
        meanStdDev(error_per_sample(samples), error_mean, error_stddev); // calc the stats
        error_per_coord.at<int>(i,EPC_DIST_MEAN) = error_mean[0]; // mean of euclidean distance error
        error_per_coord.at<int>(i,EPC_DIST_STDDEV) = error_stddev[0]; // stddev of euclidean distance error

        samples = cv::Rect(EPS_X,spp*i,1,spp); // mean & stddev of euc. dist. errors
        meanStdDev(error_per_sample(samples), error_mean, error_stddev); // calc the stats
        error_per_coord.at<int>(i,EPC_X_MEAN) = error_mean[0]; // mean of abs. error in x
        error_per_coord.at<int>(i,EPC_X_STDDEV) = error_stddev[0]; // stddev of abs. error in x

        samples = cv::Rect(EPS_Y,spp*i,1,spp); // mean & stddev of euc. dist. errors
        meanStdDev(error_per_sample(samples), error_mean, error_stddev); // calc the stats
        error_per_coord.at<int>(i,EPC_Y_MEAN) = error_mean[0]; // mean of abs. error in x
        error_per_coord.at<int>(i,EPC_Y_STDDEV) = error_stddev[0]; // stddev of abs. error in x
    }

    // Mean and std dev over all error means and stddevs per coordinate
    cv::Mat error_per_coord_stat(1, EEPC_TOTAL, CV_32SC1);

    meanStdDev(error_per_coord.col(EPC_DIST_MEAN), error_mean, error_stddev);
    error_per_coord_stat.at<int>(0,EEPC_DIST_MEAN) = error_mean[0];
    error_per_coord_stat.at<int>(0,EEPC_DIST_STDDEV) = error_stddev[0];

    meanStdDev(error_per_coord.col(EPC_X_MEAN), error_mean, error_stddev);
    error_per_coord_stat.at<int>(0,EEPC_X_MEAN) = error_mean[0];
    error_per_coord_stat.at<int>(0,EEPC_X_STDDEV) = error_stddev[0];

    meanStdDev(error_per_coord.col(EPC_Y_MEAN), error_mean, error_stddev);
    error_per_coord_stat.at<int>(0,EEPC_Y_MEAN) = error_mean[0];
    error_per_coord_stat.at<int>(0,EEPC_Y_STDDEV) = error_stddev[0];

    // Error mean and stddev of all samples
    cv::Mat error_total(1, ET_TOTAL, CV_32SC1);
    cv::Mat squared_error_per_sample;

    meanStdDev(error_per_sample.col(EPS_DIST), error_mean, error_stddev);
    error_total.at<int>(0,ET_DIST_MEAN) = error_mean[0];
    error_total.at<int>(0,ET_DIST_STDDEV) = error_stddev[0];
    pow(error_per_sample.col(EPS_DIST),2,squared_error_per_sample);
    meanStdDev(squared_error_per_sample, error_mean, error_stddev);
    error_total.at<int>(0,ET_DIST_MSE) = sqrt(error_mean[0]);

    meanStdDev(error_per_sample.col(EPS_X), error_mean, error_stddev);
    error_total.at<int>(0,ET_X_MEAN) = error_mean[0];
    error_total.at<int>(0,ET_X_STDDEV) = error_stddev[0];
    pow(error_per_sample.col(EPS_X),2,squared_error_per_sample);
    meanStdDev(squared_error_per_sample, error_mean, error_stddev);
    error_total.at<int>(0,ET_X_MSE) = sqrt(error_mean[0]);

    meanStdDev(error_per_sample.col(EPS_Y), error_mean, error_stddev);
    error_total.at<int>(0,ET_Y_MEAN) = error_mean[0];
    error_total.at<int>(0,ET_Y_STDDEV) = error_stddev[0];
    pow(error_per_sample.col(EPS_Y),2,squared_error_per_sample);
    meanStdDev(squared_error_per_sample, error_mean, error_stddev);
    error_total.at<int>(0,ET_Y_MSE) = sqrt(error_mean[0]);

    // Saving file names
    cv::FileStorage error; std::stringstream dname, fname, pname;
    dname << "errors-" << descrip << "-" << hiddenLayerCount << "hl-" << hiddenLayerSize << "n-f" << learningRate << "-e";
    pname << dname.str() << ".png";
    fname << dname.str() << ".yml";

    // Graph error distribution
    double min, max;
    minMaxLoc(error_per_sample.col(EPS_X), &min, &max);
    cv::Mat pdfDist(1, max-min, CV_32SC1, cv::Scalar(0));
    for(int i = 0; i < error_per_sample.rows; i++)
    {
        const int idx = error_per_sample.at<int>(i,EPS_X);
        pdfDist.at<int>(0,idx-min) += 1;
    }
    minMaxLoc(pdfDist, NULL, &max);
    cv::Mat visPdfDist(max, pdfDist.cols, CV_8UC1, cv::Scalar(0));
    for(int i = 0; i < pdfDist.cols; i++)
        cv::line(visPdfDist, cv::Point(i,0), cv::Point(i,pdfDist.at<int>(0,i)), cv::Scalar(100), 1);
    cv::line(visPdfDist, cv::Point(error_total.at<int>(0,ET_X_MEAN)-min,0), cv::Point(error_total.at<int>(0,ET_X_MEAN)-min,1), cv::Scalar(255), 1);
    cv::flip(visPdfDist, visPdfDist, 0);
    imwrite(pname.str().c_str(), visPdfDist);

//    // Visualise weights
//    double* ws = mlpANN.get_weights(0);
//    cv::Mat visWeights(14,20,CV_32FC1);
//    cv::Mat visWeightsL(14,20,CV_32FC1);
//    for(int i = 0; i < 14*20; i++)
//    {
//        visWeights.at<float>(0,i) = (float)abs(ws[i]);
//        visWeightsL.at<float>(0,i) = (float)abs(ws[280+i]);
//    }
////    cv::normalize(visWeights, visWeights, 0, 255, cv::NORM_MINMAX);
////    cv::normalize(visWeightsL, visWeightsL, 0, 255, cv::NORM_MINMAX);
//    cv::threshold(visWeights, visWeights, 0, 255, cv::THRESH_BINARY);
//    cv::threshold(visWeightsL, visWeightsL, 0, 255, cv::THRESH_BINARY);
//    imwrite("input-weights-right.png", visWeights);
//    imwrite("input-weights-left.png", visWeightsL);

    // Save errors to YML file
    error.open(fname.str().c_str(), cv::FileStorage::WRITE);
    error << "training epochs - iterations" << iter
          << "error total - mean stddev mse" << error_total
          << "error per coordinate stats - mean stddev" << error_per_coord_stat
          << "error per coordinate - xcoord ycoord mean stddev" << error_per_coord
          << "error per sample - dist errx erry" << error_per_sample;
    // PERFORMANCE ANALYSIS ON TRAINING DATA
}
