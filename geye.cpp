#include <cv.h>
#include <highgui.h>
using namespace cv;


// testing for the moment
void testrun()
{
    // create window to display webcam capture
    const char *WIN_NAME = "Capture";
    namedWindow(WIN_NAME, 1);

    // create video-from-webcam capture object and capture buffer
    VideoCapture ge_cap(0); Mat ge_cap_img;

    // check that opencv found a webcam
    if(!ge_cap.isOpened()) exit(1);

    // create haar classifier cascade
    CascadeClassifier ge_cap_haar("c:\\opencv\\data\\haarcascades\\haarcascade_frontalface_default.xml");


    // temporary variables for the loop
    vector<Rect> faces; int count, i;

    // grab frames, process and display them
    while(waitKey(12) != 'q')
    {
        // grab frame and place in buffer
        ge_cap >> ge_cap_img;

        // do image processing stuff here
//        ge_cap_haar.detectMultiScale(ge_cap_img, faces, 1.1, 4, 0, Size(96,132));

        // draw rectangles of faces detected
//        count = faces.size();
//        for(i = 0; i < count; i++)
//        {
//            rectangle(ge_cap_img, faces[i].tl(), faces[i].br(), CV_RGB(0,255,0));
//        }

        // show the image stored in buffer
        imshow(WIN_NAME, ge_cap_img);
    }

//    while(waitKey(12) != 'q')
//    {
//
//    }

}
